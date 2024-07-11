#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "titleLogo.h"
#include "input.h"
#include "inputx.h"
#include "fade.h"
#include "loading.h"
#include "startButton.h"
#include "dome.h"
#include "titleField.h"
#include "meshField.h"
#include "titleCamera.h"
#include "airport.h"
#include "teamJet.h"
#include "jet.h"
#include "textureManager.h"
#include "audio.h"
#include "enemyJet.h"
#include "fire.h"

//初期化
SceneState Scene::m_SceneState = SCENE_NONE;

void Title::Init()
{
	Scene::m_SceneState = SCENE_TITLE;

	//load
	TeamJet::Load();
	Fire::Load();

	//setloading
	Loading::SetGameLoad(false);
	Loading::SetTutorialLoad(false);

	//camera
	m_Camera = AddGameObject<TitleCamera>(0);

	//dome
	Dome* dome = AddGameObject<Dome>(1);
	dome->SetScale(D3DXVECTOR3(20000.0f, 20000.0f, 20000.0f));
	
	//field
	//MeshField* field = scene->AddGameObject<MeshField>(1);
	TitleField* field = AddGameObject<TitleField>(1);
	field->SetScale(D3DXVECTOR3(7.0f, 7.0f, 7.0f));

	//airport
	Airport* air = AddGameObject<Airport>(1);
	air->SetPosition(D3DXVECTOR3(1000.0f, 0.0f, 500.0f));
	air->SetRotation(D3DXVECTOR3(0.0f,0.0f,0.0f));
	air->SetScale(D3DXVECTOR3(6.0f, 6.0f, 6.0f));

	//parts
	//AddGameObject<RightWing>(1);//主翼
	//AddGameObject<LeftWing>(1);//主翼
	//AddGameObject<BackLeftWing>(1);//水平尾翼
	//AddGameObject<BackRightWing>(1);//水平尾翼
	//AddGameObject<LeftVertical>(1);//垂直尾翼
	//AddGameObject<RightVertical>(1);//垂直尾翼

	//player
	m_Jet = AddGameObject<Jet>(1);
	m_Jet->SetPosition(D3DXVECTOR3(0.0f,25.0f,10.0f));
	m_Jet->SetScale(D3DXVECTOR3(10.0f, 10.0f, 10.0f));

	//jet
	for(int i = 0; i < 3; i++)
	{
		TeamJet* jet_start = AddGameObject<TeamJet>(1);
		jet_start->SetPosition(D3DXVECTOR3(300.0f * i - 300.0f, 1050.0f, -500.0f));
		jet_start->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		jet_start->SetScale(D3DXVECTOR3(10.0f, 10.0f, 10.0f));
		jet_start->SetPatern(3);

		TeamJet* jet_takeoff = AddGameObject<TeamJet>(1);
		jet_takeoff->SetPosition(D3DXVECTOR3(-550.0f, 30.0f, 0.0f));
		jet_takeoff->SetScale(D3DXVECTOR3(10.0f, 10.0f, 10.0f));
		jet_takeoff->SetPatern(i);
	}

	// polygon
	m_Texture = AddGameObject<TextureManager>(2);
	m_Texture->SetSceneTexture(TEXTURE_LOGO);

	m_Fade = AddGameObject<Fade>(2);

	//SE
	m_SelectSE = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	m_SelectSE->Load("asset\\audio\\select.wav");

	m_ConfirmSE = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	m_ConfirmSE->Load("asset\\audio\\confirm.wav");

	m_StartSE = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	m_StartSE->Load("asset\\audio\\start.wav");

	m_ErrorSE = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	m_ErrorSE->Load("asset\\audio\\miss.wav");

	//BGM
	m_BGM = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	m_BGM->Load("asset\\audio\\title_bgm.wav");
	m_BGM->Volume(0.0f);

	SetPause(true);
}

void Title::Uninit()
{
	TeamJet::Unload();
	Fire::Unload();
	Scene::Uninit();
}

