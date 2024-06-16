#pragma once
#include "gameObject.h"
//#include "lockOnSystem.h"
#include <vector>

//�~�T�C���i���o�[
#define DEFAULT_NUM	0	
#define SPECIAL_NUM	1	

//�~�T�C�������e��
#define MAX_MISSILE			80
#define MAX_LONG			14
#define MAX_SHORT			15
#define MAX_RAIL			10

//���ꕺ��
enum SpecialWeapon
{
	LONG_MISSILE,		//2�̓������b�N�I���ł��A�З͂�����
	SHORT_MISSILE,		//�U�����\�������A�З͂�����
	RAIL_GUN			//����������ɖ����A�c�e�������Ȃ�
};


class EnemyJet;
class MissileUI;
class LongMissileUI;

class WeaponSystem: public GameObject
{
private:
	static SpecialWeapon m_WeaponType;

	class Scene* m_Scene{};
	class Missile* m_Missile[2]{};
	class MissileUI* m_DefaultUIs[2]{};
	class LongMissile* m_LongMissile[2]{};
	class LongMissileUI* m_LongUIs[2]{};

	class Audio* m_MachineGunSE{};

	std::vector<EnemyJet*> m_EnemyJets{};


	bool m_WeaponChange{};

	bool m_DefaultSetFlg[2];			//�~�T�C�������t���O
	bool m_SpecialSetFlg[2];			//���ꕺ�������t���O
	
	int m_VibCount{};					//�p�b�h�o�C�u���[�V�����p
	bool m_VibFlg{};

	int m_Amount[2]{};					//�c�e��
	int m_PosChange[2]{};				//����pos�ύX�p

	int m_LockID{};						//�U��ID�ۑ��p
public:
	void Init();
	void Update(GameObject* obj);


	void MissileFire(GameObject* obj,int lockID);

	void MachineGunFire(GameObject* obj, D3DXVECTOR3 offset, float speed);
	void MachineGunStop();


	void WeaponChange();
	void UIChange(bool change);

	int  GetTargetLockID()	{ return m_LockID; }
	bool GetWeaponChange()	{ return m_WeaponChange; }
	int  GetDefMissAmount() { return m_Amount[DEFAULT_NUM]; }
	int  GetSpeMissAmount() { return m_Amount[SPECIAL_NUM]; }

	//�~�T�C�������֐�
	void UpdateDefault(GameObject* obj);
	void ShotDefault(GameObject* obj, int lockID);
	void UpdateLong(GameObject* obj);
	void ShotLong(GameObject* obj, int lockID);
	void UpdateShort(GameObject* obj);
	void ShotShort(GameObject* obj);
	void UpdateRail(GameObject* obj);
	void ShotRail(GameObject* obj);
};