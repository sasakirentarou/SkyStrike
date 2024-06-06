#include "main.h"
#include "textureManager.h"
#include "textureLoad.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "enemyJet.h"
#include "input.h"
#include "inputx.h"
#include "audio.h"

bool TextureManager::m_PitchFlipCheckFlg = true;
bool TextureManager::m_GameOverFlg = false;
int TextureManager::m_ResultTimer = 0;
float TextureManager::m_BGMPin = 570.0f;
float TextureManager::m_SEPin = 570.0f;

float Scene::m_BGMVolume{};
float Scene::m_SEVolume{};


TextureLoad* g_TitleLogo = new TextureLoad;
TextureLoad* g_TitleButton = new TextureLoad;

TextureLoad* g_MenuPlay = new TextureLoad;
TextureLoad* g_MenuWeapon = new TextureLoad;
TextureLoad* g_MenuDevelopment = new TextureLoad;
TextureLoad* g_MenuOption = new TextureLoad;
TextureLoad* g_MenuExit = new TextureLoad;
TextureLoad* g_MenuSelect = new TextureLoad;
TextureLoad* g_MenuBack = new TextureLoad;

TextureLoad* g_MissionTutorial = new TextureLoad;
TextureLoad* g_Mission01 = new TextureLoad;

TextureLoad* g_OptionBGM = new TextureLoad;
TextureLoad* g_OptionSE = new TextureLoad;
TextureLoad* g_OptionBGMPin = new TextureLoad;
TextureLoad* g_OptionSEPin = new TextureLoad;
TextureLoad* g_OptionPitchFlip = new TextureLoad;
TextureLoad* g_OptionPitchFlipBox = new TextureLoad;
TextureLoad* g_OptionPitchFlipCheck = new TextureLoad;

TextureLoad* g_GameWeaponSelect = new TextureLoad;
TextureLoad* g_GameMissileAmount = new TextureLoad;
TextureLoad* g_GameLongMissileAmount = new TextureLoad;
TextureLoad* g_GameFlareAmount = new TextureLoad;
TextureLoad* g_GameStealthAmount = new TextureLoad;
TextureLoad* g_GameMissileNumber = new TextureLoad;
TextureLoad* g_GameSpecialNumber = new TextureLoad;
TextureLoad* g_GameFlareNumber = new TextureLoad;
TextureLoad* g_GameStealthNumber = new TextureLoad;
TextureLoad* g_GameReport = new TextureLoad;
TextureLoad* g_GameDestroyed = new TextureLoad;
TextureLoad* g_GameMiss = new TextureLoad;
TextureLoad* g_GameHit = new TextureLoad;
TextureLoad* g_GameMissionUpdate = new TextureLoad;
TextureLoad* g_GameMissionAcco = new TextureLoad;
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
TextureLoad* g_GameTime = new TextureLoad;

TextureLoad* g_TutorialBox = new TextureLoad;
TextureLoad* g_TutorialBoxLine = new TextureLoad;
TextureLoad* g_TutorialInstructor = new TextureLoad;
TextureLoad* g_TutorialStart = new TextureLoad;
TextureLoad* g_TutorialRole = new TextureLoad;
TextureLoad* g_TutorialPitch = new TextureLoad;
TextureLoad* g_TutorialYaw = new TextureLoad;
TextureLoad* g_TutorialSpeed = new TextureLoad;
TextureLoad* g_TutorialMissile00 = new TextureLoad;
TextureLoad* g_TutorialMissile01 = new TextureLoad;
TextureLoad* g_TutorialMissile02 = new TextureLoad;
TextureLoad* g_TutorialBattle = new TextureLoad;
TextureLoad* g_TutorialClear = new TextureLoad;

TextureLoad* g_ResultLogo = new TextureLoad;
TextureLoad* g_ResultS = new TextureLoad;
TextureLoad* g_ResultA = new TextureLoad;
TextureLoad* g_ResultB = new TextureLoad;
TextureLoad* g_ResultC = new TextureLoad;
TextureLoad* g_ResultD = new TextureLoad;
TextureLoad* g_ResultClear = new TextureLoad;
TextureLoad* g_ResultFailed = new TextureLoad;
TextureLoad* g_ResultTime = new TextureLoad;
TextureLoad* g_ResultTitle = new TextureLoad;
TextureLoad* g_ResultExit = new TextureLoad;

