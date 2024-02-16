#pragma once

#include "gameObject.h"
#include "model.h"

class LeftVertical : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	Model* m_Model{};
	D3DXMATRIX m_Pearent;

	float x = -1.23f;
	float z = 0.81f;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};