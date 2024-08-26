#include"pch.h"
#include "EffectSpwaner.h"
#include"SpriteObject.h"
#include"Camera.h"
#include"Timer.h"
void EffectSpwaner::SpawnEffect(std::shared_ptr<SpriteObject> eff, TVector3 pos,float delay)
{
	for (int i = 0; i < m_vEffctList.size(); ++i)
	{
		if (!m_vEffctList[i].GetRenderState()&& !m_vEffctDelayList[i].first)
		{
			m_vEffctDelayList[i] = std::make_pair(true, delay);
			m_vEffctList[i].SetTexture(eff->GetTexture());
			m_vEffctList[i].SetSpriteInfo(eff->GetSpriteInfo());
			m_vEffctList[i].SetTransform(pos);
			m_vEffctList[i].SetScale(eff->GetScale());
			m_vEffctList[i].SetUVData(m_vEffctList[i].GetSpriteInfo()->m_UVList, m_vEffctList[i].GetSpriteInfo()->iRow, m_vEffctList[i].GetSpriteInfo()->iCol);
			break;
		}

	}


}

void EffectSpwaner::Frame()
{

	for (auto& effect : m_vEffctList)
	{
		if (effect.GetRenderState())
		{
			if (!effect.Frame())
				effect.SetRenderState(false);
		}

	}
	for (int i = 0; i < m_vEffctDelayList.size(); ++i)
	{
		if (m_vEffctDelayList[i].first)
		{
			m_vEffctDelayList[i].second -= Timer::GetInstance().GetSecPerFrame();
			if (m_vEffctDelayList[i].second <= 0)
			{
				m_vEffctDelayList[i].first = false;
				m_vEffctList[i].SetRenderState(true);
			}
		}


	}
}

void EffectSpwaner::Render()
{
	for (auto& effect : m_vEffctList)
	{
		if (effect.GetRenderState())
		{
			effect.SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(), &CameraMgr::GetInstance().GetCamera().GetProjection());
			effect.Render();

		}

	}
}

void EffectSpwaner::Init()
{
	for (auto& effect : m_vEffctList)
	{
		effect.Create(L"", L"../Shader/Defalutshader.hlsl");
	}
	
}
