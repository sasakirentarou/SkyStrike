
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
    
    
    float4 baseColor = g_Texture.Sample(g_SamplerState, In.TexCoord);


    //環境マッピング
    float3 env;
    {
        float4 envColor = g_TextureEnvCube.Sample(g_SamplerState, refv);

        env = baseColor.rgb;
        env = saturate((envColor.rgb * reflectivity) + (env.rgb * brightness)); //反射度 + 明度
        
        //outDiffuse = baseColor;
        //outDiffuse.rgb = saturate((envColor.rgb * 0.05) + (outDiffuse.rgb * 1.0)); //反射度 + 明度
        //outDiffuse.a *= In.Diffuse.a;//toon(ライティングなし)
        //outDiffuse *= In.Diffuse * 0.8 + 0.2; //ライティングあり(0.2～1.0)
    }
    
    //ハーフランバート
    float3 lambert;
    {
        lambert = saturate(dot(-Light.Direction.xyz, normal.xyz) + 0.5);
        lambert *= baseColor.rgb;
    }
    
    //スペキュラ
    float3 specular;
    {
        //フォン
        //specular = saturate(dot(-Light.Direction.xyz, refv.xyz)); //内積を計算
        //specular = pow(specular, 100); //n乗して鋭さを調整
        
        //outDiffuse.rgb += specular;
    }

    //ディゾルブ
    float3 dissolve = float3(0.0, 0.0, 0.0);
    {
        float dissolveValue = g_DissolveTexture.Sample(g_SamplerState, In.TexCoord);
        float threshold = dissolveThreshold * (1.0f + dissolveRange) - dissolveRange;
        float rate = saturate((dissolveValue - threshold) / dissolveRange);
        outDiffuse.a = rate + dissolveColor.a;
        //outDiffuse.rgb = lerp(outDiffuse.rgb, dissolveColor.rgb, 1 - pow(rate, 5));
        dissolve = lerp(dissolve, dissolveColor.rgb, 1 - pow(rate, 5));
        // オレンジ:1.0f, 0.3f, 0.0f
    }

    outDiffuse.rgb = saturate(lambert + env + dissolve) - 0.3;
    
    //デプスシャドウ
    {
        In.ShadowPosition.xyz /= In.ShadowPosition.w;
        In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5;
        In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5;

        float depthColor = g_TextureDepthShadow.Sample(g_SamplerState, In.ShadowPosition.xy);
	
        if (depthColor < In.ShadowPosition.z - 0.001)
        {
            outDiffuse.rgb *= 0.5;
        }
    }
}
