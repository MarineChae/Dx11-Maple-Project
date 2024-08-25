#include "DropObject.h"
#include"Timer.h"
#include"Scene.h"
#include"Collision.h"
#include"Collider.h"
#include"PlayerObject.h"
bool DropObject::Frame()
{
	bool chk = SpriteObject::Frame();
	if (!chk && GetObjectState() == OB_WARNING)
	{
		ChangeObjectState();
		SetSpriteInfo(GetSpriteData(GetObjectState()));
		SetScale(GetCurrentSpriteInfo()->m_vScale);
		SetTexture(GetCurrentSpriteInfo()->m_pTexture);
		GetCollider()->SetScale(GetScale());
	}
	if (GetObjectState() == OB_ING)
	{
		auto t = GetTransform();
		t.y -= static_cast<float>(700 *Timer::GetInstance().GetSecPerFrame());
		SetTransform(t);
		GetCollider()->SetTransform(t);
		auto temp = SceneMgr::GetInstance().GetCurrentScene();
		for (auto& player : SceneMgr::GetInstance().GetCurrentScene()->GetPlayerList())
		{
 			if (Collider::CheckOBBCollision(player->GetCollider(), GetCollider()))
			{
				player->SetHp(player->GetHp() - (player->GetMaxHp() * 0.15));
				ChangeObjectState();
				SetSpriteInfo(GetSpriteData(GetObjectState()));
				SetScale(GetCurrentSpriteInfo()->m_vScale);
				SetTexture(GetCurrentSpriteInfo()->m_pTexture);
				return true;
			}

		}
		for (auto& line : SceneMgr::GetInstance().GetCurrentScene()->GetLineColliderList())
		{
			if (line->type == COLLISION_TYPE::CT_FINALFLOOR)
			{
				if (Collision::isLineIntersectingOBB(line,GetCollider(),.5f))
				{
					ChangeObjectState();
					SetSpriteInfo(GetSpriteData(GetObjectState()));
					SetScale(GetCurrentSpriteInfo()->m_vScale);
					SetTexture(GetCurrentSpriteInfo()->m_pTexture);
					return true;
				}
			}
		}

	}
	if (!chk&&GetObjectState() == OB_END)
	{
		return false;
	}


    return true;
}

DropObject::DropObject()
{
}

DropObject::~DropObject()
{
}
