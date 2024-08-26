#include"Netstd.h"
#include"BehaviorTree.h"
#include"BehaviorTreeNode.h"
#include"MonsterData.h"
#include"PlayerData.h"
void BehaviorTree::RunTree()
{
	if (isRun)
	{
		m_pRootNode->Tick();
		Update();
	}
		
}

ReturnCode BehaviorTree::ChasePlayer()
{
	 return ReturnCode::SUCCESS; 
}

ReturnCode BehaviorTree::AttackPlayer()
{
	return ReturnCode::SUCCESS;
}

ReturnCode BehaviorTree::Respon()
{
	return ReturnCode::SUCCESS; 
}
ReturnCode BehaviorTree::Skill1Cooldown()
{
	return ReturnCode::SUCCESS;
}

ReturnCode BehaviorTree::Skill1()
{
	return ReturnCode::SUCCESS;
}



//ReturnCode BehaviorTree::ChasePlayer()
//{
//	if (TVector3::Distance(m_Monster.GetPos(), m_Monster.GetTargetPlayer()->GetPos()) >= 0)
//	{
//		m_Monster.MoveTo(m_Monster.GetTargetPlayer()->GetPos());
//		return ReturnCode::SUCCESS;
//	}
//	else
//	{
//	
//		return ReturnCode::RUNNING;
//	}
//	
//}

BehaviorTree::BehaviorTree(MonsterData& Monster)
	:m_pRootNode()
	, isRun(true)
	, m_Monster(Monster)
	, m_fWaitTime(0.0f)
	, m_fDieTime(0.0f)
	, m_fRespawnTime(0.0f)
{

};
