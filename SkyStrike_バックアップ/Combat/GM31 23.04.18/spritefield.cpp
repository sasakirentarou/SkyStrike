#include "main.h"
#include "renderer.h"
#include "spritefield.h"

void SpriteField::Init(float x1, float x2, float y1, float y2, float z1, float z2, float n1, float n2, float n3, float uv, const char* image)
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(x1, y1, z1);
	vertex[0].Normal = D3DXVECTOR3(n1, n2, n3);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(x1 + x2, y1, z1);
	vertex[1].Normal = D3DXVECTOR3(n1, n2, n3);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(uv, 0.0f);

	vertex[2].Position = D3DXVECTOR3(x1, y1 + y2, z1 + z2);
	vertex[2].Normal = D3DXVECTOR3(n1, n2, n3);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, uv);

	vertex[3].Position = D3DXVECTOR3(x1 + x2, y1 + y2, z1 + z2);
	vertex[3].Normal = D3DXVECTOR3(n1, n2, n3);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(uv, uv);


	// ���_�o�b�t�@����
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

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), image,
		NULL, NULL, &m_Texture, NULL);
	assert(m_Texture);
}

void SpriteField::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}

void SpriteField::Update()
{
}

void SpriteField::Draw()
{
	Renderer::SetSamplerEnable(true);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);

	Renderer::SetSamplerEnable(false);
}
