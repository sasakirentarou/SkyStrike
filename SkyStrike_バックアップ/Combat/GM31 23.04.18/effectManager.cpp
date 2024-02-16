#include "main.h"
//#include "renderer.h"
//#include "effectManager.h"
//#include "effectLoad.h"
//#include "player.h"
//#include "scene.h"
//#include "manager.h"
//
//EffectLoad* g_MuzzleFlash = new EffectLoad;
////EffectLoad* g_GroundBom = new EffectLoad;
//
//void EffectManager::Init()
//{
//	g_MuzzleFlash->Init("asset\\texture\\flash.png");
//	//g_GroundBom->Init("asset\\texture\\ground.png", true, "asset\\audio\\bom.wav");
//}
//
//void EffectManager::Uninit()
//{
//	g_MuzzleFlash->Uninit();
//	//g_GroundBom->Uninit();
//}
//
//void EffectManager::Update()
//{
//}
//
//void EffectManager::Draw()
//{
//	Scene* scene = Manager::GetScene();
//	auto player = scene->GetGameObject<Player>();
//
//	switch (m_Select)
//	{
//	case MUZZLE_FLASH:
//		g_MuzzleFlash->Draw(30, 1, 10, 3, 5.0f, 5.0f,
//							player->GetPosition() + player->GetTopQuaternion() * -1.6f);
//		break;
//
//	case GROUND_BOM:
//		//g_GroundBom->Draw(50, 1,10, 5, 5.0f, 5.0f, player->GetPosition() + player->GetTopQuaternion() * -1.6f);
//
//		break;
//
//	default:
//		break;
//	}
//}