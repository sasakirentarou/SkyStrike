#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "missile.h"
#include "enemy.h"
#include "explosion.h"
#include "player.h"
#include "smoke.h"
#include "hpGauge.h"
#include "camera.h"
#include "textureManager.h"
#include "audio.h"

Model* Missile::m_Model{};
Player* Missile::m_Player{};

void Missile::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\missiles.obj");
}

void Missile::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Missile::Init()
{
	m_Scene = Manager::GetScene();
	m_Player = m_Scene->GetGameObject<Player>();
	m_Camera = m_Scene->GetGameObject<Camera>();
	m_PlayerHp = m_Scene->GetGameObject<HpGauge>();

	m_Scale = D3DXVECTOR3(0.35f, 0.35f, 0.35f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	GameObject::Init();
}

void Missile::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Missile::Update()
{
	Scene* scene = Manager::GetScene();
	auto enemys = scene->GetGameObjects<Enemy>();
	auto texture = scene->GetGameObject<TextureManager>();

	//����
	if (m_ShotFlg)
	{
		//�X���[�N
		Smoke* smoke = scene->AddGameObject<Smoke>(1);
		smoke->SetPosition(m_Position + GetForwardQuaternion() * -10.0f);
		smoke->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		if (m_Count == 0 && !enemys.empty())
		{
			m_LockEnemyID = m_Player->GetMisLockID();
		}

		m_Count++;
		if (m_Count >= 60 * 2 && m_Count <= 60 * 8)
		{
			//���x�㏸
			m_Velocity += GetForwardQuaternion() * 0.2f;
		}
		else if (m_Count >= 60 * 8)
		{
			m_ShotFlg = false;
			// �����\�������
			SetDestroy();
			m_Count = 0;
		}


		//�U��
		if (m_EnemyTrackingFlg && !enemys.empty())
		{
			for (Enemy* enemy : enemys)
			{
				if (enemy->GetEnemyID() == m_LockEnemyID)
				{
					m_EnemyPos = enemy->GetPosition();
				}
			}

			//�v�Z����
			D3DXVECTOR3 forward = GetForwardQuaternion();
			D3DXVECTOR3 gaiseki;

			D3DXVECTOR3 direction = m_EnemyPos - m_Position;//�@���x�N�g��
			D3DXVec3Normalize(&direction, &direction);//�@���𐳋K��

			D3DXVec3Cross(&gaiseki, &direction, &forward);//�@���ƑO�x�N�g���̊O�ς����߂�
			float length = D3DXVec3Length(&gaiseki);//�O�ς̒��������߂�
			D3DXVec3Normalize(&gaiseki, &gaiseki);//�O�ς𐳋K��

			float angle = asinf(length);//�O�ς̒������A�[�N�T�C���Ōv�Z

			if (angle >= 1.0f)
			{
				m_EnemyTrackingFlg = false;
				texture->SetMissReportFlg(true);
			}

			D3DXQUATERNION quat;
			D3DXQuaternionRotationAxis(&quat, &gaiseki, -angle);//�O�ς����Ɋp�x����
			D3DXQUATERNION q = m_Quaternion * quat;
			D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &q, PLAYER_MISSILE_ROT);
		}

		m_Velocity += GetForwardQuaternion() * 0.5f;//���x
		m_Velocity += m_Velocity * -0.1f;//��C��R

		// enemy�Ƃ̏Փ˔���
		for (Enemy* enemy : enemys)
		{
			D3DXVECTOR3 enemypos = enemy->GetPosition();
			D3DXVECTOR3 direction = m_Position - enemypos;
			float length = D3DXVec3Length(&direction);

			if (length < 10.0f)
			{
				MisiileBom();
				enemy->HealthMinus(50);
				texture->SetHitReportFlg(true);
				return;
			}
		}


		//��Q���Ƃ̏Փ˔���
		{
			float groundHeight = 0.0f;

			//�ڒn
			if (m_Position.y < groundHeight)
			{
				MisiileBom();
			}
		}
	}

	//�G�̃~�T�C��
	if (m_EnemyMissile)
	{
		//�X���[�N
		Smoke* smoke = scene->AddGameObject<Smoke>(1);
		smoke->SetPosition(m_Position + GetForwardQuaternion() * -10.0f);
		smoke->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		m_Count++;
		if (m_Count >= 60 * 2 && m_Count <= 60 * 8)
		{
			//���x�㏸
			m_Velocity += GetForwardQuaternion() * 0.2f;
		}
		else if (m_Count >= 60 * 8)
		{
			//����
			SetDestroy();
			m_Count = 0;
		}

		if (m_PlayerTrackingFlg)
		{
			//�v�Z����
			D3DXVECTOR3 forward = GetForwardQuaternion();
			D3DXVECTOR3 playerpos = m_Player->GetPosition();
			D3DXVECTOR3 gaiseki;

			D3DXVECTOR3 direction = playerpos - m_Position;//�@���x�N�g��
			D3DXVec3Normalize(&direction, &direction);//�@���𐳋K��

			D3DXVec3Cross(&gaiseki, &direction, &forward);//�@���ƑO�x�N�g���̊O�ς����߂�
			float length = D3DXVec3Length(&gaiseki);//�O�ς̒��������߂�
			D3DXVec3Normalize(&gaiseki, &gaiseki);//�O�ς𐳋K��

			float angle = asinf(length);//�O�ς̒������A�[�N�T�C���Ōv�Z

			if (angle >= 1.0f)
			{
				m_PlayerTrackingFlg = false;
				m_Player->GetMissileAlertSE()->Stop();
			}

			if (!m_FlareFlg)
				angle *= -1.0f;
			else
				m_Player->GetMissileAlertSE()->Stop();

			D3DXQUATERNION quat;
			D3DXQuaternionRotationAxis(&quat, &gaiseki, angle);//�O�ς����Ɋp�x����
			D3DXQUATERNION q = m_Quaternion * quat;
			D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &q, ENEMY_MISSILE_ROT);//��]��
		
			texture->SetMissileAlertFlg(true);
		}
		else
		{
			texture->SetMissileAlertFlg(false);
		}

		m_Velocity += GetForwardQuaternion() * 0.5f;//���x
		m_Velocity += m_Velocity * -0.1f;//��C��R

		// player�Ƃ̏Փ˔���
		{
			D3DXVECTOR3 direction = m_Position - m_Player->GetPosition();
			float length = D3DXVec3Length(&direction);

			if (length < 10.0f)
			{
				MisiileBom();
				m_PlayerHp->HPMinus(10.0f);
				m_Camera->Shake(5.0f, 0.6f);

				if(m_PlayerTrackingFlg)
					m_Player->GetMissileAlertSE()->Stop();

				return;
			}
		}

		//��Q���Ƃ̏Փ˔���
		{
			float groundHeight = 0.0f;

			//�ڒn
			if (m_Position.y < groundHeight)
			{
				MisiileBom();
			}
		}
	}

	m_Position += m_Velocity;//�ړ�

	//ImGui�\��
	//ImGui::Begin("MISSILE");
	//ImGui::InputFloat3("ColiisionScale", m_CollisionScale);
	//ImGui::End();
}

void Missile::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

	GameObject::Draw();
}

void Missile::MisiileBom()
{
	//�J�����̗h��
	m_Camera->SetBomShake(m_Position);

	auto explosion = m_Scene->AddGameObject<Explosion>(1);
	explosion->SetScale(D3DXVECTOR3(4.5f, 4.5f, 0.0f));
	explosion->SetPosition(m_Position);
	explosion->BomTime(2);

	m_ShotFlg = false;
	// �����\�������
	SetDestroy();
}
