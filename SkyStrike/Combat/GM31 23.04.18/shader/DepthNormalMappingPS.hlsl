
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureDepthShadow : register(t1);
Texture2D g_TextureNormal : register(t2);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //ノーマルマップテクスチャから色をサンプリングする
	//このピクセルの法線マップのデータRGBA値を取得
    float4 normalMap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
	//スケーリングして色→ベクトルデータへ変換する
    normalMap = normalMap * 2.0 - 1.0;
    
    float4 n = normalize(In.Normal);
    float4 t = normalize(In.Tangent);
    float4 b = normalize(In.Binormal);
    float4 normal;
    normal.xyz = t.xyz * normalMap.x + b.xyz * normalMap.y + n.xyz * normalMap.z;
    normal.w = 0.0;

	//法線を正規化して格納する
    normal = normalize(normal);
    
    //光源の向きと回転済みの法線とで内積を計算し、符号を反転して輝度を求める
    float light = -dot(Light.Direction.xyz, normal.xyz);
	//飽和演算(0.0～1.0の値にクランプする)
    light = saturate(light);
    
	//テクスチャから色をサンプリングする
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;

    //マテリアルの拡散光を反映させる
    outDiffuse *= Material.Diffuse;
	
	//テクスチャの色に輝度を乗算する
    outDiffuse.rgb *= light;
	//透明度は輝度に影響しないのでそのまま使う
    outDiffuse.a *= In.Diffuse.a;
    
    //視線ベクトルを作成する
    float3 evev = In.WorldPosition.xyz - CameraPosition.xyz;
    evev = normalize(evev);
	//ハーフベクトルを作成する（視線ベクトル＋ライトベクトル）
    float3 halfv = evev + Light.Direction.xyz;
    halfv = normalize(halfv);

    float specular = -dot(halfv, normal.xyz);
    saturate(specular);
	//マテリアルのスぺキュラ強度を反映させる
    specular = pow(specular, Material.Shininess);
	//マテリアルのスぺキュラカラーを反映させる
    outDiffuse.rgb = saturate(outDiffuse.rgb + specular * Material.Specular.rgb);
    
    In.ShadowPosition.xyz /= In.ShadowPosition.w;
    In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5;
    In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5;

    float depth = g_TextureDepthShadow.Sample(g_SamplerState, In.ShadowPosition.xy);
	
    if (depth < In.ShadowPosition.z - 0.001)
    {
        outDiffuse.rgb *= 0.5;
    }

}
