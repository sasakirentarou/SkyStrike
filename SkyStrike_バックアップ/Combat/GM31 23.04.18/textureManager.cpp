#include "main.h"
#include "textureManager.h"
#include "textureLoad.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "enemy.h"

TextureLoad* g_TitleLogo = new TextureLoad;
TextureLoad* g_TitleButton = new TextureLoad;
TextureLoad* g_MenuPlay = new TextureLoad;
TextureLoad* g_MenuWeapon = new TextureLoad;
TextureLoad* g_MenuOption = new TextureLoad;
TextureLoad* g_MenuExit = new TextureLoad;
TextureLoad* g_MenuSelect = new TextureLoad;
TextureLoad* g_GameMissileAmount = new TextureLoad;
TextureLoad* g_GameFlareAmount = new TextureLoad;
TextureLoad* g_GameStealthAmount = new TextureLoad;
TextureLoad* g_GameMIssileNumber = new TextureLoad;
TextureLoad* g_GameFlareNumber = new TextureLoad;
TextureLoad* g_GameStealthNumber = new TextureLoad;
TextureLoad* g_GameReport = new TextureLoad;
TextureLoad* g_GameDestroyed = new TextureLoad;
TextureLoad* g_GameMiss = new TextureLoad;
TextureLoad* g_GameHit = new TextureLoad;
TextureLoad* g_GameAlertBox = new TextureLoad;
TextureLoad* g_GameMissileAlert = new TextureLoad;
TextureLoad* g_GameEnemyUI = new TextureLoad;
TextureLoad* g_GameEnemyNumber = new TextureLoad;
TextureLoad* g_GameSpeedBox = new TextureLoad;
TextureLoad* g_GameAltBox = new TextureLoad;
TextureLoad* g_GameSpeedNumber = new TextureLoad;
TextureLoad* g_GameAltNumber = new TextureLoad;
TextureLoad* g_GameSpeed = new TextureLoad;
TextureLoad* g_GameAlt = new TextureLoad;

void TextureManager::Init()
{
	m_Scene = Manager::GetScene();
	auto enemys = m_Scene->GetGameObjects<Enemy>();
	m_EnemyNumber = enemys.size();

	g_TitleLogo->Init("asset/texture/title.png");
	g_TitleButton->Init("asset/texture/startbutton.png");
	g_MenuPlay->Init("asset/texture/mission.png");
	g_MenuWeapon->Init("asset/texture/weapon.png");
	g_MenuOption->Init("asset/texture/option.png");
	g_MenuExit->Init("asset/texture/exit.png");
	g_MenuSelect->Init("asset/texture/menuselect01.png");
	g_GameMissileAmount->Init("asset/texture/missileamount.png");
	g_GameFlareAmount->Init("asset/texture/flareamount.png");
	g_GameStealthAmount->Init("asset/texture/stealthamount.png");
	g_GameMIssileNumber->Init("asset/texture/number.png");
	g_GameFlareNumber->Init("asset/texture/number.png");
	g_GameStealthNumber->Init("asset/texture/number.png");
	g_GameReport->Init("asset/texture/report.png");
	g_GameDestroyed->Init("asset/texture/destroy.png");
	g_GameMiss->Init("asset/texture/miss.png");
	g_GameHit->Init("asset/texture/hit.png");
	g_GameAlertBox->Init("asset/texture/alertbox.png");
	g_GameMissileAlert->Init("asset/texture/missilealert.png");
	g_GameEnemyUI->Init("asset/texture/enemy.png");
	g_GameEnemyNumber->Init("asset/texture/number.png");
	g_GameSpeedBox->Init("asset/texture/alertbox.png");
	g_GameAltBox->Init("asset/texture/alertbox.png");
	g_GameSpeedNumber->Init("asset/texture/number.png");
	g_GameAltNumber->Init("asset/texture/number.png");
	g_GameSpeed->Init("asset/texture/speed.png");
	g_GameAlt->Init("asset/texture/alt.png");
}

void TextureManager::Uninit()
{
	g_TitleLogo->Uninit();
	g_TitleButton->Uninit();

	g_MenuPlay->Uninit();
	g_MenuWeapon->Uninit();
	g_MenuOption->Uninit();
	g_MenuExit->Uninit();
	g_MenuSelect->Uninit();

	g_GameMissileAmount->Uninit();
	g_GameFlareAmount->Uninit();
	g_GameStealthAmount->Uninit();
	g_GameMIssileNumber->Uninit();
	g_GameFlareNumber->Uninit();
	g_GameStealthNumber->Uninit();
	g_GameReport->Uninit();
	g_GameDestroyed->Uninit();
	g_GameMiss->Uninit();
	g_GameHit->Uninit();
	g_GameAlertBox->Uninit();
	g_GameMissileAlert->Uninit();
	g_GameEnemyUI->Uninit();
	g_GameEnemyNumber->Uninit();
	g_GameSpeedBox->Uninit();
	g_GameAltBox->Uninit();
	g_GameSpeedNumber->Uninit();
	g_GameAltNumber->Uninit();
	g_GameSpeed->Uninit();
	g_GameAlt->Uninit();
}

