#pragma once

#include "gameObject.h"
#include "model.h"

#include "engine.h"
#include "controlStick.h"
#include "weaponSystem.h"
#include "lockOnSystem.h"
#include "stealthSystem.h"
#include "flareSystem.h"
#include "wing.h"

#define SPEED_UP			0.02	//加速度
#define SPEED_DOWN			0.005	//減速度

#define MIN_SPEED			400		//重力の影響を受ける最小速度km


#define TRAIL_WIDTH			0.02		//トレイル幅

#define MAIN_WING_RANGE		D3DX_PI / 10
#define BACK_WING_RANGE		D3DX_PI / 10
#define VERTICAL_WING_RANGE	D3DX_PI / 20


struct Jet_State
{
	float Accele;
	float Brake;
	
	float Yaw;		
	float Pitch;
	float Roll;

	bool Control = true;
};



class Jet : public GameObject
{
private:
	Model* m_Model;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11ShaderResourceView* m_DissolveTexture = NULL;

	class Audio* m_StealthSE{};
	class Audio* m_AlertSE{};

	class Scene*		m_Scene{};
	class Trail*		m_Trail[2]{};
	class CollisionBox* m_Collision{};


	//パーツ
	Engine			m_Engine;
	ControlStick	m_ControlStick;

	WeaponSystem	m_WeaponSm;
	LockOnSystem	m_LockOnSm;
	StealthSystem	m_StealthSm;
	FlareSystem		m_FlareSm;

	Wing			m_WingFR; //右主翼
	Wing			m_WingFL; //左主翼
	Wing			m_WingBR; //右尾翼
	Wing			m_WingBL; //左尾翼
	Wing			m_WingTR; //右垂直尾翼
	Wing			m_WingTL; //左垂直尾翼

	
	
	Jet_State		m_JetState;

	
	D3DXVECTOR3 m_TopVertex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	float m_Speed{};


	int m_DeathCount{};					//墜落用
	bool m_DeathFlg{};					//プレイヤー死亡フラグ


	bool m_StealthModeFlg{};			//ステルスモードフラグ
	bool m_FlareFlg{};					//フレアクールダウン用

	bool m_RoleLock{};					//ロール操作ロック
	bool m_PitchLock{};					//ピッチ操作ロック
	bool m_YawLock{};					//ヨー操作ロック
	bool m_EngineLock{};				//エンジン操作ロック
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();


	float GetSpeed() { return m_Speed; }
	bool GetDeath() { return m_DeathFlg; }

	bool GetStealthFlg() { return m_StealthModeFlg; }
	bool GetFlareFlg() { return m_FlareFlg; }


	//ロック
	void SetRoleLock(bool lock) { m_RoleLock = lock; }
	void SetPitchLock(bool lock) { m_PitchLock = lock; }
	void SetYawLock(bool lock) { m_YawLock = lock; }
	void SetEngineLock(bool lock) { m_EngineLock = lock; }
	bool GetEngineLock() { return m_EngineLock; }


	//クラス取得
	Audio* GetAlertSE() { return m_AlertSE; }
	WeaponSystem* GetWeaponSm() { return &m_WeaponSm; }
	LockOnSystem* GetLockOnSm() { return &m_LockOnSm; }
	StealthSystem* GetStealthSm() { return &m_StealthSm; }
	FlareSystem* GetFlareSm() { return &m_FlareSm; }
};