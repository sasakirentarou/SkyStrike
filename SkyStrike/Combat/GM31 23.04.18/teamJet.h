#pragma once

#include "gameObject.h"
#include "model.h"

class TeamJet : public GameObject
{
private:
	static Model* m_Model;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class Audio* m_PassSE{};
	class Trail* m_Trail01{};
	class Trail* m_Trail02{};
	class Fire* m_Fire{};

	D3DXVECTOR3 m_Top{};

	int m_Count{};
	int m_Patern{};
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPatern(int patern)
	{
		m_Patern = patern;
	}
};