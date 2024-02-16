#include "main.h"
#include "renderer.h"
#include "leftWing.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "input.h"

void LeftWing::Init()
{
	m_DepthEnable = true;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\DepthShadowEnvMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\DepthShadowEnvMappingPS.cso");

	m_Model = new Model();
	m_Model->Load("asset\\model\\F-35\\rightwing_new.obj");


	GameObject::Init();
}

void LeftWing::Uninit()
{
	m_Model->Unload();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void LeftWing::Update()
{
	//ピッチ
	float angle = 0;
	if (Input::GetKeyPress(VK_SHIFT))
	{
		angle = D3DX_PI / 18;
	}
	if (Input::GetKeyPress(VK_CONTROL))
	{
		angle = -D3DX_PI / 18;
	}
	//ロール
	if (Input::GetKeyPress('A'))
	{
		angle = D3DX_PI / 18;
	}
	if (Input::GetKeyPress('D'))
	{
		angle = -D3DX_PI / 18;
	}


	D3DXVECTOR3 axis = GetRightQuaternion();
	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis(&quat, &axis, angle);
	D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &quat, 0.1f);

	GameObject::Update();
}

void LeftWing::Draw()
{
	Scene* scene = Manager::GetScene();
	auto player = scene->GetGameObject<Player>();

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
	D3DXMatrixTranslation(&trans, m_Position.x + 5.45f, m_Position.y - 0.05f, m_Position.z - 2.25f);
	world = scale * rot * trans * m_Pearent;
	Renderer::SetWorldMatrix(&world);

	Renderer::SetATCEnable(true);
	m_Model->Draw();
	Renderer::SetATCEnable(false);

	GameObject::Draw();
}