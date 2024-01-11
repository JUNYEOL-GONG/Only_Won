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
	// m_TrainPatterns ����
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

	/// ���� ���� ����
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
	// ���� �����Ⱑ ���� ���̶��
	if (m_isRunning)
	{
		// Ʈ���Ÿ� ���� �ð� ���
		m_ElapsedTriggerTime += deltaTime;

		// Ʈ���� �߻�
		if (m_ElapsedTriggerTime >= m_TriggerTime)
		{
			// ��ȣ���� ���� �ð� ���
			m_ElapsedDelayTime += deltaTime;

			// ��ȣ�� �ִϸ��̼� ���� ����
			m_pTrafficObject->StartAnimation();

			if (m_ElapsedDelayTime >= m_DelayTime)
			{
				// ������ ���� �ð� ���
				m_ElapsedSpawnTime += deltaTime;

				// ���� ����..!
				if (m_ElapsedSpawnTime >= m_SpawnTime)
				{
					m_isRunning = false;

					m_ElapsedTriggerTime = 0.f;
					m_ElapsedDelayTime = 0.f;
					m_ElapsedSpawnTime = 0.f;

					// ���� Ŭ���� ���
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_int_distribution<> triggerTimeDis(15, 25);
					std::uniform_int_distribution<> spawnTimeDis(2, 5);
					std::uniform_int_distribution<> speedDis(300, 1000);
					std::uniform_int_distribution<> posIndexDis(0, (int)((m_SpawnPositions.size() - 1)));
					std::uniform_int_distribution<> trainTypeIndexDis(0, static_cast<int>(TRAIN_TYPE::END) - 1);
					std::uniform_int_distribution<> dropTypeIndexDis(0, static_cast<int>(TRAIN_DROP_TYPE::END) - 1);

					// ������ ����
					const int randTriggerTime = triggerTimeDis(gen);
					const int randSpawnTime = spawnTimeDis(gen);
					const int randSpeed = speedDis(gen);
					const int randPosIndex = posIndexDis(gen);
					const int randTrainTypeIndex = trainTypeIndexDis(gen);
					const int randDropTypeIndex = dropTypeIndexDis(gen);

					// ���� Ʈ���� �ð� �缳��
					m_TriggerTime = static_cast<float>(randTriggerTime);

					// ���� �����µ� �ɸ��� �ð� �缳��
					m_SpawnTime = static_cast<float>(randSpawnTime);

					// ���� ����
					Train* trainObstacle = m_pOwnerWorld->CreateGameObject<Train>(L"TrainObstacle_Clone", GROUP_TYPE::TRAIN);

					// ������ ���������� ������ ����
					trainObstacle->SetSpawner(this);

					// ������ ���ǵ� ����
					trainObstacle->SetSpeed(static_cast<float>(randSpeed));

					// ���� ���� ��ġ ����
					trainObstacle->SetSpawnPosition(m_SpawnPositions[randPosIndex].x, m_SpawnPositions[randPosIndex].y);

					// ������ Ÿ�� ����
					trainObstacle->SetTrainType(m_TrainTypeArr[randTrainTypeIndex]);

					// ������ ��� Ÿ�� ����
					trainObstacle->SetDropType(m_DropTypeArr[randDropTypeIndex]);

					// ������ �̵� ����
					if (m_SpawnPositions[randPosIndex].y < 0)
						trainObstacle->SetDirection(1);
					else if (m_SpawnPositions[randPosIndex].y > ScreenHeight)
						trainObstacle->SetDirection(-1);

					/// ���� ������Ʈ ���� �� �ʱ�ȭ Ÿ�̹�
					trainObstacle->Initialize();

					// ������ �̹��� ���� ����
					if (m_SpawnPositions[randPosIndex].y < 0)
						dynamic_cast<TextureComponent*>(trainObstacle->GetRootComponent())->SetFlipY(true);
					else if (m_SpawnPositions[randPosIndex].y > ScreenHeight)
						dynamic_cast<TextureComponent*>(trainObstacle->GetRootComponent())->SetFlipY(false);
				}
			}
		}
	}

	// ������Ʈ ������Ʈ
	GameObject::Update(deltaTime);
}