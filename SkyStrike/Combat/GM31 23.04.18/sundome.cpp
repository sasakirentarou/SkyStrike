#include "main.h"
#include "renderer.h"
#include "sundome.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "titleCamera.h"

void SunDome::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\FogVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\FogPS.cso");

	m_ReflectEnable = true;

	m_Model = new Model();
	m_Model->Load("asset\\model\\sky\\sunsky.obj");

}

void SunDome::Uninit()
{
	m_Model->Unload();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void SunDome::Update()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	if (m_GameEnable)
	{
		m_Position = camera->GetPosition();
	}
}

void SunDome::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	//IBLへ送る
	ID3D11ShaderResourceView* texture = m_Model->GetTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(4, 1, &texture);


	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y - 200.0f, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

	GameObject::Draw();
}