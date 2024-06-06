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
#include "missile.h"
#include "explosion.h"
#include "lockOn.h"
#include "lock.h"
#include "enemyJet.h"
#include "camera.h"
#include "cross.h"
#include "trail.h"
#include "missileUI.h"
#include "longMissileUI.h"
#include "hpGauge.h"
#include "flare.h"
#include "enemyDistance.h"
#include "effectManager.h"
#include "textureManager.h"
#include "inputx.h"
#include "longMissile.h"
#include "lockon2D.h"
#include "collisionBox.h"

SpecialWeapon Player::m_SpecialWeapon = WEAPON_LONG_MISSILE;

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

	m_Scene = Manager::GetScene();

	if (m_GameEnable)
	{
		m_Velocity.z = 1.0f;

		//テスト
		m_MissileSetFlg00 = true;
		m_MissileSetFlg01 = true;
		m_Missile00 = m_Scene->AddGameObject<Missile>(1);
		m_Missile01 = m_Scene->AddGameObject<Missile>(1);
		m_Missile00->SetShot(false);
		m_Missile01->SetShot(false);

		m_SpecialMissileSetFlg00 = true;
		m_SpecialMissileSetFlg01 = true;

		//特殊兵装
		switch (m_SpecialWeapon)
		{
		case WEAPON_LONG_MISSILE:
			m_LongUI_0 = m_Scene->AddGameObject<LongMissileUI>(2);
			m_LongUI_1 = m_Scene->AddGameObject<LongMissileUI>(2);
			
			m_LongMissile00 = m_Scene->AddGameObject<LongMissile>(1);
			m_LongMissile01 = m_Scene->AddGameObject<LongMissile>(1);
			
			m_SpecialMissileAmount = MAX_LONG;
			break;
		case WEAPON_SHORT_MISSILE:
			break;
		case WEAPON_RAIL_GUN:
			break;
		default:
			break;
		}

		//頂点取得
		m_Model->GetModelVertex("asset\\model\\F-35\\F-35B-Off.obj", m_Top);

		//トレイル生成
		m_Trail01 = m_Scene->AddGameObject<Trail>(1);
		m_Trail02 = m_Scene->AddGameObject<Trail>(1);

		//collision
		m_Collision = m_Scene->AddGameObject<CollisionBox>(1);
		m_Collision->SetScale(D3DXVECTOR3(6.0f, 1.0f, 8.0f));
	}

	//SE
	m_MinigunSE = AddComponet<Audio>();
	m_MinigunSE->Load("asset\\audio\\minigun.wav");

	m_LockOnSE = AddComponet<Audio>();
	m_LockOnSE->Load("asset\\audio\\lockon.wav");

	m_LockSetSE = AddComponet<Audio>();
	m_LockSetSE->Load("asset\\audio\\lockonset.wav");

	m_StealthSE = AddComponet<Audio>();
	m_StealthSE->Load("asset\\audio\\stealth.wav");

	m_MissileAlertSE = AddComponet<Audio>();
	m_MissileAlertSE->Load("asset\\audio\\alert.wav");

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
	{
		m_DissolveTexture->Release();
		m_Trail01->SetDestroy();
		m_Trail02->SetDestroy();
		m_Collision->SetDestroy();
	}

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Player::Update()
{
	//ボリューム
	m_MinigunSE->Volume(Scene::m_SEVolume * (0.05f * 2));
	m_LockOnSE->Volume(Scene::m_SEVolume * (0.03f * 2));
	m_LockSetSE->Volume(Scene::m_SEVolume * (0.03f * 2));
	m_StealthSE->Volume(Scene::m_SEVolume * (0.1f * 2));
	m_MissileAlertSE->Volume(Scene::m_SEVolume * (0.03f * 2));

	if(m_GameEnable)
	{
		auto enemys = m_Scene->GetGameObjects<EnemyJet>();
		auto missileuis = m_Scene->GetGameObjects<MissileUI>();
		auto lmissileuis = m_Scene->GetGameObjects<LongMissileUI>();
		auto camera = m_Scene->GetGameObject<Camera>();
		auto hp = m_Scene->GetGameObject<HpGauge>();
		auto distance = m_Scene->GetGameObject<EnemyDistance>();
		auto missiles = m_Scene->GetGameObjects<Missile>();
		auto texture = m_Scene->GetGameObject<TextureManager>();
		
		m_Lockon2D = m_Scene->GetGameObject<Lockon2D>();
		m_LockOn = m_Scene->GetGameObject<LockOn>();
		//m_Cross = m_Scene->GetGameObject<Cross>();

		m_fVec = D3DXVec3Length(&m_Velocity);

		//ロール
		if(!m_RoleLock)
		{
			float angle = 0.0f;

			if (Input::GetKeyPress('A'))
			{
				angle = ROLL_SPEED;
			}
			else if (Input::GetKeyPress('D'))
			{
				angle = -ROLL_SPEED;
			}

			if (InputX::GetThumbLeftX(0) != 0)
			{
				angle = -ROLL_SPEED * InputX::GetThumbLeftX(0);
			}

			//ロール回転
			D3DXVECTOR3 axis = GetForwardQuaternion();
			D3DXQUATERNION quat;
			D3DXQuaternionRotationAxis(&quat, &axis, angle);
			m_Endquat = m_Endquat * quat;
			D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &m_Endquat, 0.001f);
		}

		//ピッチ
		if(!m_PitchLock)
		{
			float angle = 0.0f;

			//減速時の重力
			if (m_fVec <= MIN_SPEED)
			{
				m_Velocity += GetForwardQuaternion() * 0.01f;
			}
			else  
			{
				if (texture->GetPitchFlipCheck())//ピッチ反転
				{
					if (Input::GetKeyPress(VK_SHIFT))
					{
						angle = PITCH_SPEED;
						//m_Velocity += GetForwardQuaternion() * SPEED_UP / 1.5f;
					}
					else if (Input::GetKeyPress(VK_CONTROL))
					{
						angle = -PITCH_SPEED;
						//m_Velocity += GetForwardQuaternion() * SPEED_UP / 1.5f;
					}

					if (InputX::GetThumbLeftY(0) != 0)
					{
						angle = PITCH_SPEED * InputX::GetThumbLeftY(0);
						//m_Velocity += GetForwardQuaternion() * SPEED_UP / 1.5f;
					}
				}
				else
				{
					if (Input::GetKeyPress(VK_SHIFT))
					{
						angle = -PITCH_SPEED;
						//m_Velocity += GetForwardQuaternion() * SPEED_UP / 1.5f;
					}
					else if (Input::GetKeyPress(VK_CONTROL))
					{
						angle = PITCH_SPEED;
						//m_Velocity += GetForwardQuaternion() * SPEED_UP / 1.5f;
					}

					if (InputX::GetThumbLeftY(0) != 0)
					{
						angle = -PITCH_SPEED * InputX::GetThumbLeftY(0);
						//m_Velocity += GetForwardQuaternion() * SPEED_UP / 1.5f;
					}
				}

				D3DXVECTOR3 axis = GetRightQuaternion();
				D3DXQUATERNION quat;
				D3DXQuaternionRotationAxis(&quat, &axis, angle);
				m_Endquat = m_Endquat * quat;
				D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &m_Endquat, 0.1f);
			}
		}

		//ヨー
		if(!m_YawLock)
		{
			float angle = 0.0f;

			if (Input::GetKeyPress('E') || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_RIGHT_SHOULDER))
			{
				m_Velocity += GetRightQuaternion() * YAW_SPEED;
				angle = YAW_SPEED;
			}
			else if (Input::GetKeyPress('Q') || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_LEFT_SHOULDER))
			{
				m_Velocity -= GetRightQuaternion() * YAW_SPEED;
				angle = -YAW_SPEED;

			}

			D3DXVECTOR3 axis = GetTopQuaternion();
			D3DXQUATERNION quat;
			D3DXQuaternionRotationAxis(&quat, &axis, angle);
			m_Endquat = m_Endquat * quat;
			D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &m_Endquat, 0.1f);
		}

		//ミサイルアラートSE
		for (Missile* missile : missiles)
		{
			if (missile->GetPlayerTrack())
			{
				if (!m_MissileAlertSE->GetPlay())
				{
					m_MissileAlertSE->Play(true);
				}
			}
		}

		//兵装切り替え
		if (Input::GetKeyTrigger('C') || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_X))
		{
			m_SpecialWeaponMode = !m_SpecialWeaponMode;
		}


		if (!m_SpecialWeaponMode)
		{
			missileuis[0]->SetDrawFlg(true);
			missileuis[1]->SetDrawFlg(true);
			lmissileuis[0]->SetDrawFlg(false);
			lmissileuis[1]->SetDrawFlg(false);

			ShotMissile();
		}
		else
		{
			missileuis[0]->SetDrawFlg(false);
			missileuis[1]->SetDrawFlg(false);

			//特殊兵装ステートマシン
			switch (m_SpecialWeapon)
			{
			case WEAPON_LONG_MISSILE:
				ShotLong();
				lmissileuis[0]->SetDrawFlg(true);
				lmissileuis[1]->SetDrawFlg(true);
				break;
			case WEAPON_SHORT_MISSILE:
				ShotShort();
				break;
			case WEAPON_RAIL_GUN:
				ShotRail();
				break;
			default:
				break;
			}
		}


		//Update処理
		{
			//特殊兵装ステートマシン
			switch (m_SpecialWeapon)
			{
			case WEAPON_LONG_MISSILE:
				UpdateLong();
				break;
			case WEAPON_SHORT_MISSILE:
				UpdateShort();
				break;
			case WEAPON_RAIL_GUN:
				UpdateRail();
				break;
			default:
				break;
			}
			UpdateMissile();
		}



		if(!m_EngineLock)
		{
			//キーボード
			if (Input::GetKeyPress('W'))//加速
			{
				m_Velocity += GetForwardQuaternion() * SPEED_UP;
			}
			else if (Input::GetKeyPress('S')) //減速
			{
				m_Velocity -= GetForwardQuaternion() * SPEED_DOWN;
			}
			//コントローラー
			if (InputX::GetRightTrigger(0) > 0)
			{
				//正規化
				float trigger = InputX::GetRightTrigger(0) / 255;

				m_Velocity += (GetForwardQuaternion() * SPEED_UP) * trigger;
			}
			else if (InputX::GetLeftTrigger(0) > 0) //減速
			{
				//正規化
				float trigger = InputX::GetLeftTrigger(0) / 255;

				m_Velocity -= (GetForwardQuaternion() * SPEED_DOWN) * trigger;
			}
		}


		if (Input::GetKeyTrigger(VK_RBUTTON) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_A))
		{
			m_MinigunSE->Play(true);
		}

		// マシンガン発射
		if (Input::GetKeyPress(VK_RBUTTON) || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_A))
		{
			Bullet* bullet = m_Scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Position + GetTopQuaternion() * -1.6f);
			bullet->SetVelocity(m_Velocity + GetForwardQuaternion() * 5.0f);
			bullet->SetQuaternion(m_Quaternion);
			InputX::SetVibration(0, 5);

			//EffectManager* effect = m_Scene->AddGameObject<EffectManager>(1);
			//effect->LoadSelect(MUZZLE_FLASH);
		}
		else
		{
			if (m_MinigunSE->GetPlay())
			{
				m_MinigunSE->Stop();
				InputX::StopVibration(0);
			}
		}

		//ステルスモード
		if (Input::GetKeyTrigger('T') || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_RIGHT_THUMB))
		{
			m_StealthSE->Play(false);
			//フラグ切り替え
			if(m_StealthAmount >= STEALTH_MAX)
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

			if (m_StealthAmount < STEALTH_MAX)
			{
				m_StealthAmount += STEALTH_UP_SPEED;
			}
		}

		//フレア発射
		if (Input::GetKeyTrigger('G') || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_LEFT_THUMB) && m_FlareAmount != 0)
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
		if (Input::GetKeyTrigger('R') || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_Y) && !enemys.empty())
		{
			SwitchTarget();

			m_Lockon2D->LockMoveReset();
			m_LockSetSE->Stop();
			m_LockSeFlg = true;
		}

		//ロックオン処理
		for (EnemyJet* enemy : enemys)
		{
			//UI処理
			auto length = enemy->GetPosition() - m_Position;
			float vecs = D3DXVec3Length(&length);

			//distance
			//distance->PrintCount(D3DXVECTOR2(0.0f, 0.0f),vecs);

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
			//distance->SetPosition(enemy->GetPosition());
			//distance->SetScale(D3DXVECTOR3(vecs, vecs, 0.0f));


			//ロックオン処理
			if (enemy->GetEnemyID() == m_EnemyID && !enemys.empty() && !enemy->GetCrash())
			{
				m_EnemyPos = enemy->GetPosition();//lockされている敵のpos保存

				//lockon
				if (m_LockOnFlg == true)
				{
					enemy->GetAddLock()->SetLockColor(true);//カラー赤

					//ロックオン完了時
					if (m_Lockon2D->GetLockFinish())
					{
						m_LockOn->SetPosition(enemy->GetPosition());
						m_LockOn->SetAlpha(1.0f);
						m_LockOn->SetScale(D3DXVECTOR3(vecs, vecs, 0.0f));

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
						m_LockOn->SetAlpha(0.0f); //lockOnのalpha非表示
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
				if(enemys.size() <= 1)
					m_LockOn->SetAlpha(0.0f);

				m_OldEnemyID = m_EnemyID;
			}
		}

		//enemyが空になった時(全敵撃破時)
		if (enemys.empty())
		{
			m_LockOn->SetAlpha(0.0f);//lockOnが残らないように非表示
			m_LockSetSE->Stop();
			m_LockOnSE->Stop();
		}


		//プレイヤーの視界内に入ったらロックオン
		if (PlayerView(80.0f, 2000.0f))
		{
			for (EnemyJet* enemy : enemys)
			{
				//lock中の敵をロックオン
				if (enemy->GetEnemyID() == m_EnemyID)
				{
					m_LockOnFlg = true;
				}
			}

			if(m_LockSeFlg)
			{
				m_LockOnSE->Play(true);
				m_LockSeFlg = false;
			}
			
			m_Lockon2D->LockonMove();
		}
		else
		{
			for (EnemyJet* enemy : enemys)
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
			m_LockSeFlg = true;
			m_Lockon2D->LockMoveReset();
			m_Lockon2D->SetDraw(false);
		}

		//空気抵抗
		m_Velocity += m_Velocity * -0.01f + GetForwardQuaternion() * 0.012f;

		//移動減衰
		m_Position += m_Velocity;

		//障害物との衝突判定
		float groundHeight = 0.0f;

		//死亡時
		if (m_Position.y < groundHeight || hp->GetHP() < 0.0f)
		{
			if (m_DeathCount == 0)
			{
				auto explosion = m_Scene->AddGameObject<Explosion>(1);
				explosion->Spawn(m_Position, D3DXVECTOR2(5.5, 5.5), 5);

				m_Velocity *= 0;
				m_DeathFlg = true;
				
				m_DeathCount++;
			}
		}

		float speed = m_fVec * 1000.0f;

		//クロスヘア
		//m_Cross->SetPosition(m_Position + GetForwardQuaternion() * 60.0f + GetTopQuaternion() * 2.0f);

		D3DXVECTOR3 OldPosition = m_Position;

		//トレイル
		if(speed > 1300.0f)
		{
			//トレイル設定
			m_Trail01->SetVertexPos(OldPosition + GetRightQuaternion() * m_Top.x + GetForwardQuaternion() * -2.5f,
									OldPosition + GetRightQuaternion() * (m_Top.x * 0.98f) + GetForwardQuaternion() * -2.5f);

			m_Trail02->SetVertexPos(OldPosition - GetRightQuaternion() * m_Top.x + GetForwardQuaternion() * -2.5f,
									OldPosition - GetRightQuaternion() * (m_Top.x * 0.98f) + GetForwardQuaternion() * -2.5f);
		}

		//ImGui表示
		//ImGui::Begin("PLAYER");
		//ImGui::InputFloat3("Position", m_Position);
		//ImGui::InputFloat3("Velocity", m_Velocity);
		//ImGui::InputFloat4("Quaternion", m_Quaternion);
		//ImGui::End();
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

	if(m_GameEnable)
	{
		m_Collision->SetPearent(m_WorldMatrix);
		SetCollider(m_Collision->GetMatrix());
	}


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

void Player::UpdateMissile()
{
	auto missileuis = m_Scene->GetGameObjects<MissileUI>();

	//通常ミサイル
	{
		//ミサイル装着時
		if (m_MissileSetFlg00)
		{
			m_Missile00->SetPosition(m_Position + GetForwardQuaternion() * -0.8f + GetRightQuaternion() * 5.0f + GetTopQuaternion() * -0.6f);
			m_Missile00->SetQuaternion(m_Quaternion);
			m_Missile00->SetVelocity(m_Velocity);
		}
		if (m_MissileSetFlg01)
		{
			m_Missile01->SetPosition(m_Position + GetForwardQuaternion() * -0.8f + GetRightQuaternion() * -5.0f + GetTopQuaternion() * -0.6f);
			m_Missile01->SetQuaternion(m_Quaternion);
			m_Missile01->SetVelocity(m_Velocity);
		}

		//ミサイルクールタイム
		if (missileuis[0]->GetGauge() == 100)
		{
			m_Missile00 = m_Scene->AddGameObject<Missile>(1);

			m_MissileSetFlg00 = true;
		}
		if (missileuis[1]->GetGauge() == 100)
		{
			m_Missile01 = m_Scene->AddGameObject<Missile>(1);

			m_MissileSetFlg01 = true;
		}
	}
}

void Player::ShotMissile()
{
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();
	auto missileuis = m_Scene->GetGameObjects<MissileUI>();

	// ミサイル発射
	if ((Input::GetKeyTrigger(VK_LBUTTON) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_B)))
	{
		//ミサイルを撃った対象のID取得
		if (!enemys.empty())
		{
			m_MissileLockID = m_EnemyID;
		}

		if (m_MissilePosChange == 0 && m_MissileSetFlg00 && m_MissileAmount > 0)
		{

			m_Missile00->SetVelocity(GetForwardQuaternion() * 2.0f + m_Velocity);
			m_Missile00->SetShot(true);

			missileuis[0]->SetGauge(0);

			InputX::SetVibration(0, 20);
			m_MissileVibCount = 0;

			m_MissileSetFlg00 = false;
			m_MissilePosChange++;
			m_MissileAmount--;
		}
		else if (m_MissileSetFlg01 && m_MissileAmount > 0)
		{

			m_Missile01->SetVelocity(GetForwardQuaternion() * 2.0f + m_Velocity);
			m_Missile01->SetShot(true);

			missileuis[1]->SetGauge(0);

			InputX::SetVibration(0, 10);
			m_MissileVibCount = 0;

			m_MissileSetFlg01 = false;
			m_MissilePosChange = 0;
			m_MissileAmount--;
		}
	}
	else
	{
		m_MissileVibCount++;
		if (m_MissileVibCount > 60)
		{
			InputX::StopVibration(0);
			m_MissileVibCount = 0;
		}
	}
}

void Player::UpdateLong()
{
	auto missileuis = m_Scene->GetGameObjects<LongMissileUI>();

	{
		//ミサイル装着
		if (m_SpecialMissileSetFlg00 && m_SpecialMissileAmount > 0)
		{
			m_LongMissile00->SetPosition(m_Position + GetForwardQuaternion() * -0.8f + GetRightQuaternion() * 4.0f + GetTopQuaternion() * -0.8f);
			m_LongMissile00->SetQuaternion(m_Quaternion);
			m_LongMissile00->SetVelocity(m_Velocity);
		}
		if (m_SpecialMissileSetFlg01 && m_SpecialMissileAmount > 0)
		{
			m_LongMissile01->SetPosition(m_Position + GetForwardQuaternion() * -0.8f + GetRightQuaternion() * -4.0f + GetTopQuaternion() * -0.8f);
			m_LongMissile01->SetQuaternion(m_Quaternion);
			m_LongMissile01->SetVelocity(m_Velocity);
		}

		//ミサイルチャージ完了時
		if (missileuis[0]->GetGauge() == 100)
		{
			m_LongMissile00 = m_Scene->AddGameObject<LongMissile>(1);

			m_SpecialMissileSetFlg00 = true;
		}
		if (missileuis[1]->GetGauge() == 100)
		{
			m_LongMissile01 = m_Scene->AddGameObject<LongMissile>(1);

			m_SpecialMissileSetFlg01 = true;
		}
	}
}

void Player::UpdateShort()
{

}

void Player::UpdateRail()
{

}

void Player::ShotLong()
{
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();
	auto missileuis = m_Scene->GetGameObjects<LongMissileUI>();

	// ミサイル発射
	if ((Input::GetKeyTrigger(VK_LBUTTON) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_B)))
	{
		//ミサイルを撃った対象のID取得
		if (!enemys.empty())
		{
			m_MissileLockID = m_EnemyID;
		}

		//発射
		if (m_SpecialMissilePosChange == 0 && m_SpecialMissileSetFlg00 && m_SpecialMissileAmount > 0)
		{

			m_LongMissile00->SetVelocity(m_Velocity - GetForwardQuaternion() * 0.02f);
			m_LongMissile00->SetShot(true);

			missileuis[0]->SetGauge(0);

			InputX::SetVibration(0, 20);
			m_MissileVibCount = 0;

			m_SpecialMissileSetFlg00 = false;
			m_SpecialMissilePosChange++;
			m_SpecialMissileAmount--;
		}
		else if (m_SpecialMissileSetFlg01 && m_SpecialMissileAmount > 0)
		{

			m_LongMissile01->SetVelocity(m_Velocity - GetForwardQuaternion() * 0.02f);
			m_LongMissile01->SetShot(true);

			missileuis[1]->SetGauge(0);

			InputX::SetVibration(0, 10);
			m_MissileVibCount = 0;

			m_SpecialMissileSetFlg01 = false;
			m_SpecialMissilePosChange = 0;
			m_SpecialMissileAmount--;
		}
	}
	else
	{
		m_MissileVibCount++;
		if (m_MissileVibCount > 60)
		{
			InputX::StopVibration(0);
			m_MissileVibCount = 0;
		}
	}
}

