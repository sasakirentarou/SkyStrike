#pragma once

#include "gameObject.h"

class Lock : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	bool m_AlphaFlg{};
	int m_AlphaCount{};

	float m_Red{};
	float m_Green{};
	float m_Alpha{};
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

	void BlinkingFlg(bool alpha)
	{
		m_AlphaFlg = alpha;
		
		if (!alpha)
		{
			m_Alpha = 1.0f;
		}
	}
};