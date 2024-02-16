#include "main.h"
#include "renderer.h"
#include "loadingLogo.h"
#include "sprite.h"

void LoadingLogo::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	AddComponet<Sprite>()->Init(0.0f,0.0f, 100.0f, 100.0f, "asset/texture/load.png");

	GameObject::Init();
}

void LoadingLogo::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void LoadingLogo::Update()
{
	GameObject::Update();
	m_Position = D3DXVECTOR3(SCREEN_WIDTH - 170.0f, SCREEN_HEIGHT - 150.0f, 0.0f);

	m_Count++;
	if (m_Count > 5)
	{
		m_Rotation.z += 0.785f;
		m_Count = 0;
	}

}

void LoadingLogo::Draw()
{

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans, offset;
	D3DXMatrixTranslation(&offset, -50.0f, -50.0f, 0.0f);
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = offset * scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// ���z�֐��Ȃ̂ŋ����I�ɌĂяo��(���N���X�̃��]�b�g�Ăяo��)
	GameObject::Draw();
}