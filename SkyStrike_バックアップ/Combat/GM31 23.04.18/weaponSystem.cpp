#include "main.h"
#include "weaponSystem.h"

#include "scene.h"
#include "manager.h"
#include "bullet.h"
#include "missile.h"
#include "missileUI.h"
#include "longMissile.h"
#include "longMissileUI.h"
#include "inputx.h"
#include "enemyJet.h"
#include "audio.h"

SpecialWeapon WeaponSystem::m_WeaponType = LONG_MISSILE;

void WeaponSystem::Init()
{
	m_MachineGunSE = AddComponet<Audio>();
	m_MachineGunSE->Load("asset\\audio\\minigun.wav");

	m_Scene = Manager::GetScene();

	m_EnemyJets = m_Scene->GetGameObjects<EnemyJet>();

	m_DefaultSetFlg[0] = true;
	m_DefaultSetFlg[1] = true;

	m_DefaultUIs[0] = m_Scene->AddGameObject<MissileUI>(2);
	m_DefaultUIs[1] = m_Scene->AddGameObject<MissileUI>(2);
	m_DefaultUIs[0]->SetDrawFlg(true);
	m_DefaultUIs[1]->SetDrawFlg(true);

	m_Missile[0] = m_Scene->AddGameObject<Missile>(1);
	m_Missile[1] = m_Scene->AddGameObject<Missile>(1);
	m_Missile[0]->SetShot(false);
	m_Missile[1]->SetShot(false);
	
	m_SpecialSetFlg[0] = true;
	m_SpecialSetFlg[1] = true;

	//���ꕺ�������ݒ�
	switch (m_WeaponType)
	{
	case LONG_MISSILE:
		m_LongUIs[0] = m_Scene->AddGameObject<LongMissileUI>(2);
		m_LongUIs[1] = m_Scene->AddGameObject<LongMissileUI>(2);

		m_LongMissile[0] = m_Scene->AddGameObject<LongMissile>(1);
		m_LongMissile[1] = m_Scene->AddGameObject<LongMissile>(1);

		m_Amount[SPECIAL_NUM] = MAX_LONG;
		break;
	case SHORT_MISSILE:
		break;
	case RAIL_GUN:
		break;
	default:
		break;
	}

	m_Amount[DEFAULT_NUM] = MAX_MISSILE;
}

void WeaponSystem::Update(GameObject* obj)
{
	m_MachineGunSE->Volume(Scene::m_SEVolume * (0.05f * 2));


	//���ꕺ���X�e�[�g�}�V��
	switch (m_WeaponType)
	{
	case LONG_MISSILE:
		UpdateLong(obj);
		break;
	case SHORT_MISSILE:
		UpdateShort(obj);
		break;
	case RAIL_GUN:
		UpdateRail(obj);
		break;
	default:
		break;
	}

	//�ʏ�~�T�C��
	UpdateDefault(obj);

	m_VibCount++;
	if (m_VibCount > 60 && m_VibFlg)
	{
		InputX::StopVibration(0);
		m_VibCount = 0;
		m_VibFlg = false;
	}
}


//�~�T�C������
void WeaponSystem::MissileFire(GameObject* obj, int lockID)
{
	if (!m_WeaponChange)
	{
		ShotDefault(obj, lockID);
	}
	else
	{
		//���ꕺ���X�e�[�g�}�V��
		switch (m_WeaponType)
		{
		case LONG_MISSILE:
			ShotLong(obj, lockID);
			break;
		case SHORT_MISSILE:
			ShotShort(obj);
			break;
		case RAIL_GUN:
			ShotRail(obj);
			break;
		default:
			break;
		}
	}
}




//�}�V���K������
void WeaponSystem::MachineGunFire(GameObject* obj, D3DXVECTOR3 offset, float speed)
{
	m_MachineGunSE->Play(true, true);

	Bullet* bullet = m_Scene->AddGameObject<Bullet>(1);
	bullet->SetPosition(obj->GetPosition() + obj->LocalVector(obj->GetQuaternion(), offset));
	bullet->SetVelocity(obj->GetVelocity() + obj->GetForwardQuaternion() * speed);
	bullet->SetQuaternion(obj->GetQuaternion());

	InputX::SetVibration(0, 5);
}