void TextureManager::Init()
{
	m_Scene = Manager::GetScene();
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();
	m_EnemyNumber = enemys.size();

	g_TitleLogo->Init("asset/texture/title.png");
	g_TitleButton->Init("asset/texture/startbutton01.png");

	g_MenuPlay->Init("asset/texture/mission.png");
	g_MenuWeapon->Init("asset/texture/weapon.png");
	g_MenuDevelopment->Init("asset/texture/development.png");
	g_MenuOption->Init("asset/texture/option.png");
	g_MenuExit->Init("asset/texture/exit.png");
	g_MenuSelect->Init("asset/texture/menuselect01.png");
	g_MenuBack->Init("asset/texture/back.png");

	g_MissionTutorial->Init("asset/texture/tutorial.png");
	g_Mission01->Init("asset/texture/mission01.png");

	g_OptionBGM->Init("asset/texture/bgm.png");
	g_OptionSE->Init("asset/texture/se.png");
	g_OptionBGMPin->Init("asset/texture/whitebox.png");
	g_OptionSEPin->Init("asset/texture/whitebox.png");
	g_OptionPitchFlip->Init("asset/texture/pitchflip.png");
	g_OptionPitchFlipBox->Init("asset/texture/checkbox.png");
	g_OptionPitchFlipCheck->Init("asset/texture/check.png");

	g_GameWeaponSelect->Init("asset/texture/selectweapon.png");
	g_GameMissileAmount->Init("asset/texture/missileamount.png");
	g_GameLongMissileAmount->Init("asset/texture/longmissileamount.png");
	g_GameFlareAmount->Init("asset/texture/flareamount.png");
	g_GameStealthAmount->Init("asset/texture/stealthamount.png");
	g_GameMissileNumber->Init("asset/texture/number.png");
	g_GameSpecialNumber->Init("asset/texture/number.png");
	g_GameFlareNumber->Init("asset/texture/number.png");
	g_GameStealthNumber->Init("asset/texture/number.png");
	g_GameReport->Init("asset/texture/report.png");
	g_GameDestroyed->Init("asset/texture/destroy.png");
	g_GameMiss->Init("asset/texture/miss.png");
	g_GameHit->Init("asset/texture/hit.png");
	g_GameMissionUpdate->Init("asset/texture/missionupdate.png");
	g_GameMissionAcco->Init("asset/texture/missionacco.png");
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
	g_GameTime->Init("asset/texture/time.png");

	g_TutorialBox->Init("asset/texture/whitebox.png");
	g_TutorialBoxLine->Init("asset/texture/whitebox.png");
	g_TutorialInstructor->Init("asset/texture/instructor.png");
	g_TutorialStart->Init("asset/texture/messagestart.png");
	g_TutorialRole->Init("asset/texture/messagerole.png");
	g_TutorialPitch->Init("asset/texture/messagepitch.png");
	g_TutorialYaw->Init("asset/texture/messageyaw.png");
	g_TutorialSpeed->Init("asset/texture/messagespeed.png");
	g_TutorialMissile00->Init("asset/texture/messagemissile00.png");
	g_TutorialMissile01->Init("asset/texture/messagemissile01.png");
	g_TutorialMissile02->Init("asset/texture/messagemissile02.png");
	g_TutorialBattle->Init("asset/texture/messagebattle.png");
	g_TutorialClear->Init("asset/texture/messageclear.png");

	g_ResultLogo->Init("asset/texture/resultlogo.png");
	g_ResultS->Init("asset/texture/s.png");
	g_ResultA->Init("asset/texture/a.png");
	g_ResultB->Init("asset/texture/b.png");
	g_ResultC->Init("asset/texture/c.png");
	g_ResultD->Init("asset/texture/d.png");
	g_ResultClear->Init("asset/texture/clear.png");
	g_ResultFailed->Init("asset/texture/failed.png");
	g_ResultTime->Init("asset/texture/number.png");
	g_ResultTitle->Init("asset/texture/titlechange.png");
	g_ResultExit->Init("asset/texture/exit.png");

	g_ResultS->SetAlpha(0.0f);
	g_ResultA->SetAlpha(0.0f);
	g_ResultB->SetAlpha(0.0f);
	g_ResultC->SetAlpha(0.0f);
	g_ResultD->SetAlpha(0.0f);
	g_ResultClear->SetAlpha(0.0f);
	g_ResultFailed->SetAlpha(0.0f);

	m_MissionUpdateSE = AddComponet<Audio>();
	m_MissionUpdateSE->Load("asset\\audio\\missionupdate.wav");

	m_MissionAccoSE = AddComponet<Audio>();
	m_MissionAccoSE->Load("asset\\audio\\missionacco.wav");
}

