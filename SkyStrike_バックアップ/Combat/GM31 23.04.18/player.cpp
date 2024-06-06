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

		//�e�X�g
		m_MissileSetFlg00 = true;
		m_MissileSetFlg01 = true;
		m_Missile00 = m_Scene->AddGameObject<Missile>(1);
		m_Missile01 = m_Scene->AddGameObject<Missile>(1);
		m_Missile00->SetShot(false);
		m_Missile01->SetShot(false);

		m_SpecialMissileSetFlg00 = true;
		m_SpecialMissileSetFlg01 = true;

		//���ꕺ��
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

		//���_�擾
		m_Model->GetModelVertex("asset\\model\\F-35\\F-35B-Off.obj", m_Top);

		//�g���C������
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

	// �e�N�X�`���ǂݍ���
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
	//�{�����[��
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

		//���[��
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

			//���[����]
			D3DXVECTOR3 axis = GetForwardQuaternion();
			D3DXQUATERNION quat;
			D3DXQuaternionRotationAxis(&quat, &axis, angle);
			m_Endquat = m_Endquat * quat;
			D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &m_Endquat, 0.001f);
		}

		//�s�b�`
		if(!m_PitchLock)
		{
			float angle = 0.0f;

			//�������̏d��
			if (m_fVec <= MIN_SPEED)
			{
				m_Velocity += GetForwardQuaternion() * 0.01f;
			}
			else  
			{
				if (texture->GetPitchFlipCheck())//�s�b�`���]
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

		//���[
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

		//�~�T�C���A���[�gSE
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

		//�����؂�ւ�
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

			//���ꕺ���X�e�[�g�}�V��
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


		//Update����
		{
			//���ꕺ���X�e�[�g�}�V��
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
			//�L�[�{�[�h
			if (Input::GetKeyPress('W'))//����
			{
				m_Velocity += GetForwardQuaternion() * SPEED_UP;
			}
			else if (Input::GetKeyPress('S')) //����
			{
				m_Velocity -= GetForwardQuaternion() * SPEED_DOWN;
			}
			//�R���g���[���[
			if (InputX::GetRightTrigger(0) > 0)
			{
				//���K��
				float trigger = InputX::GetRightTrigger(0) / 255;

				m_Velocity += (GetForwardQuaternion() * SPEED_UP) * trigger;
			}
			else if (InputX::GetLeftTrigger(0) > 0) //����
			{
				//���K��
				float trigger = InputX::GetLeftTrigger(0) / 255;

				m_Velocity -= (GetForwardQuaternion() * SPEED_DOWN) * trigger;
			}
		}


		if (Input::GetKeyTrigger(VK_RBUTTON) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_A))
		{
			m_MinigunSE->Play(true);
		}

		// �}�V���K������
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

		//�X�e���X���[�h
		if (Input::GetKeyTrigger('T') || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_RIGHT_THUMB))
		{
			m_StealthSE->Play(false);
			//�t���O�؂�ւ�
			if(m_StealthAmount >= STEALTH_MAX)
				m_StealthModeFlg = true;
			else
				m_StealthModeFlg = false;
		}

		//�X�e���X����(�f�B�]���u)
		if (m_StealthModeFlg)
		{
			m_Threshold += 0.02f;

			//���S�ɏ������Ƃ�
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
			//���S�ɏo���Ƃ�
			if (m_Threshold < 0.0f)
			{
				m_Threshold = 0.0f;
			}

			if (m_StealthAmount < STEALTH_MAX)
			{
				m_StealthAmount += STEALTH_UP_SPEED;
			}
		}

		//�t���A����
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


		//�^�[�Q�b�g�ύX
		if (Input::GetKeyTrigger('R') || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_Y) && !enemys.empty())
		{
			SwitchTarget();

			m_Lockon2D->LockMoveReset();
			m_LockSetSE->Stop();
			m_LockSeFlg = true;
		}

		//���b�N�I������
		for (EnemyJet* enemy : enemys)
		{
			//UI����
			auto length = enemy->GetPosition() - m_Position;
			float vecs = D3DXVec3Length(&length);

			//distance
			//distance->PrintCount(D3DXVECTOR2(0.0f, 0.0f),vecs);

			vecs /= 50.0f;//�T�C�Y����

			//�ŏ��T�C�Y�l
			if (vecs <= 3.0f)
			{
				vecs = 3.0f;
			}

			//UI�Z�b�g
			enemy->GetAddLock()->SetPosition(enemy->GetPosition());
			enemy->GetAddLock()->SetScale(D3DXVECTOR3(vecs, vecs, 0.0f));
			
			//distance
			//distance->SetPosition(enemy->GetPosition());
			//distance->SetScale(D3DXVECTOR3(vecs, vecs, 0.0f));


			//���b�N�I������
			if (enemy->GetEnemyID() == m_EnemyID && !enemys.empty() && !enemy->GetCrash())
			{
				m_EnemyPos = enemy->GetPosition();//lock����Ă���G��pos�ۑ�

				//lockon
				if (m_LockOnFlg == true)
				{
					enemy->GetAddLock()->SetLockColor(true);//�J���[��

					//���b�N�I��������
					if (m_Lockon2D->GetLockFinish())
					{
						m_LockOn->SetPosition(enemy->GetPosition());
						m_LockOn->SetAlpha(1.0f);
						m_LockOn->SetScale(D3DXVECTOR3(vecs, vecs, 0.0f));

						enemy->GetAddLock()->BlinkingFlg(false);//lock
						m_LockSetFlg = true;
						m_LockOnSE->Stop();

						//���b�N�I���Z�b�gSE
						if (m_LockSetSeFlg)
						{
							m_LockSetSE->Play(true);
							m_LockSetSeFlg = false;
						}
					}
					else
					{
						enemy->GetAddLock()->BlinkingFlg(true);//lock�_��
						m_LockOn->SetAlpha(0.0f); //lockOn��alpha��\��
						m_LockSetFlg = false;
					}
				}
				else
				{
					enemy->GetAddLock()->SetLockColor(false); //�J���[��
					enemy->GetAddLock()->BlinkingFlg(true); //�_�ŕϐ����Z�b�g

					m_LockOn->SetAlpha(0.0f); //lockOn��alpha��\��

					m_LockSetFlg = false;
				}
			}
			else
			{
				enemy->GetAddLock()->SetLockColor(false);//�J���[��
				enemy->GetAddLock()->BlinkingFlg(false);//lock�\��
			}

			//enemy�j��
			if (enemy->GetCrash())
			{
				if(enemys.size() <= 1)
					m_LockOn->SetAlpha(0.0f);

				m_OldEnemyID = m_EnemyID;
			}
		}

		//enemy����ɂȂ�����(�S�G���j��)
		if (enemys.empty())
		{
			m_LockOn->SetAlpha(0.0f);//lockOn���c��Ȃ��悤�ɔ�\��
			m_LockSetSE->Stop();
			m_LockOnSE->Stop();
		}


		//�v���C���[�̎��E���ɓ������烍�b�N�I��
		if (PlayerView(80.0f, 2000.0f))
		{
			for (EnemyJet* enemy : enemys)
			{
				//lock���̓G�����b�N�I��
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
				//lock���̓G�����b�N�I�t
				if (enemy->GetEnemyID() == m_EnemyID)
				{
					m_LockOnFlg = false;
					m_LockOnSE->Stop();
					m_LockSetSE->Stop();

					m_LockSetSeFlg = true;//SE�Đ��p
				}
			}
			m_LockSeFlg = true;
			m_Lockon2D->LockMoveReset();
			m_Lockon2D->SetDraw(false);
		}

		//��C��R
		m_Velocity += m_Velocity * -0.01f + GetForwardQuaternion() * 0.012f;

		//�ړ�����
		m_Position += m_Velocity;

		//��Q���Ƃ̏Փ˔���
		float groundHeight = 0.0f;

		//���S��
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

		//�N���X�w�A
		//m_Cross->SetPosition(m_Position + GetForwardQuaternion() * 60.0f + GetTopQuaternion() * 2.0f);

		D3DXVECTOR3 OldPosition = m_Position;

		//�g���C��
		if(speed > 1300.0f)
		{
			//�g���C���ݒ�
			m_Trail01->SetVertexPos(OldPosition + GetRightQuaternion() * m_Top.x + GetForwardQuaternion() * -2.5f,
									OldPosition + GetRightQuaternion() * (m_Top.x * 0.98f) + GetForwardQuaternion() * -2.5f);

			m_Trail02->SetVertexPos(OldPosition - GetRightQuaternion() * m_Top.x + GetForwardQuaternion() * -2.5f,
									OldPosition - GetRightQuaternion() * (m_Top.x * 0.98f) + GetForwardQuaternion() * -2.5f);
		}

		//ImGui�\��
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
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �L���[�u�}�b�v�p�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, Renderer::GetCubeReflectShaderResourceView());

	// �}�g���N�X�ݒ�
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


	// �f�B�]���u�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(3, 1, &m_DissolveTexture);

	//�f�B�]���u�X�V�ݒ�
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

	//�ʏ�~�T�C��
	{
		//�~�T�C��������
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

		//�~�T�C���N�[���^�C��
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

	// �~�T�C������
	if ((Input::GetKeyTrigger(VK_LBUTTON) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_B)))
	{
		//�~�T�C�����������Ώۂ�ID�擾
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
		//�~�T�C������
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

		//�~�T�C���`���[�W������
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

	// �~�T�C������
	if ((Input::GetKeyTrigger(VK_LBUTTON) || InputX::IsButtonTriggered(0, XINPUT_GAMEPAD_B)))
	{
		//�~�T�C�����������Ώۂ�ID�擾
		if (!enemys.empty())
		{
			m_MissileLockID = m_EnemyID;
		}

		//����
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

//�v���C���[�̎��E
bool Player::PlayerView(float fieldOfViewRadians, float viewDistancee)
{
	// ����͈͓����ǂ����̔���
	D3DXVECTOR3 normalizedDirection;
	D3DXVECTOR3 direction = m_EnemyPos - m_Position;
	D3DXVec3Normalize(&normalizedDirection, &direction);
	D3DXVECTOR3 houkou = GetForwardQuaternion();
	float dotProduct = D3DXVec3Dot(&houkou, &normalizedDirection);
	float angle = acos(dotProduct);
	fieldOfViewRadians = D3DXToRadian(fieldOfViewRadians);
	bool isInFieldOfView = angle <= fieldOfViewRadians / 2.0f;

	// ���싗�������ǂ����̔���
	D3DXVECTOR3 dice = m_Position - m_EnemyPos;
	float distance = D3DXVec3Length(&dice);
	bool isInViewDistance = distance <= viewDistancee;

	return isInFieldOfView && isInViewDistance;
}

//�^�[�Q�b�g�؂�ւ��֐�
void Player::SwitchTarget()
{
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();
	auto camera = m_Scene->GetGameObject<Camera>();

	D3DXMATRIX screenMatrix = camera->GetScreenMatrix();//�}�g���b�N�X�擾
	D3DXVECTOR3 screenCameraEnemy;//�X�N���[����̓G�̈ʒu
	float oldDistance_1 = 1000.0f;
	float oldDistance_2 = 1000.0f;
	int keepEnemyID_1 = 0;
	int keepEnemyID_2 = 0;

	for (EnemyJet* enemy : enemys)
	{
		if(!enemy->GetCrash())
		{
			D3DXVECTOR3 enemypos = enemy->GetPosition();
			D3DXVec3TransformCoord(&screenCameraEnemy, &enemypos, &screenMatrix);//�X�N���[����̓G�ʒu���擾

			//2�����ɕϊ�
			D3DXVECTOR2 screenEnemy = D3DXVECTOR2(screenCameraEnemy.x, screenCameraEnemy.y - 0.330f);
			float distance = D3DXVec2LengthSq(&screenEnemy);

			if (distance < oldDistance_1)//��ԋ������߂��GID���擾
			{
				m_EnemyID = enemy->GetEnemyID();
				oldDistance_1 = distance;//������ۑ�
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
		
		//���b�N�I�����̓G���j�󂳂ꂽ�ꍇ
		if(enemy->GetEnemyID() == m_EnemyID && enemy->GetCrash())
		{
			m_Lockon2D->LockMoveReset();
		}
	}
}

//�t���A���ˊ֐�
void Player::ShotFlare()
{
	for (int i = 0; i < 4; i++)
	{
		Flare* flare = m_Scene->AddGameObject<Flare>(1);

		//-10����10�͈̔�
		float rightRandom = static_cast<float>(rand()) / RAND_MAX * 20.0f - 10.0f;
		//5����15�͈̔�
		float forwardRandom = rand() % 11 + 5;

		flare->SetPosition(m_Position + GetForwardQuaternion() * -2.0f + GetTopQuaternion() * -2.0f);
		flare->SetVelocity(GetForwardQuaternion() * forwardRandom + GetRightQuaternion() * rightRandom);
	}

	auto missiles = m_Scene->GetGameObjects<Missile>();
}
