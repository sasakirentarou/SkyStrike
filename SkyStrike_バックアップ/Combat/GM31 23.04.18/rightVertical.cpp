#include "main.h"
#include "renderer.h"
#include "rightVertical.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "inputx.h"

void RightVertical::Init()
{
	m_DepthEnable = true;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\DepthShadowEnvMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\DepthShadowEnvMappingPS.cso");

	m_Model = new Model();
	m_Model->Load("asset\\model\\F-35\\rightvertical_new.obj");


	GameObject::Init();
}

void RightVertical::Uninit()
{
	m_Model->Unload();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void RightVertical::Update()
{
	float angle = 0;
	if (Input::GetKeyPress('Q') || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		angle = -0.1;
	}
	else if (Input::GetKeyPress('E') || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		angle = 0.1;
	}

	D3DXVECTOR3 axis = GetTopQuaternion();
	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis(&quat, &axis, angle);
	D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &quat, 0.1f);

	GameObject::Update();
}

void RightVertical::Draw()
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
	D3DXMatrixTranslation(&trans, m_Position.x + 1.9f, m_Position.y + 1.45f, m_Position.z - 4.9f);
	world = scale * rot * trans * m_Pearent;
	Renderer::SetWorldMatrix(&world);

	Renderer::SetATCEnable(true);
	m_Model->Draw();
	Renderer::SetATCEnable(false);

	GameObject::Draw();
}