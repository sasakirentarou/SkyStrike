#pragma once

#include "gameObject.h"

#define MISSILE_COOLTIME_SPEED  0.5 //ÉGÉXÉRÉì 0.5
#define UI_PLUS_POS_X			20

class LongMissileUI : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	static float m_OffsetCountx;
	static float m_OffsetCounty;
	static int m_IDCount;

	float m_Gauge{};
	float m_GaugeMax{};
	float m_Red{};
	float m_Green{};

	float x1{};
	float y1{};
	float x2{};
	float y2{};
	float m_Offsetx{};
	float m_Offsety{};

	int m_MyID{};

	bool m_DrawFlg{};
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

	void SetDrawFlg(bool draw)
	{
		m_DrawFlg = draw;
	}
};