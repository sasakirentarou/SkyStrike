#pragma once

#define MAX_STEALTH			100		//ステルス最大量
#define STEALTH_DOWN_SPEED	0.1		//ステルス消費スピード
#define STEALTH_UP_SPEED	0.15	//ステルス回復スピード

class StealthSystem
{
private:
	float m_Amount{};			//残量
	float m_Threshold{};		//ディゾルブしきい値

public:
	void Init();

	void Active();
	void Charge();

	float GetAmount() { return m_Amount; }
	float GetThreshold() { return m_Threshold; }
};