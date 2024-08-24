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



/////������ ��ġ ////
//// ������ �ڸ��� /2���ؼ� ���°��� �̿�
//// �������� ������ ��ŭ �̵�
//// �������� ������  �� -1 + 0.5��ŭ�̵�

//������ ���� ��ų���� ������ ����Ʈ�� �̾Ƽ� �����ָ� �װɷ� ����ؼ� ���� �������ش�
//������ �ִ� 10Ÿ�� 10�ڸ��������� �غ���