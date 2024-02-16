#pragma once

#include <list>
#include "component.h"
#include "renderer.h"

class GameObject
{
protected:
	bool m_Destroy = false;
	bool m_DepthEnable = false;
	bool m_ReflectEnable = false;
	bool m_GameEnable = false;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMATRIX m_WorldMatrix{};
	D3DXQUATERNION m_Quaternion = D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f);//Rotation

	std::list<Component*> m_Component;
public:
	void SetDestroy()
	{
		m_Destroy = true;
	}

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetPosition(D3DXVECTOR3 pos) { m_Position = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_Rotation = rot; }
	void SetScale(D3DXVECTOR3 scale) { m_Scale = scale; }
	void SetVelocity(D3DXVECTOR3 velo) { m_Velocity = velo; }

	void SetQuaternion(D3DXQUATERNION qua) { m_Quaternion = qua; }
	void SetGameEnable(bool Enable) { m_GameEnable = Enable; }

	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetScale() { return m_Scale; }
	D3DXVECTOR3 GetVelocity() { return m_Velocity; }
	D3DXMATRIX GetMatrix() { return m_WorldMatrix; }
	D3DXQUATERNION GetQuaternion() { return m_Quaternion; }

	bool GetDepthEnable() { return m_DepthEnable; }
	bool GetReflectEnable() { return m_ReflectEnable; }
	bool GetGameEnable() { return m_GameEnable; }

	// 前方向ベクトル取得
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	// 右方向ベクトル取得
	D3DXVECTOR3 GetRight()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;

		return forward;
	}

	// 上方向ベクトル取得
	D3DXVECTOR3 GetTop()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._21;
		forward.y = rot._22;
		forward.z = rot._23;

		return forward;
	}

	// クォータニオンの前方向ベクトル取得
	D3DXVECTOR3 GetForwardQuaternion()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	// クォータニオンの右方向ベクトル取得
	D3DXVECTOR3 GetRightQuaternion()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);

		D3DXVECTOR3 forward;
		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;

		return forward;
	}

	// クォータニオンの上方向ベクトル取得
	D3DXVECTOR3 GetTopQuaternion()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);

		D3DXVECTOR3 forward;
		forward.x = rot._21;
		forward.y = rot._22;
		forward.z = rot._23;

		return forward;
	}

	//視界
	bool View(float fieldOfViewRadians, float viewDistancee,
		D3DXVECTOR3 myPos, D3DXVECTOR3 targetPos, D3DXVECTOR3 myForward)
	{
		// 視野範囲内かどうかの判定
		D3DXVECTOR3 normalizedDirection;
		D3DXVECTOR3 direction = targetPos - myPos;
		D3DXVec3Normalize(&normalizedDirection, &direction);
		D3DXVECTOR3 houkou = myForward;
		float dotProduct = D3DXVec3Dot(&houkou, &normalizedDirection);
		float angle = acos(dotProduct);
		fieldOfViewRadians = D3DXToRadian(fieldOfViewRadians);
		bool isInFieldOfView = angle <= fieldOfViewRadians / 2.0f;

		// 視野距離内かどうかの判定
		D3DXVECTOR3 dice = myPos - targetPos;
		float distance = D3DXVec3Length(&dice);
		bool isInViewDistance = distance <= viewDistancee;

		return isInFieldOfView && isInViewDistance;
	}

	virtual void Init() {}

	virtual void Uninit()
	{
		for (Component* component : m_Component)
		{
			component->Uninit();
			delete component;
		}

		// リスト構造の削除
		m_Component.clear();
	}

	virtual void Update()
	{
		for (Component* component : m_Component)
		{
			component->Update();
		}
	}

	virtual void Draw()
	{
		if (m_DepthEnable)
		{
			// テクスチャ設定
			ID3D11ShaderResourceView* depthShadowTexture = Renderer::GetDepthShadowTexture();
			Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &depthShadowTexture);
		}

		for (Component* component : m_Component)
		{
			component->Draw();
		}
	}

	// テンプレート関数
	template <typename T>
	T* AddComponet()
	{
		Component* componet = new T();
		m_Component.push_back(componet);
		componet->Init();

		return (T*)componet;
	}
};