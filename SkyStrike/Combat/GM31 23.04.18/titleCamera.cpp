#include"main.h"
#include"manager.h"
#include"renderer.h"
#include"titleCamera.h"
#include"player.h"
#include"scene.h"
#include "input.h"
#include "jet.h"
#include "inputx.h"

void TitleCamera::Init()
{
	m_Position = D3DXVECTOR3(-50.0f, 1000.0f, -100.0f);
	m_Target = D3DXVECTOR3(0.0f, 1400.0f, 500.0f);

	m_FogStart = 1500.0f;
	m_FogEnd = 4000.0f;
	m_FogHeight = 500.0f;
}

void TitleCamera::Update()
{
	Scene* scene = Manager::GetScene();

	m_ShakeCount++;
	if (m_ShakeCount == 45)
	{
		InputX::SetVibration(0,10);
		m_ShakeAmplitude = 50.0f;
		m_ShakeSpeed = 0.6f;
	}

	if (m_StartFlg)
	{
		D3DXVECTOR3 pgoal = D3DXVECTOR3(-50.0f, 50.0f, -100.0f);
		D3DXVECTOR3 tgoal = D3DXVECTOR3(0.0f, 30.0f, 30.0f);

		D3DXVec3Lerp(&m_Position, &m_Position , &pgoal,0.05f);
		D3DXVec3Lerp(&m_Target, &m_Target, &tgoal, 0.05f);
	}
	else if (m_WeaponFlg)
	{
		D3DXVECTOR3 pgoal = D3DXVECTOR3(-50.0f, 50.0f, 100.0f);
		D3DXVec3Lerp(&m_Position, &m_Position, &pgoal, 0.05f);
	}
	//m_Position = D3DXVECTOR3(-50.0f, 50.0f, -100.0f);
	//m_Target = D3DXVECTOR3(0.0f, 30.0f, 30.0f);

	if (Input::GetKeyPress('B'))
	{
		if (m_Position.z > -100.0f)
		{
			m_Position.z -= 1.0f;
		}
	}

	if (Input::GetKeyPress('B'))
	{
		m_FogHeight += 10.0f;
	}
	if (Input::GetKeyPress('N'))
	{
		m_FogHeight -= 10.0f;
	}
	if (Input::GetKeyPress('C'))
	{
		m_FogStart += 10.0f;
	}
	if (Input::GetKeyPress('V'))
	{
		m_FogStart -= 10.0f;
	}

	//ImGui::SetNextWindowSize(ImVec2(500, 250));
	//ImGui::Begin("Camera");
	//ImGui::InputFloat3("Pos", m_Position);
	//ImGui::InputFloat3("Target", m_Target);
	//ImGui::End();
}

void TitleCamera::Draw()
{
	m_ShakeOffsetX = sinf(m_ShakeTime * m_ShakeSpeed * 6.0f) * m_ShakeAmplitude;
	m_ShakeOffsetY = sinf(m_ShakeTime * m_ShakeSpeed) * m_ShakeAmplitude;
	m_ShakeTime++;
	m_ShakeAmplitude *= 0.9f;

	if (m_ShakeAmplitude < 0.01f)
	{
		InputX::StopVibration(0);
	}

	D3DXVECTOR3 position = m_Position + GetRight() * m_ShakeOffsetX + GetTop() * m_ShakeOffsetY;
	D3DXVECTOR3 target = m_Target + GetRight() * m_ShakeOffsetX + GetTop() * m_ShakeOffsetY;

	//ビューマトリクス設定
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクションマトリックス設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.5f/*視野*/, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 20000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);

	//fog設定
	CAMERA camera;
	D3DXVECTOR4 cpos = D3DXVECTOR4(m_Position.x, m_Position.y, m_Position.z, 1.0f);
	camera.Position = cpos;
	camera.FogParam.x = m_FogStart;//FogStart
	camera.FogParam.y = m_FogEnd;//FogEnd
	camera.FogParam.z = m_FogHeight;//FogHeight
	camera.FogParam.w = 0.0f;//temp
	camera.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	camera.GroundFogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetCameraPosition(camera);
}