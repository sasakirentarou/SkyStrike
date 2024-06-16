#pragma once

#include "gameObject.h"

class Fade : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class Sprite* m_Sprite{};

	float m_Fade = 1.0f;
	bool m_Fade_Flug = false;
	bool m_FadeFinish = false;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void FadeOut() { m_Fade_Flug = true; };
	bool GetFadeFinish() { return m_FadeFinish; }
};