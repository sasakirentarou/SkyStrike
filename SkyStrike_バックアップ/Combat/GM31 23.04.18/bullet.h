#pragma once

#include "gameObject.h"
#include "model.h"

#define AUTO_RANGE 50

class Bullet : public GameObject
{
private:
	static Model* m_Model;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	int m_Count = 0;

	float m_BulletTime{};
	float m_Period = 2.0f;

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();
};