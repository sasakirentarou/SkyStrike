#pragma once

#include "gameObject.h"

class JetUI : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	float m_Red{};
	float m_Green{};
	float m_Blue{};

	float x1{};
	float y1{};
	float x2{};
	float y2{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	//0:óŒ 1:â©êF 2:ê‘
	void SetColor(int number)
	{
		switch (number)
		{
		case 0:
			m_Red = 0.0f;
			m_Green = 1.0f;
			m_Blue = 0.0f;
			break;

		case 1:
			m_Red = 1.0f;
			m_Green = 1.0f;
			m_Blue = 0.0f;
			break;

		case 2:
			m_Red = 1.0f;
			m_Green = 0.0f;
			m_Blue = 0.0f;
			break;

		default:
			break;
		}
	}
};