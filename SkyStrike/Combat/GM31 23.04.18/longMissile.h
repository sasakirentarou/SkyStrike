#pragma once

#include "weaponManager.h"
#include "model.h"

#define PLAYER_MISSILE_ROT		0.1		//��]��
#define MISSILE_SHOT_TIME		30		//���˂܂ł̎���
#define DEFAULT_VELOCITY		0.8		//�ʏ푬�x
#define PLUS_VELOCITY			0.2		//���Ԍo�ߒǉ����x
#define VELOCITY_MINUS_PERCENT	-0.15	//��C��R�̊���(%)


class LongMissile : public WeaponManager
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	static class Jet* m_Jet;
	static Model* m_Model;

	class Audio* m_ShotSE{};
	class Scene* m_Scene{};
	class Camera* m_Camera{};
	class MissileFire* m_Fire{};
	class TextureManager* m_Texture{};
	class CollisionBox* m_Collision{};

	int m_Time{};
	bool m_UpsideDown{};
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Shot() override;
};