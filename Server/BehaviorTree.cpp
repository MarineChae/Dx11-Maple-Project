#include"Netstd.h"
#include"BehaviorTree.h"
#include"BehaviorTreeNode.h"
#include"MonsterData.h"
#include"PlayerData.h"
void BehaviorTree::RunTree()
{
	if (isRun)
		m_pRootNode->Tick();
}

ReturnCode BehaviorTree::ChasePlayer()
{
	 return ReturnCode::SUCCESS; 
}

ReturnCode BehaviorTree::AttackPlayer()
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
{
	
};
