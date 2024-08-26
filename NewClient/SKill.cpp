#include "SKill.h"
#include"Texture.h"
#include"Collider.h"
#include"Camera.h"


std::shared_ptr<Skill> SkillMgr::GetSkill(std::string num)
{

    auto it = m_SkillMap.find(num);
    if (it != m_SkillMap.end())
    {
        return it->second;
    }
    else
    {
        //로드해서 넘겨줌
        return LoadSkill(num);
    }

}

std::shared_ptr<Skill> SkillMgr::LoadSkill(std::string num)
{

	FILE* fpRead = nullptr;
	std::string LoadPath = "../resource/Skill/";
	LoadPath += num;
	LoadPath += "/";
	LoadPath += num;
	LoadPath += ".txt";
	std::shared_ptr<Skill> retSkill = std::make_shared<Skill>();
	retSkill->SetSkillNum(num);
	if (fopen_s(&fpRead, LoadPath.c_str(), "rt") == 0)
	{

		TCHAR buffer[256] = { 0, };

		while (_fgetts(buffer, _countof(buffer), fpRead) != 0)
		{
			TCHAR type[36] = { 0, };

			_stscanf_s(buffer, _T("%s"), type, (unsigned int)_countof(type));

			if (_tcscmp(type, L"#SkillName") == 0)
			{

				TCHAR tex[80] = { 0, };
				_fgetts(buffer, _countof(buffer), fpRead);
				_stscanf_s(buffer, _T("%s\n"), tex, (unsigned int)_countof(tex));

				retSkill->SetSkillName(wtm(tex));
			}
			else if (_tcscmp(type, L"#SkillSprite") == 0)
			{

				_fgetts(buffer, _countof(buffer), fpRead);
				int iSize = 0;
				_stscanf_s(buffer, _T("%d"), &iSize);

				float colliderScaleX;
				float colliderScaleY;
				   
				for (int i = 0; i < iSize; ++i)
				{
					TCHAR tex[80] = { 0, };
					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%s\n"), tex, (unsigned int)_countof(tex));

					retSkill->Init();

					std::shared_ptr<SpriteData> SpriteInfo = std::make_shared<SpriteData>();
					SpriteInfo->iCol = 1;
					SpriteInfo->iRow = 1;
					SpriteInfo->iMaxImageCount = 1;
					SpriteInfo->m_fDelay = 0.18f;

					float offsetX;
					float offsetY;

					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%f %f %f %f %d %d %d %f \n"), 
						&offsetX,
						&offsetY,
						&colliderScaleX,
						&colliderScaleY,
						&SpriteInfo->iCol,
						&SpriteInfo->iRow,
						&SpriteInfo->iMaxImageCount,
						&SpriteInfo->m_fDelay);

					retSkill->SetSpriteInfo(SpriteInfo);
					retSkill->Create(tex, L"../Shader/Defalutshader.hlsl");
					retSkill->SetOffset({ offsetX, offsetY ,1});
					SpriteInfo->m_vScale = { static_cast<float>(retSkill->GetTexture()->GetWidth() / retSkill->GetSpriteInfo()->iCol),
											  static_cast<float>(retSkill->GetTexture()->GetHeight() / retSkill->GetSpriteInfo()->iRow),
											 1 };

				}
				retSkill->SetScale({ static_cast<float>(retSkill->GetTexture()->GetWidth() / retSkill->GetSpriteInfo()->iCol),
							   static_cast<float>(retSkill->GetTexture()->GetHeight() / retSkill->GetSpriteInfo()->iRow),
								1 });
				retSkill->GetCollider()->SetTransform(retSkill->GetTransform());
				retSkill->GetCollider()->SetScale({ colliderScaleX,colliderScaleY,1 });
				retSkill->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");



			}

		}
		fclose(fpRead);
	}

	return retSkill;
}

void Skill::CopySkill(std::shared_ptr<Skill> skill)
{
	Init();
	SetSpriteInfo(skill->GetSpriteInfo());
	Create(skill->GetTexture()->GetName(), L"../Shader/Defalutshader.hlsl");
	SetOffset({ skill->m_vOffset.x, skill->m_vOffset.y ,1 });

	SetScale({ static_cast<float>(skill->GetTexture()->GetWidth() / skill->GetSpriteInfo()->iCol),
	   static_cast<float>(skill->GetTexture()->GetHeight() / skill->GetSpriteInfo()->iRow),
		1 });
	GetCollider()->SetTransform(skill->GetTransform());
	GetCollider()->SetScale(skill->GetCollider()->GetScale());
	GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");

}

bool Skill::Frame()
{
	if (!m_bEnable)
		return false;
	Object::Frame();

	if (!TextureChangeProc())
	{
		m_bEnable = false;

	}
		

	return true;
}

bool Skill::Render()
{
	if (!m_bEnable)
		return false;

	SpriteObject::Render();

	return true;
}

Skill::Skill()
{
}

Skill::~Skill()
{
}
