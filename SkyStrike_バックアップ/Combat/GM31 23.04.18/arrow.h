#pragma once

#include "gameObject.h"
#include "model.h"

class Arrow : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	Model* m_Model{};
	D3DXMATRIX m_Pearent{};
	D3DXVECTOR3 m_Target = D3DXVECTOR3(0.0f,0.0f,0.0f);
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};