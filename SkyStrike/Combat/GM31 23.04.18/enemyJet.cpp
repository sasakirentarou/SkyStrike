#include "main.h"
#include "renderer.h"
#include "enemyJet.h"
#include "lock.h"
#include "lockOn.h"
#include "scene.h"
#include "manager.h"
#include "jet.h"
#include "explosion.h"
#include "missile.h"
#include "smoke.h"
#include "trail.h"
#include "enemyDistance.h"
#include "textureManager.h"
#include "audio.h"
#include "camera.h"
#include "fire.h"
#include "tgt.h"
#include "collisionBox.h"

Model* EnemyJet::m_Model{};

void EnemyJet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\F-15\\F-15.obj");
}

void EnemyJet::Unload()
{
	m_EnemyCount = 0;
	m_Model->Unload();
	delete m_Model;
}

void EnemyJet::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\DepthShadowMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\DepthShadowMappingPS.cso");

	m_DepthEnable = true;
	m_ReflectEnable = true;


	m_KillSE = AddComponet<Audio>();
	m_KillSE->Load("asset\\audio\\kill01.wav");

	m_Scene = Manager::GetScene();
	m_Jet = m_Scene->GetGameObject<Jet>();

	//UI表示
	m_Lock = m_Scene->AddGameObject<Lock>(1);

	//トレイル生成
	m_Trail01 = m_Scene->AddGameObject<Trail>(1);
	m_Trail02 = m_Scene->AddGameObject<Trail>(1);

	//ジェットエンジン炎生成
	m_Fire01 = m_Scene->AddGameObject<Fire>(1);
	m_Fire01->SetScale(D3DXVECTOR3(0.25f, 0.25f, 0.5f));
	m_Fire01->SetOffset(0.55f, -0.25f, -4.0f);
	m_Fire02 = m_Scene->AddGameObject<Fire>(1);
	m_Fire02->SetScale(D3DXVECTOR3(0.25f, 0.25f, 0.5f));
	m_Fire02->SetOffset(-0.55f, -0.25f, -4.0f);

	//collision
	m_Collision = m_Scene->AddGameObject<CollisionBox>(1);
	m_Collision->SetScale(D3DXVECTOR3(7.0f, 2.0f, 8.0f));
	m_Collision->SetName("EnemyJet");

	m_Health = MAX_HEALTH;
	m_OldHealth = m_Health;

	m_EnemyID = m_EnemyCount;
	m_EnemyCount++;

	//頂点取得
	m_Model->GetModelVertex("asset\\model\\F-15\\F-15.obj", m_Top);

	// 0から200までの範囲に10を足して調整
	m_RandomTracking = rand() % 401 + 10;

	GameObject::Init();
}

