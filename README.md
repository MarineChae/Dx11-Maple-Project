## MapleStory 모작 (서버,클라이언트)

* 제작기간 : 2024.06.01 ~ 2024.08.08 (타 프로젝트 약 1달간 진행)
  
* 개발환경 : C++, DirectX11

* 시연 영상 : https://youtu.be/J0OHyz_0OKE

* DownloadLink :

* 멀티스레딩을 사용한 IOCP 게임서버 및 게임클라이언트 

![Alt text](readImage/Lobby.png)
![Alt text](readImage/s1.png)

<br>

## NetWork

* Accept Thread
  - 서버에 접속하는 클라이언트들을 처리해주는 Accept Thread입니다.


<details>
<summary>Accept Thread코드샘플</summary>

```c++

bool AcceptIocp::ThreadRun()
{
	if (m_pServer == nullptr)return false;
	std::shared_ptr<Packet> pack = std::make_shared<Packet>();
	SOCKADDR_IN clientaddr;
	int addlen = sizeof(clientaddr);
	SOCKET clientsock = accept(m_pServer->GetNetWork().GetSocket(), (SOCKADDR*)&clientaddr, &addlen);
	if (clientsock == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			
			return false;
		}
	}
	else
	{

		std::shared_ptr<User> user = std::make_shared<User>(clientsock, clientaddr);
 		user->bind(m_pServer->GetIocpModel().GetIocpHandle());
		user->Recv();

		for (int iSize = 0; iSize < MAX_USER_SIZE; ++iSize)
		{
 			if (SessionMgr::GetInstance().ConnectUser(user))
			{
				break;
			}
			else
				printf("Client Connect Failed IP: %s Port:%d\n", inet_ntoa(user->GetUserAddr().sin_addr), ntohs(user->GetUserAddr().sin_port));

		}
		printf("Client Connect IP: %s Port:%d\n", inet_ntoa(user->GetUserAddr().sin_addr), ntohs(user->GetUserAddr().sin_port));

	}

	return true;
}


```
</details>


<br>

* IOCP Model
	- 비동기 I/O 작업을 마치면 큐에서 정보를 꺼내 서버에서 처리합니다.

<details>
<summary>IOCP Thread코드샘플</summary>
	
```c++
DWORD WINAPI WorkerThread(LPVOID param)
{
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* overlap;
	IocpModel* iocp = (IocpModel*)param;
	
	while (1)
	{
		if (WaitForSingleObject(iocp->GetKillEvent(), 0) == WAIT_OBJECT_0)
		{
			break;
		}
		BOOL bRet = GetQueuedCompletionStatus(iocp->GetIocpHandle(), &dwTransfer, &KeyValue, &overlap,0);
		User* pUser = (User*)KeyValue;
		if (bRet == TRUE)
		{
			
			if (pUser != nullptr)
			{
				
				pUser->Dispatch(dwTransfer,overlap);
				
			}
		}
		else
		{
			DWORD Errmsg = GetLastError();
			if (Errmsg == WAIT_TIMEOUT)
			{

				continue;
			}
			if (Errmsg == ERROR_HANDLE_EOF)
			{

				SetEvent(iocp->GetKillEvent());
			}
			if (Errmsg == ERROR_NETNAME_DELETED)
			{
				pUser->SetConnect(false);
				continue;
			}

			SetEvent(iocp->GetKillEvent());
			
			break;
		}
	}

	return 0;

}


```
</details>

<br>

* MainThread
  - Main Thread에서는 패킷들을 BroadCast와 동시에 게임의 로직을 담당합니다.

<details>
<summary>Main Thread코드샘플</summary>

