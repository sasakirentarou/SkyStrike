#pragma once

#include "gameObject.h"


#define AUTO_RANGE	20 //ÉIÅ[ÉgîÕàÕ


class Cross : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;	

	class Scene* m_Scene{};
	class Jet* m_Jet{};

	float m_Red{};
	float m_Green{};
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	bool AutoRange();
};