
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureDepthShadow : register(t1);
TextureCube g_TextureEnvCube : register(t2); //キューブマップ受け取り
Texture2D g_DissolveTexture : register(t3);
Texture2D g_TextureIBL : register(t4);

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
    
    
    float4 baseColor = g_Texture.Sample(g_SamplerState, In.TexCoord) * brightness;


    
    //ランバート
    float3 lambert = float3(0.0, 0.0, 0.0);
    if (lambertEnable)
    {
        //ハーフ
        //lambert = saturate(dot(-Light.Direction.xyz, normal.xyz) + 0.5);
        //通常
        lambert = saturate(dot(-Light.Direction.xyz, normal.xyz));
        lambert *= baseColor.rgb;
    }
        
    //IBL
    if (iblEnable)
    {
        float2 iblTexCoord;
        iblTexCoord.x = atan2(normal.x, normal.z) / (PI * 2);
        iblTexCoord.y = acos(normal.y) / PI;
        
        lambert.rgb += baseColor.rgb * g_TextureIBL.SampleLevel(g_SamplerState, iblTexCoord, 9).rgb;
        lambert.rgb += GroundFogColor.rgb * (FogParam.z * 0.00001); //フォグ色を加算
    }

    
    //スペキュラ
    float3 specular = float3(0.0, 0.0, 0.0);
    if (specularEnable)
    {
        //フォン
        specular = saturate(dot(-Light.Direction.xyz, refv.xyz)); //内積を計算
        specular = pow(specular, shapness); //n乗して鋭さを調整
        
        //outDiffuse.rgb += specular;
    }

    
    //環境マッピング
    float3 env = float3(0.0, 0.0, 0.0);
    if (envEnable)
    {
        float4 envColor = g_TextureEnvCube.Sample(g_SamplerState, refv);

        env = saturate(envColor.rgb * reflectivity); //反射度
        
        //outDiffuse = baseColor;
        //outDiffuse.rgb = saturate((envColor.rgb * 0.05) + (outDiffuse.rgb * 1.0)); //反射度 + 明度
        //outDiffuse.a *= In.Diffuse.a;//toon(ライティングなし)
        //outDiffuse *= In.Diffuse * 0.8 + 0.2; //ライティングあり(0.2～1.0)
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

    
    //合成
    outDiffuse.rgb = saturate(lambert + env + dissolve + specular);
    
    
    
    
    
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
