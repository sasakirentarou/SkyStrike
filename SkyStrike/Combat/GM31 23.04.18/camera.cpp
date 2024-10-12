#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "jet.h"
#include "scene.h"
#include "input.h"
#include "inputx.h"
#include "enemyJet.h"
#include "missile.h"
#include "inputx.h"
#include "weaponSystem.h"
#include "longMissile.h"

void Camera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_FogStart = 3000.0f;
	m_FogEnd = 20000.0f;
	m_FogHeight = 2000.0f;

	m_Hweel = 16.0f;

	m_Scene = Manager::GetScene();
}

void Camera::Update()
{
	m_Jet = m_Scene->GetGameObject<Jet>();
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();
	auto enemy = m_Scene->GetGameObject<EnemyJet>();
	auto missiles = m_Scene->GetGameObjects<Missile>();
	auto lmissiles = m_Scene->GetGameObjects<LongMissile>();

	if (m_GameEnable)
	{
		//速度視野角変更
		if (Input::GetKeyPress('W') || (InputX::GetRightTrigger(0) > 0) && m_Jet->GetSpeed() >= 2000.0f)
		{
			if (m_Fov <= MAX_FOV)
			{
				m_Fov += FOV_SPEED;
			}
		}
		else
		{
			if (m_Fov >= MIN_FOV)
			{
				m_Fov -= FOV_SPEED;
			}
		}


		if (Input::GetKeyPress('F')) //マウスフリールック
		{
			m_Rotation.y += GetMouseX() / 300;
			m_Rotation.x += GetMouseY() / 300;

			//マウスホイール操作
			m_Hweel = GetMouseHwheel() / 100;

			if (m_Hweel < 15.0f)
			{
				m_Hweel = 15.0f;
			}
			else if (m_Hweel > 100.0f)
			{
				m_Hweel = 100.0f;
			}

			// カメラの向きに基づいてビューマトリックスを更新
			m_Target = m_Jet->GetPosition() + m_Jet->GetForwardQuaternion() * 2.0f;
			m_Position = m_Target - GetForward() * m_Hweel + m_Jet->GetTopQuaternion() * 4.0f;

			m_UpMode = true;
		}
		else if (Input::GetKeyPress(VK_SPACE) || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_Y) && !enemys.empty())//ターゲットカメラ
		{
			m_Count++;
			if(m_Count > HOLD_BUTTON)
			{
				//すべての敵をforで回す
				for (EnemyJet* enemy : enemys)
				{
					//Lockされている敵を特定
					if (enemy->GetEnemyID() == m_Jet->GetLockOnSm()->GetLockEnemyID())
					{
						// カメラの位置から敵の位置を向くベクトルを計算
						D3DXVECTOR3 length = m_Target - m_Position;
						D3DXVec3Normalize(&length, &length);//正規化

						// カメラの向きを設定
						m_Rotation.x = asinf(length.y) * -1.0f;
						m_Rotation.y = atan2f(length.x, length.z);

						//注視点は敵
						D3DXVECTOR3 goal = enemy->GetPosition();
						enemypos = enemy->GetPosition();
						D3DXVec3Lerp(&m_Target, &m_Target, &goal, 0.1f);

						//ポジションはプレイヤー
						m_Position = m_Jet->GetPosition() - GetForward() * 20.0f + D3DXVECTOR3(0.0f, 2.0f, 0.0f);

						m_UpMode = false;
					}
				}
			}
			else
			{
				DefaultCamera();
			}
		}
		else if (Input::GetKeyPress(VK_LBUTTON) || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_B))//ミサイルカメラ
		{
			m_Count++;
			if (m_Count > 20 && !m_Jet->GetWeaponSm()->GetWeaponChange() && !missiles.empty()) //通常ミサイル
			{
				for (Missile* missile : missiles)
				{
					if (missile->GetShot())
					{
						m_Target = missile->GetPosition();
						m_Position = m_Target - missile->GetForwardQuaternion() * 15.0f + missile->GetTopQuaternion() * 4.0f;
					}
				}
				m_UpMode = true;
			}
			else if (m_Count > 20 && m_Jet->GetWeaponSm()->GetWeaponChange() && !lmissiles.empty()) //ロングミサイル
			{
				for (LongMissile* missile : lmissiles)
				{
					if (missile->GetShot())
					{	
						m_Target = missile->GetPosition();
						m_Position = m_Target - missile->GetForwardQuaternion() * 15.0f + missile->GetTopQuaternion() * 4.0f;
					}
				}
				m_UpMode = true;
			}
			else
			{
				DefaultCamera();
			}
		}
		else //通常
		{
			DefaultCamera();
			m_Count = 0;
		}


		m_ShakeOffsetX = sinf(m_ShakeTime * m_ShakeSpeed + 6.0f) * m_ShakeAmplitude;
		m_ShakeOffsetY = sinf(m_ShakeTime * m_ShakeSpeed) * m_ShakeAmplitude;
		m_ShakeTime++;
		m_ShakeAmplitude *= 0.9f;

		//振動が収まったらパッドの振動ストップ
		if (m_ShakeAmplitude < 0.01f && m_StopVibFlg)
		{
			InputX::StopVibration(0);
			m_StopVibFlg = false;
		}
	}
	else
	{
		//テスト
		if (Input::GetKeyPress('B'))
			m_FogHeight += 100.0f;
		if (Input::GetKeyPress('N'))
			m_FogHeight -= 100.0f;
		if (Input::GetKeyPress('C'))
			m_FogStart += 100.0f;
		if (Input::GetKeyPress('V'))
			m_FogStart -= 100.0f;

		if (Input::GetKeyPress('W'))
			m_Position += GetForward() * 1.0f;
		if (Input::GetKeyPress('S'))
			m_Position += GetForward() * -1.0f;
		if (Input::GetKeyPress('A'))
			m_Position += GetRight() * -1.0f;
		if (Input::GetKeyPress('D'))
			m_Position += GetRight() * 1.0f;

		if (Input::GetKeyPress('E'))
			m_Rotation.y += 0.03f;
		if (Input::GetKeyPress('Q'))
			m_Rotation.y -= 0.03f;
		if (Input::GetKeyPress(VK_SHIFT))
			m_Rotation.x -= 0.03f;
		if (Input::GetKeyPress(VK_CONTROL))
			m_Rotation.x += 0.03f;

		m_Fov = 1.5f;
		m_Position += m_Velocity;
		m_Target = m_Position + GetForward() * 10.0f;
	}

	// ファーストパーソンビュー(valorant)
	//m_Position = player->GetPosition() + D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	//m_Traget = m_Position + player->GetForward();
}

