#pragma once

#include "component.h"

// 2D(x,y)描画コンポーネント
class Sprite : public Component
{
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
public:
	void Init(float x1, float y1, float x2, float y2, const char* image);
	void Uninit();
	void Update();
	void Draw();

	void SetColor(D3DXCOLOR Color) { m_Color = Color; }
};