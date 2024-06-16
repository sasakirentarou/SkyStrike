


cbuffer WorldBuffer : register(b0)
{
    matrix World;
}
cbuffer ViewBuffer : register(b1)
{
    matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}




struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float3 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}

struct LIGHT
{
    bool Enable;
    bool3 Dummy;
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
	
	
    matrix View; //ライトカメラのビュー行列
    matrix Projection; //ライトカメラのプロジェクション行列
};

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}


struct VS_IN
{
    float4 Position : POSITION0;
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
	
	
    float4 Tangent : TANGENT0;
    float4 Binormal : BINORMAL0;
};


struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
	
	
    float4 WorldPosition : POSITION0;
    float4 Normal : NORMAL0;
    float4 Tangent : TANGENT0;
    float4 Binormal : BINORMAL0;

    float4 ShadowPosition : POSITION1;
};


//shader
cbuffer CameraBuffer : register(b5)
{
    float4 CameraPosition;
       
    //fog
    float4 FogParam;
    float4 FogColor;
    float4 GroundFogColor;
    float4 SkyColor;
}

cbuffer ParamBuffer : register(b6)
{
    //hp
    float4 hp;
    float4 baseColor;
    float4 lostColor;
    float4 diffColor;
    
    //dissolve
    float dissolveThreshold;
    float dissolveRange;
    float2 dummy;
    
    //box
    float4 boxColor;
}
