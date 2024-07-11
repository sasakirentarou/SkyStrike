#pragma once
#include "game.h"

#define MENU_DOWN_MAX		80.0f
#define MISSION_DOWN_MAX	20.0f
#define OPTION_DOWN_MAX		80.0f

enum SelectMode
{
	SELECT_NONE,
	SELECT_MENU,
	SELECT_MISSION,
	SELECT_WEAPON,
	SELECT_OPTION
};

class Title :public Scene
{
private:
	class Audio* m_SelectSE{};
	class Audio* m_ConfirmSE{};
	class Audio* m_ErrorSE{};
	class Audio* m_StartSE{};
	class Audio* m_BGM{};
	class Fade* m_Fade{};
	class TitleCamera* m_Camera{};
	class TextureManager* m_Texture{};
	class Jet* m_Jet{};

	SelectMode m_SelectMode = SELECT_NONE;
	//D3DXVECTOR2 m_MousePos{};

	bool m_Start{};
	float m_SelectDownMax{};
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};