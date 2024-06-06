#pragma once

#include "gameObject.h"

class Field : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11ShaderResourceView* m_Texture = nullptr;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};