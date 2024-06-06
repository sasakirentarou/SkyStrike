#pragma once

#include "gameObject.h"

class HitBullet : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;

	class Audio* m_Bom{};
	class Scene* m_Scene{};
	class Camera* m_Camera{};

	int m_Count{};
	int m_TimeCount{};
	int m_Time = 0;
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void AnimTime(int time) { m_Time = time; }
};