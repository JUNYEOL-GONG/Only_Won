#include "pch.h"
#include "CollisionManager.h"

#include "World.h"

#include "BoxCollider2D.h"

void CollisionManager::Initialize()
{
	// ��� �׷��� ������ ��ȸ
	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); ++i)
	{
		for (UINT j = i; j < static_cast<UINT>(GROUP_TYPE::END); ++j)
			m_CollCheckArr[i][j] = m_CollCheckArr[j][i] = false;
	}
}

void CollisionManager::Update(float deltaTime, const std::vector<GameObject*>* gameObjects)
{
	// Player, Item, Environment, UI, ..
	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); ++i)
	{
		// Player, Item, Environment, UI, ..
		// j = i�� A -> B�� �����ϰ� B -> A�� ���� �ʴ´�
		for (UINT j = i; j < static_cast<UINT>(GROUP_TYPE::END); ++j)
		{
			// i �׷�� j �׷��� �浹üũ�� �ʿ��� �������� Ȯ��
			if (m_CollCheckArr[i][j] == true && m_CollCheckArr[j][i] == true)
			{
				// i �׷�� j �׷��� ������Ʈ ����Ʈ�� �����´�
				const std::vector<GameObject*>& thisGroup  = gameObjects[i];
				const std::vector<GameObject*>& otherGroup = gameObjects[j];

				// �׷찣 �浹üũ
				CollisionGroupUpdate(thisGroup, otherGroup, deltaTime);
			}
		}
	}
}

void CollisionManager::Finalize()
{
}

void CollisionManager::EnableCollisionType(GROUP_TYPE left, GROUP_TYPE right)
{
	m_CollCheckArr[static_cast<UINT>(left)][static_cast<UINT>(right)] = true;
	m_CollCheckArr[static_cast<UINT>(right)][static_cast<UINT>(left)] = true;
}

