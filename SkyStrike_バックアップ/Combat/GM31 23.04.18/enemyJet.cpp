#include "main.h"
#include "renderer.h"
#include "enemyJet.h"
#include "lock.h"
#include "lockOn.h"
#include "scene.h"
#include "manager.h"
#include "player.h"
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
	m_Player = m_Scene->GetGameObject<Player>();

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

	m_Health = MAX_HEALTH;
	m_OldHealth = m_Health;

	m_EnemyID = m_EnemyCount;
	m_EnemyCount++;

	//頂点取得
	m_Model->GetModelVertex("asset\\model\\F-15\\F-15.obj", m_Top);

	// 0から200までの範囲に10を足して調整
	m_RandomTracking = std::rand() % 201 + 10;

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
			m_Player->SwitchTarget();
			m_Player->GetLockOn()->SetAlpha(0.0f);
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

			case ENAMY_GROUNDRISK:
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


		//地面に当たらないようにする
		m_RiskPoint = m_Position + GetForwardQuaternion() * 500.0f;
		if (m_RiskPoint.y < 0.0f)
		{
			m_StateKeep = m_EnemyState;

			if (m_StateKeep == ENAMY_GROUNDRISK)
				m_StateKeep = ENEMY_SEACH;

			m_EnemyState = ENAMY_GROUNDRISK;
		}

		//移動範囲制限
		D3DXVECTOR3 distance = m_Player->GetPosition() - m_Position;
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

		//ImGui表示
		//ImGui::Begin("Enemy");
		//ImGui::InputFloat3("pos", m_Position);
		//ImGui::InputFloat("length", &m_PlayerLength);
		//ImGui::InputFloat4("qua", m_Quaternion);
		//ImGui::InputFloat3("vel", m_Velocity);
		//ImGui::InputInt("HP", &m_Health);
		//ImGui::End();
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

//視界関数
bool EnemyJet::EnemyView(D3DXVECTOR3 forward, float fieldOfViewRadians, float viewDistancee)
{
	// 視野範囲内かどうかの判定
	D3DXVECTOR3 normalizedDirection;
	D3DXVECTOR3 direction = m_Player->GetPosition() - m_Position;
	D3DXVec3Normalize(&normalizedDirection, &direction);
	D3DXVECTOR3 houkou = forward;
	float dotProduct = D3DXVec3Dot(&houkou, &normalizedDirection);
	float angle = acos(dotProduct);
	fieldOfViewRadians = D3DXToRadian(fieldOfViewRadians);
	bool isInFieldOfView = angle <= fieldOfViewRadians / 2.0f;

	// 視野距離内かどうかの判定
	D3DXVECTOR3 dice = m_Position - m_Player->GetPosition();
	float distance = D3DXVec3Length(&dice);
	bool isInViewDistance = distance <= viewDistancee;

	return isInFieldOfView && isInViewDistance;
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
	MoveHoming(m_Player->GetPosition() + m_Player->GetForwardQuaternion() * -m_RandomTracking, false);

	//ミサイル発射
	if (EnemyView(GetForwardQuaternion(), 60.0f, MAX_VIEW_LENGTH))
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
	if (m_Player->GetStealthFlg())
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
	MoveHoming(m_Player->GetPosition() + m_Player->GetForwardQuaternion() * -m_RandomTracking, true);

	//速度
	m_Velocity -= GetForwardQuaternion() * 0.02f;

	D3DXVECTOR3 direction = m_Player->GetPosition() - m_Position;
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
		m_RandomSeachPoint.x = rand() % (4001) - 2000;
		m_RandomSeachPoint.y = rand() % (4001) - 2000;
		m_RandomSeachPoint.z = rand() % (4001) - 2000;
		m_SeachCount = 0;
	}
	D3DXVECTOR3 targetPos = m_Position + m_RandomSeachPoint;
	MoveHoming(targetPos, false);

	//発見
	if (EnemyView(GetForwardQuaternion(), 90.0f, 2000.0f) && !m_Player->GetStealthFlg())
	{
		m_EnemyState = ENEMY_ATTACK;
	}
}

//地面回避
void EnemyJet::GroundRisk()
{
	MoveHoming(D3DXVECTOR3(m_Position.x, 1000.0f, m_Position.z) + GetForwardQuaternion() * 50.0f, false);

	if (m_RiskPoint.y > 1000.0f)
	{
		m_EnemyState = m_StateKeep;
	}
}

//範囲外
void EnemyJet::OutOfRange()
{
	MoveHoming(m_Player->GetPosition(), false);
	m_Velocity += GetForwardQuaternion() * 0.03f;

	if (m_PlayerLength < MAX_RANGE)
	{
		m_EnemyState = m_StateKeep;
	}
}
