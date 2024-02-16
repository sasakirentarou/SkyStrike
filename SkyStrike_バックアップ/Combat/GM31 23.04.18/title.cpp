#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "titleLogo.h"
#include "input.h"
#include "fade.h"
#include "loading.h"
#include "startButton.h"
#include "dome.h"
#include "titleField.h"
#include "meshField.h"
#include "titleCamera.h"
#include "airport.h"
#include "jet.h"
#include "leftWing.h"
#include "rightWing.h"
#include "backLeftWing.h"
#include "backRightWing.h"
#include "leftVertical.h"
#include "rightVertical.h"
#include "player.h"
#include "textureManager.h"

void Title::Init()
{
	//load
	Jet::Load();

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
	AddGameObject<RightWing>(1);//主翼
	AddGameObject<LeftWing>(1);//主翼
	AddGameObject<BackLeftWing>(1);//水平尾翼
	AddGameObject<BackRightWing>(1);//水平尾翼
	AddGameObject<LeftVertical>(1);//垂直尾翼
	AddGameObject<RightVertical>(1);//垂直尾翼

	//player
	Player* player = AddGameObject<Player>(1);
	player->SetPosition(D3DXVECTOR3(0.0f,25.0f,10.0f));
	player->SetScale(D3DXVECTOR3(10.0f, 10.0f, 10.0f));

	//jet
	for(int i = 0; i < 3; i++)
	{
		Jet* jet_start = AddGameObject<Jet>(1);
		jet_start->SetPosition(D3DXVECTOR3(300.0f * i - 300.0f, 1050.0f, -500.0f));
		jet_start->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		jet_start->SetScale(D3DXVECTOR3(10.0f, 10.0f, 10.0f));
		jet_start->SetPatern(3);

		Jet* jet_takeoff = AddGameObject<Jet>(1);
		jet_takeoff->SetPosition(D3DXVECTOR3(-550.0f, 30.0f, 0.0f));
		jet_takeoff->SetScale(D3DXVECTOR3(10.0f, 10.0f, 10.0f));
		jet_takeoff->SetPatern(i);
	}

	// polygon
	m_Texture = AddGameObject<TextureManager>(2);
	m_Texture->SetLogo(true);

	m_Fade = AddGameObject<Fade>(2);
}

void Title::Uninit()
{
	Jet::Unload();
	Scene::Uninit();
}

void Title::Update()
{
	Scene::Update();

	m_MousePos.x = GetMouse2DX();
	m_MousePos.y = GetMouse2DY();

	if (Input::GetKeyTrigger(VK_SPACE))
	{
		m_Camera->SetStart(true);
		m_Texture->SetLogo(false);
		m_Texture->SetMenu(true);
		m_Start = true;
	}

	if (Input::GetKeyTrigger(VK_UP))
	{
		m_Texture->SetSelectUpDown(false);
	}
	else if (Input::GetKeyTrigger(VK_DOWN))
	{
		m_Texture->SetSelectUpDown(true);
	}

	if (Input::GetKeyTrigger(VK_RETURN) && m_Start)
	{
		if (m_Texture->GetSelectPos() == -100.0f)
		{
			m_Fade->FadeOut();
		}
		if (m_Texture->GetSelectPos() == 0.0f)
		{
			m_Camera->SetStart(false);
			m_Texture->SetMenu(false);
			m_Camera->SetWeapon(true);
		}
		if (m_Texture->GetSelectPos() == 100.0f)
		{

		}
		if (m_Texture->GetSelectPos() == 200.0f)
		{
			DestroyWindow(GetWindow());
		}
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
