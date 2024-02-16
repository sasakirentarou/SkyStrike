#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"

#include "polygon2D.h"
#include "field.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "cylinder.h"
#include "box.h"
#include "score.h"
#include "input.h"
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
#include "leftWing.h"
#include "rightWing.h"
#include "backLeftWing.h"
#include "backRightWing.h"
#include "leftVertical.h"
#include "rightVertical.h"
#include "collisionBox.h"
#include "enemyDistance.h"
#include "textureManager.h"
#include "arrow.h"

GameObject* g_Player;
GameObject* g_Dome;
GameObject* g_Field;
GameObject* g_Camera;
GameObject* g_Enemy0;
GameObject* g_Enemy1;
GameObject* g_Enemy2;
GameObject* g_Enemy3;
GameObject* g_Enemy4;
GameObject* g_Enemy5;
GameObject* g_Enemy6;
GameObject* g_Enemy7;
GameObject* g_Enemy8;
GameObject* g_Hp;
GameObject* g_CollisionBox;

bool Game::m_LoadFinish = false;

void Game::Load()
{
	//まとめてロード
	Bullet::Load();
	Missile::Load();
	Smoke::Load();
	Flare::Load();

	m_LoadFinish = true;
}

void Game::Unload()
{
	m_LoadFinish = false;

	Bullet::Unload();
	Missile::Unload();
	Smoke::Unload();
	Flare::Unload();
}


void Game::Init()
{
	// camera
	g_Camera = AddGameObject<Camera>(0);
	g_Camera->SetGameEnable(true);

	//dome
	g_Dome = AddGameObject<Dome>(1);
	g_Dome->SetScale(D3DXVECTOR3(15000.0f, 15000.0f, 15000.0f));
	g_Dome->SetGameEnable(true);

	// field
	g_Field = AddGameObject<Field>(1);

	//parts
	AddGameObject<RightWing>(1);//主翼
	AddGameObject<LeftWing>(1);//主翼
	AddGameObject<BackLeftWing>(1);//水平尾翼
	AddGameObject<BackRightWing>(1);//水平尾翼
	AddGameObject<LeftVertical>(1);//垂直尾翼
	AddGameObject<RightVertical>(1);//垂直尾翼

	// player
	g_Player = AddGameObject<Player>(1);
	g_Player->SetPosition(D3DXVECTOR3(1000.0f, 1000.0f, 0.0f));
	g_Player->SetGameEnable(true);
	g_Player->Init();

	//enemy
	g_Enemy0 = AddGameObject<Enemy>(1);
	g_Enemy1 = AddGameObject<Enemy>(1);
	g_Enemy2 = AddGameObject<Enemy>(1);
	g_Enemy3 = AddGameObject<Enemy>(1);
	g_Enemy4 = AddGameObject<Enemy>(1);
	g_Enemy5 = AddGameObject<Enemy>(1);
	g_Enemy6 = AddGameObject<Enemy>(1);
	//g_Enemy7 = AddGameObject<Enemy>(1);
	//g_Enemy8 = AddGameObject<Enemy>(1);

	g_Enemy0->SetPosition(D3DXVECTOR3(1400.0f, 800.0f, 3000.0f));
	g_Enemy0->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	g_Enemy1->SetPosition(D3DXVECTOR3(1000.0f, 800.0f, 3500.0f));
	g_Enemy1->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	g_Enemy2->SetPosition(D3DXVECTOR3(800.0f, 800.0f, 4800.0f));
	g_Enemy2->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	g_Enemy3->SetPosition(D3DXVECTOR3(1200.0f, 900.0f, 2400.0f));
	g_Enemy3->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	g_Enemy4->SetPosition(D3DXVECTOR3(1100.0f, 900.0f, 2000.0f));
	g_Enemy4->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	g_Enemy5->SetPosition(D3DXVECTOR3(900.0f, 900.0f, 4000.0f));
	g_Enemy5->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	g_Enemy6->SetPosition(D3DXVECTOR3(1000.0f, 900.0f, 2100.0f));
	g_Enemy6->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	//g_Enemy7->SetPosition(D3DXVECTOR3(-1600.0f, 900.0f, 1900.0f));
	//g_Enemy7->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	//g_Enemy8->SetPosition(D3DXVECTOR3(-1000.0f, 900.0f, 1800.0f));
	//g_Enemy8->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));


	// polygon
	//AddGameObject<Polygon2D>(2);

	//3DUI
	LockOn* lockon = AddGameObject<LockOn>(1);
	lockon->SetAlpha(0.0f);

	Cross* cross = AddGameObject<Cross>(1);
	cross->SetScale(D3DXVECTOR3(1.6f, 1.2f, 0.0f));

	EnemyDistance* distance = AddGameObject<EnemyDistance>(1);

	Arrow* arrow = AddGameObject<Arrow>(1);

	//速度と高さ
	Score* score01 = AddGameObject<Score>(2);
	Score* score02 = AddGameObject<Score>(2);
	
	//HPバー
	g_Hp = AddGameObject<HpGauge>(2);

	//ミサイルUI
	MissileUI* missileUI_0;
	MissileUI* missileUI_1;
	missileUI_0 = AddGameObject<MissileUI>(2);
	missileUI_1 = AddGameObject<MissileUI>(2);

	//機体UI
	JetUI* jetUI;
	jetUI = AddGameObject<JetUI>(2);

	//残量UI
	TextureManager* texture = AddGameObject<TextureManager>(2);
	texture->SetGame(true);

	m_Fade = AddGameObject<Fade>(2);

	//BGM再生
	//m_BGM = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	//m_BGM->Load("asset\\audio\\bgm.wav");
	//m_BGM->Play(true);
}

void Game::Uninit()
{
	Scene::Uninit();

	Unload();
}

void Game::Update()
{
	Scene::Update();

	Player* player = GetGameObject<Player>();
	auto enemys = GetGameObjects<Enemy>();

	if (enemys.empty())
	{
		m_Fade->FadeOut();
	}

	if (player->GetDeath())
	{
		m_Fade->FadeOut();
	}
	
	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Result>();
	}
}

void Game::Draw()
{
	//ライトカメラ構造体の初期化
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(-1.0f, -1.0f, -1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトカメラのビュー行列を作成
	D3DXVECTOR3 lightPos = D3DXVECTOR3(10000.0f, 4000.0f, 10000.0f);
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
	D3DXVECTOR3 vPlayerPos = g_Player->GetPosition();
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