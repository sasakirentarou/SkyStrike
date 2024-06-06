#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "scene.h"
#include "input.h"
#include "enemy.h"
#include "missile.h"

void Camera::Init()
{
	m_Scene = Manager::GetScene();

	m_Position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_FogStart = 3000.0f;
	m_FogEnd = 20000.0f;
	m_FogHeight = 2000.0f;

	m_Hweel = 16.0f;

}

void Camera::Update()
{
	auto player = m_Scene->GetGameObject<Player>();
	auto enemys = m_Scene->GetGameObjects<Enemy>();
	auto enemy = m_Scene->GetGameObject<Enemy>();
	auto missiles = m_Scene->GetGameObjects<Missile>();

	if (m_GameEnable)
	{
		//速度視野角変更
		if (Input::GetKeyPress('W') && player->GetSpeed() * 1000.0f >= 2000.0f)
		{
			m_Fov += m_FovSpeed;
			if (m_Fov >= m_MaxFov)
			{
				m_Fov = m_MaxFov;
			}
		}
		else
		{
			if (m_Fov >= m_MinFov)
			{
				m_Fov -= m_FovSpeed;
			}
		}


		if (Input::GetKeyPress('F'))//フリールック
		{
			// カメラの向きをRotに足す
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
			m_Target = player->GetPosition() + player->GetForwardQuaternion() * 2.0f;
			m_Position = m_Target - GetForward() * m_Hweel + player->GetTopQuaternion() * 4.0f;

			m_UpMode = true;
		}
		else if (Input::GetKeyPress(VK_SPACE) && !enemys.empty())//ターゲットカメラ
		{
			//すべての敵をforで回す
			for (Enemy* enemy : enemys)
			{
				//Lockされている敵を特定
				if (enemy->GetEnemyID() == player->GetLockEnemy())
				{
					// カメラの位置から敵の位置を向くベクトルを計算
					D3DXVECTOR3 length = m_Target - m_Position;
					D3DXVec3Normalize(&length, &length);//正規化

					// カメラの向きを設定
					m_Rotation.x = asinf(length.y) * -1.0f;
					m_Rotation.y = atan2f(length.x, length.z);

					//注視点は敵
					//m_Target = enemy->GetPosition();
					D3DXVECTOR3 goal = enemy->GetPosition();
					enemypos = enemy->GetPosition();
					D3DXVec3Lerp(&m_Target, &m_Target, &goal, 0.1f);

					//ポジションはプレイヤー
					m_Position = player->GetPosition() - GetForward() * 20.0f + D3DXVECTOR3(0.0f, 2.0f, 0.0f);

					m_UpMode = false;
				}
			}
		}
		else if (Input::GetKeyPress(VK_LBUTTON))//ミサイルカメラ
		{
			m_Count++;
			if (m_Count > 30 && !missiles.empty())
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
			else
			{
				// サードパーソンビュー(通常)
				m_Target = player->GetPosition();
				m_Position = m_Target - player->GetForwardQuaternion() * 15.0f + player->GetTopQuaternion() * 4.0f;

				m_UpMode = false;
			}
		}
		else
		{
			// サードパーソンビュー(通常)
			m_Target = player->GetPosition();
			m_Position = m_Target - player->GetForwardQuaternion() * 15.0f + player->GetTopQuaternion() * 4.0f;

			m_UpMode = false;
			m_Count = 0;
		}

		m_ShakeOffsetX = sinf(m_ShakeTime * m_ShakeSpeed + 6.0f) * m_ShakeAmplitude;
		m_ShakeOffsetY = sinf(m_ShakeTime * m_ShakeSpeed) * m_ShakeAmplitude;
		m_ShakeTime++;
		m_ShakeAmplitude *= 0.9f;

		if (Input::GetKeyPress('B'))
		{
			m_FogHeight += 100.0f;
		}
		if (Input::GetKeyPress('N'))
		{
			m_FogHeight -= 100.0f;
		}
		if (Input::GetKeyPress('C'))
		{
			m_FogStart += 100.0f;
		}
		if (Input::GetKeyPress('V'))
		{
			m_FogStart -= 100.0f;
		}
	}
	else
	{
		//テスト
		if (Input::GetKeyPress('B'))
		{
			m_FogHeight += 100.0f;
		}
		if (Input::GetKeyPress('N'))
		{
			m_FogHeight -= 100.0f;
		}
		if (Input::GetKeyPress('C'))
		{
			m_FogStart += 100.0f;
		}
		if (Input::GetKeyPress('V'))
		{
			m_FogStart -= 100.0f;
		}

		if (Input::GetKeyPress('W'))
		{
			m_Position += GetForward() * 1.0f;
		}
		if (Input::GetKeyPress('S'))
		{
			m_Position += GetForward() * -1.0f;
		}
		if (Input::GetKeyPress('A'))
		{
			m_Position += GetRight() * -1.0f;
		}
		if (Input::GetKeyPress('D'))
		{
			m_Position += GetRight() * 1.0f;
		}

		if (Input::GetKeyPress('E'))
		{
			m_Rotation.y += 0.03f;
		}
		if (Input::GetKeyPress('Q'))
		{
			m_Rotation.y -= 0.03f;
		}
		if (Input::GetKeyPress(VK_SHIFT))
		{
			m_Rotation.x -= 0.03f;
		}
		if (Input::GetKeyPress(VK_CONTROL))
		{
			m_Rotation.x += 0.03f;
		}
		m_Fov = 1.5f;
		m_Position += m_Velocity;
		m_Target = m_Position + GetForward() * 10.0f;
	}

	//ImGui表示
	//ImGui::Begin("CAMERA");
	//ImGui::InputFloat3("Target", m_Target);
	//ImGui::InputFloat3("enemypos", enemypos);
	//ImGui::End();

	// ファーストパーソンビュー(valorant)
	//m_Position = player->GetPosition() + D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	//m_Traget = m_Position + player->GetForward();
}

