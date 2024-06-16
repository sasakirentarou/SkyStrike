#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "flare.h"
#include "smoke.h"
#include "jet.h"

Model* Flare::m_Model{};

void Flare::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\sphere.obj");
}

void Flare::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Flare::Init()
{
	Scene* scene = Manager::GetScene();

	m_Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	GameObject::Init();
}

void Flare::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Flare::Update()
{
	GameObject::Update();
	Scene* scene = Manager::GetScene();
	Jet* jet = scene->GetGameObject<Jet>();

	m_Count++;
	if (m_Count > 60 * 4)
	{
		SetDestroy();
	}

	//スモーク
	Smoke* smoke = scene->AddGameObject<Smoke>(1);
	smoke->SetPosition(m_Position);
	smoke->SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	//自由落下
	{
		D3DXVECTOR3 accele = D3DXVECTOR3(0.0f, -0.5f, 0.0f);
		m_BulletTime += 0.0005f;

		m_Velocity += accele * m_BulletTime;
		m_Position += m_Velocity * m_BulletTime;
	}
}

void Flare::Draw()
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