#pragma once

#include "weaponManager.h"
#include "model.h"

#define PLAYER_MISSILE_ROT	0.15	//âÒì]ó¶
#define ENEMY_MISSILE_ROT	0.06	//âÒì]ó¶
#define DEFAULT_VELOCITY		0.5		//í èÌë¨ìx
#define PLUS_VELOCITY			0.2		//éûä‘åoâﬂí«â¡ë¨ìx
#define VELOCITY_MINUS_PERCENT	-0.1	//ãÛãCíÔçRÇÃäÑçá(%)

class Missile : public WeaponManager
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;
	
	static Model* m_Model;
	static class Player* m_Player;

	class Audio* m_ShotSE{};
	class Scene* m_Scene{};
	class Camera* m_Camera{};
	class HpGauge* m_PlayerHp{};
	class MissileFire* m_Fire{};
	class TextureManager* m_Texture{};
	class CollisionBox* m_Collision{};

	bool m_PlayerTrackingFlg{};
	bool m_EnemyMissile{};
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Shot() override;
	void EnemyShot();

	void SetPlayerTrack(bool tracking) { m_PlayerTrackingFlg = tracking; }
	bool GetPlayerTrack() { return m_PlayerTrackingFlg; }

	void SetEnemyMissile(bool enemy) { m_EnemyMissile = enemy; }
	bool GetEnemyMissile() { return m_EnemyMissile; }
};