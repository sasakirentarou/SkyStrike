#pragma once
#include "gameObject.h"

class TextureLoad :public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	float m_Width = 100.0f;
	float m_Height = 100.0f;
	float m_DigitWidth = 40.0f;

	float m_Count{};

	int m_Digit{};
	int m_Number{};
public:
	void Init(const char* texture);
	void Uninit();
	void Update();
	void Draw(float x,float y,bool number = false);

	void SetSize(float width,float height)
	{
		m_Width = width;
		m_Height = height;
	}

	void AlphaBlinking(int time = 120)
	{
		m_Count++;
		if (m_Count < time)
		{
			m_Color.a -= 0.01f;

			if (m_Color.a < 0.4f)
				m_Color.a = 0.4f;

		}
		else if (m_Count > time && m_Count < time * 2)
		{
			m_Color.a += 0.01f;

			if (m_Color.a > 1.0f)
				m_Color.a = 1.0f;
		}
		else if (m_Count > time * 2)
		{
			m_Count = 0;
		}
	}

	void SetRGB(float red,float green,float blue)
	{
		m_Color.r = red;
		m_Color.g = green;
		m_Color.b = blue;
	}

	void SetAlpha(float alpha)
	{
		m_Color.a = alpha;
	}

	void SetNumberOption(int digit,float digitwidth,int number)
	{
		m_Digit = digit;
		m_DigitWidth = digitwidth;
		m_Number = number;
	}
};