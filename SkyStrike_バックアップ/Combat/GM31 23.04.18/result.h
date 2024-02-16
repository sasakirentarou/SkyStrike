#pragma once
#include "game.h"

class Result :public Scene
{
private:
	class Fade* m_Fade{};
public:
	void Init();
	void Update();
};