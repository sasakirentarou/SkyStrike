#pragma once

#include "gameObject.h"
#include "model.h"

struct Jet_State
{
	float Accele;
	float Brake;
	
	float Yaw;
	float Pitch;
	float Roll;

	bool MainWeapon;
	bool SubWeapon;
};

class Jet : public GameObject
{
private:
	static Model* m_Model;



	//ID3D11VertexShader* m_VertexShader = nullptr;
	//ID3D11PixelShader* m_PixelShader = nullptr;
	//ID3D11InputLayout* m_VertexLayout = nullptr;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};