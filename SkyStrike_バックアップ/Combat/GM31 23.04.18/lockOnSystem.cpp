#include "main.h"
#include "lockOnSystem.h"

#include "scene.h"
#include "manager.h"
#include "enemyJet.h"
#include "camera.h"
#include "lock.h"
#include "lockOn.h"
#include "lockon2D.h"
#include "audio.h"

void LockOnSystem::Init()
{
	m_Scene = Manager::GetScene();

	m_LockOnSE = AddComponet<Audio>();
	m_LockOnSE->Load("asset\\audio\\lockon.wav");
	
	m_LockFinishSE = AddComponet<Audio>();
	m_LockFinishSE->Load("asset\\audio\\lockonset.wav");
}




void LockOnSystem::Update(GameObject* obj)
{
	m_EnemyJets = m_Scene->GetGameObjects<EnemyJet>();
	m_Lockon2D = m_Scene->GetGameObject<Lockon2D>();
	m_LockOn = m_Scene->GetGameObject<LockOn>();


	m_LockOnSE->Volume(Scene::m_SEVolume * (0.03f * 2));
	m_LockFinishSE->Volume(Scene::m_SEVolume * (0.03f * 2));



	for (EnemyJet* enemy : m_EnemyJets)
	{
		//UI処理
		auto length = enemy->GetPosition() - obj->GetPosition();
		float vecs = D3DXVec3Length(&length);

		vecs /= 50.0f;//サイズ調整

		//最小サイズ値
		if (vecs <= 3.0f)
		{
			vecs = 3.0f;
		}

		//UIセット
		enemy->GetAddLock()->SetPosition(enemy->GetPosition());
		enemy->GetAddLock()->SetScale(D3DXVECTOR3(vecs, vecs, 0.0f));




		//ロックオン処理
		if (enemy->GetEnemyID() == m_EnemyID && !m_EnemyJets.empty() && !enemy->GetCrash())
		{
			m_LockPos = enemy->GetPosition();//lockされている敵のpos保存

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
					m_LockFinishFlg = true;
					m_LockOnSE->Stop();

					//ロックオン完了SE
					if (m_LockFinishSeFlg)
					{
						m_LockFinishSE->Play(true);
						m_LockFinishSeFlg = false;
					}
				}
				else //ロックオン中
				{
					enemy->GetAddLock()->BlinkingFlg(true);//lock点滅
					m_LockOn->SetAlpha(0.0f); //lockOnのalpha非表示
					m_LockFinishFlg = false;
				}

			}
			else
			{

				enemy->GetAddLock()->SetLockColor(false); //カラー緑
				enemy->GetAddLock()->BlinkingFlg(true); //点滅変数をセット

				m_LockOn->SetAlpha(0.0f); //lockOnのalpha非表示

				m_LockFinishFlg = false;

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
			if (m_EnemyJets.size() <= 1)
				m_LockOn->SetAlpha(0.0f);
		}
	}





	//enemyが空になった時
	if (m_EnemyJets.empty())
	{
		m_LockOn->SetAlpha(0.0f);
		m_LockFinishSE->Stop();
		m_LockOnSE->Stop();
	}





	//プレイヤーの視界内
	if (obj->View(80.0f, 2000.0f, obj->GetPosition(), m_LockPos, obj->GetForwardQuaternion()))
	{
		for (EnemyJet* enemy : m_EnemyJets)
		{
			//lock中の敵をロックオン
			if (enemy->GetEnemyID() == m_EnemyID)
			{
				m_LockOnFlg = true;
			}
		}

		if (m_LockSeFlg)
		{
			m_LockOnSE->Play(true);
			m_LockSeFlg = false;
		}

		m_Lockon2D->LockonMove();
	}
	else
	{
		for (EnemyJet* enemy : m_EnemyJets)
		{
			//lock中の敵をロックオフ
			if (enemy->GetEnemyID() == m_EnemyID)
			{
				m_LockOnFlg = false;
				m_LockOnSE->Stop();
				m_LockFinishSE->Stop();

				m_LockFinishSeFlg = true;
			}
		}
		m_LockSeFlg = true;
		m_Lockon2D->LockMoveReset();
		m_Lockon2D->SetDraw(false);
	}

}





//ターゲット切り替え関数
void LockOnSystem::SwitchTarget()
{
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();
	auto camera = m_Scene->GetGameObject<Camera>();

	D3DXMATRIX screenMatrix = camera->GetScreenMatrix();
	D3DXVECTOR3 screenCameraEnemy;
	float oldDistance_1 = 1000.0f;
	float oldDistance_2 = 1000.0f;
	int keepEnemyID_1 = 0;
	int keepEnemyID_2 = 0;

	for (EnemyJet* enemy : enemys)
	{
		if (!enemy->GetCrash())
		{
			D3DXVECTOR3 enemypos = enemy->GetPosition();
			D3DXVec3TransformCoord(&screenCameraEnemy, &enemypos, &screenMatrix);//スクリーン上の敵位置を取得

			//2次元に変換
			D3DXVECTOR2 screenEnemy = D3DXVECTOR2(screenCameraEnemy.x, screenCameraEnemy.y - 0.330f);
			float distance = D3DXVec2LengthSq(&screenEnemy);

			//一番距離が近い敵IDを取得
			if (distance < oldDistance_1)
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
		if (enemy->GetEnemyID() == m_EnemyID && enemy->GetCrash())
		{
			m_Lockon2D->LockMoveReset();
		}
	}
}

void LockOnSystem::SwitchReset()
{
	m_Lockon2D->LockMoveReset();
	m_LockFinishSE->Stop();
	m_LockSeFlg = true;
	m_LockFinishSeFlg = true;
}
