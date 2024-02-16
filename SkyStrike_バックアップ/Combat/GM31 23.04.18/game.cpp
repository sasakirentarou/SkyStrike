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
	//�܂Ƃ߂ă��[�h
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
	AddGameObject<RightWing>(1);//�嗃
	AddGameObject<LeftWing>(1);//�嗃
	AddGameObject<BackLeftWing>(1);//��������
	AddGameObject<BackRightWing>(1);//��������
	AddGameObject<LeftVertical>(1);//��������
	AddGameObject<RightVertical>(1);//��������

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

	//���x�ƍ���
	Score* score01 = AddGameObject<Score>(2);
	Score* score02 = AddGameObject<Score>(2);
	
	//HP�o�[
	g_Hp = AddGameObject<HpGauge>(2);

	//�~�T�C��UI
	MissileUI* missileUI_0;
	MissileUI* missileUI_1;
	missileUI_0 = AddGameObject<MissileUI>(2);
	missileUI_1 = AddGameObject<MissileUI>(2);

	//�@��UI
	JetUI* jetUI;
	jetUI = AddGameObject<JetUI>(2);

	//�c��UI
	TextureManager* texture = AddGameObject<TextureManager>(2);
	texture->SetGame(true);

	m_Fade = AddGameObject<Fade>(2);

	//BGM�Đ�
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
	//���C�g�J�����\���̂̏�����
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(-1.0f, -1.0f, -1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�J�����̃r���[�s����쐬
	D3DXVECTOR3 lightPos = D3DXVECTOR3(10000.0f, 4000.0f, 10000.0f);
	D3DXVECTOR3 lightTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos, &lightTarget, &lightUp);

	//���C�g�J�����̃v���W�F�N�V�����s����쐬
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
		(float)1.0f, 100.0f, -600.0f);

	Renderer::SetLight(light);

	//reflect
	//�r���[�ϊ��s����쐬����
	//�����_�I�t�Z�b�g�e�[�u��
	D3DXVECTOR3 lookatOffset[6] = {
		{ 1.0f, 0.0f, 0.0f },//+X D3D11_TEXTURECUBE_FACE_POSITIVE_X
		{ -1.0f, 0.0f, 0.0f },//-X D3D11_TEXTURECUBE_FACE_NEGATIVE_X
		{ 0.0f, 1.0f, 0.0f },//+Y D3D11_TEXTURECUBE_FACE_POSITIVE_Y
		{ 0.0f, -1.0f, 0.0f },//-Y D3D11_TEXTURECUBE_FACE_NEGATIVE_Y
		{ 0.0f, 0.0f, 1.0f },//+Z D3D11_TEXTURECUBE_FACE_POSITIVE_Z
		{ 0.0f, 0.0f, -1.0f },//-Z D3D11_TEXTURECUBE_FACE_NEGATIVE_Z
	};

	//up�x�N�g���e�[�u��
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

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 2, 1.0f, 0.01f, 15000.0f);
	Renderer::SetProjectionMatrix(&projectionMatrix);
	//�r���[�|�[�g�ύX
	Renderer::SetReflectViewport();
	//6�ʕ��`�悷��
	for (int i = 0; i < 6; i++)
	{
		Renderer::BeginCube();
		//�r���[�ϊ��}�g���N�X�ݒ�
		Renderer::SetViewMatrix(&viewMatrixArray[i]);
		Scene::ReflectDraw();

		//�`�悵���e�N�X�`�����L���[�u�}�b�v�p�e�N�X�`���ɃR�s�[���Ă���
		Renderer::GetDeviceContext()->CopySubresourceRegion(
			Renderer::GetCubeReflectTexture(),
			D3D11CalcSubresource(0, i, 1),
			0, 0, 0, Renderer::GetReflectTexture(), 0, nullptr);
	}

	//shader
	// 1�p�X�� �V���h�E�o�b�t�@�̍쐬 //
	Renderer::BeginDepth();
	Renderer::SetDepthViewport();

	//���C�g�J�����̍s����Z�b�g
	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);

	//�e�𗎂Ƃ������I�u�W�F�N�g�̕`��
	Scene::ShadowDraw();

	// 2�p�X�� �ʏ�̍쐬 //
	Renderer::Begin();
	Renderer::SetDefaultViewport();

	//�{���̃J����&�v���W�F�N�V�����s����Z�b�g
	light.Enable = false;
	Renderer::SetLight(light);

	Scene::Draw();
}