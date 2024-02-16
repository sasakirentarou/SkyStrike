#pragma once

#include "model.h"
#include "gameObject.h"

#define FRARE_TIME			30
#define MIN_SPEED			0.4

#define MAX_MISSILE			80
#define MAX_FLARE			5
#define MAX_STEALTH			100

#define STEALTH_DOWN_SPEED	0.1
#define STEALTH_UP_SPEED	0.15



class Player : public GameObject
{
private:
	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11ShaderResourceView* m_DissolveTexture = NULL;

	D3DXVECTOR3 m_EnemyPos{};
	D3DXVECTOR3 m_Top{};

	D3DXQUATERNION m_Endquat = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

	class Audio* m_MinigunSE{};
	class Audio* m_MissileSE{};
	class Audio* m_LockOnSE{};
	class Audio* m_LockSetSE{};


	class Scene* m_Scene{};
	class LockOn* m_LockOn{};
	class Cross* m_Cross{};
	class Trail* m_Trail01{};
	class Trail* m_Trail02{};

	//テスト
	class Missile* missile00{};
	class Missile* missile01{};

	int m_EnemyID{};			//敵ID保存用
	int m_OldEnemyID = 100;
	int m_MissileLockID{};		//ミサイル発射時誘導ID保存用
	int m_DeathCount{};
	int m_FlareCount{};
	int m_MissilePosChange{};	//発射ミサイルpos変更用
	int m_MissileAmount{};		//ミサイル数
	int m_FlareAmount{};		//フレア数

	float m_fVec{};				//現在スピード
	float m_LockOnType{};		//ロックオン移動用
	float m_Threshold{};		//ディゾルブしきい値
	float m_StealthAmount{};	//ステルス残量

	bool m_LockSetFlg{};		//ロックオン完了フラグ
	bool m_LockSetSeFlg{};		//ロックオン完了SEフラグ
	bool m_StealthModeFlg{};	//ステルスモードフラグ
	bool m_DeathFlg{};
	bool m_FlareFlg{};
	bool m_MissileSetFlg00{};	//ミサイル装着フラグ
	bool m_MissileSetFlg01{};
	bool m_LockOnFlg{};			//ロックオン開始フラグ
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	bool PlayerView(float fieldOfViewRadians, float viewDistancee);
	void SwitchTarget();
	void ShotFlare();

	int GetLockEnemy()
	{
		return m_EnemyID;
	}

	int GetMisLockID()
	{
		return m_MissileLockID;
	}

	float GetSpeed()
	{
		return m_fVec;
	}

	bool GetDeath()
	{
		return m_DeathFlg;
	}

	int GetMissileAmount()
	{
		return m_MissileAmount;
	}

	int GetFlareAmount()
	{
		return m_FlareAmount;
	}

	float GetStealthAmount()
	{
		return m_StealthAmount;
	}

	//テスト

};