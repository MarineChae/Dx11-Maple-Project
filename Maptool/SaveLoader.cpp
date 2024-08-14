#include"pch.h"
#include "SaveLoader.h"
#include"Scene.h"
#include"Texture.h"
#include"Collider.h"
#include"SpriteObject.h"
bool SaveLoader::SaveData(std::shared_ptr<Scene> pSceneData, std::string SavePath)
{

	FILE* fpWrite = nullptr;

	if (fopen_s(&fpWrite, SavePath.c_str(), "w") == 0)
	{
		bool bRet = true;
		std::string header = "#MapName";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		std::string path = "../resource/MapObejct/" + wtm(pSceneData->GetMapName()) + ".png";
		bRet = fprintf_s(fpWrite, "%s\n", path.c_str());

		header = "#LineCollider";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%d\n", static_cast<int>(pSceneData->GetLineColliderList().size()));
		for (auto& line : pSceneData->GetLineColliderList())
		{
			bRet = fprintf_s(fpWrite, "%f\t", line->From.x);
			bRet = fprintf_s(fpWrite, "%f\t", line->From.y); 
			bRet = fprintf_s(fpWrite, "%f\t", line->To.x);
			bRet = fprintf_s(fpWrite, "%f\n", line->To.y);
		}

		header = "#ObjectList";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%d\n", static_cast<int>(pSceneData->GetObjectList().size()));
		for (auto& obj : pSceneData->GetObjectList())
		{

			std::string path = "../resource/MapObejct/" + wtm(obj->GetTexture()->GetName());
			bRet = fprintf_s(fpWrite, "%s\n", path.c_str());

			bRet = fprintf_s(fpWrite, "%d\t", obj->GetObjectType());
			bRet = fprintf_s(fpWrite, "%f\t", obj->GetTransform().x);
			bRet = fprintf_s(fpWrite, "%f\t", obj->GetTransform().y);
			bRet = fprintf_s(fpWrite, "%d\t", obj->GetSpriteInfo()->iCol);
			bRet = fprintf_s(fpWrite, "%d\t", obj->GetSpriteInfo()->iRow);
			bRet = fprintf_s(fpWrite, "%d\t", obj->GetSpriteInfo()->iMaxImageCount);
			bRet = fprintf_s(fpWrite, "%lf\n", obj->GetSpriteInfo()->m_fDelay);
		}


	}




	return false;
}

bool SaveLoader::LoadData(std::shared_ptr<Scene> pSceneData, std::string LoadPath)
{
	FILE* fpRead = nullptr;

	if (fopen_s(&fpRead, LoadPath.c_str(), "rt") == 0)
	{

		TCHAR buffer[256] = { 0, };

		while (_fgetts(buffer, _countof(buffer), fpRead) != 0)
		{
			TCHAR type[36] = { 0, };

			_stscanf_s(buffer, _T("%s"), type, (unsigned int)_countof(type));

			if (_tcscmp(type, L"#MapName") == 0)
			{

				TCHAR tex[80] = { 0, };
				_fgetts(buffer, _countof(buffer), fpRead);
				_stscanf_s(buffer, _T("%s\n"), tex, (unsigned int)_countof(tex));

				pSceneData->ResetMap(tex);
				pSceneData->GetMap()->SetScale({ static_cast<float>(pSceneData->GetMap()->GetTexture()->GetWidth()),
												 static_cast<float>(pSceneData->GetMap()->GetTexture()->GetHeight()),0});
			}
			else if (_tcscmp(type, L"#LineCollider") == 0)
			{

				_fgetts(buffer, _countof(buffer), fpRead);
				int iSize = 0;
				_stscanf_s(buffer, _T("%d"), &iSize);
				std::vector<PNCT_VERTEX> v;
				for (int i = 0; i < iSize; i++)
				{
					_fgetts(buffer, _countof(buffer), fpRead);
					std::shared_ptr<Line> line = std::make_shared<Line>();
					_stscanf_s(buffer, _T("%f %f %f %f \n"), &line->From.x, &line->From.y, &line->To.x, &line->To.y);
					v.push_back({});
					v[v.size()-1].Pos = line->From;
					v.push_back({});
					v[v.size() - 1].Pos = line->To;
					
					pSceneData->PushLineCollider(line);
				}
				pSceneData->GetCollider()->SetVertexList(v);

			}
			else if (_tcscmp(type, L"#ObjectList") == 0)
			{

				_fgetts(buffer, _countof(buffer), fpRead);
				int iSize = 0;
				_stscanf_s(buffer, _T("%d"), &iSize);
				
				for (int i = 0; i < iSize; ++i)
				{
					std::shared_ptr<SpriteObject> obj = std::make_shared<SpriteObject>();
					std::shared_ptr<SpriteData> data = std::make_shared<SpriteData>();
					TCHAR tex[80] = { 0, };
					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%s\n"), tex, (unsigned int)_countof(tex));
					obj->Create(tex, L"../Shader/Defalutshader.hlsl");

					int objectType;
					TVector3 temp;
					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%d %f %f %d %d %d %lf \n"), &objectType,&temp.x, &temp.y, &data->iCol, &data->iRow, &data->iMaxImageCount, &data->m_fDelay);
					obj->SetObejctType((ObejctType)objectType);
					obj->SetTransform(temp);
					obj->SetSpriteInfo(data);

				
					obj->SetUVData(data->m_UVList, data->iRow,data->iCol);
					obj->GetSpriteInfo()->m_vScale = { static_cast<float>(obj->GetTexture()->GetWidth() / data->iCol),
													   static_cast<float>(obj->GetTexture()->GetHeight() / data->iRow),1 };

					obj->SetScale(data->m_vScale);

					pSceneData->PushObject(obj);
				}
				
			
			}



		}
		fclose(fpRead);
	}


	return false;
}
