#pragma once

#define MAX_STEALTH			100		//�X�e���X�ő��
#define STEALTH_DOWN_SPEED	0.1		//�X�e���X����X�s�[�h
#define STEALTH_UP_SPEED	0.15	//�X�e���X�񕜃X�s�[�h

class StealthSystem
{
private:
	float m_Amount{};			//�c��
	float m_Threshold{};		//�f�B�]���u�������l

public:
	void Init();

	void Active();
	void Charge();

	float GetAmount() { return m_Amount; }
	float GetThreshold() { return m_Threshold; }
};