#pragma once
#include "gameObject.h"
#include <vector>

class EnemyJet;

class LockOnSystem : public GameObject
{
private:
	class Audio* m_LockOnSE{};
	class Audio* m_LockFinishSE{};

	class Scene* m_Scene{};
	class LockOn* m_LockOn{};
	class Lockon2D* m_Lockon2D{};

	std::vector<EnemyJet*> m_EnemyJets{};
	
	D3DXVECTOR3 m_LockPos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	int m_EnemyID{};					//敵ID保存用

	bool m_LockOnFlg{};					//ロックオン開始フラグ
	bool m_LockFinishFlg{};				//ロックオン完了フラグ

	bool m_LockSeFlg{};					//ロックオン中SE用フラグ
	bool m_LockFinishSeFlg{};			//ロックオン完了SE用フラグ
public:
	void Init();
	void Update(GameObject* obj);

	void SwitchTarget();
	void SwitchReset();
	

	int GetLockEnemyID() { return m_EnemyID; }
	bool GetLockFlg() { return m_LockFinishFlg; }

	LockOn* GetLockOn() { return m_LockOn; }
};