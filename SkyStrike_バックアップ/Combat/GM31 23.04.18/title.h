#pragma once
#include "game.h"

class Title :public Scene
{
private:
	class Fade* m_Fade{};
	class TitleCamera* m_Camera{};
	class TextureManager* m_Texture{};

	D3DXVECTOR2 m_MousePos{};

	bool m_Start{};
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};