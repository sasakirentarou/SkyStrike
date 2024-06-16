#include "main.h"
#include "renderer.h"
#include "titleField.h"
#include "spritefield.h"

void TitleField::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\DepthShadowFogMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\DepthShadowFogMappingPS.cso");

	//滑走路
	AddComponet<SpriteField>()->Init(-20.0f, 40.0f, 0.0f, 0.0f, 300.0f, -450.0f, 0.0f, 1.0f, 0.0f, 1.0f, "asset/texture/runway.png");
	AddComponet<SpriteField>()->Init(20.0f, 400.0f, 0.0f, 0.0f, 300.0f, -450.0f, 0.0f, 1.0f, 0.0f, 1.0f, "asset/texture/runway.png");
	AddComponet<SpriteField>()->Init(-100.0f, 40.0f, 0.0f, 0.0f, 300.0f, -450.0f, 0.0f, 1.0f, 0.0f, 1.0f, "asset/texture/runway.png");
	
	//grass
	AddComponet<SpriteField>()->Init(-1500.0f, 3000.0f, -0.1f, -0.1f, 1500.0f, -3000.0f, 0.0f, 1.0f, 0.0f, 100.0f, "asset/texture/grass.jpg");

	m_DepthEnable = true;
	m_ReflectEnable = true;

	GameObject::Init();
}

void TitleField::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void TitleField::Update()
{
	GameObject::Update();
}

void TitleField::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	GameObject::Draw();
}