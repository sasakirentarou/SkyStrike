#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "missile.h"
#include "enemyJet.h"
#include "explosion.h"
#include "jet.h"
#include "smoke.h"
#include "hpGauge.h"
#include "camera.h"
#include "textureManager.h"
#include "audio.h"
#include "inputx.h"
#include "missileFire.h"
#include "collisionBox.h"

Model* Missile::m_Model{};
Jet* Missile::m_Jet{};

void Missile::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\missiles.obj");
}

void Missile::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Missile::Init()
{
	m_Scene = Manager::GetScene();
	m_Jet = m_Scene->GetGameObject<Jet>();
	m_Camera = m_Scene->GetGameObject<Camera>();
	m_Fire = m_Scene->AddGameObject<MissileFire>(1);
	m_Fire->SetOffset(0.0f,0.0f,-3.5f);
	m_Collision = m_Scene->AddGameObject<CollisionBox>(1);
	m_Collision->SetScale(D3DXVECTOR3(1.5f, 1.5f, 5.0f));
	m_Collision->SetName("Missile");

	m_Scale = D3DXVECTOR3(0.3f, 0.3f, 0.35f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_ShotSE = AddComponet<Audio>();
	m_ShotSE->Load("asset\\audio\\missile.wav");

	GameObject::Init();
}

void Missile::Uninit()
{
	m_Fire->SetDestroy();
	m_Collision->SetDestroy();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Missile::Update()
{
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();
	m_Texture = m_Scene->GetGameObject<TextureManager>();
	m_PlayerHp = m_Scene->GetGameObject<HpGauge>();

	m_ShotSE->Volume(Scene::m_SEVolume * (0.3f * 2));

	//発射
	if (m_ShotFlg)
	{
		Shot();
	}

	//敵のミサイル
	if (m_EnemyMissile)
	{
		EnemyShot();
	}

	m_Position += m_Velocity;//移動

	//ImGui表示
	//ImGui::Begin("MISSILE");
	//ImGui::InputFloat3("ColiisionScale", m_CollisionScale);
	//ImGui::End();
}

void Missile::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	m_WorldMatrix = scale * rot * trans;
	Renderer::SetWorldMatrix(&m_WorldMatrix);

	//噴射
	m_Fire->SetPearent(m_WorldMatrix);

	//collision
	m_Collision->SetPearent(m_WorldMatrix);
	SetCollider(m_Collision->GetMatrix());

	m_Model->Draw();

	GameObject::Draw();
}




void Missile::Shot()
{
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();
	//スモーク
	Smoke* smoke = m_Scene->AddGameObject<Smoke>(1);
	smoke->SetPosition(m_Position + GetForwardQuaternion() * -10.0f);
	smoke->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));


	if (m_ShotCount == 0)
	{
		if(!enemys.empty())
		{
			m_LockTargetID = m_Jet->GetWeaponSm()->GetTargetLockID();
			m_TrackingFlg = m_Jet->GetLockOnSm()->GetLockFlg();
		}

		m_Fire->DrawFlg(true);
		m_ShotSE->Play();
	}

	m_ShotCount++;
	if (m_ShotCount >= 60 * 2)
	{
		//速度上昇
		m_Velocity += GetForwardQuaternion() * PLUS_VELOCITY;
	}


	//誘導
	if (m_TrackingFlg && !enemys.empty())
	{
		for (EnemyJet* enemy : enemys)
		{
			if (enemy->GetEnemyID() == m_LockTargetID)
			{
				m_EnemyPos = enemy->GetPosition();
			}
		}

		//計算処理
		D3DXVECTOR3 forward = GetForwardQuaternion();
		D3DXVECTOR3 gaiseki;

		D3DXVECTOR3 direction = m_EnemyPos - m_Position;//法線ベクトル
		D3DXVec3Normalize(&direction, &direction);//法線を正規化

		D3DXVec3Cross(&gaiseki, &direction, &forward);//法線と前ベクトルの外積を求める
		float length = D3DXVec3Length(&gaiseki);//外積の長さを求める
		D3DXVec3Normalize(&gaiseki, &gaiseki);//外積を正規化

		float angle = asinf(length);//外積の長さをアークサインで計算

		if (angle >= 1.0f)
		{
			m_TrackingFlg = false;
			m_Texture->SetMissReportFlg(true);
		}

		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &gaiseki, -angle);//外積を軸に角度分回す
		D3DXQUATERNION q = m_Quaternion * quat;
		D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &q, PLAYER_MISSILE_ROT);
	}
	else
	{
		if (m_ShotCount >= 60 * 8)
		{
			m_ShotFlg = false;
			// 消す予約をする
			SetDestroy();
			m_ShotCount = 0;
		}
	}

	m_Velocity += GetForwardQuaternion() * DEFAULT_VELOCITY;//速度
	m_Velocity += m_Velocity * VELOCITY_MINUS_PERCENT;//空気抵抗

	// enemyとの衝突判定
	for (EnemyJet* enemy : enemys)
	{
		//-------OBB-------
		if (m_Collision->SetOBB(this, enemy, 1.0f))
		{
			MisiileBom();
			enemy->HealthDamage(50);
			m_Texture->SetHitReportFlg(true);
			return;
		}


		//-------AABB-------
		//D3DXVECTOR3 enemypos = enemy->GetPosition();
		//D3DXVECTOR3 direction = m_Position - enemypos;
		//float length = D3DXVec3Length(&direction);

		//if (length < 10.0f)
		//{
		//	MisiileBom();
		//	enemy->HealthDamage(50);
		//	m_Texture->SetHitReportFlg(true);
		//	return;
		//}
	}


	//障害物との衝突判定
	{
		float groundHeight = 0.0f;

		//接地
		if (m_Position.y < groundHeight)
		{
			MisiileBom();
		}
	}
}




