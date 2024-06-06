#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "camera.h"
#include "cross.h"
#include "textureManager.h"

Model* Bullet::m_Model{};

void Bullet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\jetbullet.obj");
}

void Bullet::Unload()
{
	m_Model->Unload();
	delete m_Model;
}



void Bullet::Init()
{
	m_Scale = D3DXVECTOR3(0.08f, 0.08f, 0.08f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	GameObject::Init();
}

void Bullet::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Bullet::Update()
{
	Scene* scene = Manager::GetScene();
	auto enemys = scene->GetGameObjects<Enemy>();
	auto player = scene->GetGameObject<Player>();
	auto camera = scene->GetGameObject<Camera>();
	auto cross = scene->GetGameObject<Cross>();
	auto texture = scene->GetGameObject<TextureManager>();

	//マトリックス取得
	D3DXMATRIX screenMatrix = camera->GetScreenMatrix();

	//敵のの2Dposを取得
	D3DXVECTOR3 enemypos;
	D3DXVECTOR3 screenCameraEnemy;//スクリーン上の敵の位置
	for (Enemy* enemy : enemys)
	{
		if (enemy->GetEnemyID() == player->GetLockEnemy())
		{
			enemypos = enemy->GetPosition();
			D3DXVec3TransformCoord(&screenCameraEnemy, &enemypos, &screenMatrix);
		}
	}

	//クロスヘアの2Dposを取得
	D3DXVECTOR3 crosspos = cross->GetPosition();
	D3DXVECTOR3 screenCameraCross;
	D3DXVec3TransformCoord(&screenCameraCross, &crosspos, &screenMatrix);
	
	//端の最大値が1なので端の値をスクリーン座標の半分の値に変換
	float CEx = ((SCREEN_WIDTH  / 2) / 100) * (screenCameraEnemy.x * 100.0f);
	float CEy = ((SCREEN_HEIGHT / 2) / 100) * (screenCameraEnemy.y * 100.0f);
	float CCx = ((SCREEN_WIDTH  / 2) / 100) * (screenCameraCross.x * 100.0f);
	float CCy = ((SCREEN_HEIGHT / 2) / 100) * (screenCameraCross.y * 100.0f);

	//クロスヘアを起点にAUTO_RANGEの内の範囲を指定
	if( CCx + AUTO_RANGE > CEx &&
		CCx - AUTO_RANGE < CEx &&
		CCy + AUTO_RANGE > CEy &&
		CCy - AUTO_RANGE < CEy)
	{
		//誘導計算
		D3DXVECTOR3 forward = GetForwardQuaternion();
		D3DXVECTOR3 gaiseki;

		D3DXVECTOR3 direction = enemypos - m_Position;//法線ベクトル
		D3DXVec3Normalize(&direction, &direction);//法線を正規化

		D3DXVec3Cross(&gaiseki, &direction, &forward);//法線と前ベクトルの外積を求める
		float length = D3DXVec3Length(&gaiseki);//外積の長さを求める
		D3DXVec3Normalize(&gaiseki, &gaiseki);//外積を正規化

		float angle = asinf(length);//外積の長さをアークサインで計算

		if (angle >= 1.0f)
		{
			SetDestroy();
		}

		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &gaiseki, -angle);//外積を軸に角度分回す
		D3DXQUATERNION q = m_Quaternion * quat;
		D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &q, 1.0f);

		m_Count = 0;
	}
	else
	{
		m_Count++;
		if (m_Count >= 60 * 3)
		{
			// 消す
			SetDestroy();
			m_Count = 0;
		}
	}

	//-0.02から0.02の間をランダムで格納
	float randomRight = float(rand()) / RAND_MAX * 0.02f - 0.01f;
	int randomForward = rand() % 11 + 5;
		
	//左右にランダムに速度を与えてばらつかせる
	m_Velocity += GetForwardQuaternion() * randomForward + GetRightQuaternion() * randomRight;
	
	//空気抵抗
	m_Velocity += m_Velocity * -0.03f;
	m_Position += m_Velocity;

	for (Enemy* enemy : enemys)
	{
		D3DXVECTOR3 enemypos = enemy->GetPosition();
		D3DXVECTOR3 direction = m_Position - enemypos;
		float length = D3DXVec3Length(&direction);

		if (length < 10.0f)
		{
			SetDestroy();
			enemy->HealthMinus(5);//敵の体力を減らす
			texture->SetHitReportFlg(true);
			return;
		}
	}
}

void Bullet::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x * 1.5f, m_Scale.y * 1.5f, m_Scale.z * 2.0f);
	D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y - 0.5f, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

	GameObject::Draw();
}