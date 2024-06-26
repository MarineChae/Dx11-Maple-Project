#include "pch.h"
#include "ClientMain.h"
#include "Input.h"
#include "Timer.h"
#include "Packet.h"
#include "MakePacket.h"
#include "ClientNet.h"
#include"Collider.h"
#include"Collision.h"
bool ClientMain::Init()
{

	test = std::make_shared<Object>();	
	test->SetTransform(TVector3(0, -600, 0));
	test->SetScale(TVector3(1800, 64, 0));
	test->Init();
	test->Create(L"../resource/MapObejct/2Phase BackCollision.png", L"../Shader/Defalutshader.hlsl");
	test->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");
	test->GetCollider()->SetTransform(test->GetTransform());
	test->GetCollider()->SetScale(TVector3(1800, 64, 0));
	test->GetCollider()->SetCollsionType(COLLISION_TYPE::CT_FLOOR);
	test->m_vRotate.z = -45;
	test->GetCollider()->m_vRotate.z = -45;
	ObejctMgr::GetInstance().PushObject(test,60);


	return true;
}

bool ClientMain::Frame()
{

	for (auto& obj : ObejctMgr::GetInstance().GetObjectList())
	{
		if (obj != nullptr)
		{

			obj->Frame();
			if (obj->GetCollider() != nullptr)
				obj->GetCollider()->Frame();
			
			Line line;
			line.From = test->GetCollider()->GetVertexList()[0].Pos;
			line.To = test->GetCollider()->GetVertexList()[1].Pos;
			auto ma = test->GetCollider()->GetWorldMat();
			D3DXVec3TransformCoord(&line.From, &line.From, &test->GetCollider()->GetWorldMat());
			D3DXVec3TransformCoord(&line.To, &line.To, &test->GetCollider()->GetWorldMat());
			auto pl = dynamic_cast<PlayerObject*>(obj.get());
			if (Collision::PointToLine(obj->GetCollider()->GetCollisionPoint(), line))
			{
				auto ret = Collision::ClosestPoint(obj->GetCollider()->GetCollisionPoint(), line);
				auto p = obj->GetTransform();
				p.y = ret.y + obj->GetCollider()->GetHeight();
				obj->SetTransform(p);

			}

		}
	}
	//test->m_vRotate.z += Timer::GetInstance().GetSecPerFrame()/10;
	//test->GetCollider()->m_vRotate.z += Timer::GetInstance().GetSecPerFrame()/10;


	//test->Frame();

	return true;
}

bool ClientMain::Render()
{

	for (auto& obj : ObejctMgr::GetInstance().GetObjectList())
	{
		if (obj != nullptr)
		{
			obj->SetMatrix(nullptr, &GetCamera().GetView(), &GetCamera().GetProjection());
			obj->Render();
			if (obj->GetCollider() != nullptr)
			{
				obj->GetCollider()->SetMatrix(nullptr, &GetCamera().GetView(), &GetCamera().GetProjection());
				obj->GetCollider()->Render();
			}
		}
		
	}

  	return true;
}

bool ClientMain::Release()
{
	//networkClean();
	return true;
}

ClientMain::ClientMain()
{
}

ClientMain::~ClientMain()
{
}


GAME_PLAY(L"MapleStory", 1388, 766);