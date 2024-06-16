#pragma once

#include "gameObject.h"

#define MOVE_SPEED 0.02f;

class Lockon2D : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	D3DXVECTOR2 m_Pos{};

	float m_X{};
	float m_Y{};
	float m_Width = 30.0f;
	float m_Height = 30.0f;
	float m_Time;
	
	bool m_LockFinish{};
	bool m_DrawFlg{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	//void SetOffset(float x, float y) { m_X = x; m_Y = y; };
	//void SetSize(float width, float height) { m_Width = width; m_Height = height; }
	void SetDraw(bool draw) { m_DrawFlg = draw; }
	bool GetLockFinish() { return m_LockFinish; }
	void LockMoveReset() { m_Time = 0.0f; m_LockFinish = false; }

	void LockonMove();
};