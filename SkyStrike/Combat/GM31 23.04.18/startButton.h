#pragma once
#include"gameobject.h"

class	StartButton :public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_Count{};
	float m_Alpha{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
