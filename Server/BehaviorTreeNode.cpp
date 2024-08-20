#include"Netstd.h"
#include "BehaviorTreeNode.h"

ReturnCode SelectorNode::Tick()
{
    int icur = GetCurrentChild();
    int iListSize = GetChildList().size();

    for (int iSize = icur; iSize < iListSize; ++iSize)
    {
        ReturnCode State = GetChild(iSize)->Tick();
        SetCurrentChild(iSize);

        //�̹� ƽ���� �׼��� �����߰� ������ �ʾҴٸ� 
        //���� ƽ���� �̾ �����ϱ�����
        if (State == ReturnCode::RUNNING)
        {
            return ReturnCode::RUNNING;
        }
        //�������� �ڽ��� ���������� �����ٸ� ���� ƽ���� �������� ù ��° ���� ����
        else if (State == ReturnCode::SUCCESS)
        {
            SetCurrentChild(0);
            return ReturnCode::SUCCESS;

        }

    }
    //�����ص� ó������ ���ư���
    SetCurrentChild(0);
    return ReturnCode::FAILURE;

}

ReturnCode SequenceNode::Tick()
{
    int icur = GetCurrentChild();
    int iListSize = GetChildList().size();

    for (int iSize = icur; iSize < iListSize; ++iSize)
    {
        ReturnCode State = GetChild(iSize)->Tick();
        SetCurrentChild(iSize);

        //�̹� ƽ���� �׼��� �����߰� ������ �ʾҴٸ� 
        //���� ƽ���� �̾ �����ϱ�����
        if (State == ReturnCode::RUNNING)
        {
            return ReturnCode::RUNNING;
        }
        //������ �ڽ��� �����üũ�� ���� �� ��� �������� ����ó��
        //������ �������� ������ 0�� �ڽĺ��� �����ϱ� ����
        else if (State == ReturnCode::FAILURE)
        {
            SetCurrentChild(0);
            return ReturnCode::FAILURE;

        }
    }
    SetCurrentChild(0);
    return ReturnCode::SUCCESS;
}

ReturnCode DecoratorNode::Tick()
{
    return m_decorateFunc();
}

ReturnCode ActionNode::Tick()
{
    return m_actionFunc(GetTree());
}
