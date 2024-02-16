#pragma once

#include "gameObject.h"

class Explosion : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	class Audio* m_Bom{};

	int m_Count{};
	int m_TimeCount{};
	int m_Time = 0;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void BomTime(int time) { m_Time = time; }
};