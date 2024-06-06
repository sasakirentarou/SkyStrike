#pragma once
#include "main.h"
#include "gameObject.h"

class WeaponManager : public GameObject
{
protected:
	D3DXVECTOR3 m_EnemyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�G�̈ʒu�ۑ��p
	
	int m_ShotCount{};			//���ˌ�̃J�E���g
	int m_LockEnemyID{};		//���b�N����ID
	bool m_TrackingFlg{};		//���b�N�t���O
	bool m_ShotFlg{};			//���˃t���O

public:
	virtual void Shot() {};

	bool GetTrack() { return m_TrackingFlg; }
	bool GetShot() { return m_ShotFlg; }

	void SetShot(bool shot) { m_ShotFlg = shot; }

	void MisiileBom();
};