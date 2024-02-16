#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "player.h"
#include "bullet.h"
#include "cylinder.h"
#include "box.h"
#include "audio.h"
#include "score.h"
#include "missile.h"
#include "explosion.h"
#include "lockOn.h"
#include "lock.h"
#include "enemy.h"
#include "camera.h"
#include "cross.h"
#include "trail.h"
#include "missileUI.h"
#include "hpGauge.h"
#include "flare.h"
#include "enemyDistance.h"
#include "effectManager.h"
#include "textureManager.h"

void Player::Init()
{
	m_DepthEnable = true;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\DepthShadowEnvMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\DepthShadowEnvMappingPS.cso");


	m_Model = new Model();
	if(m_GameEnable)
		m_Model->Load("asset\\model\\F-35\\F-35B-Off.obj");
	else
		m_Model->Load("asset\\model\\F-35\\F-35B-On.obj");


	if (m_GameEnable)
	{
		m_Velocity.z = 1.0f;

		//テスト
		m_MissileSetFlg00 = true;
		m_MissileSetFlg01 = true;
		missile00 = m_Scene->AddGameObject<Missile>(1);
		missile01 = m_Scene->AddGameObject<Missile>(1);
		missile00->SetShot(false);
		missile01->SetShot(false);

		//頂点取得
		m_Model->GetModelVertex("asset\\model\\F-35\\F-35B-Off.obj", m_Top);

		//トレイル生成
		m_Trail01 = m_Scene->AddGameObject<Trail>(1);
		m_Trail02 = m_Scene->AddGameObject<Trail>(1);
	}

	m_Scene = Manager::GetScene();

	m_MinigunSE = AddComponet<Audio>();
	m_MinigunSE->Load("asset\\audio\\minigun.wav");
	m_MinigunSE->Volume(0.05f);

	m_MissileSE = AddComponet<Audio>();
	m_MissileSE->Load("asset\\audio\\missile.wav");
	m_MissileSE->Volume(0.3f);

	m_LockOnSE = AddComponet<Audio>();
	m_LockOnSE->Load("asset\\audio\\lockon.wav");
	m_LockOnSE->Volume(0.01f);

	m_LockSetSE = AddComponet<Audio>();
	m_LockSetSE->Load("asset\\audio\\lockonset.wav");
	m_LockSetSE->Volume(0.01f);

	m_MissileAmount = MAX_MISSILE;
	m_FlareAmount = MAX_FLARE;
	m_StealthAmount = MAX_STEALTH;

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/dissolve.png",
		NULL,
		NULL,
		&m_DissolveTexture,
		NULL);
	assert(m_DissolveTexture);

	GameObject::Init();
}

