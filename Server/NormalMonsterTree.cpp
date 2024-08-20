#include"Netstd.h"
#include "NormalMonsterTree.h"
#include "BehaviorTreeNode.h"
#include "PlayerData.h"
#include "MonsterData.h"

void NormalMonsterTree::Init()
{
	std::shared_ptr<SequenceNode> testnode = std::make_shared<SequenceNode>(*this);
	SetRootNode(testnode);

	std::shared_ptr<ActionNode> acttest = std::make_shared<ActionNode>(*this,&BehaviorTree::ChasePlayer);
	testnode->PushChild(acttest);

}




NormalMonsterTree::NormalMonsterTree(MonsterData& data)
	:BehaviorTree(data)
{
}

NormalMonsterTree::~NormalMonsterTree()
{
}