void CollisionManager::CollisionGroupUpdate(const std::vector<GameObject*>& thisGroup, const std::vector<GameObject*>& otherGroup, float deltaTime)
{
	// thisGroup ��ȸ
	for (UINT i = 0; i < thisGroup.size(); ++i)
	{
		// thisGroup�� ��� �浹ü�� �����´�
		const std::vector<Collider2D*> thisColliders = thisGroup[i]->GetComponents<Collider2D>();
		for(Collider2D* thisCollider : thisColliders)
		{
			// �浹ü�� ã�� �������� ���� �浹ü�� �Ѿ��
			if (!thisCollider)
				continue;

			// ���� �׷��̸� �ߺ� ������ ���� j=i ���� ����
			const UINT startJ = (thisGroup == otherGroup) ? i : 0;

			// otherGroup ��ȸ
			for (UINT j = startJ; j < otherGroup.size(); ++j)
			{
				// ���� ������Ʈ������ �˻��� �ʿ䰡 ����
				if (thisGroup[i] == otherGroup[j])
					continue;

				// otherGroup�� ��� �浹ü�� �����´�
				const std::vector<Collider2D*> otherColliders = otherGroup[j]->GetComponents<Collider2D>();
				for (Collider2D* otherCollider : otherColliders)
				{
					// �浹ü�� ã�� �������� ���� �浹ü�� �Ѿ��
					if (!otherCollider)
						continue;

					/// �� �浹ü ���̵� ���� left + right
					COLLIDER_ID colliderID = {};

					// �� �浹ü�� ID�� �����´� -> left�� right�� ID�� �������� ID�� �����Ѵ� (Union : ���� �޸𸮰��� ���)
					colliderID.left_ID = thisCollider->GetID();
					colliderID.right_ID = otherCollider->GetID();

					// �浹ü�� ID�� ã�´�
					std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned long long, bool>>>> iter = m_mapCollisionInfo.find(colliderID.ID);

					// �浹 ID�� ������ ���� ����
					if (m_mapCollisionInfo.end() == iter)
					{
						// map�� �浹 ID, �浹 ���θ� ����
						m_mapCollisionInfo.insert(std::make_pair(colliderID.ID, false));

						// �浹 ID�� Iterator�� �����´�
						iter = m_mapCollisionInfo.find(colliderID.ID);
					}

					////////////////////////////////////////////////////////////////////////////////////
					//					iter->second : �� �浹ü ������ �浹 ����						  //
					////////////////////////////////////////////////////////////////////////////////////

					// �� �ݶ��̴��� �浹�� ���
					if (thisCollider->CheckIntersect(otherCollider))
					{
						// �浹 Ÿ���� Collision�� ��� �浹������ But �ϳ��� NONE�̸� �浹 X
						if ((thisCollider->m_CollisionType == COLLISION_TYPE::COLLISION && otherCollider->m_CollisionType == COLLISION_TYPE::COLLISION) &&
								(thisCollider->m_CollisionType != COLLISION_TYPE::NONE && otherCollider->m_CollisionType != COLLISION_TYPE::NONE))
						{
							// �̹� �浹�� �ִ� ���
							if (iter->second)
							{
								// IsCollision = true
								thisCollider->TurnOn_IsCollision(otherCollider);

								iter->second = true;

								thisCollider->OnCollisionStay(otherCollider);
								otherCollider->OnCollisionStay(thisCollider);
							}
							// ó�� �浹�ϴ� ���
							else
							{
								// IsCollision = true
								thisCollider->TurnOn_IsCollision(otherCollider);

								iter->second = true;

								thisCollider->OnCollisionEnter(thisCollider, otherCollider, iter);
								otherCollider->OnCollisionEnter(otherCollider, thisCollider, iter);
							}

							// A �ݶ��̴�->ProcessBlock(B �ݶ��̴�) : A �ݶ��̴��� B �ݶ��̴��� ���� "�з�����"
							thisCollider->ProcessBlock(otherCollider);

							// �÷��̾�� �и�����
							if(thisCollider->GetOwner()->GetGroupType() == GROUP_TYPE::PLAYER && otherCollider->GetOwner()->GetGroupType() == GROUP_TYPE::PLAYER)
								otherCollider->ProcessBlock(thisCollider);
						}
						// �浹 Ÿ���� Trigger�� ��� (�� �ݶ��̴��� TRIGGER�� Ʈ���� �ߵ�)
						else if ((thisCollider->m_CollisionType == COLLISION_TYPE::TRIGGER || otherCollider->m_CollisionType == COLLISION_TYPE::TRIGGER) &&
									(thisCollider->m_CollisionType != COLLISION_TYPE::NONE && otherCollider->m_CollisionType != COLLISION_TYPE::NONE))
						{
							// �̹� �浹�� �ִ� ���
							if (iter->second)
							{
								thisCollider->TurnOn_IsTrigger(otherCollider);

								iter->second = true;

								thisCollider->OnTriggerStay(otherCollider);
								otherCollider->OnTriggerStay(thisCollider);
							}
							// ó�� �浹�ϴ� ���
							else
							{
								thisCollider->TurnOn_IsTrigger(otherCollider);

								iter->second = true;

								thisCollider->OnTriggerEnter(thisCollider, otherCollider, iter);
								otherCollider->OnTriggerEnter(otherCollider, thisCollider, iter);
							}
						}
					}
					// �� �ݶ��̴��� �浹���� ���� ���
					else
					{
						// �� �� �浹 Ÿ���� Collision�� ��� + �� �� NONE�� �ƴ� ���
						if ((thisCollider->m_CollisionType == COLLISION_TYPE::COLLISION && otherCollider->m_CollisionType == COLLISION_TYPE::COLLISION) &&
								(thisCollider->m_CollisionType != COLLISION_TYPE::NONE && otherCollider->m_CollisionType != COLLISION_TYPE::NONE))
						{
							// �̹� �浹�� �־��� ���
							if (iter->second)
							{
								thisCollider->TurnOff_IsCollision(otherCollider);

								iter->second = false;

								thisCollider->OnCollisionExit(otherCollider);
								otherCollider->OnCollisionExit(thisCollider);
							}
							// ������ �浹���� �ʾҴ� ���
							else
							{
								iter->second = false;
							}
						}
						// �浹 Ÿ���� Trigger�� ���
						else if ((thisCollider->m_CollisionType == COLLISION_TYPE::TRIGGER || otherCollider->m_CollisionType == COLLISION_TYPE::TRIGGER) &&
									(thisCollider->m_CollisionType != COLLISION_TYPE::NONE && otherCollider->m_CollisionType != COLLISION_TYPE::NONE))
						{
							// �̹� �浹�� �־��� ���
							if (iter->second)
							{
								thisCollider->TurnOff_IsTrigger(otherCollider);

								iter->second = false;

								thisCollider->OnTriggerExit(otherCollider);
								otherCollider->OnTriggerExit(thisCollider);
							}
							// ������ �浹���� �ʾҴ� ���
							else
							{
								iter->second = false;
							}
						}
					}
				}
			}
		}
	}
}