void Title::Update()
{
	Scene::Update();

	//マウス座標取得
	//m_MousePos.x = GetMouse2DX();
	//m_MousePos.y = GetMouse2DY();

	//BGMボリューム
	if(m_Start)
		m_BGM->Volume(Scene::m_BGMVolume * (0.08f * 2));

	//SEボリューム
	m_SelectSE->Volume(Scene::m_SEVolume * (0.3f * 2));
	m_ConfirmSE->Volume(Scene::m_SEVolume * (0.3f * 2));
	m_StartSE->Volume(Scene::m_SEVolume * (0.3f * 2));
	m_ErrorSE->Volume(Scene::m_SEVolume * (0.3f * 2));



	if (Input::GetKeyTrigger(VK_UP) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_UP) && m_Start)
	{
		m_SelectSE->Play(false);
		m_Texture->SetSelectUpDown(false, m_SelectDownMax);
	}
	else if (Input::GetKeyTrigger(VK_DOWN) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_DOWN) && m_Start)
	{
		m_SelectSE->Play(false);
		m_Texture->SetSelectUpDown(true, m_SelectDownMax);
	}



	switch (m_SelectMode)
	{
	case SELECT_NONE:
		if (Input::GetKeyTrigger(VK_SPACE) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_B))
		{		
			m_StartSE->Play(false);
			m_BGM->Play(true);
			m_Camera->SetStart(true);
			m_Texture->SetSceneTexture(TEXTURE_MENU);
			m_SelectMode = SELECT_MENU;
			m_Start = true;
		}
		break;

	case SELECT_MENU: //Menu
		m_SelectDownMax = MENU_DOWN_MAX;

		if (Input::GetKeyTrigger(VK_RETURN) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_A) && m_Start)
		{
			m_ConfirmSE->Play(false);

			if (m_Texture->GetSelectPos() == -100.0f)//Mission
			{
				m_Texture->SetSelectPos(-100.0f);
				m_Texture->SetSceneTexture(TEXTURE_MISSION);
				m_SelectMode = SELECT_MISSION;
			}
			if (m_Texture->GetSelectPos() == -40.0f)//Weapon 開発中
			{
				m_ErrorSE->Play(false);

				//m_Camera->SetStart(false);
				//m_Texture->SetSelectPos(-100.0f);
				//m_Texture->SetSceneTexture(SCENE_WEAPON);
				//m_Camera->SetWeapon(true);
				//m_SelectMode = SELECT_WEAPON;
			}
			if (m_Texture->GetSelectPos() == 20.0f)//Option
			{
				m_Texture->SetSelectPos(-100.0f);
				m_Texture->SetSceneTexture(TEXTURE_OPTION);
				m_SelectMode = SELECT_OPTION;
			}
			if (m_Texture->GetSelectPos() == 80.0f)//Exit
			{
				DestroyWindow(GetWindow());
			}
		}
		break;

	case SELECT_MISSION: //Mission
		m_SelectDownMax = MISSION_DOWN_MAX;

		if (Input::GetKeyTrigger(VK_RETURN) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_A) && m_Start)
		{
			m_ConfirmSE->Play(false);

			if (m_Texture->GetSelectPos() == -100.0f)//Tutorial
			{
				Loading::SetTutorialLoad(true);
				m_Fade->FadeOut();
			}
			if (m_Texture->GetSelectPos() == -40.0f)//Mission01
			{
				Loading::SetGameLoad(true);
				m_Fade->FadeOut();
			}
			if (m_Texture->GetSelectPos() == 20.0f)//Back
			{
				m_Texture->SetSelectPos(-100.0f);
				m_Texture->SetSceneTexture(TEXTURE_MENU);
				m_SelectMode = SELECT_MENU;
			}
		}

		break;

	case SELECT_WEAPON: //Weapon

		break;

	case SELECT_OPTION: //Option
		m_SelectDownMax = OPTION_DOWN_MAX;

		if (m_Texture->GetSelectPos() == -100.0f)//BGM
		{
			m_Texture->SetSEPinRGB(1.0f, 1.0f, 1.0f);
			m_Texture->SetBGMPinRGB(1.0f, 0.6f, 0.0f);

			if (Input::GetKeyPress(VK_RIGHT) || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				m_ConfirmSE->Play(false);
				m_Texture->SetBGMPinMinus();
			}
			if (Input::GetKeyPress(VK_LEFT) || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_LEFT))
			{
				m_ConfirmSE->Play(false);
				m_Texture->SetBGMPinPlus();
			}
		}
		if (m_Texture->GetSelectPos() == -40.0f)//SE
		{
			m_Texture->SetBGMPinRGB(1.0f, 1.0f, 1.0f);
			m_Texture->SetSEPinRGB(1.0f, 0.6f, 0.0f);
			m_Texture->SetPitchFlipCheckRGB(1.0f, 1.0f, 1.0f);

			if (Input::GetKeyPress(VK_RIGHT) || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				m_ConfirmSE->Play(false);
				m_Texture->SetSEPinMinus();
			}
			if (Input::GetKeyPress(VK_LEFT) || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_LEFT))
			{
				m_ConfirmSE->Play(false);
				m_Texture->SetSEPinPlus();
			}
		}
		if (m_Texture->GetSelectPos() == 20.0f)//Flip
		{
			m_Texture->SetSEPinRGB(1.0f, 1.0f, 1.0f);
			m_Texture->SetPitchFlipCheckRGB(1.0f, 0.6f, 0.0f);

			if (Input::GetKeyTrigger(VK_RETURN) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_A) && m_Start)
			{
				m_ConfirmSE->Play(false);
				m_Texture->SetPitchFlipCheck();
			}
		}
		if (m_Texture->GetSelectPos() == 80.0f)//Back
		{
			m_Texture->SetBGMPinRGB(1.0f, 1.0f, 1.0f);
			m_Texture->SetPitchFlipCheckRGB(1.0f, 1.0f, 1.0f);

			if (Input::GetKeyTrigger(VK_RETURN) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_A) && m_Start)
			{
				m_ConfirmSE->Play(false);
				m_Texture->SetSelectPos(-100.0f);
				m_Texture->SetSceneTexture(TEXTURE_MENU);
				m_SelectMode = SELECT_MENU;
			}
		}
		break;

	default:
		break;
	}

	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Loading>();
	}
}