void Player::Uninit()
{
	m_Model->Unload();

	if(m_GameEnable)
	m_DissolveTexture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Player::Update()
{
	auto enemys = m_Scene->GetGameObjects<Enemy>();
	auto missileuis = m_Scene->GetGameObjects<MissileUI>();
	auto camera = m_Scene->GetGameObject<Camera>();
	auto hp = m_Scene->GetGameObject<HpGauge>();
	auto distance = m_Scene->GetGameObject<EnemyDistance>();
	auto texture = m_Scene->GetGameObject<TextureManager>();

	if(m_GameEnable)
	{
		m_fVec = D3DXVec3Length(&m_Velocity);

		//ロックオンUI
		m_LockOn = m_Scene->GetGameObject<LockOn>();
		//クロスヘアUI
		m_Cross = m_Scene->GetGameObject<Cross>();

		//ロール
		{
			float angle = 0.0f;

			if (Input::GetKeyPress('A'))
			{
				angle = 0.03f;
			}
			else if (Input::GetKeyPress('D'))
			{
				angle = -0.03f;
			}

			//ロール回転
			D3DXVECTOR3 axis = GetForwardQuaternion();
			D3DXQUATERNION quat;
			D3DXQuaternionRotationAxis(&quat, &axis, angle);
			m_Endquat = m_Endquat * quat;
			D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &m_Endquat, 0.001f);
		}

		//ピッチ
		{
			float angle = 0.0f;

			//減速時の重力
			if (m_fVec <= MIN_SPEED)
			{
				m_Velocity += GetForwardQuaternion() * 0.01f;
				//angle = 0.01f;
				//D3DXVECTOR3 axis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				//D3DXQUATERNION quat;
				//D3DXQuaternionRotationAxis(&quat, &axis, angle);
				//m_Endquat = m_Endquat * quat;
				//D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &m_Endquat, 0.1f);
			}
			else
			{
				if (Input::GetKeyPress(VK_SHIFT))
				{
					angle = -0.015f;
				}
				else if (Input::GetKeyPress(VK_CONTROL))
				{
					angle = 0.015f;
				}

				D3DXVECTOR3 axis = GetRightQuaternion();
				D3DXQUATERNION quat;
				D3DXQuaternionRotationAxis(&quat, &axis, angle);
				m_Endquat = m_Endquat * quat;
				D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &m_Endquat, 0.1f);
			}
		}

		//ヨー
		{
			float angle = 0.0f;

			if (Input::GetKeyPress('E'))
			{
				m_Velocity += GetRightQuaternion() * 0.005f;
				angle = 0.005f;
			}
			else if (Input::GetKeyPress('Q'))
			{
				m_Velocity -= GetRightQuaternion() * 0.005f;
				angle = -0.005f;

			}

			D3DXVECTOR3 axis = GetTopQuaternion();
			D3DXQUATERNION quat;
			D3DXQuaternionRotationAxis(&quat, &axis, angle);
			m_Endquat = m_Endquat * quat;
			D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &m_Endquat, 0.1f);
		}


		//ミサイル装着時
		if (m_MissileSetFlg00)
		{
			missile00->SetPosition(m_Position + GetForwardQuaternion() * -0.8f + GetRightQuaternion() * 5.0f + GetTopQuaternion() * -0.6f);
			missile00->SetQuaternion(m_Quaternion);
			missile00->SetVelocity(m_Velocity);
		}
		if (m_MissileSetFlg01)
		{
			missile01->SetPosition(m_Position + GetForwardQuaternion() * -0.8f + GetRightQuaternion() * -5.0f + GetTopQuaternion() * -0.6f);
			missile01->SetQuaternion(m_Quaternion);
			missile01->SetVelocity(m_Velocity);
		}

		//ミサイルクールタイム
		if (missileuis[0]->GetGauge() == 100)
		{
			missile00 = m_Scene->AddGameObject<Missile>(1);

			m_MissileSetFlg00 = true;
		}
		if (missileuis[1]->GetGauge() == 100)
		{
			missile01 = m_Scene->AddGameObject<Missile>(1);

			m_MissileSetFlg01 = true;
		}

		// ミサイル発射
		if (Input::GetKeyTrigger(VK_LBUTTON))
		{
			//ミサイルを撃った対象のID取得
			if(!enemys.empty())
			{
				m_MissileLockID = m_EnemyID;
			}

			if (m_MissilePosChange == 0 && m_MissileSetFlg00 && m_MissileAmount > 0)
			{

				missile00->SetVelocity(GetForwardQuaternion() * 2.0f + m_Velocity);
				missile00->SetShot(true);

				if (m_LockSetFlg)
				{
					missile00->SetEnemyTrack(true);
				}

				missileuis[0]->SetGauge(0);

				m_MissileSE->Play(false);
				m_MissileSetFlg00 = false;
				m_MissilePosChange++;
				m_MissileAmount--;
			}
			else if (m_MissileSetFlg01 && m_MissileAmount > 0)
			{

				missile01->SetVelocity(GetForwardQuaternion() * 2.0f + m_Velocity);
				missile01->SetShot(true);

				if (m_LockSetFlg)
				{
					missile01->SetEnemyTrack(true);
				}

				missileuis[1]->SetGauge(0);

				m_MissileSE->Play(false);
				m_MissileSetFlg01 = false;
				m_MissilePosChange = 0;
				m_MissileAmount--;
			}
		}


		//加速
		if (Input::GetKeyPress('W'))
		{
			m_Velocity += GetForwardQuaternion() * 0.02f;
		}
		else if (Input::GetKeyPress('S')) //減速
		{
			m_Velocity -= GetForwardQuaternion() * 0.01f;
		}

		if (Input::GetKeyTrigger(VK_RBUTTON))
		{
			m_MinigunSE->Play(true);
		}

		// マシンガン発射
		if (Input::GetKeyPress(VK_RBUTTON))
		{
			Bullet* bullet = m_Scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Position + GetTopQuaternion() * -1.6f);
			bullet->SetVelocity(m_Velocity + GetForwardQuaternion() * 5.0f);
			bullet->SetQuaternion(m_Quaternion);
			//EffectManager* effect = m_Scene->AddGameObject<EffectManager>(1);
			//effect->LoadSelect(MUZZLE_FLASH);
		}
		else
		{
			if (m_MinigunSE->GetPlay() == true)
			{
				m_MinigunSE->Stop();
			}
		}

		//ステルスモード
		if (Input::GetKeyTrigger('T'))
		{
			//フラグ切り替え
			if(m_StealthAmount >= 100.0f)
				m_StealthModeFlg = true;
			else
				m_StealthModeFlg = false;
		}

		//ステルス処理(ディゾルブ)
		if (m_StealthModeFlg)
		{
			m_Threshold += 0.02f;

			//完全に消えたとき
			if (m_Threshold > 1.1f)
			{
				m_Threshold = 1.1f;
			}

			for (Enemy* enemy : enemys)
			{
				enemy->SetPlayerStealth(true);
			}

			if (m_StealthAmount > 0)
			{
				m_StealthAmount -= STEALTH_DOWN_SPEED;
			}
			else
			{
				m_StealthModeFlg = false;
			}
		}
		else
		{
			m_Threshold -= 0.02f;
			//完全に出たとき
			if (m_Threshold < 0.0f)
			{
				m_Threshold = 0.0f;
			}

			for (Enemy* enemy : enemys)
			{
				enemy->SetPlayerStealth(false);
			}

			if (m_StealthAmount < 100)
			{
				m_StealthAmount += STEALTH_UP_SPEED;
			}
		}

		//フレア発射
		if (Input::GetKeyTrigger('G') && m_FlareAmount != 0)
		{
			if (!m_FlareFlg)
			{
				m_FlareFlg = true;
				m_FlareAmount--;
				m_FlareCount = 0;
			}
		}

		if (m_FlareFlg)
		{
			if (m_FlareCount == 0)
			{
				ShotFlare();
			}
			else if (m_FlareCount == FRARE_TIME)
			{
				ShotFlare();
			}
			else if (m_FlareCount == FRARE_TIME * 2)
			{
				ShotFlare();
			}
			else if (m_FlareCount == FRARE_TIME * 3)
			{
				ShotFlare();
			}
			else if (m_FlareCount > FRARE_TIME * 8)
			{
				m_FlareFlg = false;
			}
			m_FlareCount++;
		}


		//ターゲット変更
		if (Input::GetKeyTrigger('R'))
		{
			SwitchTarget();

			m_LockSetSE->Stop();
			if (m_OldEnemyID != m_EnemyID)
			{
				m_LockOnType = 50.0f;

				m_OldEnemyID = m_EnemyID;
				m_LockOnSE->Play(true);
			}
		}

		//ロックオン処理
		for (Enemy* enemy : enemys)
		{
			//UI処理
			auto length = enemy->GetPosition() - m_Position;
			float vecs = D3DXVec3Length(&length);

			//distance
			distance->PrintCount(D3DXVECTOR2(enemy->GetPosition().x, enemy->GetPosition().y),vecs);

			vecs /= 50.0f;//サイズ調整

			//最小サイズ値
			if (vecs <= 3.0f)
			{
				vecs = 3.0f;
			}

			//UIセット
			enemy->GetAddLock()->SetPosition(enemy->GetPosition());
			enemy->GetAddLock()->SetScale(D3DXVECTOR3(vecs, vecs, 0.0f));
			
			//distance
			distance->SetScale(D3DXVECTOR3(vecs, vecs, 0.0f));

			//ロックオン処理
			if (enemy->GetEnemyID() == m_EnemyID)
			{
				m_EnemyPos = enemy->GetPosition();//lockされている敵のpos保存

				//lockon
				if (m_LockOnFlg == true)
				{
					if (m_LockOnType > 0)
					{
						m_LockOnType -= 0.5f;
					}

					//lockOnセット
					m_LockOn->SetPosition(D3DXVECTOR3(enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z) + m_LockOn->GetRight() * m_LockOnType + m_LockOn->GetTop() * m_LockOnType);
					m_LockOn->SetAlpha(1.0f);
					m_LockOn->SetScale(D3DXVECTOR3(vecs, vecs, 0.0f));

					enemy->GetAddLock()->SetLockColor(true);//カラー赤

					//ロックオン完了時
					if (m_LockOn->GetPosition() == enemy->GetPosition())
					{
						enemy->GetAddLock()->BlinkingFlg(false);//lock
						m_LockSetFlg = true;
						m_LockOnSE->Stop();

						//ロックオンセットSE
						if (m_LockSetSeFlg)
						{
							m_LockSetSE->Play(true);
							m_LockSetSeFlg = false;
						}
					}
					else
					{
						enemy->GetAddLock()->BlinkingFlg(true);//lock点滅
						m_LockSetFlg = false;
					}
				}
				else
				{
					enemy->GetAddLock()->SetLockColor(false); //カラー緑
					enemy->GetAddLock()->BlinkingFlg(true); //点滅変数をセット

					m_LockOn->SetAlpha(0.0f); //lockOnのalpha非表示

					m_LockSetFlg = false;
				}
			}
			else
			{
				enemy->GetAddLock()->SetLockColor(false);//カラー緑
				enemy->GetAddLock()->BlinkingFlg(false);//lock表示
			}

			//enemy破壊時
			if (enemy->GetCrash())
			{
				m_LockOn->SetAlpha(0.0f); //lockOnのalpha非表示
				texture->SetDesReportFlg(true);
				m_OldEnemyID = m_EnemyID;
			}
		}



		//enemyが空になった時(全敵撃破時)
		if (enemys.empty())
		{
			m_LockOn->SetAlpha(0.0f);//lockOnが残らないように非表示
			m_LockSetSE->Stop();
		}

		//プレイヤーの視界内に入ったらロックオン
		if (PlayerView(80.0f, 2000.0f))
		{
			for (Enemy* enemy : enemys)
			{
				//lock中の敵をロックオン
				if (enemy->GetEnemyID() == m_EnemyID)
				{
					m_LockOnFlg = true;
				}
			}
		}
		else
		{
			for (Enemy* enemy : enemys)
			{
				//lock中の敵をロックオフ
				if (enemy->GetEnemyID() == m_EnemyID)
				{
					m_LockOnFlg = false;
					m_LockOnSE->Stop();
					m_LockSetSE->Stop();

					m_LockSetSeFlg = true;//SE再生用
				}
			}
		}

		//空気抵抗
		m_Velocity += m_Velocity * -0.01f + GetForwardQuaternion() * 0.012f;

		//移動減衰
		m_Position += m_Velocity;

		auto score = m_Scene->GetGameObjects<Score>();

		//速度と高度を表示
		score[0]->PrintCount(D3DXVECTOR2(500.0f, 400.0f), m_fVec * 1000.0f);
		score[1]->PrintCount(D3DXVECTOR2(1420.0f, 400.0f), m_Position.y);


		//障害物との衝突判定
		float groundHeight = 0.0f;

		//死亡時
		if (m_Position.y < groundHeight || hp->GetHP() < 0.0f)
		{
			if (m_DeathCount == 0)
			{
				auto explosion = m_Scene->AddGameObject<Explosion>(1);
				explosion->SetScale(D3DXVECTOR3(5.5f, 5.5f, 0.0f));
				explosion->SetPosition(m_Position);
				explosion->BomTime(5);
				m_Velocity *= 0;
				m_DeathFlg = true;
				m_DeathCount++;
			}
		}

		float speed = m_fVec * 1000.0f;

		//クロスヘア
		m_Cross->SetPosition(m_Position + GetForwardQuaternion() * 60.0f + GetTopQuaternion() * 2.0f);

		D3DXVECTOR3 OldPosition = m_Position;

		//トレイル
		if(speed > 1300.0f)
		{
			//トレイル設定
			m_Trail01->SetVertexPos(OldPosition + GetRightQuaternion() * m_Top.x + GetForwardQuaternion() * -2.0f,
									OldPosition + GetRightQuaternion() * (m_Top.x * 0.98f) + GetForwardQuaternion() * -2.0f);

			m_Trail02->SetVertexPos(OldPosition - GetRightQuaternion() * m_Top.x + GetForwardQuaternion() * -2.0f,
									OldPosition - GetRightQuaternion() * (m_Top.x * 0.98f) + GetForwardQuaternion() * -2.0f);
		}

		auto camera = m_Scene->GetGameObject<Camera>();
		D3DXMATRIX screenMatrix = camera->GetScreenMatrix();//マトリックス取得

		D3DXVECTOR3 crosspos = m_Cross->GetPosition();
		D3DXVECTOR3 screenCameraCross;//スクリーン上の敵の位置
		D3DXVec3TransformCoord(&screenCameraCross, &crosspos, &screenMatrix);//スクリーン上の敵位置を取得

		D3DXVECTOR3 enemypos;
		D3DXVECTOR3 screenCameraEnemy;//スクリーン上の敵の位置
		for (Enemy* enemy : enemys)
		{
			if (enemy->GetEnemyID() == GetMisLockID())
			{
				enemypos = enemy->GetPosition();
				D3DXVec3TransformCoord(&screenCameraEnemy, &enemypos, &screenMatrix);//スクリーン上の敵位置を取得
			}
		}

		//ImGui表示
		ImGui::Begin("PLAYER");
		ImGui::InputFloat3("Position", m_Position);
		ImGui::InputFloat3("PosVelocity", m_Velocity);
		ImGui::InputFloat4("Quaternion", m_Quaternion);
		ImGui::End();
	}

	GameObject::Update();
}

