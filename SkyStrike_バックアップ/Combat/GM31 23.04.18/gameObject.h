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

	//collision
	D3DXVECTOR3 m_MatrixPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_MatrixScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_MatrixForward = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_MatrixRight = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_MatrixUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	D3DXQUATERNION GetQuaternion() { return m_Quaternion; }
	D3DXMATRIX GetMatrix() { return m_WorldMatrix; }

	//collision
	D3DXVECTOR3 GetMatrixPosition() { return m_MatrixPosition; }
	D3DXVECTOR3 GetMatrixScale() { return m_MatrixScale; }
	D3DXVECTOR3 GetMatrixForward() { return m_MatrixForward; }
	D3DXVECTOR3 GetMatrixRight() { return m_MatrixRight; }
	D3DXVECTOR3 GetMatrixUp() { return m_MatrixUp; }

	bool GetDepthEnable() { return m_DepthEnable; }
	bool GetReflectEnable() { return m_ReflectEnable; }
	bool GetGameEnable() { return m_GameEnable; }

	// 前方向ベクトル取得(オイラー角)
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

	// 右方向ベクトル取得(オイラー角)
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

	// 上方向ベクトル取得(オイラー角)
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


	//collision
	D3DXVECTOR3 MatrixtoPosition(D3DXMATRIX matrix) 
	{
		D3DXVECTOR3 pos;
		pos.x = matrix._41;
		pos.y = matrix._42;
		pos.z = matrix._43;
		return pos;
	}
	D3DXVECTOR3 MatrixtoScale(D3DXMATRIX matrix) 
	{
		D3DXVECTOR3 scale;
		scale.x = matrix._11;
		scale.y = matrix._22;
		scale.z = matrix._33;
		return scale;
	}
	D3DXVECTOR3 MatrixtoForward(D3DXMATRIX matrix) //前方面ベクトルを取得
	{
		D3DXVECTOR3 forward;
		forward.x = matrix._31;
		forward.y = matrix._32;
		forward.z = matrix._33;

		return forward;
	}
	D3DXVECTOR3 MatrixtoRight(D3DXMATRIX matrix) //右方面ベクトルを取得
	{

		D3DXVECTOR3 right;
		right.x = matrix._11;
		right.y = matrix._12;
		right.z = matrix._13;

		return right;
	}
	D3DXVECTOR3 MatrixtoUp(D3DXMATRIX matrix) //上方面ベクトル
	{

		D3DXVECTOR3 up;
		up.x = matrix._21;
		up.y = matrix._22;
		up.z = matrix._23;

		return up;
	}
	void SetCollider(D3DXMATRIX matrix)
	{
		m_MatrixPosition = MatrixtoPosition(matrix);
		m_MatrixScale = MatrixtoScale(matrix);
		m_MatrixForward = MatrixtoForward(matrix);
		m_MatrixRight = MatrixtoRight(matrix);
		m_MatrixUp = MatrixtoUp(matrix);
	}


	D3DXVECTOR3 LocalVector(D3DXVECTOR3 rot, D3DXVECTOR3 offset)
	{
		D3DXVECTOR3 vector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vector += GetForward()  * offset.z;
		vector += GetRight()	* offset.x;
		vector += GetTop()		* offset.y;

		return vector;
	}
	D3DXVECTOR3 LocalVector(D3DXQUATERNION qua, D3DXVECTOR3 offset)
	{
		D3DXVECTOR3 vector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vector += GetForwardQuaternion() * offset.z;
		vector += GetRightQuaternion()   * offset.x;
		vector += GetTopQuaternion()	 * offset.y;

		return vector;
	}

	//軸周りのクォータニオン回転
	void SetQuaternionRotation(const D3DXVECTOR3& axis, float angle)
	{
		float radian = D3DXToRadian(angle);
		D3DXQuaternionRotationAxis(&m_Quaternion,&axis, radian);
	}

	//整数ランダム範囲指定  第一引数:ランダム範囲,第二引数:マイナス範囲
	int RandomInt(int range,bool minus)
	{
		if (minus)
		{
			return rand() % (range * 2) - range;
		}
		else
		{
			return rand() % range;
		}
	}


	//クランプ
	float Clamp(float clamp, float min, float max) {
		return (clamp < min) ? min : (clamp > max) ? max : clamp;
	}
	

	//視界: 視野角,視距離,位置,ターゲット,前方向ベクトル
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