#pragma once
#include "gameObject.h"

#define MAX_FLARE			5		//�����t���A��
#define FLARE_TIME			30		//���ˊԊu

class FlareSystem
{
private:
	class Scene* m_Scene{};

	int m_Time{};			//�Ԋu�p
	int m_Amount{};			//�c��t���A��
	bool m_Finish{};		//�I��
public:
	void Init();
	void Active(GameObject* obj);
	void Fire(GameObject* obj);

	int GetAmount() { return m_Amount; }
	bool GetFinish() { return m_Finish; }
};