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

	int m_EnemyID{};					//�GID�ۑ��p

	bool m_LockOnFlg{};					//���b�N�I���J�n�t���O
	bool m_LockFinishFlg{};				//���b�N�I�������t���O

	bool m_LockSeFlg{};					//���b�N�I����SE�p�t���O
	bool m_LockFinishSeFlg{};			//���b�N�I������SE�p�t���O
public:
	void Init();
	void Update(GameObject* obj);

	void SwitchTarget();
	void SwitchReset();
	

	int GetLockEnemyID() { return m_EnemyID; }
	bool GetLockFlg() { return m_LockFinishFlg; }

	LockOn* GetLockOn() { return m_LockOn; }
};