void Camera::Draw()
{
	//カメラの上方向ベクトル
	if (m_UpMode)
	{
		D3DXVECTOR3 goal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXVec3Lerp(&m_UpCamera, &m_UpCamera, &goal, 0.1f);
	}
	else
	{
		D3DXVECTOR3 goal = m_Jet->GetTopQuaternion();
		D3DXVec3Lerp(&m_UpCamera, &m_UpCamera, &goal, 0.3f);
	}

	//振動を追加
	D3DXVECTOR3 position = m_Position + m_Jet->GetRightQuaternion() * m_ShakeOffsetX + m_Jet->GetTopQuaternion() * m_ShakeOffsetY;
	D3DXVECTOR3 target = m_Target + m_Jet->GetRightQuaternion() * m_ShakeOffsetX + m_Jet->GetTopQuaternion() * m_ShakeOffsetY;

	//ビュー行列作成
	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &m_UpCamera);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
	//↓引数：マトリックス,視野角,アスペクト比,ニアクリップ,ファークリップ(描画距離)
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, m_Fov,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 20000.0f);
	Renderer::SetProjectionMatrix(&projectionMatrix);

	m_ScreenMatrix = m_ViewMatrix * projectionMatrix;

	//fog設定
	CAMERA camera;
	D3DXVECTOR4 cpos = D3DXVECTOR4(m_Position.x, m_Position.y, m_Position.z, 1.0f);
	camera.Position = cpos;
	camera.FogParam.x = m_FogStart;//FogStart
	camera.FogParam.y = m_FogEnd;//FogEnd
	camera.FogParam.z = m_FogHeight;//FogHeight
	camera.FogParam.w = 0.0f;//temp
	camera.FogColor = m_FogColor; 
	camera.GroundFogColor = m_GroundFogColor;
	camera.SkyColor = m_SkyColor;
	Renderer::SetCameraPosition(camera);
}


