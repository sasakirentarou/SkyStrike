
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureDepthShadow : register(t1);
TextureCube g_TextureEnvCube : register(t2); //キューブマップ受け取り
Texture2D g_DissolveTexture : register(t3);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//法線を正規化して格納する
    float4 normal = normalize(In.Normal);

	//視線ベクトルを作る
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);

	//反射ベクトルを作る
    float3 refv = reflect(eyev, normal.xyz);
    refv = normalize(refv);

    float4 envColor = g_TextureEnvCube.Sample(g_SamplerState, refv);

    //環境マッピング処理
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb = saturate((envColor.rgb * 0.2) + (outDiffuse.rgb * 1.0)); //反射度 + 明度
    //outDiffuse.a *= In.Diffuse.a;//toon(ライティングなし)
    outDiffuse *= In.Diffuse * 0.8 + 0.2;//ライティングあり(0.2～1.0)

    //ディゾルブ処理
    {
        float dissolveValue = g_DissolveTexture.Sample(g_SamplerState, In.TexCoord);
        float threshold = dissolveThreshold * (1.0f + dissolveRange) - dissolveRange;
        float rate = saturate((dissolveValue - threshold) / dissolveRange);
        outDiffuse.a = rate + 0.3;
        outDiffuse.rgb = lerp(outDiffuse.rgb, float3(1.0f, 1.0f, 1.0f), 1 - pow(rate, 5));
        // オレンジ:1.0f, 0.3f, 0.0f
    }

    //Shadow処理
    {
        In.ShadowPosition.xyz /= In.ShadowPosition.w;
        In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5;
        In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5;

        float depth = g_TextureDepthShadow.Sample(g_SamplerState, In.ShadowPosition.xy);
	
        if (depth < In.ShadowPosition.z - 0.001)
        {
            outDiffuse.rgb *= 0.5;
        }
    }
}
