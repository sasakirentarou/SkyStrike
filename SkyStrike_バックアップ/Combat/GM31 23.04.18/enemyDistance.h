#pragma once

#include "gameObject.h"

class EnemyDistance : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	int m_Count{};

	float PosX = 2000.0f;
	float PosY = 300.0f;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
	void PrintCount(D3DXVECTOR2 pos, int Count)
	{
		m_Count = Count;
		PosX = pos.x;
		PosY = pos.y;
	}
};