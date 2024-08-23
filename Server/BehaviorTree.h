#pragma once
class BranchNode;
class MonsterData;
enum ReturnCode;
class BehaviorTree
{
private:

	std::shared_ptr<BranchNode> m_pRootNode;
	bool isRun;
	MonsterData& m_Monster;
public:
	void RunTree();
	void SetRunState(bool state) { isRun = state; }
	bool GetRunState() const { return isRun; }
	std::shared_ptr<BranchNode> GetRootNode() const { return m_pRootNode; }
	void SetRootNode(std::shared_ptr<BranchNode> node) { m_pRootNode = node; };
	MonsterData& GetMonsterData() {return m_Monster;}

public:
	virtual ReturnCode ChasePlayer();
	virtual ReturnCode AttackPlayer();


public:
	virtual void Init() {};

public:
	BehaviorTree(MonsterData& Monster);
	virtual ~BehaviorTree() {};
};


//ReturnCode NormalMonsterTree::ChasePlayer()
//{
//
//	return ReturnCode::SUCCESS;
//}

