#pragma once
#include<vector>
#include<functional>
class BehaviorTree;
enum ReturnCode
{
	FAILURE,
	SUCCESS,
	RUNNING
};

class BaseNode
{
private:
	BehaviorTree& m_tree;
public:
	
	virtual ReturnCode Tick(){	return ReturnCode::SUCCESS;}
	BehaviorTree& GetTree() { return m_tree; };
public:
	BaseNode(BehaviorTree& tree) 
	:m_tree(tree)
	{};
	virtual ~BaseNode() {};
};

class BranchNode :public BaseNode
{
private:
	int m_iCurrentChild;
	std::vector<std::shared_ptr<BaseNode>> m_vChildList;

public:

	virtual ReturnCode Tick() override { return ReturnCode::SUCCESS; };


public:
	void SetCurrentChild(int ichild) { m_iCurrentChild = ichild; }
	int  GetCurrentChild() const { return m_iCurrentChild; };

	std::vector<std::shared_ptr<BaseNode>> GetChildList() const { return m_vChildList; };
	std::shared_ptr<BaseNode> GetChild(int ichild) const { return m_vChildList[ichild]; }

	void PushChild(std::shared_ptr<BaseNode> node) { m_vChildList.push_back(node); }

public:
	BranchNode(BehaviorTree& tree)
		:BaseNode(tree)
		,m_iCurrentChild()
		, m_vChildList()
	{};
	virtual ~BranchNode() { m_vChildList.clear(); };

};

class SelectorNode :public BranchNode
{

	virtual ReturnCode Tick() override;


public:
	SelectorNode(BehaviorTree& tree)
		:BranchNode(tree)
	{};
	virtual ~SelectorNode() {};

};

class SequenceNode :public BranchNode
{

	virtual ReturnCode Tick() override;


public:
	SequenceNode(BehaviorTree& tree)
		:BranchNode(tree)
	{};
	virtual ~SequenceNode() {};

};

class DecoratorNode : public BaseNode
{
private:
	std::function<ReturnCode(BehaviorTree&)> m_decorateFunc;

public:
	virtual ReturnCode Tick() override;


public:
	DecoratorNode(BehaviorTree& tree, std::function<ReturnCode(BehaviorTree&)> func)
		:BaseNode(tree)
		,m_decorateFunc(func)
	{};
	virtual ~DecoratorNode() {};

};

class ActionNode : public BaseNode
{
private:
	std::function<ReturnCode(BehaviorTree&)> m_actionFunc;

public:
	virtual ReturnCode Tick() override;


public:
	ActionNode(BehaviorTree& tree, std::function<ReturnCode(BehaviorTree&)> func)
		:BaseNode(tree)
		, m_actionFunc(func)
	{};
	virtual ~ActionNode() {};

};
