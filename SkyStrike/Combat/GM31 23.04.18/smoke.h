#pragma once

#include "gameObject.h"

class Smoke : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;

	int m_Count{};

	float m_Red = 1.0f;
	float m_Green = 1.0f;
	float m_Blue = 1.0f;
	float m_Alpha = 1.0f;

	float m_RandomRot{};
	float m_RadomScale{};

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetColor(float color)
	{
		m_Red   = color;
		m_Green = color;
		m_Blue  = color;
	}
};