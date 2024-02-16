#include "main.h"
#include "renderer.h"
#include "hpGauge.h"
#include "sprite.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "jetUI.h"

void HpGauge::Init()
{

	//サイズ
	x2 = 600.0f;
	y2 = 20.0f;

	//位置
	//x1 = (SCREEN_WIDTH / 2.0f) - (x2 / 2) - 30.0f; //中心
	x1 = SCREEN_WIDTH - 700.0f;
	y1 = SCREEN_HEIGHT - 120.0f;
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(x1 + 20.0f, y1, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(x1 + x2 + 20.0f, y1, 0.0f);
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
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;


	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/contrail.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\gaugeVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\gaugePS.cso");

	m_hpMax = 100.0f;
	m_hp = 100.0f;
	m_beforhp = 100.0f;

	GameObject::Init();
}

void HpGauge::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void HpGauge::Update()
{
	GameObject::Update();

	Scene* scene = Manager::GetScene();
	auto jetui = scene->GetGameObject<JetUI>();

	if (m_hp > 60)
	{
		jetui->SetColor(0);
	}
	else if(m_hp <= 60 && m_hp > 20)
	{
		jetui->SetColor(1);
	}
	else
	{
		jetui->SetColor(2);
	}


	//赤を遅れて減らす
	if (m_hp != m_beforhp)
	{
		m_beforhp -= 0.5f;
	}

	//テスト
	if (Input::GetKeyTrigger('5'))
	{
		m_hp -= 10.0f;
	}
}

void HpGauge::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	PARAMETER param;
	param.hitpoint.x = m_hp;	//現在のHP
	param.hitpoint.y = m_hpMax;	//最大HP
	param.hitpoint.z = m_beforhp;
	param.baseColor = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f); //緑
	param.lostColor = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.8f); //灰:0.2f, 0.2f, 0.2f
	param.diffColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); //赤
	Renderer::SetParameter(param);


	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

	GameObject::Draw();
}