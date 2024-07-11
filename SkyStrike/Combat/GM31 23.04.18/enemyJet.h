#pragma once

#include "enemyManager.h"
#include "model.h"
#include <string>

#define ROTATION_AMOUNT		0.05 //機体の回転量 ※小さくしすぎるとnanになる0.015
#define ROTATION_FRAME		3	 //回転頻度

#define MISSILE_COOLDOWN	8	 //ミサイルのクールダウン

#define MAX_HEALTH			100	 //最大体力
#define MAX_RANGE			2500 //最大移動範囲
#define MAX_ESCAPE			800  //最大撤退距離
#define MAX_VIEW_LENGTH		1000 //ミサイル発射距離
#define RISK_RELEASE		500	 //墜落回避解除高度

enum ENEMY_STATE
{
	ENEMY_ATTACK,		// 攻撃
	ENEMY_ESCAPE,		// 退避
	ENEMY_SEACH,		// 探索
	ENEMY_GROUNDRISK,	// 墜落回避
	ENEMY_OUT_OF_RANGE,	// 範囲外
};

class EnemyJet : public EnemyManager
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

	static Model* m_Model;

	class Audio* m_KillSE{};

	class Scene* m_Scene{};
	class Lock* m_Lock{};
	class Tgt* m_Tgt{};
	class Jet* m_Jet{};
	class Trail* m_Trail01{};
	class Trail* m_Trail02{};
	class Fire* m_Fire01{};
	class Fire* m_Fire02{};
	class CollisionBox* m_Collision{};

	int m_BomCount{};			// 爆発カウント
	int m_OldHealth{};			// 体力比較用
	int m_SmokeCount{};			// 煙を出す間隔
	int m_RandomTracking{};		// プレイヤーの後ろに付く距離
	int m_SeachCount{};
	int m_QuaAmountCount{};

	float m_PlayerLength{};

	bool m_StateLock{};
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Debug();

	void MoveHoming(D3DXVECTOR3 target,bool reverse);//移動誘導
	void EnemyStateChange(int number);//ステート変更

	// ステート関数
	void Attack() override;		//攻撃
	void Escape();				//撤退
	void Seach() override;		//捜索
	void GroundRisk();			//地面回避
	void OutOfRange();			//飛行範囲

	Lock* GetAddLock() { return m_Lock; }
	
	void SetStateLock(bool lock) { m_StateLock = lock; }
};