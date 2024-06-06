#include "main.h"
#include "renderer.h"
#include "enemy.h"
#include "lock.h"
#include "lockOn.h"
#include "scene.h"
#include "manager.h"
#include "player.h"
#include "explosion.h"
#include "missile.h"
#include "smoke.h"
#include "trail.h"
#include "enemyDistance.h"
#include "textureManager.h"
#include "audio.h"
#include "camera.h"

int Enemy::m_EnemyCount = 0;
Model* Enemy::m_Model{};

void Enemy::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\F-15\\F-15.obj");
}

void Enemy::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Enemy::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\DepthShadowMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\DepthShadowMappingPS.cso");

	m_DepthEnable = true;
	m_ReflectEnable = true;


	m_KillSE = AddComponet<Audio>();
	m_KillSE->Load("asset\\audio\\kill01.wav");
	m_KillSE->Volume(0.2f);

	m_Scene = Manager::GetScene();
	m_Player = m_Scene->GetGameObject<Player>();

	//UI�\��
	m_Lock = m_Scene->AddGameObject<Lock>(1);

	m_Health = MAX_HEALTH;
	m_OldHealth = m_Health;

	m_EnemyID = m_EnemyCount;
	m_EnemyCount++;

	//���_�擾
	m_Model->GetModelVertex("asset\\model\\F-15\\F-15.obj", m_Top);

	//�g���C������
	m_Trail01 = m_Scene->AddGameObject<Trail>(1);
	m_Trail02 = m_Scene->AddGameObject<Trail>(1);

	// 0����200�܂ł͈̔͂�10�𑫂��Ē���
	m_RandomTracking = std::rand() % 201 + 10;

	GameObject::Init();
}

void Enemy::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Enemy::Update()
{
	auto camera = m_Scene->GetGameObject<Camera>();
	auto texture = m_Scene->GetGameObject<TextureManager>();


	//�̗͂��s������
	if (m_Health <= 0)
	{
		if (m_BomCount == 0)
		{
			//UI�폜
			m_Lock->SetDestroy();
			m_CrashFlg = true;
			m_Player->SwitchTarget();
			m_Player->GetLockOn()->SetAlpha(0.0f);
			texture->SetDesReportFlg(true);
			texture->SetEnemyMinus();
			m_KillSE->Play(false);

			//�J�����̗h��
			camera->SetBomShake(m_Position);

			auto explosion = m_Scene->AddGameObject<Explosion>(1);
			explosion->SetScale(D3DXVECTOR3(6.0f, 6.0f, 0.0f));
			explosion->SetPosition(m_Position);
			explosion->BomTime(1);

			m_Trail01->SetDestroy();
			m_Trail02->SetDestroy();
		}


		//���[����]
		D3DXVECTOR3 axis = GetForwardQuaternion();
		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &axis, 0.05f);
		m_Quaternion *= quat;

		//�X���[�N
		m_SmokeCount++;
		if (m_SmokeCount > 5)
		{
			Smoke* smoke = m_Scene->AddGameObject<Smoke>(1);
			smoke->SetPosition(m_Position + GetForwardQuaternion() * -2.0f);
			smoke->SetScale(D3DXVECTOR3(5.0f, 5.0f, 5.0f));
			smoke->SetColor(0.3f);
			m_SmokeCount = 0;
		}

		m_BomCount++;
		if (m_BomCount >= 60 * 5)
		{
			auto explosion = m_Scene->AddGameObject<Explosion>(1);
			explosion->SetScale(D3DXVECTOR3(6.0f, 6.0f, 0.0f));
			explosion->SetPosition(m_Position);
			explosion->BomTime(1);

			//�J�����̗h��
			camera->SetBomShake(m_Position);

			SetDestroy();
		}
	}
	else
	{
		//�X�e�[�g�}�V��
		switch (m_EnemyState)
		{
		case ENEMY_ATTACK:
			UpdateAttack();
			break;

		case ENEMY_ESCAPE:
			UpdateEscape();
			break;

		case ENEMY_SEACH:
			UpdateSeach();
			break;

		case ENAMY_GROUNDRISK:
			UpdateGroundRisk();
			break;

		default:
			break;
		}


		//�n�ʂɓ�����Ȃ��悤�ɂ���
		m_RiskPoint = m_Position + GetForwardQuaternion() * 500.0f;
		if (m_RiskPoint.y < 0.0f)
		{
			m_StateKeep = m_EnemyState;

			if (m_StateKeep == ENAMY_GROUNDRISK)
				m_StateKeep = ENEMY_SEACH;

			m_EnemyState = ENAMY_GROUNDRISK;
		}

		float speed = D3DXVec3Length(&m_Velocity) * 1000.0f;

		//�g���C��
		if (speed > 1300.0f)
		{
			//�g���C���ݒ�
			m_Trail01->SetVertexPos(m_Position + GetRightQuaternion() * m_Top.x + GetForwardQuaternion() * -2.0f,
				m_Position + GetRightQuaternion() * (m_Top.x * 0.98f) + GetForwardQuaternion() * -2.0f);

			m_Trail02->SetVertexPos(m_Position - GetRightQuaternion() * m_Top.x + GetForwardQuaternion() * -2.0f,
				m_Position - GetRightQuaternion() * (m_Top.x * 0.98f) + GetForwardQuaternion() * -2.0f);
		}
	}

	//�O�i
	m_Velocity += GetForwardQuaternion() * 0.05f;

	//��C��R
	m_Velocity += m_Velocity * -0.02f;

	//�ړ�����
	m_Position += m_Velocity;

	//ImGui�\��
	//ImGui::Begin("Enemy");
	//ImGui::InputFloat3("pos", m_Position);
	//ImGui::InputFloat4("qua", m_Quaternion);
	//ImGui::InputFloat3("vel", m_Velocity);
	//ImGui::InputInt("HP", &m_Health);
	//ImGui::End();

	GameObject::Update();
}

