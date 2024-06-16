#pragma once
#include "scene.h"

class Game : public Scene
{
private:
	class Audio* m_Wind0{};
	class Audio* m_Wind1{};
	class Audio* m_BGM{};

	class Fade* m_Fade{};
	class TextureManager* m_Texture{};
	class Timer* m_Timer{};
	class Jet* m_Jet{};

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