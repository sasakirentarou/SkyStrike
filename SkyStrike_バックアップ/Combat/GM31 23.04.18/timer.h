#pragma once
#include "gameObject.h"

class Timer :public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	float m_Count{};

	int m_Digit{};
	int m_Number{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	int GetTime() { return m_Number; }
};