void Enemy::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	m_WorldMatrix = scale * rot * trans;
	Renderer::SetWorldMatrix(&m_WorldMatrix);

	m_Model->Draw();

	GameObject::Draw();
}

//���E�֐�
bool Enemy::EnemyView(D3DXVECTOR3 forward,float fieldOfViewRadians, float viewDistancee)
{
	// ����͈͓����ǂ����̔���
	D3DXVECTOR3 normalizedDirection;
	D3DXVECTOR3 direction = m_Player->GetPosition() - m_Position;
	D3DXVec3Normalize(&normalizedDirection, &direction);
	D3DXVECTOR3 houkou = forward;
	float dotProduct = D3DXVec3Dot(&houkou, &normalizedDirection);
	float angle = acos(dotProduct);
	fieldOfViewRadians = D3DXToRadian(fieldOfViewRadians);
	bool isInFieldOfView = angle <= fieldOfViewRadians / 2.0f;

	// ���싗�������ǂ����̔���
	D3DXVECTOR3 dice = m_Position - m_Player->GetPosition();
	float distance = D3DXVec3Length(&dice);
	bool isInViewDistance = distance <= viewDistancee;

	return isInFieldOfView && isInViewDistance;
}

//�U���֐�
void Enemy::EnemyHoming(D3DXVECTOR3 target, bool reverse)
{
	m_QuaAmountCount++;
	if(m_QuaAmountCount > ROTATION_FRAME)
	{
		//�U���v�Z
		D3DXVECTOR3 gaiseki;
		D3DXVECTOR3 forward = GetForwardQuaternion();

		D3DXVECTOR3 direction = target - m_Position;//�@���x�N�g��
		D3DXVec3Normalize(&direction, &direction);//�@���𐳋K��

		D3DXVec3Cross(&gaiseki, &direction, &forward);//�@���ƑO�x�N�g���̊O�ς����߂�
		float length = D3DXVec3Length(&gaiseki);//�O�ς̒��������߂�

		float angle = asinf(length);//�O�ς̒������A�[�N�T�C���Ōv�Z

		if (angle > 1.0f)
		{
			m_Velocity += GetForwardQuaternion() * 0.03f;
		}

		if (!reverse)
		{
			angle *= -1.0f;
		}

		if (angle != 0.0f)
		{
			D3DXVec3Normalize(&gaiseki, &gaiseki);//�O�ς𐳋K��
			D3DXQUATERNION quat;
			D3DXQuaternionRotationAxis(&quat, &gaiseki, angle);//�O�ς����Ɋp�x����
			D3DXQUATERNION q = m_Quaternion * quat;
			D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &q, ROTATION_AMOUNT);
		}
		m_QuaAmountCount = 0;
	}
}

