
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//法線を正規化して格納する
    float4 normal = normalize(In.Normal);

	//光源の向きと回転済みの法線とで内積を計算し、符号を反転して輝度を求める
    float light = -dot(Light.Direction.xyz, normal.xyz);
	//飽和演算(0.0～1.0の値にクランプする)
    light = saturate(light);

	//テクスチャから色をサンプリングする
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	//テクスチャの色に輝度を乗算する
    outDiffuse.rgb *= outDiffuse.rgb; //light
	//透明度は輝度に影響しないのでそのまま使う
    outDiffuse.a *= In.Diffuse.a;

    
	//線形フォグ
    float fog_start = FogParam.x;
    float fog_end = FogParam.y;
	
    
	//distance → 2点間の距離を返す
    float dist = distance(In.WorldPosition.xyz, CameraPosition.xyz);
    float fog = (fog_end - dist) / (fog_end - fog_start);
    fog = saturate(fog);
    float3 fog_color = SkyColor.rgb;
    //outDiffuse.rgb = outDiffuse.rgb * fog + fog_color * (1.0 - fog);

	//グラウンドフォグ
    float fog_height = FogParam.z;

    fog = In.WorldPosition.y / fog_height;
    fog = saturate(fog);
    fog_color = GroundFogColor.rgb;
    outDiffuse.rgb = outDiffuse.rgb * fog + fog_color * (1.0 - fog);
   
}