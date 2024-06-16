//#pragma once
//
//#include "model.h"
//#include "gameObject.h"
////テスト
//#include "engine.h"
//
//#define ROLL_SPEED			0.03	//ロール(Z)回転時の感度
//#define PITCH_SPEED			0.015	//ピッチ(X)回転時の感度
//#define YAW_SPEED			0.005	//ヨー(Y)回転時の感度
//
//#define SPEED_UP			0.02	//加速度
//#define SPEED_DOWN			0.005	//減速度
//
//#define FRARE_TIME			30		//フレア発射間隔
//#define MIN_SPEED			0.4		//重力の影響を受ける最小速度
//
//#define MAX_MISSILE			80		//通常ミサイル弾数
//#define MAX_LONG			14		//ロングミサイル弾数
//#define MAX_SHORT			15		//ショートミサイル弾数
//#define MAX_RAIL			10		//レールガン弾数
//#define MAX_FLARE			5		//フレア数
//#define MAX_STEALTH			100		//ステルス残量
//
//#define STEALTH_DOWN_SPEED	0.1		//ステルス消費スピード
//#define STEALTH_UP_SPEED	0.15	//ステルス回復スピード
//
////特殊兵装
//enum SpecialWeaponType
//{
//	WEAPON_LONG_MISSILE,	//2体同時ロックオンでき、威力が高い
//	WEAPON_SHORT_MISSILE,	//誘導性能が高く、威力が高い
//	WEAPON_RAIL_GUN			//撃った直後に命中、残弾数が少ない
//};
//
//class Player : public GameObject
//{
//private:
//	Model* m_Model{};
//
//	ID3D11VertexShader* m_VertexShader = nullptr;
//	ID3D11PixelShader* m_PixelShader = nullptr;
//	ID3D11InputLayout* m_VertexLayout = nullptr;
//
//	ID3D11ShaderResourceView* m_DissolveTexture = NULL;
//
//	D3DXVECTOR3 m_EnemyPos{};
//	D3DXVECTOR3 m_Top{};
//
//	D3DXQUATERNION m_Endquat = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
//
//	class Audio* m_MinigunSE{};
//	class Audio* m_LockOnSE{};
//	class Audio* m_LockSetSE{};
//	class Audio* m_StealthSE{};
//	class Audio* m_MissileAlertSE{};
//
//	class Scene* m_Scene{};
//	class LockOn* m_LockOn{};
//	class Cross* m_Cross{};
//	class Trail* m_Trail01{};
//	class Trail* m_Trail02{};
//	class Lockon2D* m_Lockon2D{};
//	class CollisionBox* m_Collision{};
//
//	//ミサイル
//	class Missile* m_Missile00{};
//	class Missile* m_Missile01{};
//	class LongMissile* m_LongMissile00{};
//	class LongMissile* m_LongMissile01{};
//
//	//class MissileUI* m_specialUI_0{};
//	//class MissileUI* m_specialUI_1{};
//	class LongMissileUI* m_LongUI_0{};
//	class LongMissileUI* m_LongUI_1{};
//
//	Engine m_Engine;
//
//	static SpecialWeaponType m_SpecialWeapon;
//
//	int m_EnemyID{};					//敵ID保存用
//	int m_OldEnemyID = 100;
//	int m_MissileLockID{};				//ミサイル発射時誘導ID保存用
//	int m_DeathCount{};					//墜落用
//	int m_FlareCount{};					//フレア間隔用
//	int m_MissileVibCount{};			//ミサイルバイブレーション用
//	int m_MissilePosChange{};			//発射ミサイルpos変更用
//	int m_SpecialMissilePosChange{};	//特殊兵装pos変更用
//	int m_MissileAmount{};				//ミサイル数
//	int m_SpecialMissileAmount{};		//特殊兵装数
//	int m_FlareAmount{};				//フレア数
//
//	float m_fVec{};						//現在スピード
//	float m_LockOnType{};				//ロックオン移動用
//	float m_Threshold{};				//ディゾルブしきい値
//	float m_StealthAmount{};			//ステルス残量
//	float m_ControllAngle{};
//
//	bool m_LockSetFlg{};				//ロックオン完了フラグ
//	bool m_LockSetSeFlg{};				//ロックオン完了SEフラグ
//	bool m_LockSeFlg{};					//ロックオン中SEフラグ
//	bool m_StealthModeFlg{};			//ステルスモードフラグ
//	bool m_DeathFlg{};					//プレイヤー死亡フラグ
//	bool m_FlareFlg{};			
//	bool m_MissileSetFlg00{};			//ミサイル装着フラグ
//	bool m_MissileSetFlg01{};
//	bool m_SpecialMissileSetFlg00{};	//特殊兵装装着フラグ
//	bool m_SpecialMissileSetFlg01{};
//	bool m_SpecialWeaponMode{};
//	bool m_LockOnFlg{};					//ロックオン開始フラグ
//	bool m_RoleLock{};					//ロール操作ロック
//	bool m_PitchLock{};					//ピッチ操作ロック
//	bool m_YawLock{};					//ヨー操作ロック
//	bool m_EngineLock{};				//エンジン操作ロック
//
//public:
//	void Init();
//	void Uninit();
//	void Update();
//	void Draw();
//
//	//通常ミサイル関数
//	void UpdateMissile();
//	void ShotMissile();
//	
//	//Weaponステート関数
//	void UpdateLong();
//	void UpdateShort();
//	void UpdateRail();
//	void ShotLong();
//	void ShotShort();
//	void ShotRail();
//
//	//処理関数
//	bool PlayerView(float fieldOfViewRadians, float viewDistancee);
//	void SwitchTarget();
//	void ShotFlare();
//
//
//	//ゲッター,セッター
//	int GetLockEnemy() { return m_EnemyID; }
//	int GetMisLockID() { return m_MissileLockID; }
//	float GetSpeed() { return m_fVec; }
//	bool GetDeath() { return m_DeathFlg; }
//	bool GetWeaponChange() { return m_SpecialWeaponMode; }
//
//	Audio* GetMissileAlertSE() { return m_MissileAlertSE; }
//	LockOn* GetLockOn() { return m_LockOn; }
//
//	int GetMissileAmount() { return m_MissileAmount; }
//	int GetSpecialAmount() { return m_SpecialMissileAmount; }
//	int GetFlareAmount() { return m_FlareAmount; }
//	float GetStealthAmount() { return m_StealthAmount; }
//	bool GetStealthFlg() { return m_StealthModeFlg; }
//	bool GetFlareFlg() { return m_FlareFlg; }
//	bool GetLockFlg() { return m_LockSetFlg; }
//
//	//機能ロック
//	void SetRoleLock(bool lock) { m_RoleLock = lock; }
//	void SetPitchLock(bool lock) { m_PitchLock = lock; }
//	void SetYawLock(bool lock) { m_YawLock = lock; }
//	void SetEngineLock(bool lock) { m_EngineLock = lock; }
//	bool GetEngineLock() { return m_EngineLock; }
//};