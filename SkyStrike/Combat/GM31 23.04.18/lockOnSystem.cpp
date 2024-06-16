#include "main.h"
#include "lockOnSystem.h"

#include "scene.h"
#include "manager.h"
#include "enemyJet.h"
#include "camera.h"
#include "lock.h"
#include "lockOn.h"
#include "lockon2D.h"
#include "audio.h"

void LockOnSystem::Init()
{
	m_Scene = Manager::GetScene();

	m_LockOnSE = AddComponet<Audio>();
	m_LockOnSE->Load("asset\\audio\\lockon.wav");
	
	m_LockFinishSE = AddComponet<Audio>();
	m_LockFinishSE->Load("asset\\audio\\lockonset.wav");
}




void LockOnSystem::Update(GameObject* obj)
{
	m_EnemyJets = m_Scene->GetGameObjects<EnemyJet>();
	m_Lockon2D = m_Scene->GetGameObject<Lockon2D>();
	m_LockOn = m_Scene->GetGameObject<LockOn>();


	m_LockOnSE->Volume(Scene::m_SEVolume * (0.03f * 2));
	m_LockFinishSE->Volume(Scene::m_SEVolume * (0.03f * 2));



	for (EnemyJet* enemy : m_EnemyJets)
	{
		//UI����
		auto length = enemy->GetPosition() - obj->GetPosition();
		float vecs = D3DXVec3Length(&length);

		vecs /= 50.0f;//�T�C�Y����

		//�ŏ��T�C�Y�l
		if (vecs <= 3.0f)
		{
			vecs = 3.0f;
		}

		//UI�Z�b�g
		enemy->GetAddLock()->SetPosition(enemy->GetPosition());
		enemy->GetAddLock()->SetScale(D3DXVECTOR3(vecs, vecs, 0.0f));




		//���b�N�I������
		if (enemy->GetEnemyID() == m_EnemyID && !m_EnemyJets.empty() && !enemy->GetCrash())
		{
			m_LockPos = enemy->GetPosition();//lock����Ă���G��pos�ۑ�

			//lockon
			if (m_LockOnFlg == true)
			{

				enemy->GetAddLock()->SetLockColor(true);//�J���[��

				//���b�N�I��������
				if (m_Lockon2D->GetLockFinish())
				{
					m_LockOn->SetPosition(enemy->GetPosition());
					m_LockOn->SetAlpha(1.0f);
					m_LockOn->SetScale(D3DXVECTOR3(vecs, vecs, 0.0f));

					enemy->GetAddLock()->BlinkingFlg(false);//lock
					m_LockFinishFlg = true;
					m_LockOnSE->Stop();

					//���b�N�I������SE
					if (m_LockFinishSeFlg)
					{
						m_LockFinishSE->Play(true);
						m_LockFinishSeFlg = false;
					}
				}
				else //���b�N�I����
				{
					enemy->GetAddLock()->BlinkingFlg(true);//lock�_��
					m_LockOn->SetAlpha(0.0f); //lockOn��alpha��\��
					m_LockFinishFlg = false;
				}

			}
			else
			{

				enemy->GetAddLock()->SetLockColor(false); //�J���[��
				enemy->GetAddLock()->BlinkingFlg(true); //�_�ŕϐ����Z�b�g

				m_LockOn->SetAlpha(0.0f); //lockOn��alpha��\��

				m_LockFinishFlg = false;

			}
		}
		else
		{
			enemy->GetAddLock()->SetLockColor(false);//�J���[��
			enemy->GetAddLock()->BlinkingFlg(false);//lock�\��
		}

		//enemy�j��
		if (enemy->GetCrash())
		{
			if (m_EnemyJets.size() <= 1)
				m_LockOn->SetAlpha(0.0f);
		}
	}





	//enemy����ɂȂ�����
	if (m_EnemyJets.empty())
	{
		m_LockOn->SetAlpha(0.0f);
		m_LockFinishSE->Stop();
		m_LockOnSE->Stop();
	}





	//�v���C���[�̎��E��
	if (obj->View(80.0f, 2000.0f, obj->GetPosition(), m_LockPos, obj->GetForwardQuaternion()))
	{
		for (EnemyJet* enemy : m_EnemyJets)
		{
			//lock���̓G�����b�N�I��
			if (enemy->GetEnemyID() == m_EnemyID)
			{
				m_LockOnFlg = true;
			}
		}

		if (m_LockSeFlg)
		{
			m_LockOnSE->Play(true);
			m_LockSeFlg = false;
		}

		m_Lockon2D->LockonMove();
	}
	else
	{
		for (EnemyJet* enemy : m_EnemyJets)
		{
			//lock���̓G�����b�N�I�t
			if (enemy->GetEnemyID() == m_EnemyID)
			{
				m_LockOnFlg = false;
				m_LockOnSE->Stop();
				m_LockFinishSE->Stop();

				m_LockFinishSeFlg = true;
			}
		}
		m_LockSeFlg = true;
		m_Lockon2D->LockMoveReset();
		m_Lockon2D->SetDraw(false);
	}

}





//�^�[�Q�b�g�؂�ւ��֐�
void LockOnSystem::SwitchTarget()
{
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();
	auto camera = m_Scene->GetGameObject<Camera>();

	D3DXMATRIX screenMatrix = camera->GetScreenMatrix();
	D3DXVECTOR3 screenCameraEnemy;
	float oldDistance_1 = 1000.0f;
	float oldDistance_2 = 1000.0f;
	int keepEnemyID_1 = 0;
	int keepEnemyID_2 = 0;

	for (EnemyJet* enemy : enemys)
	{
		if (!enemy->GetCrash())
		{
			D3DXVECTOR3 enemypos = enemy->GetPosition();
			D3DXVec3TransformCoord(&screenCameraEnemy, &enemypos, &screenMatrix);//�X�N���[����̓G�ʒu���擾

			//2�����ɕϊ�
			D3DXVECTOR2 screenEnemy = D3DXVECTOR2(screenCameraEnemy.x, screenCameraEnemy.y - 0.330f);
			float distance = D3DXVec2LengthSq(&screenEnemy);

			//��ԋ������߂��GID���擾
			if (distance < oldDistance_1)
			{
				m_EnemyID = enemy->GetEnemyID();
				oldDistance_1 = distance;//������ۑ�
			}

			//if (distance < oldDistance_1)
			//{
			//	keepEnemyID_2 = keepEnemyID_1;
			//	oldDistance_2 = oldDistance_1;
			//	m_EnemyID = enemy->GetEnemyID();
			//	oldDistance_1 = distance;
			//}
			//else if (distance < oldDistance_2)
			//{
			//	keepEnemyID_2 = enemy->GetEnemyID();
			//	oldDistance_2 = distance;
			//}
		}

		//���b�N�I�����̓G���j�󂳂ꂽ�ꍇ
		if (enemy->GetEnemyID() == m_EnemyID && enemy->GetCrash())
		{
			m_Lockon2D->LockMoveReset();
		}
	}
}

void LockOnSystem::SwitchReset()
{
	m_Lockon2D->LockMoveReset();
	m_LockFinishSE->Stop();
	m_LockSeFlg = true;
	m_LockFinishSeFlg = true;
}