void TextureManager::Uninit()
{
	g_TitleLogo->Uninit();
	g_TitleButton->Uninit();

	g_MenuPlay->Uninit();
	g_MenuWeapon->Uninit();
	g_MenuDevelopment->Uninit();
	g_MenuOption->Uninit();
	g_MenuExit->Uninit();
	g_MenuSelect->Uninit();
	g_MenuBack->Uninit();

	g_MissionTutorial->Uninit();
	g_Mission01->Uninit();

	g_OptionBGM->Uninit();
	g_OptionSE->Uninit();
	g_OptionBGMPin->Uninit();
	g_OptionSEPin->Uninit();
	g_OptionPitchFlip->Uninit();
	g_OptionPitchFlipBox->Uninit();
	g_OptionPitchFlipCheck->Uninit();

	g_GameWeaponSelect->Uninit();
	g_GameMissileAmount->Uninit();
	g_GameLongMissileAmount->Uninit();
	g_GameFlareAmount->Uninit();
	g_GameStealthAmount->Uninit();
	g_GameMissileNumber->Uninit();
	g_GameSpecialNumber->Uninit();
	g_GameFlareNumber->Uninit();
	g_GameStealthNumber->Uninit();
	g_GameReport->Uninit();
	g_GameDestroyed->Uninit();
	g_GameMiss->Uninit();
	g_GameHit->Uninit();
	g_GameMissionUpdate->Uninit();
	g_GameMissionAcco->Uninit();
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
	g_GameTime->Uninit();

	g_TutorialBox->Uninit();
	g_TutorialBoxLine->Uninit();
	g_TutorialInstructor->Uninit();
	g_TutorialStart->Uninit();
	g_TutorialRole->Uninit();
	g_TutorialPitch->Uninit();
	g_TutorialYaw->Uninit();
	g_TutorialSpeed->Uninit();
	g_TutorialMissile00->Uninit();
	g_TutorialMissile01->Uninit();
	g_TutorialMissile02->Uninit();
	g_TutorialBattle->Uninit();
	g_TutorialClear->Uninit();

	g_ResultLogo->Uninit();
	g_ResultS->Uninit();
	g_ResultA->Uninit();
	g_ResultB->Uninit();
	g_ResultC->Uninit();
	g_ResultD->Uninit();
	g_ResultClear->Uninit();
	g_ResultFailed->Uninit();
	g_ResultTime->Uninit();
	g_ResultTitle->Uninit();
	g_ResultExit->Uninit();
}

void TextureManager::Update()
{
	if (m_BGMPin < MIN_BGMPIN)
		m_BGMPin = MIN_BGMPIN;
	if (m_BGMPin > MAX_BGMPIN)
		m_BGMPin = MAX_BGMPIN;

	if (m_SEPin < MIN_SEPIN)
		m_SEPin = MIN_SEPIN;
	if (m_SEPin > MAX_SEPIN)
		m_SEPin = MAX_SEPIN;

	//正規化した値を代入
	Scene::m_BGMVolume =
		(m_BGMPin - MAX_BGMPIN) /
		(MIN_BGMPIN - MAX_BGMPIN) *
		(1.0f - 0.0f) + 0.0f;
	//正規化した値を代入
	Scene::m_SEVolume =
		(m_SEPin - MAX_SEPIN) /
		(MIN_SEPIN - MAX_SEPIN) *
		(1.0f - 0.0f) + 0.0f;

	m_MissionUpdateSE->Volume(Scene::m_SEVolume * 0.05f * 2);
	m_MissionAccoSE->Volume(Scene::m_SEVolume * 0.05f * 2);
	
	//ImGui
	//ImGui::SetNextWindowSize(ImVec2(400, 250));
	//ImGui::Begin("Texture");
	//ImGui::InputFloat("SelectPos", &m_SelectPos);
	//ImGui::InputFloat("BGMPinPos", &m_BGMPin);
	//ImGui::InputFloat("SEPinPos", &m_SEPin);
	//ImGui::End();
}

