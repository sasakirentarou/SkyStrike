#include "main.h"
#include "renderer.h"
#include "collisionBox.h"



void CollisionBox::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\boxTexturePS.cso");

	m_Model = new Model();
	m_Model->Load("asset\\model\\collision\\collision.obj");


	GameObject::Init();
}

void CollisionBox::Uninit()
{
	m_Model->Unload();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void CollisionBox::Update()
{
	GameObject::Update();
}

void CollisionBox::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world,scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans * m_Pearent;
	Renderer::SetWorldMatrix(&world);

	//param設定
	PARAMETER param;
	param.boxColor = m_Color;
	Renderer::SetParameter(param);

	Renderer::SetATCEnable(true);
	m_Model->Draw();
	Renderer::SetATCEnable(false);

	GameObject::Draw();
}


//A(obb1)とB(obb2)のOBB当たり判定関数
//bool CollisionBox::SetOBB(GameObject* obb1, GameObject* obb2, float offset)
//{
//	//Aの軸に格納
//	D3DXVECTOR3 NAe1 = obb1->GetCollisionRight()  , Ae1 = NAe1 * (obb1->GetCollisionSca().x * offset);
//	D3DXVECTOR3 NAe2 = obb1->GetCollisionTop()    , Ae2 = NAe2 * (obb1->GetCollisionSca().y * offset);
//	D3DXVECTOR3 NAe3 = obb1->GetCollisionForward(), Ae3 = NAe3 * (obb1->GetCollisionSca().z * offset);
//
//	//Bの軸に格納
//	D3DXVECTOR3 NBe1 = obb2->GetCollisionRight()  , Be1 = NBe1 * (obb2->GetCollisionSca().x * offset);
//	D3DXVECTOR3 NBe2 = obb2->GetCollisionTop()    , Be2 = NBe2 * (obb2->GetCollisionSca().y * offset);
//	D3DXVECTOR3 NBe3 = obb2->GetCollisionForward(), Be3 = NBe3 * (obb2->GetCollisionSca().z * offset);
//
//	//中心座標の距離
//	D3DXVECTOR3 Interval = obb1->GetCollisionPos() - obb2->GetCollisionPos();
//
//	//分離軸の計算//
//
//	//投影線分の長さ
//	float rA, rB, L;
//	//外積分離軸
//	D3DXVECTOR3 Cross;
//
//	//Ae1
//	rA = D3DXVec3Length(&Ae1);
//	rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NAe1));
//	//判定
//	if (L > rA + rB)
//		return false; // 衝突していない
//
//	//Ae2
//	rA = D3DXVec3Length(&Ae2);
//	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NAe2));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//Ae3
//	rA = D3DXVec3Length(&Ae3);
//	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NAe3));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//Be1
//	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be1);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe1));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//Be2
//	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe2));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//Be3
//	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe3));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//C11
//	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3, 0);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3, 0);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//C12
//	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3, 0);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3, 0);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//C13
//	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3, 0);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2, 0);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//C21
//	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3, 0);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3, 0);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//C22
//	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3, 0);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3, 0);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//C23
//	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3, 0);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2, 0);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//C31
//	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2, 0);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3, 0);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//C32
//	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2, 0);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3, 0);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//C33
//	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2, 0);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2, 0);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	//判定
//	if (L > rA + rB)
//		return false;
//
//	//判定
//	return true; // 衝突している
//}

// 投影線分長関数 //
// 分離軸に投影された軸成分から投影線分長を算出
// 分離軸Sepは標準化されていること
float CollisionBox::LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	float r1 = fabs(D3DXVec3Dot(Sep, e1));
	float r2 = fabs(D3DXVec3Dot(Sep, e2));
	//e3が0だった時0をr3に代入
	//0以外だった時fabsの計算をしr3に代入
	float r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;

	return r1 + r2 + r3;
}