void EnemyJet::Uninit()
{
	//削除予約(ここで消さないとバッファオーバーフローになる)
	m_Lock->SetDestroy();
	m_Trail01->SetDestroy();
	m_Trail02->SetDestroy();
	m_Fire01->SetDestroy();
	m_Fire02->SetDestroy();
	m_Collision->SetDestroy();
	
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void EnemyJet::Update()
{
	auto camera = m_Scene->GetGameObject<Camera>();
	auto texture = m_Scene->GetGameObject<TextureManager>();

	//ボリューム
	m_KillSE->Volume(Scene::m_SEVolume * (0.2f * 2));

	//体力が尽きたら
	if (m_CrashFlg)
	{
		if (m_BomCount == 0)
		{

			//設定変更
			m_Jet->GetLockOnSm()->SwitchTarget();
			m_Jet->GetLockOnSm()->GetLockOn()->SetAlpha(0.0f);
			texture->SetDesReportFlg(true);
			texture->SetEnemyMinus();
			m_KillSE->Play(false);

			//爆発
			auto explosion00 = m_Scene->AddGameObject<Explosion>(1);
			explosion00->Spawn(m_Position, D3DXVECTOR2(6.0f, 6.0f));
			auto explosion01 = m_Scene->AddGameObject<Explosion>(1);
			explosion01->Spawn(m_Position, D3DXVECTOR2(4.0f, 4.0f));
			explosion01->RandomShiftPos(40);
			auto explosion02 = m_Scene->AddGameObject<Explosion>(1);
			explosion02->Spawn(m_Position, D3DXVECTOR2(8.0f, 8.0f), 2);
			explosion02->RandomShiftPos(45);
		}

		//ロール回転
		D3DXVECTOR3 axis = GetForwardQuaternion();
		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &axis, 0.08f);
		m_Quaternion *= quat;

		//速度
		m_Velocity += GetForwardQuaternion() * 0.02f;

		//スモーク
		m_SmokeCount++;
		if (m_SmokeCount > 5)
		{
			Smoke* smoke = m_Scene->AddGameObject<Smoke>(1);
			smoke->SetPosition(m_Position + GetForwardQuaternion() * -2.0f);
			smoke->SetScale(D3DXVECTOR3(5.0f, 5.0f, 5.0f));
			smoke->SetColor(0.3f);
			m_SmokeCount = 0;
		}

		m_BomCount++;
		if (m_BomCount >= 60 * 5)
		{
			//爆発
			auto explosion00 = m_Scene->AddGameObject<Explosion>(1);
			explosion00->Spawn(m_Position, D3DXVECTOR2(6.0f, 6.0f), 2);
			auto explosion01 = m_Scene->AddGameObject<Explosion>(1);
			explosion01->Spawn(m_Position, D3DXVECTOR2(3.0f, 3.0f));
			explosion01->RandomShiftPos(15);
			auto explosion02 = m_Scene->AddGameObject<Explosion>(1);
			explosion02->Spawn(m_Position, D3DXVECTOR2(5.0f, 5.0f));
			explosion02->RandomShiftPos(20);

			SetDestroy();
		}
	}
	else
	{
		if (!m_StateLock)
		{
			//ステートマシン
			switch (m_EnemyState)
			{
			case ENEMY_ATTACK:
				Attack();
				break;

			case ENEMY_ESCAPE:
				Escape();
				break;

			case ENEMY_SEACH:
				Seach();
				break;

			case ENEMY_GROUNDRISK:
				GroundRisk();
				break;

			case ENEMY_OUT_OF_RANGE:
				OutOfRange();
				break;

			default:
				break;
			}

			//速度
			m_Velocity += GetForwardQuaternion() * 0.04f;
		}

		//墜落
		if (m_Position.y < 0.0f)
		{
			m_CrashFlg = true;
		}

		//地面に当たらないようにする
		m_RiskPoint = m_Position + GetForwardQuaternion() * 500.0f;
		if (m_RiskPoint.y < 0.0f && m_Position.y < RISK_RELEASE)
		{
			//m_StateKeep = m_EnemyState;

			//if (m_StateKeep == ENEMY_GROUNDRISK)
			//	m_StateKeep = ENEMY_SEACH;

			m_EnemyState = ENEMY_GROUNDRISK;
		}

		//移動範囲制限
		D3DXVECTOR3 distance = m_Jet->GetPosition() - m_Position;
		m_PlayerLength = D3DXVec3Length(&distance);
		if (m_PlayerLength > 1800.0f)
		{
			m_StateKeep = m_EnemyState;

			if (m_StateKeep == ENEMY_OUT_OF_RANGE)
				m_StateKeep = ENEMY_SEACH;

			m_EnemyState = ENEMY_OUT_OF_RANGE;
		}


		//トレイル
		float speed = D3DXVec3Length(&m_Velocity) * 1000.0f;
		if (speed > 1300.0f)
		{
			//トレイル設定
			m_Trail01->SetVertexPos(m_Position + GetRightQuaternion() * (m_Top.x * 1.5f) + GetForwardQuaternion() * -2.0f,
				m_Position + GetRightQuaternion() * (m_Top.x * 1.48f) + GetForwardQuaternion() * -2.0f);

			m_Trail02->SetVertexPos(m_Position - GetRightQuaternion() * (m_Top.x * 1.5f) + GetForwardQuaternion() * -2.0f,
				m_Position - GetRightQuaternion() * (m_Top.x * 1.48f) + GetForwardQuaternion() * -2.0f);
		}
	}

	//前進
	m_Velocity += GetForwardQuaternion() * 0.01f;

	//空気抵抗
	m_Velocity += m_Velocity * -0.02f;

	//移動減衰
	m_Position += m_Velocity;

	GameObject::Update();
}

void EnemyJet::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	m_WorldMatrix = scale * rot * trans;
	Renderer::SetWorldMatrix(&m_WorldMatrix);

	//ジェット噴射
	m_Fire01->SetPearent(m_WorldMatrix);
	m_Fire02->SetPearent(m_WorldMatrix);

	//collision
	m_Collision->SetPearent(m_WorldMatrix);
	SetCollider(m_Collision->GetMatrix());

	m_Model->Draw();

	GameObject::Draw();
}

void EnemyJet::Debug()
{
	const char* stateName[] = { "Attack","Escape","Seach","GroundRisk","OutofRange" };
	
	ImGui::Begin("EnemyJet");
	ImGui::Text("ID : %d", m_EnemyID);
	//ImGui::InputInt("ID", &m_EnemyID);
	ImGui::Text("EnemyState : %s", stateName[m_EnemyState]);
	ImGui::InputFloat3("Position", m_Position);
	ImGui::InputFloat4("Quaternion", m_Quaternion);
	ImGui::InputFloat3("Velocity", m_Velocity);
	ImGui::InputFloat3("RandomSeachPoint", m_Position + m_RandomSeachPoint);
	ImGui::InputInt("HP", &m_Health);
	ImGui::End();
}



