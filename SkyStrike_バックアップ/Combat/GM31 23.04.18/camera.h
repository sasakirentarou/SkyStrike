#pragma once

#include "gameObject.h"

class Camera : public GameObject
{
private:
	D3DXVECTOR3 m_UpCamera{};

	D3DXVECTOR3 m_Target{};
	D3DXMATRIX m_ViewMatrix{};
	D3DXVECTOR3 enemypos{};
	D3DXMATRIX m_ScreenMatrix{};

	class Scene* m_Scene{};
	class Player* m_Player{};

	float m_ShakeOffsetX{};
	float m_ShakeOffsetY{};
	int	  m_ShakeTime{};

	float m_Hweel{};

	float m_ShakeAmplitude{};
	float m_ShakeSpeed{};

	float m_Count{};

	float m_MaxFov = 1.3f;//Å‘å‹–ìŠp
	float m_MinFov = 1.1f;//Å¬‹–ìŠp
	float m_FovSpeed = 0.002f;
	float m_Fov = m_MinFov;//•Ï“®‹–ìŠp

	float		m_FogStart;
	float		m_FogEnd;
	float		m_FogHeight;

	bool m_UpMode{};
public:
	void Init();
	void Update();
	void Draw();

	void SetBomShake(D3DXVECTOR3 pos);

	D3DXMATRIX GetViewMatrix()
	{
		return m_ViewMatrix;
	}

	void Shake(float amplitude,float speed)
	{
		m_ShakeAmplitude = amplitude;
		m_ShakeSpeed = speed;
	}

	D3DXMATRIX GetScreenMatrix()
	{
		return m_ScreenMatrix;
	}
};