void WeaponSystem::MachineGunStop()
{
	if (m_MachineGunSE->GetPlay())
	{
		m_MachineGunSE->Stop();
		InputX::StopVibration(0);
	}
}



//�����ύX
void WeaponSystem::WeaponChange()
{
	m_WeaponChange = !m_WeaponChange;
	UIChange(m_WeaponChange);
}


//UI�ύX
void WeaponSystem::UIChange(bool change)
{
	m_DefaultUIs[0]->SetDrawFlg(!change);
	m_DefaultUIs[1]->SetDrawFlg(!change);
	m_LongUIs[0]->SetDrawFlg(change);
	m_LongUIs[1]->SetDrawFlg(change);
}


void WeaponSystem::UpdateDefault(GameObject* obj)
{
	//m_DefaultUIs = m_Scene->GetGameObjects<MissileUI>();

	//�ʏ�~�T�C��
	{
		//�~�T�C��������
		if (m_DefaultSetFlg[0])
		{
			m_Missile[0]->SetPosition(obj->GetPosition()
									+ obj->LocalVector(obj->GetQuaternion(),
									D3DXVECTOR3(5.0f, -0.6f, -0.8f)));
			m_Missile[0]->SetQuaternion(obj->GetQuaternion());
			m_Missile[0]->SetVelocity(obj->GetVelocity());
		}
		if (m_DefaultSetFlg[1])
		{
			m_Missile[1]->SetPosition(obj->GetPosition() 
									+ obj->LocalVector(obj->GetQuaternion(),
									D3DXVECTOR3(-5.0f, -0.6f, -0.8f)));
			m_Missile[1]->SetQuaternion(obj->GetQuaternion());
			m_Missile[1]->SetVelocity(obj->GetVelocity());
		}

		//�~�T�C���N�[���^�C��
		if (m_DefaultUIs[0]->GetGauge() == 100)
		{
			m_Missile[0] = m_Scene->AddGameObject<Missile>(1);

			m_DefaultSetFlg[0] = true;
		}
		if (m_DefaultUIs[1]->GetGauge() == 100)
		{
			m_Missile[1] = m_Scene->AddGameObject<Missile>(1);

			m_DefaultSetFlg[1] = true;
		}
	}
}

void WeaponSystem::ShotDefault(GameObject* obj, int lockID)
{
	m_EnemyJets = m_Scene->GetGameObjects<EnemyJet>();
	//m_DefaultUIs = m_Scene->GetGameObjects<MissileUI>();

	//�~�T�C�����������Ώۂ�ID�擾
	if (!m_EnemyJets.empty())
	{
		m_LockID = lockID;
	}

	if(m_Amount[DEFAULT_NUM] > 0)
	{
		if (m_PosChange[DEFAULT_NUM] % 2 == 0 && m_DefaultSetFlg[0])
		{

			m_Missile[0]->SetVelocity(obj->GetForwardQuaternion() * 2.0f + obj->GetVelocity());
			m_Missile[0]->SetShot(true);

			m_DefaultUIs[0]->SetGauge(0);

			InputX::SetVibration(0, 10);
			m_VibCount = 0;
			m_VibFlg = true;

			m_DefaultSetFlg[0] = false;
			m_PosChange[DEFAULT_NUM]++;
			m_Amount[DEFAULT_NUM]--;
		}
		else if (m_PosChange[DEFAULT_NUM] % 2 == 1 && m_DefaultSetFlg[1])
		{

			m_Missile[1]->SetVelocity(obj->GetForwardQuaternion() * 2.0f + obj->GetVelocity());
			m_Missile[1]->SetShot(true);

			m_DefaultUIs[1]->SetGauge(0);

			InputX::SetVibration(0, 10);
			m_VibCount = 0;
			m_VibFlg = true;

			m_DefaultSetFlg[1] = false;
			m_PosChange[DEFAULT_NUM]++;
			m_Amount[DEFAULT_NUM]--;
		}
	}
}

