#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "sprite.h"

void Polygon2D::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	AddComponet<Sprite>()->Init(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 210, 10.0f, 10.0f, "asset/texture/cross.jpg");

	GameObject::Init();
}

void Polygon2D::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Polygon2D::Update()
{
	GameObject::Update();
}

void Polygon2D::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 仮想関数なので強制的に呼び出す(基底クラスのメゾット呼び出し)
	GameObject::Draw();
}