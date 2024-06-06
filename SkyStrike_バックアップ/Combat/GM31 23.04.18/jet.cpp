#include "main.h"
#include "renderer.h"
#include "jet.h"
#include "scene.h"
#include "manager.h"
#include "trail.h"
#include "audio.h"

Model* Jet::m_Model{};

void Jet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\F-35\\F-35.obj");
}

void Jet::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Jet::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	Scene* scene = Manager::GetScene();
	//トレイル生成
	m_Trail01 = scene->AddGameObject<Trail>(1);
	m_Trail02 = scene->AddGameObject<Trail>(1);
	
	//SE
	m_PassSE = AddComponet<Audio>();
	m_PassSE->Load("asset\\audio\\pass.wav");
	m_PassSE->Volume(0.1f);
	
	//頂点取得
	m_Model->GetModelVertex("asset\\model\\F-35\\F-35.obj", m_Top);

	GameObject::Init();
}

void Jet::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Jet::Update()
{
	switch (m_Patern)
	{
	case 0:
		m_Count++;
		if (m_Count > 60 * 10 && m_Count < 60 * 15)
		{
			m_Velocity += GetForward() * 0.3f;
			m_Rotation.x -= 0.002f;
		}
		else if (m_Count > 60 * 15)
		{
			SetDestroy();
		}
		break;
	case 1:
		m_Count++;
		if (m_Count > 60 * 25 && m_Count < 60 * 30)
		{
			m_Velocity += GetForward() * 0.3f;
			m_Rotation.x -= 0.001f;
		}
		else if (m_Count > 60 * 30)
		{
			SetDestroy();
		}
		break;
	case 2:
		m_Count++;
		if (m_Count > 60 * 40 && m_Count < 60 * 45)
		{
			m_Velocity += GetForward() * 0.3f;
			m_Rotation.x -= 0.0015f;
		}
		else if (m_Count > 60 * 45)
		{
			SetDestroy();
		}
		break;

	case 3:

		m_Velocity += GetForward() * 0.3f;

		if (m_Count == 0)
		{
			m_PassSE->Play();
		}

		m_Count++;
		if (m_Count > 60 * 10)
		{
			SetDestroy();
		}
		break;

	default:
		break;
	}

	m_Position += m_Velocity;

	//トレイル
	m_Trail01->SetVertexPos(m_Position + GetRightQuaternion() * (m_Top.x * 10.0f) + GetForwardQuaternion() * -2.0f,
		m_Position + GetRightQuaternion() * ((m_Top.x * 10.0f) * 0.98f) + GetForwardQuaternion() * -2.0f);

	m_Trail02->SetVertexPos(m_Position - GetRightQuaternion() * (m_Top.x * 10.0f) + GetForwardQuaternion() * -2.0f,
		m_Position - GetRightQuaternion() * ((m_Top.x * 10.0f) * 0.98f) + GetForwardQuaternion() * -2.0f);

	GameObject::Update();
}

void Jet::Draw()
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