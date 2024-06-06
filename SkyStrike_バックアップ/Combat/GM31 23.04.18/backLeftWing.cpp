#include "main.h"
#include "renderer.h"
#include "backLeftWing.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "inputx.h"
#include "textureManager.h"

void BackLeftWing::Init()
{
	m_DepthEnable = true;
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\DepthShadowEnvMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\DepthShadowEnvMappingPS.cso");

	m_Model = new Model();
	m_Model->Load("asset\\model\\F-35\\backleftwing_new.obj");

	m_Scene = Manager::GetScene();

	GameObject::Init();
}

void BackLeftWing::Uninit()
{
	m_Model->Unload();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void BackLeftWing::Update()
{
	auto texture = m_Scene->GetGameObject<TextureManager>();

	float angle = 0;

	//ピッチ
	if (texture->GetPitchFlipCheck())
	{
		if (Input::GetKeyPress(VK_SHIFT))
		{
			angle = -D3DX_PI / 15;
		}
		if (Input::GetKeyPress(VK_CONTROL))
		{
			angle = D3DX_PI / 15;
		}
	}
	else
	{
		if (Input::GetKeyPress(VK_SHIFT))
		{
			angle = D3DX_PI / 15;
		}
		if (Input::GetKeyPress(VK_CONTROL))
		{
			angle = -D3DX_PI / 15;
		}
	}


	//ロール
	if (Input::GetKeyPress('A'))
	{
		angle = -D3DX_PI / 15;
	}
	if (Input::GetKeyPress('D'))
	{
		angle = D3DX_PI / 15;
	}

	//コントローラー
	if (InputX::GetThumbLeftX(0) != 0 || InputX::GetThumbLeftY(0) != 0)
	{
		if(texture->GetPitchFlipCheck())
		{
			float stick = InputX::GetThumbLeftX(0) + -InputX::GetThumbLeftY(0);
			angle = (D3DX_PI / 15) * stick;
		}
		else
		{
			float stick = InputX::GetThumbLeftX(0) + InputX::GetThumbLeftY(0);
			angle = (D3DX_PI / 15) * stick;
		}
	}

	D3DXVECTOR3 axis = GetRightQuaternion();
	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis(&quat, &axis, angle);
	D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &quat, 0.1f);


	GameObject::Update();
}

void BackLeftWing::Draw()
{
	auto player = m_Scene->GetGameObject<Player>();

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	m_Pearent = player->GetMatrix();
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x - 1.16f, m_Position.y - 0.2f, m_Position.z - 5.0f);
	world = scale * rot * trans * m_Pearent;
	Renderer::SetWorldMatrix(&world);

	Renderer::SetATCEnable(true);
	m_Model->Draw();
	Renderer::SetATCEnable(false);

	GameObject::Draw();
}