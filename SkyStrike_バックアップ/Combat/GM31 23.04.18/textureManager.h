#pragma once
#include "gameObject.h"

class TextureManager :public GameObject
{
private:
	class Scene* m_Scene{};

	bool m_LogoFlg{};
	bool m_MenuFlg{};
	bool m_WeaponFlg{};
	bool m_OptionFlg{};
	bool m_GameFlg{};
	bool m_DesReportFlg{};
	bool m_MissReportFlg{};
	bool m_HitReportFlg{};
	bool m_MissileAlertFlg{};

	int m_DestroyCount{};
	int m_MissCount{};
	int m_HitCount{};
	int m_EnemyNumber{};

	float m_SelectPos = -100.0f;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetLogo(bool logo){m_LogoFlg = logo;}
	void SetMenu(bool menu){m_MenuFlg = menu;}
	void SetGame(bool game){m_GameFlg = game;}

	void SetSelectUpDown(bool select)
	{
		if (select)
		{
			if (m_SelectPos < 200.0f)
				m_SelectPos += 100.0f;
		}
		else
		{
			if (m_SelectPos > -100.0f)
				m_SelectPos -= 100.0f;
		}
	}

	float GetSelectPos(){return m_SelectPos;}

	void SetDesReportFlg(bool flg){m_DesReportFlg = flg;}
	void SetMissReportFlg(bool flg){m_MissReportFlg = flg;}
	void SetHitReportFlg(bool flg){m_HitReportFlg = flg;}
	void SetMissileAlertFlg(bool flg){m_MissileAlertFlg = flg;}

	void SetEnemyMinus() { m_EnemyNumber--; }
};