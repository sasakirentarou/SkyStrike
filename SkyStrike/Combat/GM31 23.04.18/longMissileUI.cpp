#include "main.h"
#include "renderer.h"
#include "longMissileUI.h"
#include "sprite.h"
#include "jet.h"
#include "scene.h"
#include "manager.h"

float LongMissileUI::m_OffsetCountx = 200.0f;
float LongMissileUI::m_OffsetCounty{};
int LongMissileUI::m_IDCount = 0;

void LongMissileUI::Init()
{
	x1 = 0;
	y1 = 0;

	//サイズ
	x2 = 140.0f / 1.2;
	y2 = 35.0f / 1.2;


	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(x1, y1, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(x1 + x2, y1, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(x1, y1 + y2, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(x1 + x2, y1 + y2, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/longmissile.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\gaugeVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\missileGaugePS.cso");

	m_Green = 1.0f;

	m_GaugeMax = 101.0f;
	m_Gauge = 101.0f;

	m_Offsetx = m_OffsetCountx;
	m_Offsety = m_OffsetCounty;
	m_Position = D3DXVECTOR3(SCREEN_WIDTH - (m_Offsetx + UI_PLUS_POS_X), SCREEN_HEIGHT - (m_Offsety + 150.0f), 0.0f);

	m_MyID = m_IDCount;

	if (m_MyID % 2 == 1)
	{
		m_OffsetCountx -= 200.0f + (30.0f * m_MyID);
		m_OffsetCounty -= 10.0f;
	}
	else
	{
		m_OffsetCountx += 200.0f + (30.0f * m_MyID);
	}
	m_IDCount++;

	GameObject::Init();
}

void LongMissileUI::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	m_OffsetCountx = 200.0f;
	m_OffsetCounty = 0;
	m_IDCount = 0;
	GameObject::Uninit();
}

void LongMissileUI::Update()
{
	Scene* scene = Manager::GetScene();
	Jet* jet = scene->GetGameObject<Jet>();

	if (jet->GetWeaponSm()->GetSpeMissAmount() > 0)
	{
		if (m_Gauge < m_GaugeMax)
		{
			//クールタイム
			m_Gauge += MISSILE_COOLTIME_SPEED;
		}
		if (m_Gauge > m_GaugeMax)
		{
			m_Gauge = 101;
		}
	}
}

void LongMissileUI::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z - D3DX_PI / 2);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
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

	//ゲージ設定
	PARAMETER param;
	param.hitpoint.x = m_Gauge;	//現在のHP
	param.hitpoint.y = m_GaugeMax;	//最大HP
	param.baseColor = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f); //緑
	param.lostColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); //透明
	Renderer::SetParameter(param);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	if (m_DrawFlg)
		Renderer::GetDeviceContext()->Draw(4, 0);

	GameObject::Draw();
}