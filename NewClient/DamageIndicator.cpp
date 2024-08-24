#include"pch.h"
#include "DamageIndicator.h"
#include"Texture.h"
#include"DamageObject.h"
#include"Camera.h"
#include"Timer.h"
bool DamageIndicator::Init()
{
	for (int i = 0; i < 10 ;++i)
	{
		std::shared_ptr<DamageObject> dam = std::make_shared<DamageObject>();
		dam->Init();
		m_vPrintDamage.push_back(dam);

	}
	return false;
}

bool DamageIndicator::Frame(TVector3 pos, int damage)
{
	if (m_bisValid)
	{
		StartPrintDamage();
		bool chk = false;
		TVector3 inter(0, 0, 1);
		for (auto& dam : m_vPrintDamage)
		{
			dam->Frame(pos+ inter, damage);
			chk = dam->GetIsValid();
			inter.y += 50.0f;
		}
		if (!chk)
		{

			m_bisValid = false;
			m_fRenderCnt = 0.1f;
		}
	}

	return false;
}


bool DamageIndicator::Render()
{
	if (m_bisValid)
	{
		m_fRenderCnt += Timer::GetInstance().GetSecPerFrame();
		if (m_fRenderCnt >= m_vPrintDamage.size() / 10)
		{
			for (auto& dam : m_vPrintDamage)
			{
				dam->InitCurrentTime();
			}
			m_bisValid=false;
			m_fRenderCnt = 0.1f;
			return false;
		}
			
		int size = m_fRenderCnt *10;
	
		for (int i = 0; i < size; ++i)
		{
			m_vPrintDamage[i]->Render();
		}
	}
	return false;
}

void DamageIndicator::StartPrintDamage()
{
	for (auto& dam : m_vPrintDamage)
	{
		dam->SetIsValid(true);
	}

}

DamageIndicator::DamageIndicator()
	:m_bisValid(false)
	, m_fRenderCnt(0.1f)
{
}

DamageIndicator::~DamageIndicator()
{
}



/////데미지 위치 ////
//// 데미지 자릿수 /2를해서 나온값을 이용
//// 나머지가 있으면 몫만큼 이동
//// 나머지가 없으면  몫 -1 + 0.5만큼이동

//데미지 입힌 스킬에서 데미지 리스트를 뽑아서 보내주면 그걸로 계산해서 값을 저장해준다
//지금은 최대 10타수 10자리수까지만 해보자