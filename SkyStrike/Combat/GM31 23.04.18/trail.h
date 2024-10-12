#pragma once

#include "gameObject.h"
#include <queue>

//íËêîíËã`
#define VERTEX_NUMBER	600

//ìßñæìx
#define END_UP_POINT	50

class Trail : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};
	ID3D11ShaderResourceView* m_TextureNormal{};
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	VERTEX_3D					m_Vertex[VERTEX_NUMBER * 4]{};
	
	std::queue<D3DXVECTOR3>	m_TopVertexArray;
	std::queue<D3DXVECTOR3>	m_BottomVertexArray;
	std::queue<D3DXVECTOR3> m_Topcopy;
	std::queue<D3DXVECTOR3> m_Bottomcopy;

	D3DXVECTOR3 m_Top{};
	D3DXVECTOR3 m_Bottom{};
	D3DXMATRIX m_Pearent{};

	float m_Alpha = 0.5f;
	float m_Random{};
	int m_Time{};
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetVertexPos(D3DXVECTOR3 top,D3DXVECTOR3 bottom)
	{
		m_Top = top;
		m_Bottom = bottom;
	}
};