void Player::ShotShort()
{
}

void Player::ShotRail()
{
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
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();
	auto camera = m_Scene->GetGameObject<Camera>();

	D3DXMATRIX screenMatrix = camera->GetScreenMatrix();//マトリックス取得
	D3DXVECTOR3 screenCameraEnemy;//スクリーン上の敵の位置
	float oldDistance_1 = 1000.0f;
	float oldDistance_2 = 1000.0f;
	int keepEnemyID_1 = 0;
	int keepEnemyID_2 = 0;

	for (EnemyJet* enemy : enemys)
	{
		if(!enemy->GetCrash())
		{
			D3DXVECTOR3 enemypos = enemy->GetPosition();
			D3DXVec3TransformCoord(&screenCameraEnemy, &enemypos, &screenMatrix);//スクリーン上の敵位置を取得

			//2次元に変換
			D3DXVECTOR2 screenEnemy = D3DXVECTOR2(screenCameraEnemy.x, screenCameraEnemy.y - 0.330f);
			float distance = D3DXVec2LengthSq(&screenEnemy);

			if (distance < oldDistance_1)//一番距離が近い敵IDを取得
			{
				m_EnemyID = enemy->GetEnemyID();
				oldDistance_1 = distance;//距離を保存
			}

			//if (distance < oldDistance_1)
			//{
			//	keepEnemyID_2 = keepEnemyID_1;
			//	oldDistance_2 = oldDistance_1;
			//	m_EnemyID = enemy->GetEnemyID();
			//	oldDistance_1 = distance;
			//}
			//else if (distance < oldDistance_2)
			//{
			//	keepEnemyID_2 = enemy->GetEnemyID();
			//	oldDistance_2 = distance;
			//}
		}
		
		//ロックオン中の敵が破壊された場合
		if(enemy->GetEnemyID() == m_EnemyID && enemy->GetCrash())
		{
			m_Lockon2D->LockMoveReset();
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
}
