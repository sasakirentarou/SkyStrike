#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"

#include "field.h"
#include "camera.h"
#include "jet.h"
#include "enemyJet.h"
#include "explosion.h"
#include "cylinder.h"
#include "box.h"
#include "input.h"
#include "inputx.h"
#include "result.h"
#include "audio.h"
#include "dome.h"
#include "fade.h"
#include "bullet.h"
#include "missile.h"
#include "lock.h"
#include "smoke.h"
#include "hpGauge.h"
#include "missileUI.h"
#include "cross.h"
#include "lockOn.h"
#include "jetUI.h"
#include "flare.h"
#include "collisionBox.h"
#include "enemyDistance.h"
#include "textureManager.h"
#include "arrow.h"
#include "fire.h"
#include "engineFire.h"
#include "timer.h"
#include "hitBullet.h"
#include "missileFire.h"
#include "longMissile.h"
#include "lockon2D.h"
#include "longMissileUI.h"
#include "debug.h"

bool Game::m_LoadFinish = false;

void Game::Load()
{
	//まとめてロード
	CollisionBox::Load();
	Bullet::Load();
	Missile::Load();
	Smoke::Load();
	Flare::Load();
	EnemyJet::Load();
	Explosion::Load();
	Fire::Load();
	HitBullet::Load();
	MissileFire::Load();
	LongMissile::Load();

	m_LoadFinish = true;
}

void Game::Unload()
{
	m_LoadFinish = false;

	CollisionBox::Unload();
	Bullet::Unload();
	Missile::Unload();
	Smoke::Unload();
	Flare::Unload();
	EnemyJet::Unload();
	Explosion::Unload();
	Fire::Unload();
	HitBullet::Unload();
	MissileFire::Unload();
	LongMissile::Unload();
}


