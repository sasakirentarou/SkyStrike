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
		//���x����p�ύX
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


		if (Input::GetKeyPress('F')) //�}�E�X�t���[���b�N
		{
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
			m_Target = m_Jet->GetPosition() + m_Jet->GetForwardQuaternion() * 2.0f;
			m_Position = m_Target - GetForward() * m_Hweel + m_Jet->GetTopQuaternion() * 4.0f;

			m_UpMode = true;
		}
		else if (Input::GetKeyPress(VK_SPACE) || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_Y) && !enemys.empty())//�^�[�Q�b�g�J����
		{
			m_Count++;
			if(m_Count > HOLD_BUTTON)
			{
				//���ׂĂ̓G��for�ŉ�
				for (EnemyJet* enemy : enemys)
				{
					//Lock����Ă���G�����
					if (enemy->GetEnemyID() == m_Jet->GetLockOnSm()->GetLockEnemyID())
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
		else if (Input::GetKeyPress(VK_LBUTTON) || InputX::IsButtonPressed(0, XINPUT_GAMEPAD_B))//�~�T�C���J����
		{
			m_Count++;
			if (m_Count > 20 && !m_Jet->GetWeaponSm()->GetWeaponChange() && !missiles.empty()) //�ʏ�~�T�C��
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
			else if (m_Count > 20 && m_Jet->GetWeaponSm()->GetWeaponChange() && !lmissiles.empty()) //�����O�~�T�C��
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

	// �t�@�[�X�g�p�[�\���r���[(valorant)
	//m_Position = player->GetPosition() + D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	//m_Traget = m_Position + player->GetForward();
}

void Camera::Draw()
{
	//�J�����̏�����x�N�g��
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

	//�U����ǉ�
	D3DXVECTOR3 position = m_Position + m_Jet->GetRightQuaternion() * m_ShakeOffsetX + m_Jet->GetTopQuaternion() * m_ShakeOffsetY;
	D3DXVECTOR3 target = m_Target + m_Jet->GetRightQuaternion() * m_ShakeOffsetX + m_Jet->GetTopQuaternion() * m_ShakeOffsetY;

	//�r���[�s��쐬
	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &m_UpCamera);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	// �v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMATRIX projectionMatrix;
	//�������F�}�g���b�N�X,����p,�A�X�y�N�g��,�j�A�N���b�v,�t�@�[�N���b�v(�`�拗��)
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, m_Fov,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 20000.0f);
	Renderer::SetProjectionMatrix(&projectionMatrix);

	m_ScreenMatrix = m_ViewMatrix * projectionMatrix;

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



	//�t���[�J�����e�X�g(�ŏI�ڕW��sin,cos�ȂǂŎ���)
	float forwardplus, rightplus, topplus, rightdiff, topdiff;
	{//�X�e�B�b�N�ɉ����������x�N�g�����v�Z

		//�J������Z�x�N�g���A�E�X�e�B�b�N�̒l���Βl�ɂ���
		forwardplus = abs((InputX::GetThumbRightX(0) + (InputX::GetThumbRightY(0) * 0.5f)) * (MAX_FORWARD_DIFF * 2)) - MAX_FORWARD_DIFF;

		//���Ԓl��1.0�A�ő�l,�ŏ��l��0.0�ɂȂ�
		rightdiff = -4 * pow(abs(InputX::GetThumbRightX(0)) - 0.5f, 2.0f) + 1;

		if (InputX::GetThumbRightX(0) <= 0)
			rightplus = (rightdiff * -MAX_RIGHT_DIFF);
		else
			rightplus = (rightdiff * MAX_RIGHT_DIFF);

		topplus = InputX::GetThumbRightY(0) * -MAX_LEFT_DIFF;
	}



	if(false)
	{
		//���`��� �����_����
		D3DXVECTOR3 endCameraPos = m_Jet->GetPosition() + m_Jet->LocalVector(m_Jet->GetQuaternion(), D3DXVECTOR3(rightplus, (topplus + 4.0f), forwardplus));

		if (InputX::GetThumbRightX(0) != 0 || InputX::GetThumbRightY(0) != 0)
			D3DXVec3Lerp(&m_Position, &m_Position, &endCameraPos, 0.5f);
		else
			m_Position = m_Jet->GetPosition() + m_Jet->LocalVector(m_Jet->GetQuaternion(), D3DXVECTOR3(0.0f, 4.0f, -MAX_FORWARD_DIFF));
	}
	else
	{
		//���̂܂ܑ��
		m_Position = m_Jet->GetPosition() +
			m_Jet->LocalVector(
				m_Jet->GetQuaternion(),
				D3DXVECTOR3(rightplus, (topplus + 4.0f), forwardplus)
			);
	}


	m_Target = m_Jet->GetPosition() + m_Jet->GetForwardQuaternion() * 2.0f;


	m_UpMode = false;
}



//�������J�����U��  ������:position,������:�傫����������,��O����:��������������
void Camera::SetBomShake(D3DXVECTOR3 pos,float shortDistance,float maxDistance)
{
	//�����v�Z
	D3DXVECTOR3 direction = m_Jet->GetPosition() - pos;
	float length = D3DXVec3Length(&direction);

	//�����ɉ����ĐU��
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