void TextureManager::Update()
{
	//ImGui::SetNextWindowSize(ImVec2(400, 250));
	//ImGui::Begin("Select");
	//ImGui::InputFloat("pos", &m_SelectPos);
	//ImGui::End();
}

void TextureManager::Draw()
{
	if(m_LogoFlg)
	{
		g_TitleLogo->Draw((SCREEN_WIDTH / 2) - 400, (SCREEN_HEIGHT / 2) - 500);
		g_TitleLogo->SetSize(800.0f, 400.0f);

		g_TitleButton->Draw((SCREEN_WIDTH / 2) - 200, (SCREEN_HEIGHT / 2) + 250);
		g_TitleButton->SetSize(400.0f, 100.0f);
		g_TitleButton->AlphaBlinking(60);
	}
	if (m_MenuFlg)
	{
		g_MenuSelect->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + m_SelectPos);
		g_MenuSelect->SetSize(600.0f, 80.0f);
		g_MenuSelect->SetRGB(1.0f,0.8f,0.5f);

		g_MenuPlay->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) - 120);
		g_MenuPlay->SetSize(300.0f, 150.0f);

		g_MenuWeapon->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) - 20);
		g_MenuWeapon->SetSize(300.0f, 150.0f);

		g_MenuOption->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + 80);
		g_MenuOption->SetSize(300.0f, 150.0f);

		g_MenuExit->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + 180);
		g_MenuExit->SetSize(250.0f, 150.0f);
	}
	if (m_WeaponFlg)
	{

	}
	if (m_OptionFlg)
	{

	}
	if (m_GameFlg)
	{
		Scene* scene = Manager::GetScene();
		auto player = scene->GetGameObject<Player>();

		//AmountUI
		g_GameMissileAmount->Draw(SCREEN_WIDTH - 350, SCREEN_HEIGHT - 480);
		g_GameMissileAmount->SetSize(120.0f,65.0f);
		if (player->GetMissileAmount() == 0)
		{
			g_GameMissileAmount->SetRGB(1.0f, 0.0f, 0.0f);
		}
		else
		{
			g_GameMissileAmount->SetRGB(0.0f, 1.0f, 0.0f);
		}

		g_GameMIssileNumber->Draw(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 475, true);
		g_GameMIssileNumber->SetSize(22.5f,30.0f);
		g_GameMIssileNumber->SetNumberOption(3, 20.0f,player->GetMissileAmount());


		g_GameFlareAmount->Draw(SCREEN_WIDTH - 350, SCREEN_HEIGHT - 440);
		g_GameFlareAmount->SetSize(120.0f, 65.0f);
		if (player->GetFlareAmount() == 0)
		{
			g_GameFlareAmount->SetRGB(1.0f, 0.0f, 0.0f);
		}
		else
		{
			g_GameFlareAmount->SetRGB(0.0f, 1.0f, 0.0f);
		}

		g_GameFlareNumber->Draw(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 435, true);
		g_GameFlareNumber->SetSize(22.5f, 30.0f);
		g_GameFlareNumber->SetNumberOption(1,0.0f, player->GetFlareAmount());


		g_GameStealthAmount->Draw(SCREEN_WIDTH - 350, SCREEN_HEIGHT - 400);
		g_GameStealthAmount->SetSize(120.0f, 60.0f);
		if (player->GetStealthAmount() <= 30)
		{
			g_GameStealthAmount->SetRGB(1.0f, 0.0f, 0.0f);
		}
		else if (player->GetStealthAmount() > 30 && player->GetStealthAmount() <= 60)
		{
			g_GameStealthAmount->SetRGB(1.0f, 1.0f, 0.0f);
		}
		else
		{
			g_GameStealthAmount->SetRGB(0.0f, 1.0f, 0.0f);
		}

		g_GameStealthNumber->Draw(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 395, true);
		g_GameStealthNumber->SetSize(22.5f, 30.0f);
		g_GameStealthNumber->SetNumberOption(3,20.0f, player->GetStealthAmount());



		//ReportUI
		if(m_DesReportFlg && m_DestroyCount < 60 * 2)//Destroyed
		{
			m_DestroyCount++;
			m_MissCount = 0;
			m_HitCount = 0;
			m_MissReportFlg = false;
			m_HitReportFlg = false;

			g_GameReport->Draw((SCREEN_WIDTH / 2) - 250.0f, (SCREEN_HEIGHT / 2) - 250.0f, false);
			g_GameReport->SetSize(500.0f, 30.0f);
			g_GameReport->SetRGB(0.0f, 1.0f, 0.0f);
			g_GameReport->SetAlpha(0.5f);

			g_GameDestroyed->Draw((SCREEN_WIDTH / 2) - 75.0f, (SCREEN_HEIGHT / 2) - 255.0f, false);
			g_GameDestroyed->SetSize(150.0f, 50.0f);

		}
		else if (m_MissReportFlg && m_MissCount < 60)//Miss
		{
			m_MissCount++;
			m_DestroyCount = 0;
			m_HitCount = 0;
			m_DesReportFlg = false;
			m_HitReportFlg = false;
			g_GameReport->Draw((SCREEN_WIDTH / 2) - 250.0f, (SCREEN_HEIGHT / 2) - 250.0f, false);
			g_GameReport->SetSize(500.0f, 30.0f);
			g_GameReport->SetRGB(1.0f, 0.0f, 0.0f);
			g_GameReport->SetAlpha(0.5f);

			g_GameMiss->Draw((SCREEN_WIDTH / 2) - 75.0f, (SCREEN_HEIGHT / 2) - 255.0f, false);
			g_GameMiss->SetSize(150.0f, 50.0f);

		}
		else if (m_HitReportFlg && m_HitCount < 60 && !m_DesReportFlg)//Hit
		{
			m_HitCount++;
			m_DestroyCount = 0;
			m_MissCount = 0;
			m_DesReportFlg = false;
			m_MissReportFlg = false;
			g_GameReport->Draw((SCREEN_WIDTH / 2) - 250.0f, (SCREEN_HEIGHT / 2) - 250.0f, false);
			g_GameReport->SetSize(500.0f, 30.0f);
			g_GameReport->SetRGB(0.0f, 1.0f, 0.0f);
			g_GameReport->SetAlpha(0.5f);

			g_GameHit->Draw((SCREEN_WIDTH / 2) - 75.0f, (SCREEN_HEIGHT / 2) - 255.0f, false);
			g_GameHit->SetSize(150.0f, 50.0f);

		}
		else
		{
			m_DestroyCount = 0;
			m_MissCount = 0;
			m_HitCount = 0;
			m_DesReportFlg = false;
			m_MissReportFlg = false;
			m_HitReportFlg = false;
		}

		//Alert
		if (m_MissileAlertFlg)
		{
			g_GameAlertBox->Draw((SCREEN_WIDTH / 2) - 100.0f, (SCREEN_HEIGHT / 2) - 350.0f, false);
			g_GameAlertBox->SetSize(200.0f, 40.0f);
			g_GameAlertBox->SetRGB(1.0f, 0.0f, 0.0f);
			g_GameAlertBox->SetAlpha(0.8f);

			g_GameMissileAlert->Draw((SCREEN_WIDTH / 2) - 75.0f, (SCREEN_HEIGHT / 2) - 350.0f, false);
			g_GameMissileAlert->SetSize(150.0f, 50.0f);
			g_GameMissileAlert->SetRGB(1.0f, 0.0f, 0.0f);
			g_GameMissileAlert->SetAlpha(0.8f);
		}

		//EnemyAmountUI
		{
			g_GameEnemyUI->Draw(50.0f, 50.0f);
			g_GameEnemyUI->SetSize(200.0f, 70.0f);
			g_GameEnemyUI->SetRGB(0.0f, 1.0f, 0.0f);
			g_GameEnemyUI->SetAlpha(0.7f);

			g_GameEnemyNumber->Draw(250.0f, 60.0f, true);
			g_GameEnemyNumber->SetSize(27.5f, 35.0f);
			g_GameEnemyNumber->SetRGB(0.0f, 1.0f, 0.0f);
			g_GameEnemyNumber->SetAlpha(0.7f);
			g_GameEnemyNumber->SetNumberOption(2, 25.0f, m_EnemyNumber);
		}

		//Speed,Alt
		{
			g_GameSpeedBox->Draw((SCREEN_WIDTH / 2) - 350.0f, 400.0f);
			g_GameSpeedBox->SetSize(120.0f, 40.0f);
			g_GameSpeedBox->SetRGB(0.0f, 1.0f, 0.0f);
			g_GameSpeedBox->SetAlpha(0.8f);
			g_GameSpeedNumber->Draw((SCREEN_WIDTH / 2) - 260.0f, 405.0f, true);
			g_GameSpeedNumber->SetSize(20.0f, 30.0f);
			g_GameSpeedNumber->SetNumberOption(4, 25.0f, player->GetSpeed() * 1000.0f);
			g_GameSpeedNumber->SetAlpha(0.8f);
			g_GameSpeed->Draw((SCREEN_WIDTH / 2) - 400.0f, 360.0f);
			g_GameSpeed->SetSize(160.0f, 50.0f);
			g_GameSpeed->SetRGB(0.0f, 1.0f, 0.0f);


			g_GameAltBox->Draw((SCREEN_WIDTH / 2) + 230.0f, 400.0f);
			g_GameAltBox->SetSize(120.0f, 40.0f);
			g_GameAltBox->SetRGB(0.0f, 1.0f, 0.0f);
			g_GameAltBox->SetAlpha(0.8f);
			g_GameAltNumber->Draw((SCREEN_WIDTH / 2) + 320.0f, 405.0f, true);
			g_GameAltNumber->SetSize(20.0f, 30.0f);
			g_GameAltNumber->SetNumberOption(4, 25.0f, player->GetPosition().y);
			g_GameAltNumber->SetAlpha(0.8f);
			g_GameAlt->Draw((SCREEN_WIDTH / 2) + 170.0f, 360.0f);
			g_GameAlt->SetSize(160.0f, 50.0f);
			g_GameAlt->SetRGB(0.0f, 1.0f, 0.0f);
		}
	}
}