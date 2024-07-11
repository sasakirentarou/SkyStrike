#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "bullet.h"
#include "enemyJet.h"
#include "explosion.h"
#include "cross.h"
#include "textureManager.h"
#include "hitBullet.h"
#include "jet.h"
#include "collisionBox.h"

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
	m_Scene = Manager::GetScene();

	//collision
	m_Collision = m_Scene->AddGameObject<CollisionBox>(1);
	m_Collision->SetScale(D3DXVECTOR3(2.0f, 2.0f, 10.0f));
	m_Collision->SetName("Bullet");

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

	m_Collision->SetDestroy();

	GameObject::Uninit();
}

void Bullet::Update()
{
	TextureManager* texture = m_Scene->GetGameObject<TextureManager>();
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();

	//発射
	Shot();
	
	//空気抵抗
	m_Velocity += m_Velocity * -0.03f;
	m_Position += m_Velocity;

	for (EnemyJet* enemy : enemys)
	{
		//OBB//
		if (m_Collision->SetOBB(this, enemy, 1.0f))
		{
			auto hit = m_Scene->AddGameObject<HitBullet>(1);
			hit->SetPosition(m_Position);
			hit->SetScale(D3DXVECTOR3(2.0f, 2.0f, 0.0f));
			hit->AnimTime(1);

			enemy->HealthDamage(5);//敵の体力を減らす
			texture->SetHitReportFlg(true);
			SetDestroy();
			return;
		}

		//AABB//
		//D3DXVECTOR3 enemypos = enemy->GetPosition();
		//D3DXVECTOR3 direction = m_Position - enemypos;
		//float length = D3DXVec3Length(&direction);

		//if (length < 10.0f)
		//{
		//	auto hit = m_Scene->AddGameObject<HitBullet>(1);
		//	hit->SetPosition(m_Position);
		//	hit->SetScale(D3DXVECTOR3(2.0f, 2.0f,0.0f));
		//	hit->AnimTime(1);

		//	enemy->HealthDamage(5);//敵の体力を減らす
		//	texture->SetHitReportFlg(true);
		//	SetDestroy();
		//	return;
		//}
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
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x * 1.5f, m_Scale.y * 1.5f, m_Scale.z * 2.0f);
	D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y - 0.5f, m_Position.z);
	m_WorldMatrix = scale * rot * trans;
	Renderer::SetWorldMatrix(&m_WorldMatrix);

	//collision
	m_Collision->SetPearent(m_WorldMatrix);
	SetCollider(m_Collision->GetMatrix());


	m_Model->Draw();

	GameObject::Draw();
}

void Bullet::Shot()
{
	Jet* jet = m_Scene->GetGameObject<Jet>();
	Cross* cross = m_Scene->GetGameObject<Cross>();
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();

	if (cross->AutoRange())
	{
		//誘導計算
		D3DXVECTOR3 forward = GetForwardQuaternion();
		D3DXVECTOR3 gaiseki, enemypos;

		for (EnemyJet* enemy : enemys)
		{
			if (enemy->GetEnemyID() == jet->GetLockOnSm()->GetLockEnemyID())
			{
				enemypos = enemy->GetPosition();
			}
		}

		D3DXVECTOR3 direction = enemypos - m_Position;//法線ベクトル
		D3DXVec3Normalize(&direction, &direction);//法線を正規化

		D3DXVec3Cross(&gaiseki, &direction, &forward);//法線と前ベクトルの外積を求める
		float length = D3DXVec3Length(&gaiseki);//外積の長さを求める
		D3DXVec3Normalize(&gaiseki, &gaiseki);//外積を正規化

		float angle = asinf(length);//外積の長さをアークサインで計算

		if (angle >= 1.0f)
		{
			SetDestroy();
			return;
		}

		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &gaiseki, -angle);//外積を軸に角度分回す
		D3DXQUATERNION q = m_Quaternion * quat;
		D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &q, 1.0f);

		m_ShotCount = 0;
	}
	else
	{
		m_ShotCount++;
		if (m_ShotCount >= 60 * 2)
		{
			// 消す
			SetDestroy();
			return;
		}
	}

	//-0.02から0.02の間をランダムで格納
	float randomRight = float(rand()) / RAND_MAX * 0.02f - 0.01f;
	int randomForward = rand() % 11 + 5;

	//左右にランダムに速度を与えてばらつかせる
	m_Velocity += GetForwardQuaternion() * randomForward + GetRightQuaternion() * randomRight;

}
