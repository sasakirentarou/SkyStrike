#pragma once
#include "scene.h"

class Game : public Scene
{
private:
	class Audio* m_Wind0{};
	class Audio* m_Wind1{};
	class Audio* m_Engine{};

	class Fade* m_Fade{};

	static bool m_LoadFinish;
public:
	static void Load();
	static void Unload();
	static bool GetLoadFinish() { return m_LoadFinish; }

	void Init() override;
	void Uninit() override;
	void Update()override;
	void Draw()override;
};