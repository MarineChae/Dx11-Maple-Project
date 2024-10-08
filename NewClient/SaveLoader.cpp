#include"pch.h"
#include "SaveLoader.h"
#include"Scene.h"
#include"Collider.h"
#include"PotalObject.h"
#include"Texture.h"
#include"MonsterObject.h"
#include"SpriteObject.h"
#include"SoundMgr.h"
bool SaveLoader::SaveData(std::shared_ptr<Scene> pSceneData, std::string SavePath)
{

	FILE* fpWrite = nullptr;

	if (fopen_s(&fpWrite, SavePath.c_str(), "w") == 0)
	{
		bool bRet = true;
		std::string header = "#MapName";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		std::string path;
		auto ret = pSceneData->GetMapName().find(L"../resource");
		if (!ret)
		{
			path = wtm(pSceneData->GetMapName()) + ".png";
		}
		else
		{
			path = "../resource/MapObejct/" + wtm(pSceneData->GetMapName()) + ".png";
		}

		bRet = fprintf_s(fpWrite, "%s\n", path.c_str());

		header = "#SceneNum";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%d\n", pSceneData->GetSceneNum());

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
		bRet = fprintf_s(fpWrite, "%d\n", static_cast<int>(pSceneData->GetObjectList().size() - pSceneData->GetPotalList().size()));
		for (auto& obj : pSceneData->GetObjectList())
		{
			if (obj->GetObjectType() == ObejctType::Defalut)
			{
				std::string path = "../resource/MapObejct/" + wtm(obj->GetTexture()->GetName());
				bRet = fprintf_s(fpWrite, "%s\n", path.c_str());

				bRet = fprintf_s(fpWrite, "%d\t", obj->GetObjectType());
				bRet = fprintf_s(fpWrite, "%f\t", obj->GetTransform().x);
				bRet = fprintf_s(fpWrite, "%f\t", obj->GetTransform().y);
				bRet = fprintf_s(fpWrite, "%d\t", obj->GetSpriteInfo()->iCol);
				bRet = fprintf_s(fpWrite, "%d\t", obj->GetSpriteInfo()->iRow);
				bRet = fprintf_s(fpWrite, "%d\t", obj->GetSpriteInfo()->iMaxImageCount);
				bRet = fprintf_s(fpWrite, "%f\n", obj->GetSpriteInfo()->m_fDelay);
			}

		}

		header = "#MonsterList";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%d\n", static_cast<int>(pSceneData->GetMonsterList().size()));
		for (auto& obj : pSceneData->GetMonsterList())
		{

			std::string path = "../resource/Monster/" + wtm(obj->GetTexture()->GetName());
			bRet = fprintf_s(fpWrite, "%s\n", path.c_str());
			bRet = fprintf_s(fpWrite, "%f\t", obj->GetTransform().x);
			bRet = fprintf_s(fpWrite, "%f\n", obj->GetTransform().y);


		}

		header = "#PotalList";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%d\n", static_cast<int>(pSceneData->GetPotalList().size()));
		for (auto& obj : pSceneData->GetPotalList())
		{
			if (obj->GetObjectType() == ObejctType::Portal)
			{
				std::string path = "../resource/MapObejct/" + wtm(obj->GetTexture()->GetName());
				bRet = fprintf_s(fpWrite, "%s\n", path.c_str());
				bRet = fprintf_s(fpWrite, "%d\t", obj->GetNextSceneNum());
				bRet = fprintf_s(fpWrite, "%d\t", obj->GetObjectType());
				bRet = fprintf_s(fpWrite, "%f\t", obj->GetTransform().x);
				bRet = fprintf_s(fpWrite, "%f\t", obj->GetTransform().y);
				bRet = fprintf_s(fpWrite, "%d\t", obj->GetSpriteInfo()->iCol);
				bRet = fprintf_s(fpWrite, "%d\t", obj->GetSpriteInfo()->iRow);
				bRet = fprintf_s(fpWrite, "%d\t", obj->GetSpriteInfo()->iMaxImageCount);
				bRet = fprintf_s(fpWrite, "%f\n", obj->GetSpriteInfo()->m_fDelay);
			}

		}
		fclose(fpWrite);
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
												 static_cast<float>(pSceneData->GetMap()->GetTexture()->GetHeight()),0 });
			}
			else if (_tcscmp(type, L"#SceneNum") == 0)
			{
				_fgetts(buffer, _countof(buffer), fpRead);
				int Num = 0;
				_stscanf_s(buffer, _T("%d"), &Num);
				pSceneData->SetSceneNum(Num);

			}
			else if (_tcscmp(type, L"#LineCollider") == 0)
			{
				pSceneData->ClearLineList();
				_fgetts(buffer, _countof(buffer), fpRead);
				int iSize = 0;
				_stscanf_s(buffer, _T("%d"), &iSize);
				std::vector<PNCT_VERTEX> v;
				for (int i = 0; i < iSize; i++)
				{
					_fgetts(buffer, _countof(buffer), fpRead);
					std::shared_ptr<Line> line = std::make_shared<Line>();
					_stscanf_s(buffer, _T("%d %f %f %f %f \n"), &line->type, &line->From.x, &line->From.y, &line->To.x, &line->To.y);
					v.push_back({});
					v[v.size() - 1].Pos = line->From;
					v.push_back({});
					v[v.size() - 1].Pos = line->To;

					pSceneData->PushLineCollider(line);
				}
				v.push_back({});
				pSceneData->GetCollider()->SetVertexList(v);
				auto vb = pSceneData->GetCollider()->GetVertexBuffer();
				auto vl = pSceneData->GetCollider()->GetVertexList();

				Device::GetContext()->UpdateSubresource(vb.Get(), 0, 0, &vl.at(0), 0, 0);
			}
			else if (_tcscmp(type, L"#ObjectList") == 0)
			{
				pSceneData->ClearObjectList();
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
					obj->Init();
					obj->Create(tex, L"../Shader/Defalutshader.hlsl");

					int objectType;
					TVector3 temp;
					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%d %f %f %d %d %d %f \n"), &objectType, &temp.x, &temp.y, &data->iCol, &data->iRow, &data->iMaxImageCount, &data->m_fDelay);
					obj->SetObejctType((ObejctType)objectType);
					obj->SetTransform(temp);
					obj->SetSpriteInfo(data);


					obj->SetUVData(data->m_UVList, data->iRow, data->iCol);
					obj->GetSpriteInfo()->m_vScale = { static_cast<float>(obj->GetTexture()->GetWidth() / data->iCol),
													   static_cast<float>(obj->GetTexture()->GetHeight() / data->iRow),1 };

					obj->SetScale(data->m_vScale);
					obj->GetCollider()->SetTransform(obj->GetTransform());
					obj->GetCollider()->SetScale(data->m_vScale);
					obj->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");

					pSceneData->PushObject(obj);
				}


			}
			else if (_tcscmp(type, L"#BGM") == 0)
			{
				TCHAR sound[80] = { 0, };
				_fgetts(buffer, _countof(buffer), fpRead);
				_stscanf_s(buffer, _T("%s\n"), sound, (unsigned int)_countof(sound));

				pSceneData->SetBGM(SoundMgr::GetInstance().Load(sound));
					
			}
			else if (_tcscmp(type, L"#PotalList") == 0)
			{
				pSceneData->ClearPotalList();
				_fgetts(buffer, _countof(buffer), fpRead);
				int iSize = 0;
				_stscanf_s(buffer, _T("%d"), &iSize);

				for (int i = 0; i < iSize; ++i)
				{
					std::shared_ptr<SpriteObject> obj = std::make_shared<PotalObject>();
					std::shared_ptr<SpriteData> data = std::make_shared<SpriteData>();
					TCHAR tex[80] = { 0, };
					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%s\n"), tex, (unsigned int)_countof(tex));
					obj->Init();
					obj->Create(tex, L"../Shader/Defalutshader.hlsl");

					int nextSceneNum;
					int objectType;
					TVector3 temp;
					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%d %d %f %f %d %d %d %f \n"), &nextSceneNum, &objectType, &temp.x, &temp.y, &data->iCol, &data->iRow, &data->iMaxImageCount, &data->m_fDelay);
					obj->SetNextSceneNum(nextSceneNum);
					obj->SetObejctType((ObejctType)objectType);
					obj->SetTransform(temp);
					obj->SetSpriteInfo(data);


					obj->SetUVData(data->m_UVList, data->iRow, data->iCol);
					obj->GetSpriteInfo()->m_vScale = { static_cast<float>(obj->GetTexture()->GetWidth() / data->iCol),
													   static_cast<float>(obj->GetTexture()->GetHeight() / data->iRow),1 };

					obj->SetScale(data->m_vScale);

					obj->SetScale(data->m_vScale);
					obj->GetCollider()->SetTransform(obj->GetTransform());
					obj->GetCollider()->SetScale(data->m_vScale);
					obj->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");
					pSceneData->PushObject(obj);
					pSceneData->PushPotalObject(obj);
				}


			}


		}
		fclose(fpRead);
	}


	return false;
}
bool SaveLoader::SaveMonsterData(std::shared_ptr<MonsterObject> monster)
{
	FILE* fpWrite = nullptr;

	std::string path = "../resource/MonsterData/";
	path += monster->GetMonsterName();
	path += ".txt";

	if (fopen_s(&fpWrite, path.c_str(), "w") == 0)
	{
		bool bRet = true;
		std::string header = "#MonsterName";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%s\n", monster->GetMonsterName().c_str());


		header = "#MonsterState";
		bRet = fprintf_s(fpWrite, "%s\n", header.c_str());
		bRet = fprintf_s(fpWrite, "%d\n", static_cast<int>(monster->GetSpriteList().size()));
		int i = 0;
		for (auto& sprite : monster->GetSpriteList())
		{
			std::string spritePath = "../resource/Monster/";
			spritePath += monster->GetMonsterName();
			spritePath += "/";
			spritePath += wtm(sprite->m_pTexture->GetName().c_str());

			bRet = fprintf_s(fpWrite, "%s\n", spritePath.c_str());
			bRet = fprintf_s(fpWrite, "%d\t", static_cast<int>((MONSTER_STATE)i++));
			bRet = fprintf_s(fpWrite, "%d\t", sprite->iCol);
			bRet = fprintf_s(fpWrite, "%d\t", sprite->iRow);
			bRet = fprintf_s(fpWrite, "%d\t", sprite->iMaxImageCount);
			bRet = fprintf_s(fpWrite, "%f\n", sprite->m_fDelay);
		}


		fclose(fpWrite);
	}



	return false;
}

