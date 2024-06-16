#include "main.h"
#include "stealthSystem.h"

void StealthSystem::Init()
{
	m_Amount = MAX_STEALTH;
}

void StealthSystem::Active()
{
	m_Threshold += 0.02f;

	//���S�ɏ������Ƃ�
	if (m_Threshold > 1.1f)
		m_Threshold = 1.1f;

	//����
	m_Amount -= STEALTH_DOWN_SPEED;
}

void StealthSystem::Charge()
{
	m_Threshold -= 0.02f;

	//���S�ɏo���Ƃ�
	if (m_Threshold < 0.0f)
		m_Threshold = 0.0f;

	//�`���[�W
	if (m_Amount < MAX_STEALTH)
		m_Amount += STEALTH_UP_SPEED;
}