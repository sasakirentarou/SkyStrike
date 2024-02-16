#pragma once

#include "gameObject.h"

class Cross : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	float m_Red{};
	float m_Green{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetLockColor(bool color)
	{
		if (color == true)
		{
			m_Red = 1.0f;
			m_Green = 0.0f;
		}
		else
		{
			m_Green = 1.0f;
			m_Red = 0.0f;
		}
	}
};