void Missile::EnemyShot()
{
	//スモーク
	Smoke* smoke = m_Scene->AddGameObject<Smoke>(1);
	smoke->SetPosition(m_Position + GetForwardQuaternion() * -10.0f);
	smoke->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));


	if (m_ShotCount == 0)
		m_Fire->DrawFlg(true);


	m_ShotCount++;
	if (m_ShotCount >= 60 * 2)
	{
		//速度上昇
		m_Velocity += GetForwardQuaternion() * PLUS_VELOCITY;
	}


	if (m_PlayerTrackingFlg)
	{
		//計算処理
		D3DXVECTOR3 forward = GetForwardQuaternion();
		D3DXVECTOR3 playerpos = m_Jet->GetPosition();
		D3DXVECTOR3 gaiseki;

		D3DXVECTOR3 direction = playerpos - m_Position;//法線ベクトル
		D3DXVec3Normalize(&direction, &direction);//法線を正規化

		D3DXVec3Cross(&gaiseki, &direction, &forward);//法線と前ベクトルの外積を求める
		float length = D3DXVec3Length(&gaiseki);//外積の長さを求める
		D3DXVec3Normalize(&gaiseki, &gaiseki);//外積を正規化

		float angle = asinf(length);//外積の長さをアークサインで計算

		if (angle >= 1.0f)
		{
			m_PlayerTrackingFlg = false;
			m_Jet->GetAlertSE()->Stop();
		}

		if (!m_Jet->GetFlareFlg())
			angle *= -1.0f;
		else
			m_Jet->GetAlertSE()->Stop();

		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &gaiseki, angle);//外積を軸に角度分回す
		D3DXQUATERNION q = m_Quaternion * quat;
		D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &q, ENEMY_MISSILE_ROT);//回転量

		m_Texture->SetMissileAlertFlg(true);
	}
	else
	{
		m_Texture->SetMissileAlertFlg(false);

		if (m_ShotCount >= 60 * 8)
		{
			//消滅
			SetDestroy();
			m_ShotCount = 0;
		}
	}

	m_Velocity += GetForwardQuaternion() * DEFAULT_VELOCITY;//速度
	m_Velocity += m_Velocity * VELOCITY_MINUS_PERCENT;//空気抵抗

	// playerとの衝突判定
	{
		//-------OBB-------//
		if (m_Collision->SetOBB(this, m_Jet, 1.0f))
		{
			MisiileBom();
			m_PlayerHp->HPMinus(10.0f);
			m_Camera->Shake(5.0f, 0.6f);

			if (m_PlayerTrackingFlg)
				m_Jet->GetAlertSE()->Stop();

			return;
		}

		//-------AABB-------//
		//D3DXVECTOR3 direction = m_Position - m_Player->GetPosition();
		//float length = D3DXVec3Length(&direction);

		//if (length < 10.0f)
		//{
		//	MisiileBom();
		//	m_PlayerHp->HPMinus(10.0f);
		//	m_Camera->Shake(5.0f, 0.6f);

		//	if (m_PlayerTrackingFlg)
		//		m_Player->GetMissileAlertSE()->Stop();

		//	return;
		//}
	}

	//障害物との衝突判定
	{
		float groundHeight = 0.0f;

		//接地
		if (m_Position.y < groundHeight)
		{
			MisiileBom();
		}
	}
}