bool SaveLoader::LoadMonsterData(std::shared_ptr<MonsterObject> monster, std::string LoadPath)
{
	FILE* fpRead = nullptr;

	if (fopen_s(&fpRead, LoadPath.c_str(), "rt") == 0)
	{

		monster->Init();
		TCHAR buffer[256] = { 0, };

		while (_fgetts(buffer, _countof(buffer), fpRead) != 0)
		{
			TCHAR type[36] = { 0, };

			_stscanf_s(buffer, _T("%s"), type, (unsigned int)_countof(type));

			if (_tcscmp(type, L"#MonsterName") == 0)
			{

				TCHAR tex[80] = { 0, };
				_fgetts(buffer, _countof(buffer), fpRead);
				_stscanf_s(buffer, _T("%s\n"), tex, (unsigned int)_countof(tex));

				monster->SetMonsterName(wtm(tex));
			}
			else if (_tcscmp(type, L"#MonsterState") == 0)
			{
				TCHAR tex2[80] = { 0, };
				_fgetts(buffer, _countof(buffer), fpRead);
				_stscanf_s(buffer, _T("%s\n"), tex2, (unsigned int)_countof(tex2));

				_fgetts(buffer, _countof(buffer), fpRead);
				int iSize = 0;
				_stscanf_s(buffer, _T("%d"), &iSize);

				_fgetts(buffer, _countof(buffer), fpRead);
				int hp = 0;
				_stscanf_s(buffer, _T("%d"), &hp);
				monster->SetMaxHp(hp);
				monster->SetHp(hp);

				for (int i = 0; i < iSize; ++i)
				{
					TCHAR tex[80] = { 0, };
					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%s\n"), tex, (unsigned int)_countof(tex));


					MONSTER_STATE state;
					std::shared_ptr<SpriteData> SpriteInfo = std::make_shared<SpriteData>();
					SpriteInfo->iCol = 1;
					SpriteInfo->iRow = 1;
					SpriteInfo->iMaxImageCount = 1;
					SpriteInfo->m_fDelay = 0.18f;
					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%d %f %f %d %d %d %f\n"), &state,
						&SpriteInfo->m_vOffset.x,
						&SpriteInfo->m_vOffset.y,
						&SpriteInfo->iCol,
						&SpriteInfo->iRow,
						&SpriteInfo->iMaxImageCount,
						&SpriteInfo->m_fDelay);

 					monster->SetSpriteInfo(SpriteInfo);
					monster->Create(tex, L"../Shader/Defalutshader.hlsl");
					SpriteInfo->m_vScale = { static_cast<float>(monster->GetTexture()->GetWidth() / monster->GetSpriteInfo()->iCol),
											  static_cast<float>(monster->GetTexture()->GetHeight() / monster->GetSpriteInfo()->iRow),
											 1 };

				}
				monster->SetScale({ static_cast<float>(monster->GetTexture()->GetWidth() / monster->GetSpriteInfo()->iCol),
							   static_cast<float>(monster->GetTexture()->GetHeight() / monster->GetSpriteInfo()->iRow),
								1 });
				monster->GetCollider()->SetTransform(monster->GetTransform());
				monster->GetCollider()->SetScale({ static_cast<float>(monster->GetTexture()->GetWidth() / monster->GetSpriteInfo()->iCol),
												   static_cast<float>(monster->GetTexture()->GetHeight() / monster->GetSpriteInfo()->iRow),
													1 });
				monster->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");

				monster->InitTexIndex();
				monster->SetSpriteInfo(monster->GetSpriteData(MS_IDLE));
				monster->SetScale(monster->GetCurrentSpriteInfo()->m_vScale);
				monster->SetTexture(monster->GetCurrentSpriteInfo()->m_pTexture);
				monster->SetTransform({ monster->GetTransform().x + monster->GetCurrentSpriteInfo()->m_vOffset.x
					   ,monster->GetTransform().y + monster->GetCurrentSpriteInfo()->m_vOffset.y
					   ,monster->GetTransform().z });
			}

		}
		fclose(fpRead);
	}
	return false;
}
bool SaveLoader::LoadObjectData(std::shared_ptr<SpriteObject> monster, std::string LoadPath)
{
	FILE* fpRead = nullptr;

	if (fopen_s(&fpRead, LoadPath.c_str(), "rt") == 0)
	{

		TCHAR buffer[256] = { 0, };

		while (_fgetts(buffer, _countof(buffer), fpRead) != 0)
		{
			TCHAR type[36] = { 0, };

			_stscanf_s(buffer, _T("%s"), type, (unsigned int)_countof(type));

			if (_tcscmp(type, L"#ObjectName") == 0)
			{

				TCHAR tex[80] = { 0, };
				_fgetts(buffer, _countof(buffer), fpRead);
				_stscanf_s(buffer, _T("%s\n"), tex, (unsigned int)_countof(tex));

				//monster->SetOb(wtm(tex));
			}
			else if (_tcscmp(type, L"#ObjectState") == 0)
			{
				_fgetts(buffer, _countof(buffer), fpRead);
				int iSize = 0;
				_stscanf_s(buffer, _T("%d"), &iSize);

				for (int i = 0; i < iSize; ++i)
				{
					TCHAR tex[80] = { 0, };
					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%s\n"), tex, (unsigned int)_countof(tex));

					monster->Init();

					OBJECT_STATE state;
					std::shared_ptr<SpriteData> SpriteInfo = std::make_shared<SpriteData>();
					SpriteInfo->iCol = 1;
					SpriteInfo->iRow = 1;
					SpriteInfo->iMaxImageCount = 1;
					SpriteInfo->m_fDelay = 0.18f;

					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%d %d %d %d %f\n"), &state,
						&SpriteInfo->iCol,
						&SpriteInfo->iRow,
						&SpriteInfo->iMaxImageCount,
						&SpriteInfo->m_fDelay);

					monster->SetSpriteInfo(SpriteInfo);
					monster->Create(tex, L"../Shader/Defalutshader.hlsl");
					SpriteInfo->m_vScale = { static_cast<float>(monster->GetTexture()->GetWidth() / monster->GetSpriteInfo()->iCol),
											  static_cast<float>(monster->GetTexture()->GetHeight() / monster->GetSpriteInfo()->iRow),
											 1 };

				}
				monster->SetScale({ static_cast<float>(monster->GetTexture()->GetWidth() / monster->GetSpriteInfo()->iCol),
							   static_cast<float>(monster->GetTexture()->GetHeight() / monster->GetSpriteInfo()->iRow),
								1 });
				monster->GetCollider()->SetTransform(monster->GetTransform());
				monster->GetCollider()->SetScale({ static_cast<float>(monster->GetTexture()->GetWidth() / monster->GetSpriteInfo()->iCol),
												   static_cast<float>(monster->GetTexture()->GetHeight() / monster->GetSpriteInfo()->iRow),
													1 });
				monster->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");



			}


		}
		fclose(fpRead);
	}
	return false;
}

