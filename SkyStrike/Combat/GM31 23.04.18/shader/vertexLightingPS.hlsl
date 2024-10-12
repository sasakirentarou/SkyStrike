

#include "common.hlsl"


Texture2D g_Texture : register(t0);
Texture2D g_TextureIBL : register(t4);

SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	if (Material.TextureEnable)
	{
        //outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
        //outDiffuse *= In.Diffuse;
        
        
        float4 normal = normalize(In.Normal);
        float4 baseColor = g_Texture.Sample(g_SamplerState, In.TexCoord);

        
		//ランバート
        float3 lambert = float3(0.0, 0.0, 0.0);
        {
            //通常
            lambert = saturate(dot(-Light.Direction.xyz, normal.xyz));
            lambert *= baseColor.rgb;
        }
        
        //IBL
        {
            float2 iblTexCoord;
            iblTexCoord.x = atan2(normal.x, normal.z) / (PI * 2);
            iblTexCoord.y = acos(normal.y) / PI;
        
            lambert.rgb += baseColor.rgb * g_TextureIBL.SampleLevel(g_SamplerState, iblTexCoord, 9).rgb * 0.8;
            lambert.rgb += GroundFogColor.rgb * (FogParam.z * 0.00001); //フォグ色を加算

        }
        
        outDiffuse.rgb = lambert.rgb;
        outDiffuse.a = In.Diffuse.a;
    }
	else
	{
		//outDiffuse = In.Diffuse;
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    }
}

