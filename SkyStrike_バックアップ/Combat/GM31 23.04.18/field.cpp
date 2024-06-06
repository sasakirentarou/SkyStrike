#include "main.h"
#include "renderer.h"
#include "field.h"
#include "spritefield.h"

void Field::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\DepthShadowFogMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\DepthShadowFogMappingPS.cso");

	//AddComponet<SpriteField>()->Init(-25000.0f, 50000.0f, 0.0f, 0.0f, 25000.0f, -50000.0f, 0.0f, 1.0f, 0.0f, 100.0f , "asset/texture/water.jpg");
	AddComponet<SpriteField>()->Init(-30000.0f, 60000.0f, 0.0f, 0.0f, 30000.0f, -60000.0f, 0.0f, 1.0f, 0.0f, 5.0f, "asset/texture/mapn.png");

	m_DepthEnable = true;
	m_ReflectEnable = true;

	GameObject::Init();
}

void Field::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Field::Update()
{
	GameObject::Update();
}

void Field::Draw()
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