//誘導関数
void EnemyJet::MoveHoming(D3DXVECTOR3 target, bool reverse)
{
	m_QuaAmountCount++;
	if (m_QuaAmountCount > ROTATION_FRAME)
	{
		//誘導計算
		D3DXVECTOR3 gaiseki;
		D3DXVECTOR3 forward = GetForwardQuaternion();

		D3DXVECTOR3 direction = target - m_Position;//法線ベクトル
		D3DXVec3Normalize(&direction, &direction);//法線を正規化

		D3DXVec3Cross(&gaiseki, &direction, &forward);//法線と前ベクトルの外積を求める
		float length = D3DXVec3Length(&gaiseki);//外積の長さを求める

		float angle = asinf(length);//外積の長さをアークサインで計算

		if (angle > 1.0f)
		{
			m_Velocity += GetForwardQuaternion() * 0.03f;
		}

		if (!reverse)
		{
			angle *= -1.0f;
		}

		if (angle != 0.0f)
		{
			D3DXVec3Normalize(&gaiseki, &gaiseki);//外積を正規化
			D3DXQUATERNION quat;
			D3DXQuaternionRotationAxis(&quat, &gaiseki, angle);//外積を軸に角度分回す
			D3DXQUATERNION q = m_Quaternion * quat;
			D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &q, ROTATION_AMOUNT);
		}
		m_QuaAmountCount = 0;
	}
}

//0:攻撃 1:退避 2:探索
void EnemyJet::EnemyStateChange(int number)
{
	switch (number)
	{
	case 0:
		m_EnemyState = ENEMY_ATTACK;
		break;
	case 1:
		m_EnemyState = ENEMY_ESCAPE;
		break;
	case 2:
		m_EnemyState = ENEMY_SEACH;
		break;

	default:
		break;
	}
}


//↓ステートパターン↓//

// 攻撃
void EnemyJet::Attack()
{

	//プレイヤーの後ろに回る
	MoveHoming(m_Jet->GetPosition() + m_Jet->GetForwardQuaternion() * -m_RandomTracking, false);

	//ミサイル発射
	if (View(60.0f, MAX_VIEW_LENGTH,m_Position,m_Jet->GetPosition(),GetForwardQuaternion()))
	{
		if (m_Shot)
		{
			auto missile = m_Scene->AddGameObject<Missile>(1);
			missile->SetPosition(m_Position);
			missile->SetQuaternion(m_Quaternion);
			missile->SetEnemyMissile(true);
			missile->SetPlayerTrack(true);
			m_Shot = false;
			m_ShotCount = 0;
		}
	}

	if (!m_Shot)
	{
		m_ShotCount++;
		if (m_ShotCount > 60 * MISSILE_COOLDOWN)
		{
			m_Shot = true;
		}
	}

	//プレイヤーがステルスになると探索
	if (m_Jet->GetStealthFlg())
	{
		m_EnemyState = ENEMY_SEACH;
	}

	//ダメージを受けると退避
	if (m_OldHealth > m_Health)
	{
		m_EnemyState = ENEMY_ESCAPE;
		m_OldHealth = m_Health;
	}
}

// 退避
void EnemyJet::Escape()
{
	MoveHoming(m_Jet->GetPosition() + m_Jet->GetForwardQuaternion() * -m_RandomTracking, true);

	//速度
	m_Velocity -= GetForwardQuaternion() * 0.02f;

	D3DXVECTOR3 direction = m_Jet->GetPosition() - m_Position;
	float length = D3DXVec3Length(&direction);

	if (length > MAX_ESCAPE)
	{
		m_EnemyState = ENEMY_SEACH;
	}
}

// 探索
void EnemyJet::Seach()
{
	m_SeachCount++;
	if (m_SeachCount > 60 * 4)
	{
		//-2000～2000のランダム
		m_RandomSeachPoint.x = rand() % 4001 - 2000;
		m_RandomSeachPoint.y = rand() % 4001 - 2000;
		m_RandomSeachPoint.z = rand() % 4001 - 2000;
		m_SeachCount = 0;
	}
	D3DXVECTOR3 targetPos = m_Position + m_RandomSeachPoint;
	MoveHoming(targetPos, false);

	//発見
	if (View(90.0f, 2000.0f, m_Position, m_Jet->GetPosition(), GetForwardQuaternion()) && !m_Jet->GetStealthFlg())
	{
		m_EnemyState = ENEMY_ATTACK;
	}
}

//地面回避
void EnemyJet::GroundRisk()
{
	MoveHoming(D3DXVECTOR3(m_Position.x, 1200.0f, m_Position.z) + GetForwardQuaternion() * 50.0f, false);

	if (m_Position.y > RISK_RELEASE)
	{
		m_EnemyState = ENEMY_ATTACK;
	}
}

//範囲外
void EnemyJet::OutOfRange()
{
	MoveHoming(m_Jet->GetPosition(), false);
	m_Velocity += GetForwardQuaternion() * 0.03f;

	if (m_PlayerLength < MAX_RANGE)
	{
		m_EnemyState = m_StateKeep;
	}
}
