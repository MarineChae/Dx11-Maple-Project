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

        //이번 틱에서 액션이 성공했고 끝나지 않았다면 
        //다음 틱에서 이어서 실행하기위함
        if (State == ReturnCode::RUNNING)
        {
            return ReturnCode::RUNNING;
        }
        //셀렉터의 자식이 성공적으로 끝났다면 다음 틱에서 셀렉터의 첫 번째 부터 시작
        else if (State == ReturnCode::SUCCESS)
        {
            SetCurrentChild(0);
            return ReturnCode::SUCCESS;

        }

    }
    //실패해도 처음으로 돌아간다
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

        //이번 틱에서 액션이 성공했고 끝나지 않았다면 
        //다음 틱에서 이어서 실행하기위함
        if (State == ReturnCode::RUNNING)
        {
            return ReturnCode::RUNNING;
        }
        //시퀀스 자식의 컨디션체크가 실패 한 경우 시퀀스를 실패처리
        //다음에 시퀀스에 들어오면 0번 자식부터 실행하기 위함
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
