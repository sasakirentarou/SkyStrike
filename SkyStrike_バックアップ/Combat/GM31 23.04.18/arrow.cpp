#include "main.h"
#include "renderer.h"
#include "arrow.h"
#include "enemyJet.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "input.h"

void Arrow::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\boxTexturePS.cso");

	m_Model = new Model();
	m_Model->Load("asset\\model\\arrow\\arrow.obj");


	GameObject::Init();
}

void Arrow::Uninit()
{
	m_Model->Unload();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Arrow::Update()
{
}

void Arrow::Draw()
{
	Scene* scene = Manager::GetScene();
	auto player = scene->GetGameObject<Player>();
	auto enemys = scene->GetGameObjects<EnemyJet>();
	m_Position = player->GetPosition() + player->GetTopQuaternion() * 4.0f;

	if(!player->PlayerView(60,10000) && !enemys.empty())
	{
		for (EnemyJet* enemy : enemys)
		{
			if (enemy->GetEnemyID() == player->GetLockEnemy())
			{
				m_Target = enemy->GetPosition();
			}
		}
		//�v�Z����
		D3DXVECTOR3 forward = GetForwardQuaternion();
		D3DXVECTOR3 gaiseki;

		D3DXVECTOR3 direction = m_Target - m_Position;//�@���x�N�g��
		float vec = D3DXVec3Length(&direction);
		D3DXVec3Normalize(&direction, &direction);//�@���𐳋K��

		D3DXVec3Cross(&gaiseki, &direction, &forward);//�@���ƑO�x�N�g���̊O�ς����߂�
		float length = D3DXVec3Length(&gaiseki);//�O�ς̒��������߂�
		D3DXVec3Normalize(&gaiseki, &gaiseki);//�O�ς𐳋K��

		float angle = asinf(length);//�O�ς̒������A�[�N�T�C���Ōv�Z

		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &gaiseki, -angle);//�O�ς����Ɋp�x����
		D3DXQUATERNION q = m_Quaternion * quat;
		D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &q, 1.0f);

		vec /= 1000;
		if (vec < 0.3f)
		{
			vec = 0.3f;
		}
		else if (vec > 1.3f)
		{
			vec = 1.3f;
		}


		Renderer::SetDepthEnable(false);

		// ���̓��C�A�E�g�ݒ�
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		// �V�F�[�_�[�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		// �}�g���N�X�ݒ�
		m_Pearent = player->GetMatrix();
		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, m_Scale.x * 0.4f, m_Scale.y * 0.4f, m_Scale.z * vec);
		D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
		D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
		world = scale * rot * trans;
		Renderer::SetWorldMatrix(&world);

		PARAMETER param;
		param.boxColor = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		Renderer::SetParameter(param);

		Renderer::SetATCEnable(true);
		m_Model->Draw();
		Renderer::SetATCEnable(false);

		Renderer::SetDepthEnable(true);
	}
	GameObject::Draw();
}