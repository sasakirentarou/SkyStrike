#pragma once

#include "gameObject.h"

#define IMAGE_SHEETS	30	// 枚数
#define IMAGE_SHEETS_X	10	// 横枚数
#define IMAGE_SHEETS_Y	3	// 縦枚数


class Explosion : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;

	class Audio* m_Bom{};
	class Scene* m_Scene{};
	class Camera* m_Camera{};

	D3DXVECTOR3 m_RandomPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	int m_AnimCount{};
	int m_TimeCount{};
	int m_ShiftTime{};
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//スポーン設定
	void Spawn(D3DXVECTOR3 position, D3DXVECTOR2 scale, int time = 1);

	//座標をランダムにずらす
	void RandomShiftPos(int range);
};