#pragma once

#include "gameObject.h"
#include "model.h"

class SunDome : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	Model* m_Model{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};