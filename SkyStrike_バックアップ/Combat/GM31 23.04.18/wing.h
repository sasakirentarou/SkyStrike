#pragma once
#include "gameObject.h"
#include "model.h"


class Wing : public GameObject
{
private:
	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class Scene* m_Scene;

public:
	void Init(const char* fileName);
	void Uninit();
	void Update();
	void Draw(D3DXMATRIX pearent,D3DXVECTOR3 offset);


	void Movable(D3DXVECTOR3 axis, float movable,float maxRange);
};