#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "scene.h"
#include "input.h"
#include "inputx.h"
#include "enemyJet.h"
#include "missile.h"
#include "inputx.h"

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
	m_Player = m_Scene->GetGameObject<Player>();
	auto enemys = m_Scene->GetGameObjects<EnemyJet>();
	auto enemy = m_Scene->GetGameObject<EnemyJet>();
	auto missiles = m_Scene->GetGameObjects<Missile>();

	if (m_GameEnable)
	{
		//���x����p�ύX
		if (Input::GetKeyPress('W') || (InputX::GetRightTrigger(0) > 0) && m_Player->GetSpeed() * 1000.0f >= 2000.0f)
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


		if (Input::GetKeyPress('F'))//�t���[���b�N
		{
			// �J�����̌�����Rot�ɑ���
			m_Rotation.y += GetMouseX() / 300;
			m_Rotation.x += GetMouseY() / 300;

			//�}�E�X�z�C�[������
			m_Hweel = GetMouseHwheel() / 100;
			if (m_Hweel < 15.0f)
			{
				m_Hweel = 15.0f;
			}
			else if (m_Hweel > 100.0f)
			{
				m_Hweel = 100.0f;
			}

			// �J�����̌����Ɋ�Â��ăr���[�}�g���b�N�X���X�V
			m_Target = m_Player->GetPosition() + m_Player->GetForwardQuaternion() * 2.0f;
			m_Position = m_Target - GetForward() * m_Hweel + m_Player->GetTopQuaternion() * 4.0f;

			m_UpMode = true;
		}
		else if (Input::GetKeyPress(VK_SPACE) || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_Y) && !enemys.empty())//�^�[�Q�b�g�J����
		{
			m_Count++;
			if(m_Count > 20)
			{
				//���ׂĂ̓G��for�ŉ�
				for (EnemyJet* enemy : enemys)
				{
					//Lock����Ă���G�����
					if (enemy->GetEnemyID() == m_Player->GetLockEnemy())
					{
						// �J�����̈ʒu����G�̈ʒu�������x�N�g�����v�Z
						D3DXVECTOR3 length = m_Target - m_Position;
						D3DXVec3Normalize(&length, &length);//���K��

						// �J�����̌�����ݒ�
						m_Rotation.x = asinf(length.y) * -1.0f;
						m_Rotation.y = atan2f(length.x, length.z);

						//�����_�͓G
						D3DXVECTOR3 goal = enemy->GetPosition();
						enemypos = enemy->GetPosition();
						D3DXVec3Lerp(&m_Target, &m_Target, &goal, 0.1f);

						//�|�W�V�����̓v���C���[
						m_Position = m_Player->GetPosition() - GetForward() * 20.0f + D3DXVECTOR3(0.0f, 2.0f, 0.0f);

						m_UpMode = false;
					}
				}
			}
			else
			{
				DefaultCamera();
			}
		}
		else if (Input::GetKeyPress(VK_LBUTTON) || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_B))//�~�T�C���J����
		{
			m_Count++;
			if (m_Count > 20 && !missiles.empty())
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
				DefaultCamera();
			}
		}
		//else if (InputX::GetThumbRightX(0) != 0 && InputX::GetThumbRightY(0) != 0)
		//{

		//	m_Count = 0;
		//}
		else //�ʏ�
		{
			DefaultCamera();
			m_Count = 0;
		}


		m_ShakeOffsetX = sinf(m_ShakeTime * m_ShakeSpeed + 6.0f) * m_ShakeAmplitude;
		m_ShakeOffsetY = sinf(m_ShakeTime * m_ShakeSpeed) * m_ShakeAmplitude;
		m_ShakeTime++;
		m_ShakeAmplitude *= 0.9f;

		//�U�������܂�����p�b�h�̐U���X�g�b�v
		if (m_ShakeAmplitude < 0.01f && m_StopVibFlg)
		{
			InputX::StopVibration(0);
			m_StopVibFlg = false;
		}
	}
	else
	{
		//�e�X�g
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

	//ImGui�\��
	//ImGui::Begin("CAMERA");
	//ImGui::InputFloat3("Target", m_Target);
	//ImGui::InputFloat3("enemypos", enemypos);
	//ImGui::End();

	// �t�@�[�X�g�p�[�\���r���[(valorant)
	//m_Position = player->GetPosition() + D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	//m_Traget = m_Position + player->GetForward();
}

