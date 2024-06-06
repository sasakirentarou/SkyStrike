#pragma once

#include "gameObject.h"
#include "model.h"

#define PLAYER_MISSILE_ROT	0.1
#define ENEMY_MISSILE_ROT	0.06	

class Missile : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	D3DXVECTOR3 m_EnemyPos{};

	class Scene* m_Scene{};
	class Camera* m_Camera{};
	class HpGauge* m_PlayerHp{};
	static class Player* m_Player;
	static Model* m_Model;

	int m_Count{};
	int m_LockEnemyID{};

	bool m_PlayerTrackingFlg{};
	bool m_FlareFlg{};
	bool m_EnemyTrackingFlg{};
	bool m_ShotFlg{};
	bool m_EnemyMissile{};
public:
	static void Load();
	static void Unload();


	void Init();
	void Uninit();
	void Update();
	void Draw();

	void MisiileBom();

	void SetFlare(bool flare) { m_FlareFlg = flare; }

	void SetPlayerTrack(bool tracking) { m_PlayerTrackingFlg = tracking; }
	void SetEnemyTrack(bool tracking) { m_EnemyTrackingFlg = tracking; }
	bool GetPlayerTrack() { return m_PlayerTrackingFlg; }
	bool GetEnemyTrack() { return m_EnemyTrackingFlg; }

	void SetShot(bool shot) { m_ShotFlg = shot; }
	bool GetShot() { return m_ShotFlg; }

	void SetEnemyMissile(bool enemy) { m_EnemyMissile = enemy; }
	bool GetEnemyMissile() { return m_EnemyMissile; }
};