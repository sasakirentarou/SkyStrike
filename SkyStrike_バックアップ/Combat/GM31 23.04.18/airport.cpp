#include "main.h"
#include "renderer.h"
#include "airport.h"

void Airport::Init()
{
	m_DepthEnable = true;
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\DepthShadowFogMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\DepthShadowFogMappingPS.cso");

	m_Model = new Model();
	m_Model->Load("asset\\model\\airport\\airport.obj");


	GameObject::Init();
}

void Airport::Uninit()
{
	m_Model->Unload();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Airport::Update()
{
	GameObject::Update();
}

void Airport::Draw()
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

	m_Model->Draw();

	GameObject::Draw();
}