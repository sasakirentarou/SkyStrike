#pragma once
#include "gameObject.h"

#define MAX_FLARE			5		//装備フレア数
#define FLARE_TIME			30		//発射間隔

class FlareSystem
{
private:
	class Scene* m_Scene{};

	int m_Time{};			//間隔用
	int m_Amount{};			//残りフレア数
	bool m_Finish{};		//終了
public:
	void Init();
	void Active(GameObject* obj);
	void Fire(GameObject* obj);

	int GetAmount() { return m_Amount; }
	bool GetFinish() { return m_Finish; }
};