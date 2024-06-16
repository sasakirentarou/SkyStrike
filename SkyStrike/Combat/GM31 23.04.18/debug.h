#pragma once
//#include "scene.h"
//#include "player.h"
//#include "enemyJet.h"

class Debug
{
private:
	static bool m_DebugMode;
	static float m_Frame;
public:
	static void Update();
	void DebugMode(bool mode) { m_DebugMode = mode; }
};