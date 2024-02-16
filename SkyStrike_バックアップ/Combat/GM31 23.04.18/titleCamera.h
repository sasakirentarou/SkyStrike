#pragma once

#include"gameobject.h"

class TitleCamera :public GameObject
{
private:
	D3DXVECTOR3 m_Target{};
	D3DXMATRIX m_ViewMatrix{};

	float m_ShakeOffsetX{};
	float m_ShakeOffsetY{};
	int	  m_ShakeTime{};
	float m_ShakeAmplitude{};
	float m_ShakeSpeed{};
	float m_ShakeCount{};

	float		m_FogStart;
	float		m_FogEnd;
	float		m_FogHeight;

	bool m_StartFlg{};
	bool m_WeaponFlg{};
public:
	void Init();
	void Update();
	void Draw();

	void SetStart(bool flg)
	{
		m_StartFlg = flg;
	}
	void SetWeapon(bool flg)
	{
		m_WeaponFlg = flg;
	}

	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
};