```c++
bool IOCPServer::ThreadRun()
{
	Timer::GetInstance().Frame();
	static double threadtimer=0;
	threadtimer += Timer::GetInstance().GetSecPerFrame();
	if (threadtimer <= 0.0625)
	{

		return true;
	}

	threadtimer = 0;

	for (auto& scene : ServerSceneMgr::GetInstance().GetSceneList())
	{
		scene.second->Update();
	}


	for (auto& data : m_BroadcastPacketPool.GetPacketList())
	{
		
		if (!Broadcasting(data))
		{
			
		}

		//OutputDebugString(L"send\n");
	}

	m_BroadcastPacketPool.GetPacketList().clear();

	for (std::vector<std::shared_ptr<User>>::iterator iterSend = SessionMgr::GetInstance().GetUserList().begin();
		iterSend != SessionMgr::GetInstance().GetUserList().end();)
	{
		if (*iterSend == nullptr)
		{
			iterSend++;
			continue;
		}
		std::shared_ptr<User> pUser = *iterSend;
		if (iterSend->get()->IsConnected() == false)
		{
			iterSend->get()->Close();
			iterSend = SessionMgr::GetInstance().GetUserList().erase(iterSend);
		}
		else
		{
			iterSend++;
		}

	}


	return true;
}


```
</details>


<br>

* ObjectPool Pattern
   - 잦은 생성 파괴로인한 성능 저하를 방지하기 위하여 메모리를 재사용 하도록 만들었습니다.
     
<details>
<summary>ObjectPool Pattern 코드샘플</summary>

```c++

template<typename T>
class ObjectPool
{

public:
	enum
	{
		POOL_MAX_SIZE = 1024,
		POOL_SIZE_MASK = POOL_MAX_SIZE - 1,
	};

public:
	//오브젝트 풀링을 위한 연산자 오버로딩
	
	static void* operator new(size_t size)
	{
		//여러 스레드에서 접근하는것을 방지하기 위해 사용하는 매크로임 
		//원자적 동작은 경쟁 상태(race condition)를 방지
		long long pos = InterlockedIncrement64(&m_HeadPos) - 1;
		//and 연산을 사용하여 현재 헤드의 위치값을 가져옴
		long long insert = pos & POOL_SIZE_MASK;
		// insert = 0 & 7
		// insert = 0000 0000 & 0000 0111
		// insert = 0000 0000 (결과: 0)
		void* value = InterlockedExchangePointer(&m_pool[insert], nullptr);
		
		if (value != nullptr)
		{
			return value;
		}

		//16바이트 정렬방식으로 메모리를 할당
  		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);
	
	}

	static void operator delete(void* obj)
	{
		long long pos = InterlockedIncrement64(&m_TailPos) - 1;
		long long insert = pos & POOL_SIZE_MASK;
		void* value = InterlockedExchangePointer(&m_pool[insert], obj);
		if (value != nullptr)
		{
			_aligned_free(value);
		}
	
	}
	static void AllFree()
	{
		for (int iSize = 0; iSize < POOL_MAX_SIZE; ++iSize)
		{
			void* value = InterlockedExchangePointer(m_pool[iSize], nullptr);
			if (value != nullptr)
			{
				_aligned_free(value);
			}
		}
	}


private:
	static void* volatile m_pool[POOL_MAX_SIZE];
	static long long volatile m_HeadPos;
	static long long volatile m_TailPos;


};

//초기화
template <typename T>
void* volatile ObjectPool<T>::m_pool[POOL_MAX_SIZE] = {};

template <typename T>
long long volatile ObjectPool<T>::m_HeadPos(0);

template <typename T>
long long volatile ObjectPool<T>::m_TailPos(0);


```
</details>


<br>

* Data Serialization 
   - 동적 할당된 값은 그대로 저장할 수 없기 때문에 데이터를 전송하기 위해 byte로 변환해서 전송해야하기 때문에 데이터를 직렬화 시켜주었습니다.
  
<details>
<summary>Data Serialization 코드샘플</summary>

