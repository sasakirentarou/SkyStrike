
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureDepthShadow : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//テクスチャから色をサンプリングする
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= outDiffuse.rgb;
    outDiffuse.a *= In.Diffuse.a;

	//線形フォグ
    float fog_start = FogParam.x;
    float fog_end = FogParam.y;
	
    float dist = distance(In.WorldPosition.xyz, CameraPosition.xyz);
    float fog = (fog_end - dist) / (fog_end - fog_start);
    fog = saturate(fog);
    float3 fog_color = FogColor.rgb;
    outDiffuse.rgb = outDiffuse.rgb * fog + fog_color * (1.1 - fog);

	//グラウンドフォグ
    float fog_height = FogParam.z;

    fog = In.WorldPosition.y / fog_height;
    fog = saturate(fog);
    fog_color = GroundFogColor.rgb;
    //outDiffuse.rgb = outDiffuse.rgb * fog + fog_color * (0.7 - fog);

	
	//影の処理
	In.ShadowPosition.xyz /= In.ShadowPosition.w;
	In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5;
	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5;

	float depth = g_TextureDepthShadow.Sample(g_SamplerState, In.ShadowPosition.xy);
	
	if (depth < In.ShadowPosition.z - 0.001)
	{
		outDiffuse.rgb *= 0.5;
	}
}