void Title::Draw()
{
	//ライトカメラ構造体の初期化
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, -1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトカメラのビュー行列を作成
	D3DXVECTOR3 lightPos = D3DXVECTOR3(200.0f, 400.0f, 200.0f);
	D3DXVECTOR3 lightTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos, &lightTarget, &lightUp);

	//ライトカメラのプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
		(float)1.0f, 100.0f, -600.0f);

	Renderer::SetLight(light);

	//reflect
//ビュー変換行列を作成する
//注視点オフセットテーブル
	D3DXVECTOR3 lookatOffset[6] = {
		{ 1.0f, 0.0f, 0.0f },//+X D3D11_TEXTURECUBE_FACE_POSITIVE_X
		{ -1.0f, 0.0f, 0.0f },//-X D3D11_TEXTURECUBE_FACE_NEGATIVE_X
		{ 0.0f, 1.0f, 0.0f },//+Y D3D11_TEXTURECUBE_FACE_POSITIVE_Y
		{ 0.0f, -1.0f, 0.0f },//-Y D3D11_TEXTURECUBE_FACE_NEGATIVE_Y
		{ 0.0f, 0.0f, 1.0f },//+Z D3D11_TEXTURECUBE_FACE_POSITIVE_Z
		{ 0.0f, 0.0f, -1.0f },//-Z D3D11_TEXTURECUBE_FACE_NEGATIVE_Z
	};

	//upベクトルテーブル
	D3DXVECTOR3 upOffset[6] = {
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
	};

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookat;
	D3DXVECTOR3 up;
	D3DXMATRIX viewMatrixArray[6];
	D3DXVECTOR3 vPlayerPos = m_Jet->GetPosition();
	for (int i = 0; i < 6; i++)
	{
		eye = vPlayerPos;
		lookat = vPlayerPos + lookatOffset[i];
		up = -upOffset[i];
		D3DXMatrixLookAtLH(&viewMatrixArray[i], &eye, &lookat, &up);
	}

	//プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 2, 1.0f, 0.01f, 15000.0f);
	Renderer::SetProjectionMatrix(&projectionMatrix);
	//ビューポート変更
	Renderer::SetReflectViewport();
	//6面分描画する
	for (int i = 0; i < 6; i++)
	{
		Renderer::BeginCube();
		//ビュー変換マトリクス設定
		Renderer::SetViewMatrix(&viewMatrixArray[i]);
		Scene::ReflectDraw();

		//描画したテクスチャをキューブマップ用テクスチャにコピーしていく
		Renderer::GetDeviceContext()->CopySubresourceRegion(
			Renderer::GetCubeReflectTexture(),
			D3D11CalcSubresource(0, i, 1),
			0, 0, 0, Renderer::GetReflectTexture(), 0, nullptr);
	}

	//shader
	// 1パス目 シャドウバッファの作成 //
	Renderer::BeginDepth();
	Renderer::SetDepthViewport();

	//ライトカメラの行列をセット
	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);

	//影を落としたいオブジェクトの描画
	Scene::ShadowDraw();

	// 2パス目 通常の作成 //
	Renderer::Begin();
	Renderer::SetDefaultViewport();

	//本来のカメラ&プロジェクション行列をセット
	light.Enable = false;
	Renderer::SetLight(light);

	Scene::Draw();

}