```c++

Packet& Packet::operator<<(PLAYER_STATE statevalue)
{
	PutData(reinterpret_cast<char*>(&statevalue), sizeof(PLAYER_STATE));
	return *this;
}

Packet& Packet::operator>>(PLAYER_STATE& statevalue)
{
	GetData(reinterpret_cast<char*>(&statevalue), sizeof(PLAYER_STATE));
	return *this;
}


int Packet::PutData(char* pSrc, int iSrcSize)
{
	//버퍼에 공간이 없다면
	if (m_pWritePos + iSrcSize > m_pEndPos)
		return 0;
	
	memcpy(m_pWritePos, pSrc, iSrcSize);
	m_pWritePos += iSrcSize;

	m_iDataSize += iSrcSize;

	return iSrcSize;
}

int Packet::GetData(char* pScr, int iSrcSize)
{
	if (iSrcSize > m_iDataSize)
		return 0;
	
	memcpy(pScr, m_pReadPos, iSrcSize);
	m_pReadPos += iSrcSize;
	m_iDataSize -= iSrcSize;

	return iSrcSize;
}

```
</details>


<br>

* BehaviorTree
  - 모든 몬스터들의 행동패턴을 BT를 이용하여 구현했습니다.
  - 몬스터의 로직은 서버에서 전담하여 각각의 클라이언트로 데이터를 전송합니다.
  - 몬스터의 트리는 파일을 읽어 해당되는 트리로 할당하도록 팩토리 패턴을 이용하여 구현했습니다.

   ![Alt text](readImage/2p.png)
<details>
<summary>BehaviorTree코드샘플</summary>

```c++
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

```
</details>

<details>
<summary>BossBT코드샘플</summary>

```c++
#pragma once
#include"BehaviorTree.h"
class Swoo2PhaseTree :public BehaviorTree
{
private:

	float m_fSpawnTime;
	float m_f1SkillColldown;
	float m_f2SkillColldown;
public:
	virtual void Init();
	virtual void Update()override;

public:
	virtual ReturnCode ChasePlayer() override;
	virtual ReturnCode AttackPlayer() override;
	virtual ReturnCode Skill1Cooldown()override;
	virtual ReturnCode Skill1()override;
	virtual void DeathEvent() override;


public:

	Swoo2PhaseTree(MonsterData& data);
	virtual ~Swoo2PhaseTree();


};

void Swoo2PhaseTree::Init()
{
	std::shared_ptr<SelectorNode> root = std::make_shared<SelectorNode>(*this);
	SetRootNode(root);

	/// 스킬 쓸 수 있으면 스킬먼저 사용
	std::shared_ptr<SequenceNode> Skill1seq = std::make_shared<SequenceNode>(*this);
	root->PushChild(Skill1seq);
	std::shared_ptr<DecoratorNode> coolDown = std::make_shared<DecoratorNode>(*this,&BehaviorTree::Skill1Cooldown);
	Skill1seq->PushChild(coolDown);
	std::shared_ptr<ActionNode> SKill1 = std::make_shared<ActionNode>(*this, &BehaviorTree::Skill1);
	Skill1seq->PushChild(SKill1);
	

	//추격및 공격
	std::shared_ptr<SequenceNode> ChaseAndAttack = std::make_shared<SequenceNode>(*this);
	root->PushChild(ChaseAndAttack);
	std::shared_ptr<ActionNode> chase = std::make_shared<ActionNode>(*this, &BehaviorTree::ChasePlayer);
	ChaseAndAttack->PushChild(chase);
	std::shared_ptr<ActionNode> attack = std::make_shared<ActionNode>(*this, &BehaviorTree::AttackPlayer);
	ChaseAndAttack->PushChild(attack);


	GetMonsterData().GetCollisionData().SetWidth(94.0f);

	GetMonsterData().GetCollisionData().SetHeight(108.0f);
	GetMonsterData().SetIsDead(false);
	SetRespawnTime(7777.0f);
}
```
</details>

<details>
<summary>팩토리 패턴 코드샘플</summary>