void Game::Init()
{
	SetPause(false);
	Scene::m_SceneState = SCENE_GAME;

	// camera
	Camera* camera = AddGameObject<Camera>(0);
	camera->SetGameEnable(true);
	//砂色：0.9f, 0.8f, 0.5f
	camera->SetFog(3000.0f, 20000.0f, 2000.0f,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXCOLOR(0.2f, 0.3f, 0.6f, 1.0f));
	

	//dome
	GameObject* dome = AddGameObject<Dome>(1);
	dome->SetScale(D3DXVECTOR3(15000.0f, 15000.0f, 15000.0f));
	dome->SetGameEnable(true);

	// field
	GameObject* field = AddGameObject<Field>(1);

	//噴射
	Fire* fire = AddGameObject<Fire>(1);
	fire->SetPlayerFire();
	Arrow* arrow = AddGameObject<Arrow>(1);

	// player
	m_Jet = AddGameObject<Jet>(1);
	m_Jet->SetPosition(D3DXVECTOR3(1000.0f, 1000.0f, 0.0f));
	m_Jet->SetGameEnable(true);
	m_Jet->Init();

	//enemy
	GameObject* enemy0 = AddGameObject<EnemyJet>(1);
	GameObject* enemy1 = AddGameObject<EnemyJet>(1);
	GameObject* enemy2 = AddGameObject<EnemyJet>(1);
	//GameObject* enemy4 = AddGameObject<EnemyJet>(1);
	//GameObject* enemy3 = AddGameObject<EnemyJet>(1);
	GameObject* enemy5 = AddGameObject<EnemyJet>(1);
	//GameObject* enemy6 = AddGameObject<EnemyJet>(1);
	//GameObject* enemy7 = AddGameObject<EnemyJet>(1);
	//GameObject* enemy8 = AddGameObject<EnemyJet>(1);

	enemy0->SetPosition(D3DXVECTOR3(1400.0f, 800.0f, 3000.0f));
	enemy0->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	enemy0->SetQuaternionRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), 180.0f);

	enemy1->SetPosition(D3DXVECTOR3(1000.0f, 800.0f, 3500.0f));
	enemy1->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	enemy1->SetQuaternionRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), 180.0f);

	enemy2->SetPosition(D3DXVECTOR3(800.0f, 800.0f, 4800.0f));
	enemy2->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	enemy2->SetQuaternionRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), 180.0f);

	//enemy3->SetPosition(D3DXVECTOR3(1200.0f, 900.0f, 2400.0f));
	//enemy3->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	//enemy4->SetPosition(D3DXVECTOR3(1100.0f, 900.0f, 2000.0f));
	//enemy4->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	enemy5->SetPosition(D3DXVECTOR3(900.0f, 900.0f, 4000.0f));
	enemy5->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	enemy5->SetQuaternionRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), 180.0f);

	//enemy6->SetPosition(D3DXVECTOR3(1000.0f, 900.0f, 2100.0f));
	//enemy6->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	//enemy7->SetPosition(D3DXVECTOR3(-1600.0f, 900.0f, 1900.0f));
	//enemy7->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	//enemy8->SetPosition(D3DXVECTOR3(-1000.0f, 900.0f, 1800.0f));
	//enemy8->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));


	//////Polygon//////

	//3DUI
	LockOn* lockon = AddGameObject<LockOn>(1);
	lockon->SetAlpha(0.0f);

	GameObject* cross = AddGameObject<Cross>(1);
	cross->SetScale(D3DXVECTOR3(1.6f, 1.2f, 0.0f));

	//EnemyDistance* distance = AddGameObject<EnemyDistance>(1);

	//enginefire
	GameObject* enginefire = AddGameObject<EngineFire>(1);

	//HPバー
	GameObject* hp = AddGameObject<HpGauge>(2);

	//機体UI
	GameObject* jetUI;
	jetUI = AddGameObject<JetUI>(2);

	//lockon2D
	GameObject* lockon2D;
	lockon2D = AddGameObject<Lockon2D>(2);

	//texture
	m_Texture = AddGameObject<TextureManager>(2);
	m_Texture->SetSceneTexture(TEXTURE_GAME);

	//timer
	m_Timer = AddGameObject<Timer>(2);

	//fade
	m_Fade = AddGameObject<Fade>(2);

	//SE再生
	m_Wind0 = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	m_Wind0->Load("asset\\audio\\wind0.wav");
	m_Wind0->Play(true);

	m_Wind1 = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	m_Wind1->Load("asset\\audio\\wind1.wav");
	m_Wind1->Play(true);

	//BGM再生
#if 0
	m_BGM = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	m_BGM->Load("asset\\audio\\bgm_out.wav");
	m_BGM->Play(true);
#else
	m_BGM = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	m_BGM->Load("asset\\audio\\bgm01.wav");
	m_BGM->Play(true);
#endif // 0
}

void Game::Uninit()
{
	InputX::StopVibration(0);
	Scene::Uninit();

	Unload();
}

void Game::Update()
{
	Scene::Update();
	auto enemys = GetGameObjects<EnemyJet>();

	//ボリューム
	m_BGM->Volume(Scene::m_BGMVolume * (0.04f * 2));
	m_Wind1->Volume(Scene::m_SEVolume * (0.5f * 2));
	m_Wind0->Volume(Scene::m_SEVolume * (0.5f * 2));

	if (m_Texture->GetMissionClear())
	{
		m_Fade->FadeOut();
	}

	if (m_Jet->GetDeath())
	{
		m_Texture->SetGameOverFlg(true);
		m_Fade->FadeOut();
	}
	
	if (m_Fade->GetFadeFinish())
	{
		m_Texture->SetResultTimer(m_Timer->GetTime());
		Manager::SetScene<Result>();
	}
}

void Game::Draw()
{
	//ライトカメラ構造体の初期化
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(-10.0f, -1.0f, -10.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトカメラのビュー行列を作成
	//D3DXVECTOR3 lightPos = D3DXVECTOR3(10000.0f, 8000.0f, 10000.0f);
	D3DXVECTOR3 lightPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lightTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos, &lightTarget, &lightUp);

	//ライトカメラのプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
		(float)1.0f, 10.0f, -600.0f);

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