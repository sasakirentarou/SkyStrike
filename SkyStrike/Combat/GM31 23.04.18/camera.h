#pragma once

#include "gameObject.h"

#define MAX_FOV			1.3		//最大視野
#define MIN_FOV			1.1		//最小視野
#define FOV_SPEED		0.002	//視野角変化速度

#define HOLD_BUTTON		20		//ターゲットカメラ長押し時間


//StickCamera
#define MAX_LEFT_DIFF		15	//スティックカメラ左最大値
#define MAX_RIGHT_DIFF		20	//スティックカメラ右最大値
#define MAX_FORWARD_DIFF	18	//スティックカメラ前最大値

class Camera : public GameObject
{
private:
	D3DXVECTOR3 m_UpCamera{};

	D3DXVECTOR3 m_Target{};
	D3DXMATRIX m_ViewMatrix{};
	D3DXVECTOR3 enemypos{};
	D3DXMATRIX m_ScreenMatrix{};

	class Scene* m_Scene{};
	class Jet* m_Jet{};

	//砂色：0.9f, 0.8f, 0.5f
	D3DXCOLOR m_FogColor = (1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR m_GroundFogColor = (1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR m_SkyColor = (0.2f, 0.3f, 0.4f, 1.0f);

	float m_FogStart;
	float m_FogEnd;
	float m_FogHeight;


	float m_ShakeAmplitude{};
	float m_ShakeSpeed{};
	float m_ShakeOffsetX{};
	float m_ShakeOffsetY{};
	int	  m_ShakeTime{};

	float m_Count{};
	float m_Hweel{};

	float m_FovSpeed = 0.002f;
	float m_Fov = MIN_FOV;//変動視野角

	bool m_UpMode{};
	bool m_StopVibFlg{};
public:
	void Init();
	void Update();
	void Draw();
	void Debug();

	void DefaultCamera();

	void SetBomShake(D3DXVECTOR3 pos, float shortDistance, float maxDistance);
	void SetFog(float fogstart,float fogend,float fogheight,
		D3DXCOLOR fog,D3DXCOLOR groundfog, D3DXCOLOR sky);

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