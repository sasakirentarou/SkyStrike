#include "main.h"
//#include "renderer.h"
//#include "manager.h"
//#include "scene.h"
//#include "camera.h"
//#include "effectLoad.h"
//#include "audio.h"
//
//void EffectLoad::Init(const char* texture, bool SEmode, const char* se)
//{
//	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
//	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
//
//	VERTEX_3D vertex[4];
//
//	vertex[0].Position = D3DXVECTOR3(-5.0f, 5.0f, 0.0f);
//	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
//
//	vertex[1].Position = D3DXVECTOR3(5.0f, 5.0f, 0.0f);
//	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
//
//	vertex[2].Position = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);
//	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
//
//	vertex[3].Position = D3DXVECTOR3(5.0f, -5.0f, 0.0f);
//	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
//
//
//	// 頂点バッファ生成
//	D3D11_BUFFER_DESC bd;
//	ZeroMemory(&bd, sizeof(bd));
//	bd.Usage = D3D11_USAGE_DYNAMIC;   // 変更
//	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;   // 変更
//
//	D3D11_SUBRESOURCE_DATA sd;
//	ZeroMemory(&sd, sizeof(sd));
//	sd.pSysMem = vertex;
//
//	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
//
//	// テクスチャ読み込み
//	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), texture,
//		NULL, NULL, &m_Texture, NULL);
//	assert(m_Texture);
//
//	if(SEmode)
//	{
//		m_SE = AddComponet<Audio>();
//		m_SE->Load(se);
//		m_SE->Play();
//		m_SE->Volume(0.5f);
//	}
//
//	GameObject::Init();
//}
//
//void EffectLoad::Uninit()
//{
//	m_VertexBuffer->Release();
//	m_Texture->Release();
//
//	m_VertexLayout->Release();
//	m_VertexShader->Release();
//	m_PixelShader->Release();
//
//	GameObject::Uninit();
//}
//
//void EffectLoad::Draw(int imageSplit, int animationTime,
//	int imageWidth,int imageHeight,float width,float height,D3DXVECTOR3 pos)
//{
//	m_TimeCount++;
//
//	//描画速度調整
//	if (m_TimeCount > animationTime)
//	{
//		m_Count++;
//		m_TimeCount = 0;
//	}
//
//	//全部描画したら消す
//	if (m_Count >= imageSplit)//画像枚数
//	{
//		SetDestroy();
//		return;
//	}
//
//	// テクスチャ座標算出
//	float x = m_Count % imageWidth * (1.0f / imageWidth);
//	float y = m_Count / (imageHeight * 2) * (1.0f / imageHeight);
//
//	float u = 1.0f / imageWidth;
//	float v = 1.0f / imageHeight;
//
//	// 頂点データ書き換え
//	D3D11_MAPPED_SUBRESOURCE msr;
//	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
//
//	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
//
//	vertex[0].Position = D3DXVECTOR3(-width, height, 0.0f);
//	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[0].TexCoord = D3DXVECTOR2(x, y);
//
//	vertex[1].Position = D3DXVECTOR3(width, height, 0.0f);
//	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[1].TexCoord = D3DXVECTOR2(x + u, y);
//
//	vertex[2].Position = D3DXVECTOR3(-width, -height, 0.0f);
//	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[2].TexCoord = D3DXVECTOR2(x, y + v);
//
//	vertex[3].Position = D3DXVECTOR3(width, -height, 0.0f);
//	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[3].TexCoord = D3DXVECTOR2(x + u, y + v);
//
//	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
//
//	// 入力レイアウト設定
//	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
//
//	// シェーダー設定
//	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
//	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
//
//	// カメラのビューマトリクス取得
//	Scene* scene = Manager::GetScene();
//	Camera* camera = scene->GetGameObject<Camera>();
//	D3DXMATRIX view = camera->GetViewMatrix();
//
//	// ビューの逆行列
//	D3DXMATRIX invView;
//	D3DXMatrixInverse(&invView, NULL, &view);   // 逆行列
//	invView._41 = 0.0f;
//	invView._42 = 0.0f;
//	invView._43 = 0.0f;
//
//	// マトリクス設定
//	D3DXMATRIX world, scale, rot, trans;
//	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
//	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
//	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
//	world = scale * invView * trans;
//	Renderer::SetWorldMatrix(&world);
//
//	// 頂点バッファ設定
//	UINT stride = sizeof(VERTEX_3D);
//	UINT offset = 0;
//	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
//
//	// マテリアル設定
//	MATERIAL material;
//	ZeroMemory(&material, sizeof(material));
//	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//	material.TextureEnable = true;
//	Renderer::SetMaterial(material);
//
//	// テクスチャ設定
//	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
//
//	// プリミティブトポロジ設定
//	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//
//	// ポリゴン描画
//	Renderer::GetDeviceContext()->Draw(4, 0);
//
//	GameObject::Draw();
//}