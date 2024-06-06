#pragma once

#include "gameObject.h"
#include "model.h"

class Fire : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	static Model* m_Model;
	class Audio* m_EngineSE{};

	D3DXMATRIX m_Pearent{};

	bool m_PlayerFire{};

	float m_OffsetX{};
	float m_OffsetY{};
	float m_OffsetZ{};
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPearent(D3DXMATRIX matrix) { m_Pearent = matrix; }
	void SetOffset(float x, float y, float z) { m_OffsetX = x; m_OffsetY = y; m_OffsetZ = z; };
	void SetPlayerFire() { m_PlayerFire = true; };
};