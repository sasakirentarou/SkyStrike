#pragma once

#include "gameObject.h"
#include "model.h"

class Flare : public GameObject
{
private:
	static Model* m_Model;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	float m_BulletTime{};
	float m_Period = 2.0f;
	int m_Count{};
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();
};