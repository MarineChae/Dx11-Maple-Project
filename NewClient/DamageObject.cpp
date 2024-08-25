#include "DamageObject.h"
#include "Texture.h"
#include "Timer.h"
#include "Camera.h"
#include<algorithm>
bool DamageObject::Init()
{
	for (int i = 0; i < 10; ++i)
	{
		std::wstring st = L"../resource/DamageSkin/Damage";
		st += std::to_wstring(i);
		st += L".png";
		auto tex = TextureMgr::GetInstance().Load(st);
		m_vTextureList.push_back(tex);
	}

	for (int i = 0; i < 10; ++i)
	{
		std::shared_ptr<Object> dam = std::make_shared<Object>();
		dam->Create(L"", L"../Shader/Defalutshader.hlsl");
		dam->SetScale({26,36,0});
		m_vDamageList.push_back(dam);
	}


    return true;
}

bool DamageObject::Frame(TVector3 pos, int damage)
{


	m_fCurrentTime += Timer::GetInstance().GetSecPerFrame();
	m_fAlpha -= Timer::GetInstance().GetSecPerFrame();
	if (m_fPrintTime <= m_fCurrentTime)
	{
		m_fAlpha = 1.0f;
		m_bValid = false;
		m_fCurrentTime = 0;
	}

	m_iNumSize = 0;
	if (damage > 0)
	{
		std::vector<int> vdamage;
		int Temp = damage;
		while (Temp > 0)
		{
			int damageNum = Temp % 10;
			Temp /= 10;
			vdamage.push_back(damageNum);
		}
		for (auto it = vdamage.rbegin(); it != vdamage.rend(); ++it)
		{
			m_vDamageList[m_iNumSize++]->SetTexture(m_vTextureList[*it]);
		}
	}

	float startpos = pos.x - (50 * m_iNumSize / 2);
	float interval = 50.0f;
	int itemp = 0;
	for (auto& dam : m_vDamageList)
	{
		dam->SetTransform({ startpos + interval*itemp++ ,pos.y,pos.z });
		dam->Frame();
	}


	return true;
}


bool DamageObject::Render()
{

	for (int i = 0; i < m_iNumSize; ++i)
	{
		m_vDamageList[i]->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(), &CameraMgr::GetInstance().GetCamera().GetProjection());
		m_vDamageList[i]->Render();
	}

	return true;
}

void DamageObject::SetIsValid(bool valid)
{
	m_bValid = valid;
}

DamageObject::DamageObject()
	:m_fPrintTime(1.0f)
	, m_fCurrentTime(0.0f)
	, m_fAlpha(1.0f)
	, m_bValid(false)
{
	
}

DamageObject::~DamageObject()
{
}
