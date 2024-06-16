#include "main.h"
#include "renderer.h"
#include "missileFire.h"

Model* MissileFire::m_Model{};

void MissileFire::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\missiles\\missilefire\\missilefire.obj");
}

void MissileFire::Unload()
{
	m_Model->Unload();
	delete m_Model;
}


void MissileFire::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Scale = D3DXVECTOR3(3.5f, 3.5f, 5.0f);
	GameObject::Init();
}

void MissileFire::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void MissileFire::Update()
{
	//ブレ
	float random_x = rand() % 11 - 5;
	float random_y = rand() % 11 - 5;
	m_Position.x = random_x / 100;
	m_Position.y = random_y / 100;

	GameObject::Update();
}

void MissileFire::Draw()
{
	Renderer::SetATCEnable(true);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x + m_OffsetX, m_Position.y + m_OffsetY, m_Position.z + m_OffsetZ);
	world = scale * rot * trans * m_Pearent;
	Renderer::SetWorldMatrix(&world);

	if(m_Draw)
		m_Model->Draw();

	GameObject::Draw();
	Renderer::SetATCEnable(false);
}