void WeaponSystem::UpdateLong(GameObject* obj)
{
	//m_LongMissileUIs = m_Scene->GetGameObjects<LongMissileUI>();

	{
		//�~�T�C������
		if (m_SpecialSetFlg[0] && m_Amount[SPECIAL_NUM] > 0)
		{
			m_LongMissile[0]->SetPosition(obj->GetPosition() 
										+ obj->LocalVector(obj->GetQuaternion(),
										D3DXVECTOR3(4.0f, -0.8f, -0.8f)));
			m_LongMissile[0]->SetQuaternion(obj->GetQuaternion());
			m_LongMissile[0]->SetVelocity(obj->GetVelocity());
		}
		if (m_SpecialSetFlg[1] && m_Amount[SPECIAL_NUM] > 0)
		{
			m_LongMissile[1]->SetPosition(obj->GetPosition() 
										+ obj->LocalVector(obj->GetQuaternion(),
										D3DXVECTOR3(-4.0f, -0.8f, -0.8f)));
			m_LongMissile[1]->SetQuaternion(obj->GetQuaternion());
			m_LongMissile[1]->SetVelocity(obj->GetVelocity());
		}

		//�~�T�C���`���[�W������
		if (m_LongUIs[0]->GetGauge() == 100)
		{
			m_LongMissile[0] = m_Scene->AddGameObject<LongMissile>(1);

			m_SpecialSetFlg[0] = true;
		}
		if (m_LongUIs[1]->GetGauge() == 100)
		{
			m_LongMissile[1] = m_Scene->AddGameObject<LongMissile>(1);

			m_SpecialSetFlg[1] = true;
		}
	}
}

void WeaponSystem::ShotLong(GameObject* obj,int lockID)
{
	m_EnemyJets = m_Scene->GetGameObjects<EnemyJet>();
	//m_LongMissileUIs = m_Scene->GetGameObjects<LongMissileUI>();

	// �~�T�C������
		//�~�T�C�����������Ώۂ�ID�擾
	if (!m_EnemyJets.empty())
	{
		m_LockID = lockID;
	}

	//����
	if(m_Amount[SPECIAL_NUM] > 0)
	{
		if (m_PosChange[SPECIAL_NUM] % 2 == 0 && m_SpecialSetFlg[0])
		{

			m_LongMissile[0]->SetVelocity(obj->GetVelocity() - obj->GetForwardQuaternion() * 0.02f);
			m_LongMissile[0]->SetShot(true);

			m_LongUIs[0]->SetGauge(0);

			InputX::SetVibration(0, 10);
			m_VibCount = 0;
			m_VibFlg = true;

			m_SpecialSetFlg[0] = false;
			m_PosChange[SPECIAL_NUM]++;
			m_Amount[SPECIAL_NUM]--;
		}
		else if (m_PosChange[SPECIAL_NUM] % 2 == 1 && m_SpecialSetFlg[1])
		{

			m_LongMissile[1]->SetVelocity(obj->GetVelocity() - obj->GetForwardQuaternion() * 0.02f);
			m_LongMissile[1]->SetShot(true);

			m_LongUIs[1]->SetGauge(0);

			InputX::SetVibration(0, 10);
			m_VibCount = 0;
			m_VibFlg = true;

			m_SpecialSetFlg[1] = false;
			m_PosChange[SPECIAL_NUM]++;
			m_Amount[SPECIAL_NUM]--;
		}
	}
}

void WeaponSystem::UpdateShort(GameObject* obj)
{
}

void WeaponSystem::ShotShort(GameObject* obj)
{
}

void WeaponSystem::UpdateRail(GameObject* obj)
{
}

void WeaponSystem::ShotRail(GameObject* obj)
{
}
