#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "cross.h"
#include "spritefield.h"
#include "jet.h"
#include "enemyJet.h"
#include "lockOnSystem.h"

void Cross::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;   // 変更
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;   // 変更

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), "asset/texture/cross.jpg",
		NULL, NULL, &m_Texture, NULL);
	assert(m_Texture);

	m_Green = 1.0f;

	m_Scene = Manager::GetScene();

	GameObject::Init();
}

void Cross::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Cross::Update()
{
	m_Jet = m_Scene->GetGameObject<Jet>();

	if (AutoRange())
	{
		m_Red = 1.0f;
		m_Green = 0.0f;
	}
	else
	{
		m_Green = 1.0f;
		m_Red = 0.0f;
	}

	//Jetに付ける
	D3DXVECTOR3 pos = m_Jet->GetPosition();
	D3DXVec3Lerp(&m_Position, &m_Position, &pos, 0.25f);
	m_Position += m_Jet->LocalVector(m_Jet->GetQuaternion(), D3DXVECTOR3(0.0f, 0.5f, 15.0f));

	GameObject::Update();
}

void Cross::Draw()
{
	//手前(深度オフ)
	Renderer::SetDepthEnable(false);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// カメラのビューマトリクス取得
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXMATRIX view = camera->GetViewMatrix();

	// ビューの逆行列
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);   // 逆行列
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(&world);


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(m_Red, m_Green, 0.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);


	GameObject::Draw();

	//深度を戻す
	Renderer::SetDepthEnable(true);
}

bool Cross::AutoRange()
{
	Camera* camera = m_Scene->GetGameObject<Camera>();
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();


	//オートエイム機能
	D3DXMATRIX screenMatrix = camera->GetScreenMatrix();

	D3DXVECTOR3 enemypos;
	D3DXVECTOR3 screenCameraEnemy;


	for (EnemyJet* enemy : enemys)
	{
		if (enemy->GetEnemyID() == m_Jet->GetLockOnSm()->GetLockEnemyID())
		{
			enemypos = enemy->GetPosition();
			D3DXVec3TransformCoord(&screenCameraEnemy, &enemypos, &screenMatrix);
		}
	}

	//クロスヘアの2Dposを取得
	D3DXVECTOR3 screenCameraCross;
	D3DXVec3TransformCoord(&screenCameraCross, &m_Position, &screenMatrix);

	//端の最大値が1なので端の値をスクリーン座標の半分の値に変換
	float CEx = (SCREEN_WIDTH / 2) * screenCameraEnemy.x;
	float CEy = (SCREEN_HEIGHT / 2) * screenCameraEnemy.y;
	float CCx = (SCREEN_WIDTH / 2) * screenCameraCross.x;
	float CCy = (SCREEN_HEIGHT / 2) * screenCameraCross.y;

	//クロスヘアを起点にAUTO_RANGEの内の範囲を指定
	if (CCx + AUTO_RANGE > CEx && CCx - AUTO_RANGE < CEx &&
		CCy + AUTO_RANGE > CEy && CCy - AUTO_RANGE < CEy)
		return true;


	return false;
}
