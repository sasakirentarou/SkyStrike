#pragma once

#include "weaponManager.h"
#include "model.h"

#define AUTO_RANGE 20 //ÉIÅ[ÉgîÕàÕ

class Bullet : public WeaponManager
{
private:
	static Model* m_Model;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class Scene* m_Scene{};
	class CollisionBox* m_Collision{};

	float m_BulletTime{};
	float m_Period = 2.0f;

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Shot()override;
};