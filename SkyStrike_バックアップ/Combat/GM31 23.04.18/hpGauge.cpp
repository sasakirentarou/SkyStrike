#include "main.h"
#include "renderer.h"
#include "hpGauge.h"
#include "sprite.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "jetUI.h"
#include "smoke.h"
#include "jet.h"

void HpGauge::Init()
{

	//�T�C�Y
	x2 = 600.0f;
	y2 = 20.0f;

	//�ʒu
	//x1 = (SCREEN_WIDTH / 2.0f) - (x2 / 2) - 30.0f; //���S
	x1 = SCREEN_WIDTH - 700.0f;
	y1 = SCREEN_HEIGHT - 120.0f;
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(x1 + 20.0f, y1, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(x1 + x2 + 20.0f, y1, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(x1, y1 + y2, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(x1 + x2, y1 + y2, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;


	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/contrail.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\gaugeVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\gaugePS.cso");

	m_Hp = HP_MAX;
	m_Beforhp = 100.0f;

	GameObject::Init();
}

void HpGauge::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void HpGauge::Update()
{
	GameObject::Update();

	Scene* scene = Manager::GetScene();
	auto jetui = scene->GetGameObject<JetUI>();
	auto jet = scene->GetGameObject<Jet>();

	if (m_Hp > 60.0f)
	{
		jetui->SetColor(0);
	}
	else if(m_Hp <= 60.0f && m_Hp > 20.0f)
	{
		jetui->SetColor(1);
	}
	else
	{
		m_Count++;
		if(m_Count > 6)
		{
			auto smoke = scene->AddGameObject<Smoke>(1);
			smoke->SetPosition(jet->GetPosition() - jet->GetForwardQuaternion() * 8.0f);
			smoke->SetScale(D3DXVECTOR3(0.2f, 0.2f, 0.0f));
			smoke->SetColor(0.0f);
			m_Count = 0;
		}

		jetui->SetColor(2);
	}


	//�Ԃ�x��Č��炷
	if (m_Hp != m_Beforhp)
	{
		m_Beforhp -= 0.5f;
	}

	//�e�X�g
	if (Input::GetKeyTrigger('5'))
	{
		m_Hp -= 10.0f;
	}
}

void HpGauge::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	PARAMETER param;
	param.hitpoint.x = m_Hp;	//���݂�HP
	param.hitpoint.y = HP_MAX;	//�ő�HP
	param.hitpoint.z = m_Beforhp;
	param.baseColor = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f); //��
	param.lostColor = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.8f); //�D:0.2f, 0.2f, 0.2f
	param.diffColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); //��
	Renderer::SetParameter(param);


	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);

	GameObject::Draw();
}