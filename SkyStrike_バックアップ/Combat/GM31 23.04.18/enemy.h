#pragma once

#include "gameObject.h"
#include "model.h"
#include <string>

#define ROTATION_AMOUNT		0.05 //機体の回転量 ※小さくしすぎるとnanになる0.015
#define MISSILE_COOLDOWN	6	 //ミサイルのクールダウン
#define MAX_HEALTH			100	 //最大体力

enum ENEMY_STATE
{
	ENEMY_ATTACK,	// 攻撃
	ENEMY_ESCAPE,	// 退避
	ENEMY_SEACH,	// 探索
	ENAMY_GROUNDRISK// 墜落回避
};

class Enemy : public GameObject
{
private:
	ENEMY_STATE m_EnemyState = ENEMY_ATTACK;
	ENEMY_STATE m_StateKeep{};

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	D3DXVECTOR3 m_Top = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_RiskPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_RandomSeachPoint = D3DXVECTOR3(0.0f,0.0f,0.0f);

	Model* m_Model{};

	class Scene* m_Scene{};
	class Lock* m_Lock{};
	class Player* m_Player{};
	class Trail* m_Trail01{};
	class Trail* m_Trail02{};

	static int m_EnemyCount;	// enemy数カウント
	int m_BomCount{};			// 爆発カウント
	int m_EnemyID{};			// enemy個別ID
	int m_Health{};				// 体力
	int m_OldHealth{};			// 体力比較用
	int m_MissileCount{};		// ミサイルクールタイム
	int m_SmokeCount{};			// 煙を出す感覚
	int m_RandomTracking{};		// プレイヤーの後ろに付く距離
	int m_SeachCount{};
	int m_QuaAmountCount{};

	bool m_CrashFlg = false;		// 破壊判別フラグ
	bool m_MissileShot = true;		// ミサイル発射フラグ
	bool m_PlayerStealth = false;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	bool EnemyView(D3DXVECTOR3 forward, float fieldOfViewRadians, float viewDistancee);//視界
	void EnemyHoming(D3DXVECTOR3 target,bool reverse);//誘導
	void EnemyStateChange(int number);//ステート変更

	// ステート関数
	void UpdateAttack();
	void UpdateEscape();
	void UpdateSeach();
	void UpdateGroundRisk();


	Lock* GetAddLock()
	{
		return m_Lock;
	}

	int GetMaxEnemy()
	{
		return m_EnemyCount;
	}

	int GetEnemyID()
	{
		return m_EnemyID;
	}

	void SetCrash(bool crash)
	{
		m_CrashFlg = crash;
	}

	bool GetCrash()
	{
		return m_CrashFlg;
	}

	void SetPlayerStealth(bool stealth)
	{
		m_PlayerStealth = stealth;
	}

	void HealthMinus(int minus)
	{
		m_Health -= minus;
	}

	int GetHealth()
	{
		return m_Health;
	}
};