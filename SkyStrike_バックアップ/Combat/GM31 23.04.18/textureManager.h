#pragma once
#include "gameObject.h"

#define MIN_BGMPIN 410
#define MAX_BGMPIN 720
#define MIN_SEPIN 410
#define MAX_SEPIN 720

//ランクタイム
#define TIME_S	50
#define TIME_A	80
#define TIME_B	120
#define TIME_C	160

enum TextureScene
{
	TEXTURE_LOGO,
	TEXTURE_MENU,
	TEXTURE_MISSION,
	TEXTURE_WEAPON,
	TEXTURE_OPTION,
	TEXTURE_GAME,
	TEXTURE_TUTORIAL,
	TEXTURE_RESULT,
};

class TextureManager :public GameObject
{
private:
	class Audio* m_MissionUpdateSE{};
	class Audio* m_MissionAccoSE{};

	class Scene* m_Scene{};
	class EnemyJet* m_EnemyTuto00{};
	class EnemyJet* m_EnemyTuto01{};

	TextureScene m_SceneTexture{};//textureシーン切り替え
	
	D3DXCOLOR m_BGMPinColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	D3DXCOLOR m_SEPinColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR m_PitchFlipCheckColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	static bool m_PitchFlipCheckFlg;
	static bool m_GameOverFlg;
	bool m_DesReportFlg{};
	bool m_MissReportFlg{};
	bool m_HitReportFlg{};
	bool m_UpdateReportFlg{};
	bool m_MissileAlertFlg{};
	bool m_TutorialProcessLock00{};
	bool m_TutorialProcessLock01{};
	bool m_MissionClear{};

	static int m_ResultTimer;
	int m_TutorialProcess{};
	int m_TutorialCount{};
	int m_DestroyCount{};
	int m_MissCount{};
	int m_HitCount{};
	int m_UpdateCount{};
	int m_ResultCount{};
	int m_AccoCount{};
	int m_EnemyNumber{};

	int m_DefMissAmount{};
	int m_SpeMissAmount{};
	int m_flareAmount{};
	float m_StealthAmount{};

	float m_RankAlphaLevel{};
	float m_ClearAlphaLevel{};
	float m_FailedAlphaLevel{};

	float m_SelectPos = -100.0f;

	static float m_BGMPin;
	static float m_SEPin;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetSceneTexture(TextureScene scene) { m_SceneTexture = scene; }

	void SetSelectUpDown(bool select,float downmax)
	{
		if (select)
		{
			if (m_SelectPos < downmax)
				m_SelectPos += 60.0f;
			else
				m_SelectPos = -100.0f;
		}
		else
		{
			if (m_SelectPos > -100.0f)//-100.0f
				m_SelectPos -= 60.0f;
			else
				m_SelectPos = downmax;
		}
	}

	float GetSelectPos() {return m_SelectPos;}
	void SetSelectPos(float pos) { m_SelectPos = pos; }

	void SetBGMPin(float pin) { m_BGMPin = pin; }
	void SetSEPin(float pin) { m_SEPin = pin; }

	void SetBGMPinPlus() { m_BGMPin++; }
	void SetBGMPinMinus() { m_BGMPin--; }

	void SetSEPinPlus() { m_SEPin++; }
	void SetSEPinMinus() { m_SEPin--; }

	void SetBGMPinRGB(float red, float green, float blue) { m_BGMPinColor.r = red; m_BGMPinColor.g = green; m_BGMPinColor.b = blue;};
	void SetSEPinRGB(float red, float green, float blue) { m_SEPinColor.r = red; m_SEPinColor.g = green; m_SEPinColor.b = blue; };
	void SetPitchFlipCheckRGB(float red, float green, float blue) { m_PitchFlipCheckColor.r = red; m_PitchFlipCheckColor.g = green; m_PitchFlipCheckColor.b = blue; };

	bool GetPitchFlipCheck() { return m_PitchFlipCheckFlg; }
	void SetPitchFlipCheck() { m_PitchFlipCheckFlg = !m_PitchFlipCheckFlg; }

	void SetDesReportFlg(bool flg){m_DesReportFlg = flg;}
	void SetMissReportFlg(bool flg){m_MissReportFlg = flg;}
	void SetHitReportFlg(bool flg){m_HitReportFlg = flg;}
	void SetMissileAlertFlg(bool flg){m_MissileAlertFlg = flg;}

	void SetResultTimer(int time) { m_ResultTimer = time; }
	void SetGameOverFlg(bool over) { m_GameOverFlg = over; }
	void SetEnemyMinus() { m_EnemyNumber--; }
	
	bool GetMissionClear() { return m_MissionClear; }
};