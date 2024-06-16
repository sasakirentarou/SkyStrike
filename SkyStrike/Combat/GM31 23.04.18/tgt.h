#pragma once

#include "gameObject.h"

class Tgt : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	float m_Alpha = 1.0f;
	float m_ShiftPos = 10.0f;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetAlpha(float alpha) { m_Alpha = alpha; }
};