void Camera::Draw()
{
	D3DXVECTOR3 position = m_Position + m_Player->GetRightQuaternion() * m_ShakeOffsetX + m_Player->GetTopQuaternion() * m_ShakeOffsetY;
	D3DXVECTOR3 target = m_Target + m_Player->GetRightQuaternion() * m_ShakeOffsetX + m_Player->GetTopQuaternion() * m_ShakeOffsetY;

	//�J�����̏�����x�N�g��
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

	//�r���[�s��쐬
	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &m_UpCamera);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	// �v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMATRIX projectionMatorix;
	//�������F�}�g���b�N�X,����p,�A�X�y�N�g��,�j�A�N���b�v,�t�@�[�N���b�v(�`�拗��)
	D3DXMatrixPerspectiveFovLH(&projectionMatorix, m_Fov,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 20000.0f);
	Renderer::SetProjectionMatrix(&projectionMatorix);

	m_ScreenMatrix = m_ViewMatrix * projectionMatorix;

	//fog�ݒ�
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

void Camera::DefaultCamera()
{
	m_Player = m_Scene->GetGameObject<Player>();

	// �T�[�h�p�[�\���r���[(�ʏ�)
	//m_Target = m_Player->GetPosition();
	//m_Position = m_Target - m_Player->GetForwardQuaternion() * 15.0f + m_Player->GetTopQuaternion() * 4.0f;


	//�t���[�J�����e�X�g
	float forwardplus;
	float rightplus;
	float topplus;

	//�E�X�e�B�b�N�̒l���Βl�ɂ���
	forwardplus = abs((InputX::GetThumbRightX(0) + InputX::GetThumbRightY(0)) * 36.0f) - 18.0f;

	//0.0���ŏ��l0.5���ő�l1.0���ŏ��l�ɂȂ�
	float rightdiff = -4 * pow(abs(InputX::GetThumbRightX(0)) - 0.5f, 2.0f) + 1;
	float topdiff = -4 * pow(abs(InputX::GetThumbRightY(0)) - 0.5f, 2.0f) + 1;

	if (InputX::GetThumbRightX(0) <= 0)
		rightplus = (rightdiff * 20.0f) * -1;
	else
		rightplus = (rightdiff * 20.0f);

	if (InputX::GetThumbRightY(0) <= 0)
		topplus = (topdiff * 15.0f) * -1;
	else
		topplus = (topdiff * 10.0f);

	//�J�����̌����Ɋ�Â��ăr���[�}�g���b�N�X���X�V
	m_Position = m_Player->GetPosition()
		- m_Player->GetForwardQuaternion() * -forwardplus
		+ m_Player->GetRightQuaternion() * rightplus
		+ m_Player->GetTopQuaternion() * (topplus + 4.0f);

	m_Target = m_Player->GetPosition() + m_Player->GetForwardQuaternion() * 2.0f;


	m_UpMode = false;

	//ImGui::Begin("camera");
	//ImGui::InputFloat("forward", &forwardplus);
	//ImGui::InputFloat("right", &rightplus);
	//ImGui::InputFloat("difference", &rightdiff);
	//ImGui::End();
}

//�������J�����U��  ������:position,������:�傫����������,��O����:��������������
void Camera::SetBomShake(D3DXVECTOR3 pos,float shortDistance,float maxDistance)
{
	//�����v�Z
	D3DXVECTOR3 direction = m_Player->GetPosition() - pos;
	float length = D3DXVec3Length(&direction);

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
