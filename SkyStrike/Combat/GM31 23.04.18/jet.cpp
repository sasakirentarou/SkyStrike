#include "main.h"
#include "jet.h"
#include <string>

#include "manager.h"
#include "audio.h"
#include "trail.h"
#include "collisionBox.h"
#include "input.h"
#include "inputx.h"
#include "hpGauge.h"
#include "textureManager.h"
#include "missile.h"
#include "explosion.h"
#include "enemyJet.h"

void Jet::Init()
{
	m_DepthEnable = true;
	m_ReflectEnable = true;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\jetVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\jetPS.cso");


	std::string jetName = "F-35\\F-35B";

	std::string gameFile = "asset\\model\\" + jetName + "-Off.obj";
	std::string titleFile = "asset\\model\\" + jetName + "-On.obj";

	m_Model = new Model();
	if (m_GameEnable)
		m_Model->Load(gameFile.c_str());
	else
		m_Model->Load(titleFile.c_str());


	m_WingFR.Init("asset\\model\\F-35\\leftwing_new.obj");
	m_WingFL.Init("asset\\model\\F-35\\rightwing_new.obj");
	m_WingBR.Init("asset\\model\\F-35\\backrightwing_new.obj");
	m_WingBL.Init("asset\\model\\F-35\\backleftwing_new.obj");
	m_WingTR.Init("asset\\model\\F-35\\rightvertical_new.obj");
	m_WingTL.Init("asset\\model\\F-35\\leftvertical_new.obj");


	m_Scene = Manager::GetScene();


	
	if (m_GameEnable)
	{
		m_Velocity.z = 1.0f;

		m_WeaponSm.Init();


		//頂点取得
		m_Model->GetModelVertex(gameFile.c_str(), m_TopVertex);

		m_Trail[0] = m_Scene->AddGameObject<Trail>(1);
		m_Trail[1] = m_Scene->AddGameObject<Trail>(1);


		m_Collision = m_Scene->AddGameObject<CollisionBox>(1);
		m_Collision->SetScale(D3DXVECTOR3(6.0f, 1.0f, 8.0f));
		m_Collision->SetName("Jet");
	}

	m_LockOnSm.Init();
	m_FlareSm.Init();
	m_StealthSm.Init();


	//SE
	m_StealthSE = AddComponet<Audio>();
	m_StealthSE->Load("asset\\audio\\stealth.wav");

	m_AlertSE = AddComponet<Audio>();
	m_AlertSE->Load("asset\\audio\\alert.wav");



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



void Jet::Uninit()
{
	m_Model->Unload();


	m_WingFR.Uninit();
	m_WingFL.Uninit();
	m_WingBR.Uninit();
	m_WingBL.Uninit();
	m_WingTR.Uninit();
	m_WingTL.Uninit();


	if (m_GameEnable)
	{
		m_DissolveTexture->Release();
		m_Trail[0]->SetDestroy();
		m_Trail[1]->SetDestroy();
		m_Collision->SetDestroy();
	}

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}



void Jet::Update()
{
	//ボリューム
	m_StealthSE->Volume(Scene::m_SEVolume * (0.1f * 2));
	m_AlertSE->Volume(Scene::m_SEVolume * (0.03f * 2));

	if (m_GameEnable)
	{
		auto enemys = m_Scene->GetGameObjects<EnemyJet>();
		auto hp = m_Scene->GetGameObject<HpGauge>();
		auto missiles = m_Scene->GetGameObjects<Missile>();
		auto texture = m_Scene->GetGameObject<TextureManager>();

		m_Speed = D3DXVec3Length(&m_Velocity) * 1000;




		//機体制御
		if(m_JetState.Control)
		{
			//ピッチ
			if (!m_PitchLock)
			{
				if (Input::GetKeyPress(VK_SHIFT))
				{
					m_JetState.Pitch = 1.0f;
				}
				else if (Input::GetKeyPress(VK_CONTROL))
				{
					m_JetState.Pitch = -1.0f;
				}
				else if (InputX::GetThumbLeftY(0) != 0)
				{
					m_JetState.Pitch = InputX::GetThumbLeftY(0);
				}
				else
				{
					m_JetState.Pitch = 0;
				}


				if (!texture->GetPitchFlipCheck())//ピッチ反転
					m_JetState.Pitch = -m_JetState.Pitch;


				m_ControlStick.Pitch(this, m_JetState.Pitch);

				m_WingFR.Movable(m_WingFR.GetRightQuaternion(), m_JetState.Pitch, MAIN_WING_RANGE);
				m_WingFL.Movable(m_WingFL.GetRightQuaternion(), m_JetState.Pitch, MAIN_WING_RANGE);
				m_WingBR.Movable(m_WingBR.GetRightQuaternion(), m_JetState.Pitch, BACK_WING_RANGE);
				m_WingBL.Movable(m_WingBL.GetRightQuaternion(), m_JetState.Pitch, BACK_WING_RANGE);
			}
			//ロール
			if (!m_RoleLock)
			{
				if (Input::GetKeyPress('A'))
				{
					m_JetState.Roll = 1.0f;
				}
				else if (Input::GetKeyPress('D'))
				{
					m_JetState.Roll = -1.0f;
				}
				else if (InputX::GetThumbLeftX(0) != 0)
				{
					m_JetState.Roll = -InputX::GetThumbLeftX(0);
				}
				else
				{
					m_JetState.Roll = 0;
				}


				m_ControlStick.Roll(this, m_JetState.Roll);

				m_WingFR.Movable(m_WingFR.GetRightQuaternion(), -m_JetState.Roll, MAIN_WING_RANGE);
				m_WingFL.Movable(m_WingFL.GetRightQuaternion(),  m_JetState.Roll, MAIN_WING_RANGE);
				m_WingBR.Movable(m_WingBR.GetRightQuaternion(), -m_JetState.Roll, BACK_WING_RANGE);
				m_WingBL.Movable(m_WingBL.GetRightQuaternion(),  m_JetState.Roll, BACK_WING_RANGE);
			}
			//ヨー
			if (!m_YawLock)
			{
				if (Input::GetKeyPress('E') ||
					InputX::IsButtonPressed(0, XINPUT_GAMEPAD_RIGHT_SHOULDER))
				{
					m_JetState.Yaw = 1.0f;
				}
				else if (Input::GetKeyPress('Q') ||
					InputX::IsButtonPressed(0, XINPUT_GAMEPAD_LEFT_SHOULDER))
				{
					m_JetState.Yaw = -1.0f;
				}
				else
				{
					m_JetState.Yaw = 0;
				}


				m_ControlStick.Yaw(this, m_JetState.Yaw);

				m_WingTR.Movable(m_WingTR.GetTopQuaternion(),  m_JetState.Yaw, VERTICAL_WING_RANGE);
				m_WingTL.Movable(m_WingTL.GetTopQuaternion(),  m_JetState.Yaw, VERTICAL_WING_RANGE);
			}

			//エンジン
			if (!m_EngineLock)
			{
				if (InputX::GetRightTrigger(0) > 0)
				{
					m_JetState.Accele = InputX::GetRightTrigger(0) / 255;
				}
				else if (InputX::GetLeftTrigger(0) > 0)
				{
					m_JetState.Brake = InputX::GetLeftTrigger(0) / 255;
				}
				else if (Input::GetKeyPress('W'))
				{
					m_JetState.Accele = 1.0f;
				}
				else if (Input::GetKeyPress('S'))
				{
					m_JetState.Brake = 1.0f;
				}
				else
				{
					m_JetState.Accele = 0.0f;
					m_JetState.Brake = 0.0f;
				}

				m_Engine.Active(this, SPEED_UP, m_JetState.Accele);
				m_Engine.ReverseActive(this, SPEED_DOWN, m_JetState.Brake);
			}
		}





		//ミサイルアラートSE
		for (Missile* missile : missiles)
		{
			if (missile->GetPlayerTrack())
			{
				if (!m_AlertSE->GetPlay())
				{
					m_AlertSE->Play(true);
				}
			}
		}






		//兵装切り替え
		if (Input::GetKeyTrigger('C') ||
			InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_X))
		{
			m_WeaponSm.WeaponChange();
		}




		// マシンガン発射
		if (Input::GetKeyPress(VK_RBUTTON) ||
			InputX::IsButtonPressed(0, XINPUT_GAMEPAD_A))
		{
			m_WeaponSm.MachineGunFire(this, D3DXVECTOR3(0.0f, -1.5f, 0.0f), 5.0f);
		}
		else
		{
			m_WeaponSm.MachineGunStop();
		}



		//ミサイル発射
		if (Input::GetKeyTrigger(VK_LBUTTON) ||
			InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_B))
		{
			m_WeaponSm.MissileFire(this, m_LockOnSm.GetLockEnemyID());
		}



		//ステルスモード
		{
			if (Input::GetKeyTrigger('T') ||
				InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_RIGHT_THUMB))
			{
				m_StealthSE->Play(false);

				//フラグ切り替え
				if (m_StealthSm.GetAmount() >= MAX_STEALTH)
					m_StealthModeFlg = true;
				else
					m_StealthModeFlg = false;
			}

			if (m_StealthModeFlg)
			{
				m_StealthSm.Active();

				if (m_StealthSm.GetAmount() <= 0)
					m_StealthModeFlg = false;
			}
			else
			{
				m_StealthSm.Charge();
			}
		}



		//フレア
		{
			if (Input::GetKeyTrigger('G') ||
				InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_LEFT_THUMB) &&
				m_FlareSm.GetAmount() != 0)
			{
				if (!m_FlareFlg)
				{
					m_FlareFlg = true;
				}
			}

			if (m_FlareFlg)
			{
				m_FlareSm.Active(this);

				if (m_FlareSm.GetFinish())
					m_FlareFlg = false;
			}
		}




		//ターゲット変更
		if (Input::GetKeyTrigger('R') ||
			InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_Y) &&
			!enemys.empty())
		{
			m_LockOnSm.SwitchTarget();
			m_LockOnSm.SwitchReset();
		}

		m_LockOnSm.Update(this);
		m_WeaponSm.Update(this);


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

				m_Velocity *= 0.0f;
				m_DeathFlg = true;

				m_DeathCount++;
			}
		}

		D3DXVECTOR3 OldPosition = m_Position;

		//トレイル
		if (m_Speed > 1300.0f)
		{
			//トレイル設定
			m_Trail[0]->SetVertexPos(OldPosition + GetRightQuaternion() * m_TopVertex.x + GetForwardQuaternion() * -2.5f,
				OldPosition + GetRightQuaternion() * (m_TopVertex.x * (1.0f - TRAIL_WIDTH)) + GetForwardQuaternion() * -2.5f);

			m_Trail[1]->SetVertexPos(OldPosition - GetRightQuaternion() * m_TopVertex.x + GetForwardQuaternion() * -2.5f,
				OldPosition - GetRightQuaternion() * (m_TopVertex.x * (1.0f - TRAIL_WIDTH)) + GetForwardQuaternion() * -2.5f);
		}
	}

	GameObject::Update();

}



