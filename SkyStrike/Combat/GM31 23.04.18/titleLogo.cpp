#include "main.h"
#include "renderer.h"
#include "titleLogo.h"
#include "sprite.h"

void TitleLogo::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	AddComponet<Sprite>()->Init((SCREEN_WIDTH / 2) - 400, (SCREEN_HEIGHT / 2) - 500, 800, 400, "asset/texture/title.png");

	GameObject::Init();
}

void TitleLogo::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void TitleLogo::Update()
{
	GameObject::Update();
}

void TitleLogo::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���z�֐��Ȃ̂ŋ����I�ɌĂяo��(���N���X�̃��]�b�g�Ăяo��)
	GameObject::Draw();
}