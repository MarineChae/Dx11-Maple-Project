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
private:
	float m_fWaitTime;
	float m_fDieTime;
	float m_fRespawnTime;

public:
	virtual void Update() {};
	void RunTree();
	void SetRunState(bool state) { isRun = state; }
	bool GetRunState() const { return isRun; }
	std::shared_ptr<BranchNode> GetRootNode() const { return m_pRootNode; }
	void SetRootNode(std::shared_ptr<BranchNode> node) { m_pRootNode = node; };
	MonsterData& GetMonsterData() {return m_Monster;}
public:

	float GetWaitTime() const { return m_fWaitTime; };
	float GetDieTime() const { return m_fDieTime; };
	float GetRespawnTime() const { return m_fRespawnTime; };

	void SetWaitTime(float time) { m_fWaitTime = time; };
	void SetDieTime(float time) { m_fDieTime = time; };
	void SetRespawnTime(float time) { m_fRespawnTime = time; };




public:
	virtual ReturnCode ChasePlayer();
	virtual ReturnCode AttackPlayer();
	virtual ReturnCode Respon();
	virtual ReturnCode Skill1Cooldown();
	virtual ReturnCode Skill1();
	virtual ReturnCode Skill2Cooldown();
	virtual ReturnCode Skill2();
	virtual void DeathEvent() {};

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

