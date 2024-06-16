#pragma once
#include "game.h"

class Result :public Scene
{
private:
	class Fade* m_Fade{};
	class Audio* m_BGM{};
	class Audio* m_ConfirmSE{};
	class TextureManager* m_Texture{};

public:
	void Init();
	void Update();
};