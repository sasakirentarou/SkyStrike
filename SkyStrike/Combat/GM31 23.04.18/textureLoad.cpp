#include "main.h"
#include "renderer.h"
#include "textureLoad.h"

void TextureLoad::Init(const char* texture)
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), texture,
		NULL, NULL, &m_Texture, NULL);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void TextureLoad::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void TextureLoad::Update()
{
}

void TextureLoad::Draw(float x, float y,bool number)
{
	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マテリアル設定
	Renderer::SetWorldViewProjection2D();

	D3DXMATRIX world, scale, rot, trans, offset2;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if(!number)
	{
		// 頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3(x, y, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		vertex[1].Position = D3DXVECTOR3(x + m_Width, y, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		vertex[2].Position = D3DXVECTOR3(x, y + m_Height, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		vertex[3].Position = D3DXVECTOR3(x + m_Width, y + m_Height, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		//ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
	else
	{
		int count = m_Number;

		for (int i = 0; i < m_Digit; i++)
		{
			//頂点座標算出
			float vx = x - i * m_DigitWidth;
			float vy = y;

			// テクスチャ座標算出
			int number = count % 10;
			count /= 10;

			float x = number % 5/*横*/ * (1.0f / 5/*横*/);
			float y = number / 5/*横*/ * (1.0f / 2/*縦*/);

			// 頂点データ書き換え
			D3D11_MAPPED_SUBRESOURCE msr;
			Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

			vertex[0].Position = D3DXVECTOR3(vx, vy, 0.0f);
			vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[0].TexCoord = D3DXVECTOR2(x, y);

			vertex[1].Position = D3DXVECTOR3(vx + m_Width, vy, 0.0f);
			vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[1].TexCoord = D3DXVECTOR2(x + 0.2f, y);

			vertex[2].Position = D3DXVECTOR3(vx, vy + m_Height, 0.0f);
			vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[2].TexCoord = D3DXVECTOR2(x, y + 0.5f);

			vertex[3].Position = D3DXVECTOR3(vx + m_Width, vy + m_Height, 0.0f);
			vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[3].TexCoord = D3DXVECTOR2(x + 0.2f, y + 0.5f);

			Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

			// ポリゴン描画
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}
}