void Camera::Draw()
{
	m_Player = m_Scene->GetGameObject<Player>();

	D3DXVECTOR3 position = m_Position + m_Player->GetRightQuaternion() * m_ShakeOffsetX + m_Player->GetTopQuaternion() * m_ShakeOffsetY;
	D3DXVECTOR3 target = m_Target + m_Player->GetRightQuaternion() * m_ShakeOffsetX + m_Player->GetTopQuaternion() * m_ShakeOffsetY;

	//カメラの上方向ベクトル
	if (m_UpMode)
	{
		D3DXVECTOR3 goal = (D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		D3DXVec3Lerp(&m_UpCamera, &m_UpCamera, &goal, 0.1f);
	}
	else
	{
		D3DXVECTOR3 goal = m_Player->GetTopQuaternion();
		D3DXVec3Lerp(&m_UpCamera, &m_UpCamera, &goal, 0.3f);
	}

	//ビュー行列作成
	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &m_UpCamera);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクションマトリクス設定
	D3DXMATRIX projectionMatorix;
	//↓引数：マトリックス,視野角,アスペクト比,ニアクリップ,ファークリップ(描画距離)
	D3DXMatrixPerspectiveFovLH(&projectionMatorix, m_Fov,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 20000.0f);
	Renderer::SetProjectionMatrix(&projectionMatorix);

	m_ScreenMatrix = m_ViewMatrix * projectionMatorix;

	//fog設定
	CAMERA camera;
	D3DXVECTOR4 cpos = D3DXVECTOR4(m_Position.x, m_Position.y, m_Position.z, 1.0f);
	camera.Position = cpos;
	camera.FogParam.x = m_FogStart;//FogStart
	camera.FogParam.y = m_FogEnd;//FogEnd
	camera.FogParam.z = m_FogHeight;//FogHeight
	camera.FogParam.w = 0.0f;//temp
	camera.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); //砂色：0.9f, 0.8f, 0.5f
	camera.GroundFogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	camera.SkyColor = D3DXCOLOR(0.2f, 0.3f, 0.4f, 1.0f);
	Renderer::SetCameraPosition(camera);
}

void Camera::SetBomShake(D3DXVECTOR3 pos)
{
	//距離計算
	D3DXVECTOR3 direction = m_Player->GetPosition() - pos;
	float length = D3DXVec3Length(&direction);

	if (length < 200.0f)
	{
		Shake(0.3f, 1.0f);
	}
	else if (length < 500.0f && length > 200.0f)
	{
		Shake(0.2f, 1.0f);
	}
}