void TextureManager::Draw()
{
	auto player = m_Scene->GetGameObject<Player>();

	switch (m_SceneTexture)
	{
	case TEXTURE_LOGO: //Logo
		g_TitleLogo->Draw((SCREEN_WIDTH / 2) - 400, (SCREEN_HEIGHT / 2) - 500);
		g_TitleLogo->SetSize(800.0f, 400.0f);

		g_TitleButton->Draw((SCREEN_WIDTH / 2) - 200, (SCREEN_HEIGHT / 2) + 250);
		g_TitleButton->SetSize(400.0f, 100.0f);
		g_TitleButton->AlphaBlinking(60);
		break;

	case TEXTURE_MENU: //Menu
		g_MenuSelect->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + m_SelectPos);
		g_MenuSelect->SetSize(600.0f, 50.0f);
		g_MenuSelect->SetRGB(0.5f, 0.5f, 1.0f);


		g_MenuPlay->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) - 115);
		g_MenuPlay->SetSize(200.0f, 100.0f);

		g_MenuWeapon->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) - 55);
		g_MenuWeapon->SetSize(200.0f, 100.0f);
		g_MenuDevelopment->Draw((SCREEN_WIDTH / 2) - 600, (SCREEN_HEIGHT / 2) - 40);
		g_MenuDevelopment->SetSize(150.0f, 75.0f);

		g_MenuOption->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + 5);
		g_MenuOption->SetSize(200.0f, 100.0f);

		g_MenuExit->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + 65);
		g_MenuExit->SetSize(200.0f, 100.0f);

		break;

	case TEXTURE_MISSION: //Mission
		g_MenuSelect->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + m_SelectPos);
		g_MenuSelect->SetSize(600.0f, 50.0f);
		g_MenuSelect->SetRGB(0.5f, 0.5f, 1.0f);

		g_MissionTutorial->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) - 115);
		g_MissionTutorial->SetSize(230.0f, 100.0f);

		g_Mission01->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) - 55);
		g_Mission01->SetSize(250.0f, 100.0f);

		g_MenuBack->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + 5);
		g_MenuBack->SetSize(200.0f, 100.0f);

		break;

	case TEXTURE_WEAPON: //Weapon

		break;

	case TEXTURE_OPTION: //Option
		g_MenuSelect->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + m_SelectPos);
		g_MenuSelect->SetSize(600.0f, 50.0f);
		g_MenuSelect->SetRGB(0.5f, 0.5f, 1.0f);

		g_OptionBGM->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) - 110);
		g_OptionBGM->SetSize(400.0f, 80.0f);

		g_OptionSE->Draw((SCREEN_WIDTH / 2) - 770, (SCREEN_HEIGHT / 2) - 50);
		g_OptionSE->SetSize(370.0f, 80.0f);

		g_OptionBGMPin->Draw((SCREEN_WIDTH / 2) - m_BGMPin, (SCREEN_HEIGHT / 2) - 95);
		g_OptionBGMPin->SetSize(8.0f, 40.0f);
		g_OptionBGMPin->SetRGB(m_BGMPinColor.r, m_BGMPinColor.g, m_BGMPinColor.b);

		g_OptionSEPin->Draw((SCREEN_WIDTH / 2) - m_SEPin, (SCREEN_HEIGHT / 2) - 35);
		g_OptionSEPin->SetSize(8.0f, 40.0f);
		g_OptionSEPin->SetRGB(m_SEPinColor.r, m_SEPinColor.g, m_SEPinColor.b);

		g_OptionPitchFlip->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + 5);
		g_OptionPitchFlip->SetSize(200.0f, 100.0f);

		g_OptionPitchFlipBox->Draw((SCREEN_WIDTH / 2) - 600, (SCREEN_HEIGHT / 2) + 25);
		g_OptionPitchFlipBox->SetSize(40.0f, 40.0f);
		g_OptionPitchFlipBox->SetRGB(m_PitchFlipCheckColor.r, m_PitchFlipCheckColor.g, m_PitchFlipCheckColor.b);

		if (m_PitchFlipCheckFlg)
		{
			g_OptionPitchFlipCheck->Draw((SCREEN_WIDTH / 2) - 600, (SCREEN_HEIGHT / 2) + 25);
			g_OptionPitchFlipCheck->SetSize(40.0f, 40.0f);
			g_OptionPitchFlipCheck->SetRGB(m_PitchFlipCheckColor.r, m_PitchFlipCheckColor.g, m_PitchFlipCheckColor.b);
		}

		g_MenuBack->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + 65);
		g_MenuBack->SetSize(200.0f, 100.0f);

		break;

	case TEXTURE_GAME: //Game
		//select
		if(!player->GetWeaponChange())
			g_GameWeaponSelect->Draw(SCREEN_WIDTH - 360, SCREEN_HEIGHT - 510);
		else
			g_GameWeaponSelect->Draw(SCREEN_WIDTH - 360, SCREEN_HEIGHT - 470);

		g_GameWeaponSelect->SetSize(15.0f, 25.0f);
		g_GameWeaponSelect->SetRGB(0.0f,1.0f,0.0f);

		//AmountUI
		//missile
		g_GameMissileAmount->Draw(SCREEN_WIDTH - 350, SCREEN_HEIGHT - 520);
		g_GameMissileAmount->SetSize(120.0f, 65.0f);
		if (player->GetMissileAmount() == 0)
		{
			g_GameMissileAmount->SetRGB(1.0f, 0.0f, 0.0f);
		}
		else
		{
			g_GameMissileAmount->SetRGB(0.0f, 1.0f, 0.0f);
		}

		g_GameMissileNumber->Draw(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 515, true);
		g_GameMissileNumber->SetSize(22.5f, 30.0f);
		g_GameMissileNumber->SetNumberOption(3, 20.0f, player->GetMissileAmount());

		//specialweapon
		g_GameLongMissileAmount->Draw(SCREEN_WIDTH - 350, SCREEN_HEIGHT - 480);
		g_GameLongMissileAmount->SetSize(145.0f, 65.0f);
		if (player->GetSpecialAmount() == 0)
		{
			g_GameLongMissileAmount->SetRGB(1.0f, 0.0f, 0.0f);
		}
		else
		{
			g_GameLongMissileAmount->SetRGB(0.0f, 1.0f, 0.0f);
		}

		g_GameSpecialNumber->Draw(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 475, true);
		g_GameSpecialNumber->SetSize(22.5f, 30.0f);
		g_GameSpecialNumber->SetNumberOption(3, 20.0f, player->GetSpecialAmount());

		//flare
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
		g_GameFlareNumber->SetNumberOption(1, 0.0f, player->GetFlareAmount());


		//stealth
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
		g_GameStealthNumber->SetNumberOption(3, 20.0f, player->GetStealthAmount());



		//ReportUI
		if (m_DesReportFlg && m_DestroyCount < 60 * 2)//Destroyed
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

		g_GameTime->Draw(SCREEN_WIDTH - 380.0f,10.0f);
		g_GameTime->SetSize(120.0f,60.0f);
		g_GameTime->SetRGB(0.0f, 1.0f, 0.0f);

		//ClearUI
		if (m_EnemyNumber == 0)
		{
			m_AccoCount++;
			if(m_AccoCount > 60 * 8)
			{
				if (m_AccoCount == (60 * 8) + 1)
				{
					m_MissionAccoSE->Play(false);
				}

				g_GameReport->Draw((SCREEN_WIDTH / 2) - 250.0f, (SCREEN_HEIGHT / 2) - 250.0f, false);
				g_GameReport->SetSize(500.0f, 30.0f);
				g_GameReport->SetRGB(0.0f, 0.0f, 1.0f);
				g_GameReport->SetAlpha(0.5f);

				g_GameMissionAcco->Draw((SCREEN_WIDTH / 2) - 125.0f, (SCREEN_HEIGHT / 2) - 255.0f, false);
				g_GameMissionAcco->SetSize(250.0f, 50.0f);
			}

			if (m_AccoCount > 60 * 12)
			{
				m_MissionClear = true;
			}
		}

		break;

	case TEXTURE_TUTORIAL:

		//AmountUI
		g_GameMissileAmount->Draw(SCREEN_WIDTH - 350, SCREEN_HEIGHT - 480);
		g_GameMissileAmount->SetSize(120.0f, 65.0f);
		if (player->GetMissileAmount() == 0)
		{
			g_GameMissileAmount->SetRGB(1.0f, 0.0f, 0.0f);
		}
		else
		{
			g_GameMissileAmount->SetRGB(0.0f, 1.0f, 0.0f);
		}

		g_GameMissileNumber->Draw(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 475, true);
		g_GameMissileNumber->SetSize(22.5f, 30.0f);
		g_GameMissileNumber->SetNumberOption(3, 20.0f, player->GetMissileAmount());


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
		g_GameFlareNumber->SetNumberOption(1, 0.0f, player->GetFlareAmount());


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
		g_GameStealthNumber->SetNumberOption(3, 20.0f, player->GetStealthAmount());



		//ReportUI
		if (m_DesReportFlg && m_DestroyCount < 60 * 2 && !m_UpdateReportFlg)//Destroyed
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
		else if (m_MissReportFlg && m_MissCount < 60 && !m_DesReportFlg && !m_UpdateReportFlg)//Miss
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
		else if (m_HitReportFlg && m_HitCount < 60 && !m_DesReportFlg && !m_UpdateReportFlg)//Hit
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
		else if (m_UpdateReportFlg && m_UpdateCount < 60 * 2)//Update
		{
			if(m_UpdateCount == 0)
			{
				m_MissionUpdateSE->Play(false);
			}

			m_UpdateCount++;
			m_DestroyCount = 0;
			m_MissCount = 0;
			m_HitCount = 0;
			m_DesReportFlg = false;
			m_MissReportFlg = false;
			m_HitReportFlg = false;

			g_GameReport->Draw((SCREEN_WIDTH / 2) - 250.0f, (SCREEN_HEIGHT / 2) - 250.0f, false);
			g_GameReport->SetSize(500.0f, 30.0f);
			g_GameReport->SetRGB(0.0f, 0.0f, 1.0f);
			g_GameReport->SetAlpha(0.5f);

			g_GameMissionUpdate->Draw((SCREEN_WIDTH / 2) - 125.0f, (SCREEN_HEIGHT / 2) - 255.0f, false);
			g_GameMissionUpdate->SetSize(250.0f, 50.0f);

		}
		else
		{
			m_DestroyCount = 0;
			m_MissCount = 0;
			m_HitCount = 0;
			m_UpdateCount = 0;
			m_DesReportFlg = false;
			m_MissReportFlg = false;
			m_HitReportFlg = false;
			m_UpdateReportFlg = false;
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



		//tutorial
		g_TutorialBox->Draw(30.0f, SCREEN_HEIGHT - 340.0f);
		g_TutorialBox->SetSize(900.0f,300.0f);
		g_TutorialBox->SetAlpha(0.7f);
		g_TutorialBox->SetRGB(0.3f, 0.3f, 0.3f);

		g_TutorialBoxLine->Draw(30.0f, SCREEN_HEIGHT - 295.0f);
		g_TutorialBoxLine->SetSize(900.0f, 2.0f);
		g_TutorialBoxLine->SetRGB(0.0f, 1.0f, 0.0f);

		g_TutorialInstructor->Draw(30.0f, SCREEN_HEIGHT - 350.0f);
		g_TutorialInstructor->SetSize(110.0f, 70.0f);
		g_TutorialInstructor->SetRGB(0.0f,1.0f,0.0f);

		switch (m_TutorialProcess)
		{
		case 0://開始
			player->SetRoleLock(true);
			player->SetPitchLock(true);
			player->SetYawLock(true);
			player->SetEngineLock(true);

			m_TutorialCount++;
			g_TutorialStart->Draw(30.0f, SCREEN_HEIGHT - 290.0f);
			g_TutorialStart->SetSize(450.0f, 150.0f);
			g_TutorialStart->SetRGB(0.0f, 1.0f, 0.0f);

			if (m_TutorialCount > 60 * 5)
			{
				m_TutorialCount = 0;
				m_UpdateReportFlg = true;
				m_TutorialProcess = 1;
			}

			break;
		case 1://ピッチ
			player->SetPitchLock(false);
			g_TutorialPitch->Draw(30.0f, SCREEN_HEIGHT - 290.0f);
			g_TutorialPitch->SetSize(700.0f, 150.0f);
			g_TutorialPitch->SetRGB(0.0f, 1.0f, 0.0f);

			m_TutorialCount++;
			if(m_TutorialCount > 60 * 3)
			{
				if (Input::GetKeyTrigger(VK_SHIFT) || InputX::GetThumbLeftY(0) > 0)
				{
					m_TutorialProcessLock00 = true;
				}
				if (Input::GetKeyTrigger(VK_CONTROL) || InputX::GetThumbLeftY(0) < 0)
				{
					m_TutorialProcessLock01 = true;
				}
			}

			if (m_TutorialProcessLock00 && m_TutorialProcessLock01 && InputX::GetThumbLeftY(0) == 0)
			{
				m_TutorialProcessLock00 = false;
				m_TutorialProcessLock01 = false;
				m_UpdateReportFlg = true;
				m_TutorialCount = 0;
				m_TutorialProcess = 2;
			}

			break;
		case 2://ロール
			player->SetRoleLock(false);
			g_TutorialRole->Draw(30.0f, SCREEN_HEIGHT - 290.0f);
			g_TutorialRole->SetSize(650.0f, 150.0f);
			g_TutorialRole->SetRGB(0.0f, 1.0f, 0.0f);

			m_TutorialCount++;
			if (m_TutorialCount > 60 * 3)
			{
				if (Input::GetKeyTrigger('A') || InputX::GetThumbLeftX(0) < 0)
				{
					m_TutorialProcessLock00 = true;
				}
				if (Input::GetKeyTrigger('D') || InputX::GetThumbLeftX(0) > 0)
				{
					m_TutorialProcessLock01 = true;
				}
			}

			if (m_TutorialProcessLock00 && m_TutorialProcessLock01 && InputX::GetThumbLeftX(0) == 0)
			{
				m_TutorialProcessLock00 = false;
				m_TutorialProcessLock01 = false;
				m_UpdateReportFlg = true;
				m_TutorialCount = 0;
				m_TutorialProcess = 3;
			}

			break;
		case 3://ヨー
			player->SetYawLock(false);
			g_TutorialYaw->Draw(30.0f, SCREEN_HEIGHT - 290.0f);
			g_TutorialYaw->SetSize(650.0f, 150.0f);
			g_TutorialYaw->SetRGB(0.0f, 1.0f, 0.0f);

			m_TutorialCount++;
			if (m_TutorialCount > 60 * 3)
			{
				if (Input::GetKeyTrigger('Q') || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_LEFT_SHOULDER))
				{
					m_TutorialProcessLock00 = true;
				}
				if (Input::GetKeyTrigger('E') || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_RIGHT_SHOULDER))
				{
					m_TutorialProcessLock01 = true;
				}
			}

			if (m_TutorialProcessLock00 && m_TutorialProcessLock01)
			{
				m_TutorialProcessLock00 = false;
				m_TutorialProcessLock01 = false;
				m_UpdateReportFlg = true;
				m_TutorialCount = 0;
				m_TutorialProcess = 4;
			}

			break;

		case 4://加速と減速
			player->SetEngineLock(false);
			g_TutorialSpeed->Draw(30.0f, SCREEN_HEIGHT - 290.0f);
			g_TutorialSpeed->SetSize(750.0f, 150.0f);
			g_TutorialSpeed->SetRGB(0.0f, 1.0f, 0.0f);

			m_TutorialCount++;
			if (m_TutorialCount > 60 * 3)
			{
				if (Input::GetKeyTrigger('W') || InputX::GetRightTrigger(0) > 0)
				{
					m_TutorialProcessLock00 = true;
				}
				if (Input::GetKeyTrigger('S') || InputX::GetLeftTrigger(0) > 0)
				{
					m_TutorialProcessLock01 = true;
				}
			}

			if (m_TutorialProcessLock00 && m_TutorialProcessLock01)
			{
				m_TutorialProcessLock00 = false;
				m_TutorialProcessLock01 = false;
				m_UpdateReportFlg = true;
				m_TutorialCount = 0;
				m_TutorialProcess = 5;
			}

			break;

		case 5://ミサイル
			if (m_TutorialCount == 0)
			{
				m_EnemyTuto00 = m_Scene->AddGameObject<EnemyJet>(1);
				m_EnemyTuto00->SetPosition(player->GetPosition() + player->GetForwardQuaternion() * 3000.0f);
				m_EnemyTuto00->SetStateLock(true);
			}

			m_TutorialCount++;
			if (m_TutorialCount < 60 * 6)
			{
				g_TutorialMissile00->Draw(30.0f, SCREEN_HEIGHT - 290.0f);
				g_TutorialMissile00->SetSize(750.0f, 150.0f);
				g_TutorialMissile00->SetRGB(0.0f, 1.0f, 0.0f);
			}
			else if(m_TutorialCount > 60 * 6)
			{
				g_TutorialMissile01->Draw(30.0f, SCREEN_HEIGHT - 290.0f);
				g_TutorialMissile01->SetSize(850.0f, 250.0f);
				g_TutorialMissile01->SetRGB(0.0f, 1.0f, 0.0f);

				if (m_EnemyTuto00->GetCrash())
				{
					m_TutorialCount = 0;
					m_TutorialProcess = 6;
				}
			}

			break;

		case 6://battle
			m_TutorialCount++;
			if (m_TutorialCount < 60 * 6)
			{
				g_TutorialMissile02->Draw(30.0f, SCREEN_HEIGHT - 290.0f);
				g_TutorialMissile02->SetSize(200.0f, 100.0f);
				g_TutorialMissile02->SetRGB(0.0f, 1.0f, 0.0f);
			}
			else if(m_TutorialCount > 60 * 6)
			{
				g_TutorialBattle->Draw(30.0f, SCREEN_HEIGHT - 290.0f);
				g_TutorialBattle->SetSize(550.0f, 250.0f);
				g_TutorialBattle->SetRGB(0.0f, 1.0f, 0.0f);

				if (m_EnemyTuto01->GetCrash())
				{
					m_TutorialCount = 0;
					m_TutorialProcess = 7;
				}
			}
			if (m_TutorialCount == 60 * 5)
			{
				m_EnemyTuto01 = m_Scene->AddGameObject<EnemyJet>(1);
				m_EnemyTuto01->SetPosition(player->GetPosition() + player->GetForwardQuaternion() * 4000.0f);
				player->SwitchTarget();
			}

			break;

		case 7://clear
			g_TutorialClear->Draw(30.0f, SCREEN_HEIGHT - 290.0f);
			g_TutorialClear->SetSize(650.0f, 150.0f);
			g_TutorialClear->SetRGB(0.0f, 1.0f, 0.0f);

			m_TutorialCount++;
			if (m_TutorialCount > 60 * 8 && m_TutorialCount < 60 * 12)
			{
				if (m_TutorialCount == (60 * 8) + 1)
				{
					m_MissionAccoSE->Play(false);
				}

				g_GameReport->Draw((SCREEN_WIDTH / 2) - 250.0f, (SCREEN_HEIGHT / 2) - 250.0f, false);
				g_GameReport->SetSize(500.0f, 30.0f);
				g_GameReport->SetRGB(0.0f, 0.0f, 1.0f);
				g_GameReport->SetAlpha(0.5f);

				g_GameMissionAcco->Draw((SCREEN_WIDTH / 2) - 125.0f, (SCREEN_HEIGHT / 2) - 255.0f, false);
				g_GameMissionAcco->SetSize(250.0f, 50.0f);
			}
			else if (m_TutorialCount > 60 * 12)
			{
				m_TutorialCount = 0;
				m_MissionClear = true;
			}

			break;

		default:
			break;
		}
		break;

	case TEXTURE_RESULT:
		g_ResultLogo->Draw(0.0f, 0.0f);
		g_ResultLogo->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

		g_MenuSelect->Draw((SCREEN_WIDTH / 2) - 900, (SCREEN_HEIGHT / 2) + 350);
		g_MenuSelect->SetSize(600.0f, 60.0f);
		g_MenuSelect->SetRGB(0.5f, 0.5f, 1.0f);

		g_ResultTitle->Draw((SCREEN_WIDTH / 2) - 900, (SCREEN_HEIGHT / 2) + 340);
		g_ResultTitle->SetSize(200.0f, 100.0f);
		g_ResultTitle->AlphaBlinking(60);

		g_ResultTime->Draw((SCREEN_WIDTH / 2) + 600, (SCREEN_HEIGHT / 2) + 400.0f, true);
		g_ResultTime->SetSize(50.0f, 60.0f);
		g_ResultTime->SetNumberOption(4, 60.0f, m_ResultTimer);

		//g_ResultExit->Draw((SCREEN_WIDTH / 2) - 800, (SCREEN_HEIGHT / 2) + 240);
		//g_ResultExit->SetSize(300.0f, 150.0f);

		m_ResultCount++;
		if (m_GameOverFlg)
		{
			if(m_ResultCount > 60)
			{
				m_FailedAlphaLevel += 0.01f;
				g_ResultFailed->Draw(100.0f, (SCREEN_HEIGHT / 2) - 390.0f);
				g_ResultFailed->SetSize(800.0f, 250.0f);
				g_ResultFailed->SetAlpha(m_FailedAlphaLevel);
			}
			if(m_ResultCount > 60 * 3)
			{
				m_RankAlphaLevel += 0.01f;
				g_ResultD->Draw((SCREEN_WIDTH / 2) + 125, (SCREEN_HEIGHT / 2) - 390.0f);
				g_ResultD->SetSize(650.0f, 650.0f);
				g_ResultD->SetAlpha(m_RankAlphaLevel);
			}
		}
		else
		{
			if (m_ResultCount > 60)
			{
				m_ClearAlphaLevel += 0.01f;
				g_ResultClear->Draw(100.0f, (SCREEN_HEIGHT / 2) - 390.0f);
				g_ResultClear->SetSize(650.0f, 250.0f);
				g_ResultClear->SetAlpha(m_ClearAlphaLevel);
			}

			if (m_ResultCount > 60 * 3)
			{
				m_RankAlphaLevel += 0.01f;
				if (m_ResultTimer <= TIME_S)//S
				{
					g_ResultS->Draw((SCREEN_WIDTH / 2) + 125, (SCREEN_HEIGHT / 2) - 390.0f);
					g_ResultS->SetSize(650.0f, 650.0f);
					g_ResultS->SetAlpha(m_RankAlphaLevel);
				}
				else if (m_ResultTimer <= TIME_A && m_ResultTimer > TIME_S)//A
				{
					g_ResultA->Draw((SCREEN_WIDTH / 2) + 125, (SCREEN_HEIGHT / 2) - 390.0f);
					g_ResultA->SetSize(650.0f, 650.0f);
					g_ResultA->SetAlpha(m_RankAlphaLevel);
				}
				else if (m_ResultTimer <= TIME_B && m_ResultTimer > TIME_A)//B
				{
					g_ResultB->Draw((SCREEN_WIDTH / 2) + 125, (SCREEN_HEIGHT / 2) - 390.0f);
					g_ResultB->SetSize(650.0f, 650.0f);
					g_ResultB->SetAlpha(m_RankAlphaLevel);
				}
				else if (m_ResultTimer <= TIME_C && m_ResultTimer > TIME_B)//C
				{
					g_ResultC->Draw((SCREEN_WIDTH / 2) + 125, (SCREEN_HEIGHT / 2) - 390.0f);
					g_ResultC->SetSize(650.0f, 650.0f);
					g_ResultC->SetAlpha(m_RankAlphaLevel);
				}
				else if (m_ResultTimer > TIME_C)//D
				{
					g_ResultD->Draw((SCREEN_WIDTH / 2) + 125, (SCREEN_HEIGHT / 2) - 390.0f);
					g_ResultD->SetSize(650.0f, 650.0f);
					g_ResultD->SetAlpha(m_RankAlphaLevel);
				}
			}
		}
		break;

	default:
		break;
	}
}