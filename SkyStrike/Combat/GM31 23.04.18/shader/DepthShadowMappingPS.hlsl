
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureDepthShadow : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{	
	//テクスチャから色をサンプリングする
	float4 baseColor = g_Texture.Sample(g_SamplerState, In.TexCoord);
    float4 normal = normalize(In.Normal);
	
	//ハーフランバート
    outDiffuse.rgb = saturate(dot(-Light.Direction.xyz, normal.xyz) + 0.5);
    outDiffuse.rgb *= baseColor.rgb;
	
    outDiffuse.a = In.Diffuse.a;

	In.ShadowPosition.xyz /= In.ShadowPosition.w;
	In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5;
	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5;

	float depth = g_TextureDepthShadow.Sample(g_SamplerState, In.ShadowPosition.xy);
	
	if (depth < In.ShadowPosition.z - 0.001)
	{
		outDiffuse.rgb *= 0.5;
	}
}
