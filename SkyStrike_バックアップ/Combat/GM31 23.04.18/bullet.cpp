#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "bullet.h"
#include "enemyJet.h"
#include "explosion.h"
#include "cross.h"
#include "textureManager.h"
#include "hitBullet.h"
#include "jet.h"

Model* Bullet::m_Model{};

void Bullet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\jetbullet.obj");
}

void Bullet::Unload()
{
	m_Model->Unload();
	delete m_Model;
}



void Bullet::Init()
{
	m_Scale = D3DXVECTOR3(0.08f, 0.08f, 0.08f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	GameObject::Init();
}

void Bullet::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Bullet::Update()
{
	Scene* scene = Manager::GetScene();
	Cross* cross = scene->GetGameObject<Cross>();
	TextureManager* texture = scene->GetGameObject<TextureManager>();
	Jet* jet = scene->GetGameObject<Jet>();
	auto enemys = scene->GetGameObjects<EnemyJet>();

	if(cross->AutoRange())
	{
		//�U���v�Z
		D3DXVECTOR3 forward = GetForwardQuaternion();
		D3DXVECTOR3 gaiseki, enemypos;

		for (EnemyJet* enemy : enemys)
		{
			if (enemy->GetEnemyID() == jet->GetLockOnSm()->GetLockEnemyID())
			{
				enemypos = enemy->GetPosition();
			}
		}

		D3DXVECTOR3 direction = enemypos - m_Position;//�@���x�N�g��
		D3DXVec3Normalize(&direction, &direction);//�@���𐳋K��

		D3DXVec3Cross(&gaiseki, &direction, &forward);//�@���ƑO�x�N�g���̊O�ς����߂�
		float length = D3DXVec3Length(&gaiseki);//�O�ς̒��������߂�
		D3DXVec3Normalize(&gaiseki, &gaiseki);//�O�ς𐳋K��

		float angle = asinf(length);//�O�ς̒������A�[�N�T�C���Ōv�Z

		if (angle >= 1.0f)
		{
			SetDestroy();
		}

		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &gaiseki, -angle);//�O�ς����Ɋp�x����
		D3DXQUATERNION q = m_Quaternion * quat;
		D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &q, 1.0f);

		m_Count = 0;
	}
	else
	{
		m_Count++;
		if (m_Count >= 60 * 3)
		{
			// ����
			SetDestroy();
			m_Count = 0;
		}
	}

	//-0.02����0.02�̊Ԃ������_���Ŋi�[
	float randomRight = float(rand()) / RAND_MAX * 0.02f - 0.01f;
	int randomForward = rand() % 11 + 5;
		
	//���E�Ƀ����_���ɑ��x��^���Ă΂������
	m_Velocity += GetForwardQuaternion() * randomForward + GetRightQuaternion() * randomRight;
	
	//��C��R
	m_Velocity += m_Velocity * -0.03f;
	m_Position += m_Velocity;

	for (EnemyJet* enemy : enemys)
	{
		D3DXVECTOR3 enemypos = enemy->GetPosition();
		D3DXVECTOR3 direction = m_Position - enemypos;
		float length = D3DXVec3Length(&direction);

		if (length < 10.0f)
		{
			auto hit = scene->AddGameObject<HitBullet>(1);
			hit->SetPosition(m_Position);
			hit->SetScale(D3DXVECTOR3(2.0f, 2.0f,0.0f));
			hit->AnimTime(1);

			enemy->HealthDamage(5);//�G�̗̑͂����炷
			texture->SetHitReportFlg(true);
			SetDestroy();
			return;
		}
	}
}

void Bullet::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x * 1.5f, m_Scale.y * 1.5f, m_Scale.z * 2.0f);
	D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y - 0.5f, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

	GameObject::Draw();
}