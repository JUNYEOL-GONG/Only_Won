#include "framework.h"
#include "TrainSpawner.h"

#include <locale>

#include "Train.h"
#include "TrainPattern.h"

#include "../Engine/DebugRenderComponent.h"
#include "../Engine/TextureComponent.h"
#include "../Engine/World.h"

#include <random>

#include "TrafficObject.h"
#include "../Engine/SoundManager.h"

TRAIN_TYPE TrainSpawner::m_TrainTypeArr[static_cast<UINT>(TRAIN_TYPE::END)] = { TRAIN_TYPE::ABC, TRAIN_TYPE::ABB, TRAIN_TYPE::ABD, TRAIN_TYPE::ABCB, TRAIN_TYPE::ABBBB };
TRAIN_DROP_TYPE TrainSpawner::m_DropTypeArr[static_cast<UINT>(TRAIN_DROP_TYPE::END)] = { TRAIN_DROP_TYPE::NONE, TRAIN_DROP_TYPE::MONEY, TRAIN_DROP_TYPE::ITEM };

TrainSpawner::TrainSpawner()
{
}

TrainSpawner::~TrainSpawner()
{
	// m_TrainPatterns 해지
	for (auto& pTrainPattern : m_TrainPatterns)
	{
		if(pTrainPattern != nullptr)
		{
			delete pTrainPattern;
			pTrainPattern = nullptr;
		}
	}

	m_TrainPatterns.clear();
}

bool TrainSpawner::Initialize()
{
	m_pDebugRenderComponent = CreateComponent<DebugRenderComponent>(GetName());

	/// 기차 패턴 생성
	TrainPattern* pTrainPattern_1 = new TrainPattern;
	TrainPattern* pTrainPattern_2 = new TrainPattern;
	TrainPattern* pTrainPattern_3 = new TrainPattern;

	pTrainPattern_1->m_PatternType = TRAIN_PATTERN_TYPE::PATTERN_1;
	pTrainPattern_2->m_PatternType = TRAIN_PATTERN_TYPE::PATTERN_2;
	pTrainPattern_3->m_PatternType = TRAIN_PATTERN_TYPE::PATTERN_3;

	m_TrainPatterns.push_back(pTrainPattern_1);
	m_TrainPatterns.push_back(pTrainPattern_2);
	m_TrainPatterns.push_back(pTrainPattern_3);

	const bool res = GameObject::Initialize();
	assert(res);

	return true;
}

void TrainSpawner::Update(const float deltaTime)
{
	// 기차 생성기가 동작 중이라면
	if (m_isRunning)
	{
		// 트리거를 위한 시간 경과
		m_ElapsedTriggerTime += deltaTime;

		// 트리거 발생
		if (m_ElapsedTriggerTime >= m_TriggerTime)
		{
			// 신호등을 위한 시간 경과
			m_ElapsedDelayTime += deltaTime;

			// 신호등 애니메이션 연출 시작
			m_pTrafficObject->StartAnimation();

			if (m_ElapsedDelayTime >= m_DelayTime)
			{
				// 스폰을 위한 시간 경과
				m_ElapsedSpawnTime += deltaTime;

				// 기차 스폰..!
				if (m_ElapsedSpawnTime >= m_SpawnTime)
				{
					m_isRunning = false;

					m_ElapsedTriggerTime = 0.f;
					m_ElapsedDelayTime = 0.f;
					m_ElapsedSpawnTime = 0.f;

					// 랜덤 클래스 사용
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_int_distribution<> triggerTimeDis(15, 25);
					std::uniform_int_distribution<> spawnTimeDis(2, 5);
					std::uniform_int_distribution<> speedDis(300, 1000);
					std::uniform_int_distribution<> posIndexDis(0, (int)((m_SpawnPositions.size() - 1)));
					std::uniform_int_distribution<> trainTypeIndexDis(0, static_cast<int>(TRAIN_TYPE::END) - 1);
					std::uniform_int_distribution<> dropTypeIndexDis(0, static_cast<int>(TRAIN_DROP_TYPE::END) - 1);

					// 랜덤값 생성
					const int randTriggerTime = triggerTimeDis(gen);
					const int randSpawnTime = spawnTimeDis(gen);
					const int randSpeed = speedDis(gen);
					const int randPosIndex = posIndexDis(gen);
					const int randTrainTypeIndex = trainTypeIndexDis(gen);
					const int randDropTypeIndex = dropTypeIndexDis(gen);

					// 기차 트리거 시간 재설정
					m_TriggerTime = static_cast<float>(randTriggerTime);

					// 기차 나오는데 걸리는 시간 재설정
					m_SpawnTime = static_cast<float>(randSpawnTime);

					// 기차 생성
					Train* trainObstacle = m_pOwnerWorld->CreateGameObject<Train>(L"TrainObstacle_Clone", GROUP_TYPE::TRAIN);

					// 기차를 스폰시켜준 생성기 설정
					trainObstacle->SetSpawner(this);

					// 기차의 스피드 설정
					trainObstacle->SetSpeed(static_cast<float>(randSpeed));

					// 기차 생성 위치 설정
					trainObstacle->SetSpawnPosition(m_SpawnPositions[randPosIndex].x, m_SpawnPositions[randPosIndex].y);

					// 기차의 타입 설정
					trainObstacle->SetTrainType(m_TrainTypeArr[randTrainTypeIndex]);

					// 기차의 드롭 타입 설정
					trainObstacle->SetDropType(m_DropTypeArr[randDropTypeIndex]);

					// 기차의 이동 방향
					if (m_SpawnPositions[randPosIndex].y < 0)
						trainObstacle->SetDirection(1);
					else if (m_SpawnPositions[randPosIndex].y > ScreenHeight)
						trainObstacle->SetDirection(-1);

					/// 기차 컴포넌트 부착 및 초기화 타이밍
					trainObstacle->Initialize();

					// 기차의 이미지 반전 설정
					if (m_SpawnPositions[randPosIndex].y < 0)
						dynamic_cast<TextureComponent*>(trainObstacle->GetRootComponent())->SetFlipY(true);
					else if (m_SpawnPositions[randPosIndex].y > ScreenHeight)
						dynamic_cast<TextureComponent*>(trainObstacle->GetRootComponent())->SetFlipY(false);
				}
			}
		}
	}

	// 컴포넌트 업데이트
	GameObject::Update(deltaTime);
}