#pragma once

#include "gameObject.h"
#include "model.h"

class BackLeftWing : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class Scene* m_Scene{};

	Model* m_Model{};
	D3DXMATRIX m_Pearent;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};