```c++

std::shared_ptr<BehaviorTree> MonsterData::CreateTree(std::string treename)
{

	static std::unordered_map<std::string, std::function<std::shared_ptr<BehaviorTree>(MonsterData& data)>> factoryMap =
	{
		 {"NormalMonsterTree", [this](MonsterData& data) { return std::make_shared<NormalMonsterTree>(data); }},
		 {"FlyingMonsterTree", [this](MonsterData& data) { return std::make_shared<FlyingMonsterTree>(data); }},
		 {"Swoo1PhaseTree", [this](MonsterData& data) { return std::make_shared<Swoo1PhaseTree>(data); }},
		 {"Swoo2PhaseTree", [this](MonsterData& data) { return std::make_shared<Swoo2PhaseTree>(data); }},
		 {"Swoo3PhaseTree", [this](MonsterData& data) { return std::make_shared<Swoo3PhaseTree>(data); }}
	};

	auto it = factoryMap.find(treename);

	if (it != factoryMap.end())
	{
		return it->second(*this);
	}
	else
	{
		return std::make_shared<NormalMonsterTree>(*this);
	}

}

```
</details>

## Client

<br>

* Collision
  - 게임에서 사용하는 충돌은 Line-Point , Line - OBB , OBB-OBB 를 구현하였습니다.
    
   ![Alt text](readImage/Linepoint.png)
<details>
<summary>Line-Point코드샘플</summary>

```c++
bool Collision::PointToLine(TVector3 point, std::shared_ptr<Line> line)
{

    //직선의 방정식
    float A = line->To.y - line->From.y;
    float B = line->From.x - line->To.x;
    float C = (line->To.x * line->From.y) - (line->From.x * line->To.y);

    float dist = std::abs(A * point.x + B * point.y + C) / std::sqrt(A * A + B * B);
    if (dist > 30 || dist < -30)
        return false;

    float minX = min(line->From.x, line->To.x);
    float maxX = max(line->From.x, line->To.x);
    float minY = min(line->From.y, line->To.y);
    float maxY = max(line->From.y, line->To.y);

    bool isWithinSegment = (point.x >= minX && point.x <= maxX) && (point.y >= minY - 50 && point.y <= maxY + 50);




    return isWithinSegment;
}
```
</details>

<br>

 ![Alt text](readImage/lope.png)
 
<details>
<summary> Line - OBB코드샘플</summary>

```c++

bool Collision::isLineIntersectingOBB(std::shared_ptr<Line> line, std::shared_ptr<Collider> coll1obb, float coefficient)
{

    // 선을 OBB의 로컬 좌표계로 변환
    TVector3 localLineStart = line->From - (coll1obb->GetTransform() - TVector3(0, coefficient, 0));
    TVector3 localLineEnd = line->To - (coll1obb->GetTransform() - TVector3(0, coefficient, 0));

    // OBB의 각 축에 대해 선의 시작점과 끝점을 투영
    float minProj = INFINITY, maxProj = -INFINITY;
    for (int i = 0; i < 2; ++i) {
        float startProj = coll1obb->GetAxis(i).Dot(localLineStart);
        float endProj = coll1obb->GetAxis(i).Dot(localLineEnd);

        float minVal = min(startProj, endProj);
        float maxVal = max(startProj, endProj);

        // 투영 결과가 OBB의 반경(extents) 내에 있는지 확인 
        if (minVal > coll1obb->GetWidth() || maxVal < -coll1obb->GetWidth()) {
            return false;
        }

        // 각 축의 투영 범위를 갱신
        minProj = min(minProj, minVal);
        maxProj = max(maxProj, maxVal);
    }

    return true;
}

```
</details>



<br>

   ![Alt text](readImage/OBBOBB.png)
   
<details>
<summary> OBB - OBB코드샘플</summary>

```c++

bool Collision::OBBCollision2D(std::shared_ptr<Collider> coll1, std::shared_ptr<Collider> coll2, TVector3 axis)
{
    float projection1 = 0;

    projection1 += std::abs((coll1->GetAxis(0).Dot(axis) * coll1->GetWidth()));
    projection1 += std::abs((coll1->GetAxis(1).Dot(axis) * coll1->GetHeight()));

    float projection2 = 0;

    projection2 += std::abs((coll2->GetAxis(0).Dot(axis) * coll2->GetWidth()));
    projection2 += std::abs((coll2->GetAxis(1).Dot(axis) * coll2->GetHeight()));

    auto a = coll1->GetTransform() - coll2->GetTransform();
    float distance = std::abs(a.Dot(axis));

    return distance <= projection1 + projection2;

}

```
</details>