void Player::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// キューブマップ用テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, Renderer::GetCubeReflectShaderResourceView());

	// マトリクス設定
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	m_WorldMatrix = scale * rot * trans;
	Renderer::SetWorldMatrix(&m_WorldMatrix);



	// ディゾルブテクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(3, 1, &m_DissolveTexture);

	//ディゾルブ更新設定
	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveThreshold = m_Threshold;
	param.dissolveRange = 0.05f;
	Renderer::SetParameter(param);

	Renderer::SetATCEnable(true);
	m_Model->Draw();
	Renderer::SetATCEnable(false);

	GameObject::Draw();
}

//プレイヤーの視界
bool Player::PlayerView(float fieldOfViewRadians, float viewDistancee)
{
	// 視野範囲内かどうかの判定
	D3DXVECTOR3 normalizedDirection;
	D3DXVECTOR3 direction = m_EnemyPos - m_Position;
	D3DXVec3Normalize(&normalizedDirection, &direction);
	D3DXVECTOR3 houkou = GetForwardQuaternion();
	float dotProduct = D3DXVec3Dot(&houkou, &normalizedDirection);
	float angle = acos(dotProduct);
	fieldOfViewRadians = D3DXToRadian(fieldOfViewRadians);
	bool isInFieldOfView = angle <= fieldOfViewRadians / 2.0f;

	// 視野距離内かどうかの判定
	D3DXVECTOR3 dice = m_Position - m_EnemyPos;
	float distance = D3DXVec3Length(&dice);
	bool isInViewDistance = distance <= viewDistancee;

	return isInFieldOfView && isInViewDistance;
}

