#include "main.h"
#include "renderer.h"
#include "lockon2D.h"
#include "manager.h"
#include "scene.h"
#include "jet.h"
#include "enemyJet.h"
#include "camera.h"
#include "cross.h"

void Lockon2D::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(0.0f + 100.0f, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, 0.0f + 100.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(0.0f + 100.0f, 0.0f + 100.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(), 
		"asset/texture/lockin.png",
		NULL, NULL, &m_Texture, NULL);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	//m_X = -50.0f;
	//m_Y = -50.0f;
}

void Lockon2D::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Lockon2D::Update()
{
	//Scene* scene = Manager::GetScene();
	//auto player = scene->GetGameObject<Player>();
	//auto enemys = scene->GetGameObjects<Enemy>();
	//auto camera = scene->GetGameObject<Camera>();
	//auto cross = scene->GetGameObject<Cross>();

	////�}�g���b�N�X�擾
	//D3DXMATRIX screenMatrix = camera->GetScreenMatrix();

	////�G��2Dpos���擾
	//D3DXVECTOR3 enemypos;
	//D3DXVECTOR3 screenCameraEnemy;//�X�N���[����̓G�̈ʒu
	//for (Enemy* enemy : enemys)
	//{
	//	if (enemy->GetEnemyID() == player->GetLockEnemy())
	//	{
	//		enemypos = enemy->GetPosition();
	//		D3DXVec3TransformCoord(&screenCameraEnemy, &enemypos, &screenMatrix);
	//	}
	//}

	////�N���X�w�A��2Dpos���擾
	//D3DXVECTOR3 crosspos = cross->GetPosition();
	//D3DXVECTOR3 screenCameraCross;
	//D3DXVec3TransformCoord(&screenCameraCross, &crosspos, &screenMatrix);

	////�������W
	//D3DXVECTOR2 center = D3DXVECTOR2((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2));

	////�X�N���[�����W�C��
	//D3DXVECTOR2 CE = D3DXVECTOR2(((SCREEN_WIDTH / 2) * screenCameraEnemy.x) - (m_Width / 2),
	//	((SCREEN_HEIGHT / 2) * -screenCameraEnemy.y) - (m_Height / 2)) + center;
	//D3DXVECTOR2 CC = D3DXVECTOR2(((SCREEN_WIDTH / 2) * screenCameraCross.x) - (m_Width / 2),
	//	((SCREEN_HEIGHT / 2) * -screenCameraCross.y) - (m_Height / 2)) + center;


	//ImGui::Begin("LOCKON2D");
	//ImGui::InputFloat2("CE", CE);
	//ImGui::End();
}

void Lockon2D::Draw()
{
	//���̓��C�A�E�g
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�e���A���ݒ�
	Renderer::SetWorldViewProjection2D();

	D3DXMATRIX world, scale, rot, trans, offset2;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(m_Pos.x, m_Pos.y + m_Height, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	if(m_DrawFlg)
	{
		//�|���S���`��
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}

void Lockon2D::LockonMove()
{
	Scene* scene = Manager::GetScene();
	auto jet = scene->GetGameObject<Jet>();
	auto enemys = scene->GetGameObjects<EnemyJet>();
	auto camera = scene->GetGameObject<Camera>();
	auto cross = scene->GetGameObject<Cross>();

	//�}�g���b�N�X�擾
	D3DXMATRIX screenMatrix = camera->GetScreenMatrix();

	//�G��2Dpos���擾
	D3DXVECTOR3 enemypos;
	D3DXVECTOR3 screenCameraEnemy;//�X�N���[����̓G�̈ʒu
	for (EnemyJet* enemy : enemys)
	{
		if (enemy->GetEnemyID() == jet->GetLockOnSm()->GetLockEnemyID())
		{
			enemypos = enemy->GetPosition();
			D3DXVec3TransformCoord(&screenCameraEnemy, &enemypos, &screenMatrix);
		}
	}

	//�N���X�w�A��2Dpos���擾
	D3DXVECTOR3 crosspos = cross->GetPosition();
	D3DXVECTOR3 screenCameraCross;
	D3DXVec3TransformCoord(&screenCameraCross, &crosspos, &screenMatrix);

	//�������W
	D3DXVECTOR2 center = D3DXVECTOR2((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2)) ;

	//�X�N���[�����W�C��
	D3DXVECTOR2 CE = D3DXVECTOR2(((SCREEN_WIDTH  / 2) *  screenCameraEnemy.x) - (m_Width  / 2),
								 ((SCREEN_HEIGHT / 2) * -screenCameraEnemy.y) - (m_Height / 2)) + center;
	D3DXVECTOR2 CC = D3DXVECTOR2(((SCREEN_WIDTH  / 2) *  screenCameraCross.x) - (m_Width  / 2),
								 ((SCREEN_HEIGHT / 2) * -screenCameraCross.y) - (m_Height / 2)) + center;

	//�⊮�^�C��
	if(m_Time < 1.0f)
		m_Time += MOVE_SPEED;

	D3DXVec2Lerp(&m_Pos, &CC, &CE, m_Time); //���`���

	//�G���X�N���[���̊O�ɂ������\�� ���o�O:�^�t�ɂȂ�Ɛ��l���߂�
	if ((CE.x < 0 || CE.x > SCREEN_WIDTH) ||
		(CE.y < 0 || CE.y > SCREEN_HEIGHT))
	{
		m_DrawFlg = false;
	}
	else
	{
		if (m_Time > 1.0f)
		{
			m_DrawFlg = false;
			m_LockFinish = true;
		}
		else
		{
			m_DrawFlg = true;
			m_LockFinish = false;
		}
	}
}