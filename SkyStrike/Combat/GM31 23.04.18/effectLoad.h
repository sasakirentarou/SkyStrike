//#pragma once
//
//#include "gameObject.h"
//
//class EffectLoad : public GameObject
//{
//private:
//	ID3D11VertexShader* m_VertexShader = nullptr;
//	ID3D11PixelShader* m_PixelShader = nullptr;
//	ID3D11InputLayout* m_VertexLayout = nullptr;
//
//	ID3D11Buffer* m_VertexBuffer{};
//	ID3D11ShaderResourceView* m_Texture{};
//	
//	class Audio* m_SE{};
//
//	int m_Count{};
//	int m_TimeCount{};
//public:
//	void Init(const char* texture,bool SEmode = false, const char* se = "");
//	void Uninit();
//	void Draw(int imageSplit, int animationTimeint, int imageWidth, 
//		int imageHeight,float width, float height,D3DXVECTOR3 pos);
//};