void Jet::Draw()
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



	if (m_GameEnable)
	{
		m_Collision->SetPearent(m_WorldMatrix);
		SetCollider(m_Collision->GetMatrix());
	}


	// ディゾルブテクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(3, 1, &m_DissolveTexture);

	//ディゾルブ設定
	{
		PARAMETER param;
		ZeroMemory(&param, sizeof(param));
		param.dissolveThreshold = m_StealthSm.GetThreshold();
		param.dissolveRange = 0.05f;
		param.dissolveColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		Renderer::SetParameter(param);
	}

	//環境マッピング設定
	{
		ENV_PARAMETER param;
		ZeroMemory(&param, sizeof(param));
		param.brightness = 1.0f;
		param.reflectivity = 0.05f;
		Renderer::SetEnvParameter(param);
	}

	Renderer::SetATCEnable(true);
	m_Model->Draw();
	Renderer::SetATCEnable(false);


	//ウィング (描画後にDraw)
	m_WingFR.Draw(m_WorldMatrix, D3DXVECTOR3(-5.45f, -0.05f, -2.25f));
	m_WingFL.Draw(m_WorldMatrix, D3DXVECTOR3(5.45f, -0.05f, -2.25f));
	m_WingBR.Draw(m_WorldMatrix, D3DXVECTOR3(1.16f, -0.2f, -5.0f));
	m_WingBL.Draw(m_WorldMatrix, D3DXVECTOR3(-1.16f, -0.2f, -5.0f));
	m_WingTR.Draw(m_WorldMatrix, D3DXVECTOR3(1.9f, 1.45f, -4.9f));
	m_WingTL.Draw(m_WorldMatrix, D3DXVECTOR3(-1.9f, 1.45f, -4.9f));

	GameObject::Draw();
}

void Jet::Debug()
{
	ImGui::Begin("Jet");
	ImGui::InputFloat3("Position", m_Position);
	ImGui::InputFloat4("Quaternion", m_Quaternion);
	ImGui::InputFloat3("Velocity", m_Velocity);
	ImGui::InputFloat("Speed", &m_Speed);
	ImGui::End();
}