void Camera::Debug()
{
	ImGui::Begin("Camera");
	ImGui::InputFloat3("Position", m_Position);
	ImGui::End();

	ImGui::Begin("Fog");
	ImGui::InputFloat("Start", &m_FogStart);
	ImGui::InputFloat("End", &m_FogEnd);
	ImGui::InputFloat("Height", &m_FogHeight);
	ImGui::InputFloat4("FogColor", m_FogColor);
	ImGui::InputFloat4("GroundColor", m_GroundFogColor);
	ImGui::InputFloat4("SkyColor", m_SkyColor);
	ImGui::End();
}




void Camera::DefaultCamera()
{
	m_Jet = m_Scene->GetGameObject<Jet>();



	//フリーカメラテスト(最終目標はsin,cosなどで実装)
	float forwardplus, rightplus, topplus, rightdiff, topdiff;
	{//スティックに応じた方向ベクトルを計算

		//カメラのZベクトル、右スティックの値を絶対値にする
		forwardplus = abs((InputX::GetThumbRightX(0) + (InputX::GetThumbRightY(0) * 0.5f)) * (MAX_FORWARD_DIFF * 2)) - MAX_FORWARD_DIFF;

		//中間値が1.0、最大値,最小値が0.0になる
		rightdiff = -4 * pow(abs(InputX::GetThumbRightX(0)) - 0.5f, 2.0f) + 1;

		if (InputX::GetThumbRightX(0) <= 0)
			rightplus = (rightdiff * -MAX_RIGHT_DIFF);
		else
			rightplus = (rightdiff * MAX_RIGHT_DIFF);

		topplus = InputX::GetThumbRightY(0) * -MAX_LEFT_DIFF;
	}



	if(false)
	{
		//線形補間 ※問題点あり
		D3DXVECTOR3 endCameraPos = m_Jet->GetPosition() + m_Jet->LocalVector(m_Jet->GetQuaternion(), D3DXVECTOR3(rightplus, (topplus + 4.0f), forwardplus));

		if (InputX::GetThumbRightX(0) != 0 || InputX::GetThumbRightY(0) != 0)
			D3DXVec3Lerp(&m_Position, &m_Position, &endCameraPos, 0.5f);
		else
			m_Position = m_Jet->GetPosition() + m_Jet->LocalVector(m_Jet->GetQuaternion(), D3DXVECTOR3(0.0f, 4.0f, -MAX_FORWARD_DIFF));
	}
	else
	{
		//そのまま代入
		m_Position = m_Jet->GetPosition() +
			m_Jet->LocalVector(
				m_Jet->GetQuaternion(),
				D3DXVECTOR3(rightplus, (topplus + 4.0f), forwardplus)
			);
	}


	m_Target = m_Jet->GetPosition() + m_Jet->GetForwardQuaternion() * 2.0f;


	m_UpMode = false;
}



//爆発時カメラ振動  第一引数:position,第二引数:大きい爆発距離,第三引数:小さい爆発距離
void Camera::SetBomShake(D3DXVECTOR3 pos,float shortDistance,float maxDistance)
{
	//距離計算
	D3DXVECTOR3 direction = m_Jet->GetPosition() - pos;
	float length = D3DXVec3Length(&direction);

	//距離に応じて振動
	if (length < shortDistance)
	{
		Shake(0.3f, 1.0f);
		InputX::SetVibration(0, 100);
		m_StopVibFlg = true;
	}
	else if (length < maxDistance && length > shortDistance)
	{
		Shake(0.2f, 1.0f);
		InputX::SetVibration(0, 50);
		m_StopVibFlg = true;
	}
}



void Camera::SetFog(float fogstart, float fogend, float fogheight,
					D3DXCOLOR fog, D3DXCOLOR groundfog, D3DXCOLOR sky)
{
	m_FogStart = fogstart;
	m_FogEnd = fogend;
	m_FogHeight = fogheight;
	m_FogColor = fog;
	m_GroundFogColor = groundfog;
	m_SkyColor = sky;
}
