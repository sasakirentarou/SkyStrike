#pragma once

#include "gameObject.h"

class HpGauge : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	float m_hp;
	float m_hpMax;
	float m_beforhp;

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
		m_hp -= hp;
	}

	float GetHP()
	{
		return m_hp;
	}
};