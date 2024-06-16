#pragma once
#include "gameObject.h"
//#include "lockOnSystem.h"
#include <vector>

//ミサイルナンバー
#define DEFAULT_NUM	0	
#define SPECIAL_NUM	1	

//ミサイル装備弾数
#define MAX_MISSILE			80
#define MAX_LONG			14
#define MAX_SHORT			15
#define MAX_RAIL			10

//特殊兵装
enum SpecialWeapon
{
	LONG_MISSILE,		//2体同時ロックオンでき、威力が高い
	SHORT_MISSILE,		//誘導性能が高く、威力が高い
	RAIL_GUN			//撃った直後に命中、残弾数が少ない
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

	bool m_DefaultSetFlg[2];			//ミサイル装着フラグ
	bool m_SpecialSetFlg[2];			//特殊兵装装着フラグ
	
	int m_VibCount{};					//パッドバイブレーション用
	bool m_VibFlg{};

	int m_Amount[2]{};					//残弾数
	int m_PosChange[2]{};				//発射pos変更用

	int m_LockID{};						//誘導ID保存用
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

	//ミサイル処理関数
	void UpdateDefault(GameObject* obj);
	void ShotDefault(GameObject* obj, int lockID);
	void UpdateLong(GameObject* obj);
	void ShotLong(GameObject* obj, int lockID);
	void UpdateShort(GameObject* obj);
	void ShotShort(GameObject* obj);
	void UpdateRail(GameObject* obj);
	void ShotRail(GameObject* obj);
};