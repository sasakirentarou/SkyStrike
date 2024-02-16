#pragma once

#include "gameObject.h"

class LoadingLogo : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	int m_Count{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};