//ターゲット切り替え関数
void Player::SwitchTarget()
{
	auto enemys = m_Scene->GetGameObjects<Enemy>();
	auto camera = m_Scene->GetGameObject<Camera>();

	D3DXMATRIX screenMatrix = camera->GetScreenMatrix();//マトリックス取得
	D3DXVECTOR3 screenCameraEnemy;//スクリーン上の敵の位置
	float oldDistance = 1000.0f;

	m_LockSetSeFlg = true;//SE再生用

	for (Enemy* enemy : enemys)
	{
		//撃墜された敵はスキップ
		//if (enemy->GetCrash())
		//{
		//	continue;
		//}

		if(!enemy->GetCrash())
		{
			D3DXVECTOR3 enemypos = enemy->GetPosition();
			D3DXVec3TransformCoord(&screenCameraEnemy, &enemypos, &screenMatrix);//スクリーン上の敵位置を取得

			//2次元に変換
			D3DXVECTOR2 screenEnemy = D3DXVECTOR2(screenCameraEnemy.x, screenCameraEnemy.y - 0.330f);
			float distance = D3DXVec2LengthSq(&screenEnemy);

			if (distance < oldDistance)//一番距離が近い敵IDを取得
			{
				m_EnemyID = enemy->GetEnemyID();
				oldDistance = distance;//距離を保存
			}
		}
	}
}

//フレア発射関数
void Player::ShotFlare()
{
	for (int i = 0; i < 4; i++)
	{
		Flare* flare = m_Scene->AddGameObject<Flare>(1);

		//-10から10の範囲
		float rightRandom = static_cast<float>(rand()) / RAND_MAX * 20.0f - 10.0f;
		//5から15の範囲
		float forwardRandom = rand() % 11 + 5;

		flare->SetPosition(m_Position + GetForwardQuaternion() * -2.0f + GetTopQuaternion() * -2.0f);
		flare->SetVelocity(GetForwardQuaternion() * forwardRandom + GetRightQuaternion() * rightRandom);
	}

	auto missiles = m_Scene->GetGameObjects<Missile>();

	for (Missile* missile : missiles)
	{
		missile->SetFlare(true);
	}
}
