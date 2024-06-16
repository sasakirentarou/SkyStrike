#include "main.h"
#include "renderer.h"
#include "fire.h"
#include "jet.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "inputx.h"
#include "audio.h"

Model* Fire::m_Model{};

void Fire::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\fire\\fire.obj");
}

void Fire::Unload()
{
	m_Model->Unload();
	delete m_Model;
}


void Fire::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_EngineSE = AddComponet<Audio>();
	m_EngineSE->Load("asset\\audio\\jetengine.wav");
	m_EngineSE->Volume(0.0f);
	m_EngineSE->Play(true);

	m_Scale = D3DXVECTOR3(0.35f, 0.35f, 0.0f);
	GameObject::Init();
}

void Fire::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Fire::Update()
{
	//ブレ
	float random_x = rand() % 11 - 5;
	float random_y = rand() % 11 - 5;
	m_Position.x = random_x / 200;
	m_Position.y = random_y / 200;

	if (m_PlayerFire)
	{
		//0.0から0.05に変換
		float volume = m_Scale.z / 8;
		m_EngineSE->Volume(Scene::m_SEVolume * (volume * 2));
	}

	GameObject::Update();
}

void Fire::Draw()
{
	Scene* scene = Manager::GetScene();
	Jet* jet = scene->GetGameObject<Jet>();

	if(m_PlayerFire)
	{
		m_Pearent = jet->GetMatrix();
		m_OffsetX = 0.0f;
		m_OffsetY = -0.2f;
		m_OffsetZ = -4.5f;

		//0.0から0.4に変換
		float rt = InputX::GetRightTrigger(0) / 637.5;

		//大きさ変更
		if (Input::GetKeyPress('W') && !jet->GetEngineLock())
		{
			m_Scale.z = 0.4f;
		}
		else if (m_Scale.z <= 0.4f && InputX::GetRightTrigger(0) > 0 && !jet->GetEngineLock())
		{
			m_Scale.z = rt;
		}
		else
		{
			if (m_Scale.z >= 0.0f)
				m_Scale.z = 0.0f;
		}
	}

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

	if(m_Scale.z > 0.0f)
		m_Model->Draw();

	GameObject::Draw();
	Renderer::SetATCEnable(false);
}