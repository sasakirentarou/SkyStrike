#include "main.h"
#include "renderer.h"
#include "arrow.h"
#include "enemyJet.h"
#include "jet.h"
#include "scene.h"
#include "manager.h"
#include "input.h"

void Arrow::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\boxTexturePS.cso");

	m_Model = new Model();
	m_Model->Load("asset\\model\\arrow\\arrow.obj");


	GameObject::Init();
}

void Arrow::Uninit()
{
	m_Model->Unload();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Arrow::Update()
{
}

void Arrow::Draw()
{
	Scene* scene = Manager::GetScene();
	Jet* jet = scene->GetGameObject<Jet>();
	auto enemys = scene->GetGameObjects<EnemyJet>();
	m_Position = jet->GetPosition() + jet->GetTopQuaternion() * 4.0f;

	for (EnemyJet* enemy : enemys)
	{
		if (enemy->GetEnemyID() == jet->GetLockOnSm()->GetLockEnemyID())
		{
			m_Target = enemy->GetPosition();
		}
	}

	if(!View(60,10000,jet->GetPosition(), m_Target, jet->GetForwardQuaternion()) && !enemys.empty())
	{
		//計算処理
		D3DXVECTOR3 forward = GetForwardQuaternion();
		D3DXVECTOR3 gaiseki;

		D3DXVECTOR3 direction = m_Target - m_Position;//法線ベクトル
		float vec = D3DXVec3Length(&direction);
		D3DXVec3Normalize(&direction, &direction);//法線を正規化

		D3DXVec3Cross(&gaiseki, &direction, &forward);//法線と前ベクトルの外積を求める
		float length = D3DXVec3Length(&gaiseki);//外積の長さを求める
		D3DXVec3Normalize(&gaiseki, &gaiseki);//外積を正規化

		float angle = asinf(length);//外積の長さをアークサインで計算

		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &gaiseki, -angle);//外積を軸に角度分回す
		D3DXQUATERNION q = m_Quaternion * quat;
		D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &q, 1.0f);

		vec /= 1000;
		if (vec < 0.3f)
			vec = 0.3f;
		else if (vec > 1.3f)
			vec = 1.3f;


		Renderer::SetDepthEnable(false);

		// 入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		// シェーダー設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		// マトリクス設定
		m_Pearent = jet->GetMatrix();
		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, m_Scale.x * 0.4f, m_Scale.y * 0.4f, m_Scale.z * vec);
		D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
		D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
		world = scale * rot * trans;
		Renderer::SetWorldMatrix(&world);

		PARAMETER param;
		param.boxColor = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		Renderer::SetParameter(param);

		Renderer::SetATCEnable(true);
		m_Model->Draw();
		Renderer::SetATCEnable(false);

		Renderer::SetDepthEnable(true);
	}
	GameObject::Draw();
}