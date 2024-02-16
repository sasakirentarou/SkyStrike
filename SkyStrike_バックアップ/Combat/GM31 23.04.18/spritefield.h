#pragma once

#include "component.h"

// 2D(x,y,z)描画コンポーネント
class SpriteField : public Component
{
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;
public:
	void Init(float x1, float x2, float y1, float y2, float z1, float z2, float n1, float n2, float n3,float uv, const char* image);
	void Uninit();
	void Update();
	void Draw();
};