//0:�U�� 1:�ޔ� 2:�T��
void Enemy::EnemyStateChange(int number)
{
	switch (number)
	{
	case 0:
		m_EnemyState = ENEMY_ATTACK;
		break;
	case 1:
		m_EnemyState = ENEMY_ESCAPE;
		break;
	case 2:
		m_EnemyState = ENEMY_SEACH;
		break;

	default:
		break;
	}
}


//���X�e�[�g�p�^�[����//

// �U��
void Enemy::UpdateAttack()
{
	//�v���C���[�̌��ɉ��
	EnemyHoming(m_Player->GetPosition() + m_Player->GetForwardQuaternion() * -m_RandomTracking,false);

	//�~�T�C������
	if (EnemyView(GetForwardQuaternion(), 60.0f, 2000.0f))
	{
		if (m_MissileShot)
		{
			auto missile = m_Scene->AddGameObject<Missile>(1);
			missile->SetPosition(m_Position);
			missile->SetQuaternion(m_Quaternion);
			missile->SetEnemyMissile(true);
			missile->SetPlayerTrack(true);
			m_MissileShot = false;
			m_MissileCount = 0;
		}
	}

	if (!m_MissileShot)
	{
		m_MissileCount++;
		if (m_MissileCount > 60 * MISSILE_COOLDOWN)
		{
			m_MissileShot = true;
		}
	}

	//�v���C���[���X�e���X�ɂȂ�ƒT��
	if (m_PlayerStealth)
	{
		m_EnemyState = ENEMY_SEACH;
	}

	//�_���[�W���󂯂�Ƒޔ�
	if (m_OldHealth > m_Health)
	{
		m_EnemyState = ENEMY_ESCAPE;
		m_OldHealth = m_Health;
	}
}

// �ޔ�
void Enemy::UpdateEscape()
{
	EnemyHoming(m_Player->GetPosition() + m_Player->GetForwardQuaternion() * -m_RandomTracking,true);
	m_Velocity += GetForwardQuaternion() * 0.01f;

	D3DXVECTOR3 direction = m_Player->GetPosition() - m_Position;
	float length = D3DXVec3Length(&direction);

	if (length > 100.0f)
	{
		m_EnemyState = ENEMY_SEACH;
	}
}

// �T��
void Enemy::UpdateSeach()
{
	m_SeachCount++;
	if(m_SeachCount > 60 * 3)
	{
		//-2000�`2000�̃����_��
		m_RandomSeachPoint.x = rand() % (4001) - 2000;
		m_RandomSeachPoint.y = rand() % (4001) - 2000;
		m_RandomSeachPoint.z = rand() % (4001) - 2000;
		m_SeachCount = 0;
	}
	D3DXVECTOR3 targetPos = m_Position + m_RandomSeachPoint;

	//if(targetPos.x >)

	EnemyHoming(targetPos, false);

	//����
	if (EnemyView(GetForwardQuaternion(), 90.0f, 3000.0f) && !m_PlayerStealth)
	{
		m_EnemyState = ENEMY_ATTACK;
	}
}

//�n�ʉ��
void Enemy::UpdateGroundRisk()
{
	EnemyHoming(D3DXVECTOR3(m_Position.x,1000.0f, m_Position.z) + GetForwardQuaternion() * 50.0f, false);

	if (m_RiskPoint.y > 1000.0f)
	{
		m_EnemyState = m_StateKeep;
	}
}