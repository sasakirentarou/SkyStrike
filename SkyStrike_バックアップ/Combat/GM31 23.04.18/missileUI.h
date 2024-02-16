#pragma once

#include "gameObject.h"

class MissileUI : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	float m_Gauge{};
	float m_GaugeMax{};
	float m_Red{};
	float m_Green{};

	static float offsetx;
	float x1{};
	float y1{};
	float x2{};
	float y2{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetGauge(float gauge)
	{
		m_Gauge = gauge;
	}

	float GetGauge()
	{
		return m_Gauge;
	}
};