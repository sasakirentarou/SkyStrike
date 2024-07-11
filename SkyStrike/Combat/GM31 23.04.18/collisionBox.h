#pragma once

#include "gameObject.h"
#include "model.h"
#include <string>

class CollisionBox : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	static Model* m_Model;
	D3DXMATRIX m_Pearent{};
	D3DXCOLOR m_Color = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);

	std::string m_Name{};

	bool m_HitFlg{};
	bool m_DrawFlg{};
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Debug();

	bool SetOBB(GameObject* obb1, GameObject* obb2,float offset);
	float LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3);
	
	void SetPearent(D3DXMATRIX pearent) { m_Pearent = pearent; }
	void SetColor(D3DXCOLOR color) { m_Color = color; }
	void SetDrawFlg(bool draw) { m_DrawFlg = draw; }
	void SetName(const char* name) { m_Name = name; }
};