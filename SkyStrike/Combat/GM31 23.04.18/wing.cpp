#include "main.h"
#include "wing.h"
#include <string>

#include "manager.h"


void Wing::Init(const char* fileName)
{
	m_DepthEnable = true;
	m_ReflectEnable = true;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\jetVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\jetPS.cso");

	m_Model = new Model();
	m_Model->Load(fileName);

	m_Scene = Manager::GetScene();

	GameObject::Init();

}

void Wing::Uninit()
{
	m_Model->Unload();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Wing::Update()
{
	GameObject::Update();

}

void Wing::Draw(D3DXMATRIX pearent, D3DXVECTOR3 offset)
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, offset.x, offset.y, offset.z);
	world = scale * rot * trans * pearent;
	Renderer::SetWorldMatrix(&world);

	Renderer::SetATCEnable(true);
	m_Model->Draw();
	Renderer::SetATCEnable(false);

	GameObject::Draw();
}


//可動
void Wing::Movable(D3DXVECTOR3 axis, float movable,float maxRange)
{
	float angle = 0;

	angle = maxRange * movable;

	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis(&quat, &axis, angle);
	D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &quat, 0.1f);
}