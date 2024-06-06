#pragma once

#include "gameObject.h"

#define HP_MAX 100.0f

class HpGauge : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	int  m_Count{};

	float m_Hp{};
	float m_Beforhp{};

	float x1{};
	float y1{};
	float x2{};
	float y2{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void HPMinus(float hp)
	{
		m_Hp -= hp;
	}

	float GetHP()
	{
		return m_Hp;
	}
};