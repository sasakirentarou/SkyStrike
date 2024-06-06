#pragma once
#include "main.h"
#include "gameObject.h"

class WeaponManager : public GameObject
{
protected:
	D3DXVECTOR3 m_EnemyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//敵の位置保存用
	
	int m_ShotCount{};			//発射後のカウント
	int m_LockEnemyID{};		//ロックしたID
	bool m_TrackingFlg{};		//ロックフラグ
	bool m_ShotFlg{};			//発射フラグ

public:
	virtual void Shot() {};

	bool GetTrack() { return m_TrackingFlg; }
	bool GetShot() { return m_ShotFlg; }

	void SetShot(bool shot) { m_ShotFlg = shot; }

	void MisiileBom();
};