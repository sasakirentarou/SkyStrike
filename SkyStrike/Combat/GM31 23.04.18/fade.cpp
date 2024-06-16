#include "main.h"
#include "renderer.h"
#include "fade.h"
#include "sprite.h"
#include "manager.h"
#include "scene.h"

void Fade::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Sprite = AddComponet<Sprite>();
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset/texture/black.png");

	GameObject::Init();
}

void Fade::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Fade::Update()
{
	GameObject::Update();

	if (!m_Fade_Flug)
	{
		m_Fade -= 0.05f;
		if (m_Fade <= 0.0f)
		{
			m_Fade = 0.0f;
		}
	}
	else
	{
		m_Fade += 0.05f;
		if (m_Fade >= 1.0f)
		{
			m_Fade = 1.0f;
			m_FadeFinish = true;
		}
	}

	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Fade));
}

void Fade::Draw()
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