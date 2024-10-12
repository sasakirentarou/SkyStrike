
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureDepthShadow : register(t1);
TextureCube g_TextureEnvCube : register(t2); //�L���[�u�}�b�v�󂯎��
Texture2D g_DissolveTexture : register(t3);
Texture2D g_TextureIBL : register(t4);

SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//�@���𐳋K�����Ċi�[����
    float4 normal = normalize(In.Normal);

	//�����x�N�g�������
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);

	//���˃x�N�g�������
    float3 refv = reflect(eyev, normal.xyz);
    refv = normalize(refv);
    
    
    float4 baseColor = g_Texture.Sample(g_SamplerState, In.TexCoord) * brightness;


    
    //�����o�[�g
    float3 lambert = float3(0.0, 0.0, 0.0);
    if (lambertEnable)
    {
        //�n�[�t
        //lambert = saturate(dot(-Light.Direction.xyz, normal.xyz) + 0.5);
        //�ʏ�
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
        lambert.rgb += GroundFogColor.rgb * (FogParam.z * 0.00001); //�t�H�O�F�����Z
    }

    
    //�X�y�L����
    float3 specular = float3(0.0, 0.0, 0.0);
    if (specularEnable)
    {
        //�t�H��
        specular = saturate(dot(-Light.Direction.xyz, refv.xyz)); //���ς��v�Z
        specular = pow(specular, shapness); //n�悵�ĉs���𒲐�
        
        //outDiffuse.rgb += specular;
    }

    
    //���}�b�s���O
    float3 env = float3(0.0, 0.0, 0.0);
    if (envEnable)
    {
        float4 envColor = g_TextureEnvCube.Sample(g_SamplerState, refv);

        env = saturate(envColor.rgb * reflectivity); //���˓x
        
        //outDiffuse = baseColor;
        //outDiffuse.rgb = saturate((envColor.rgb * 0.05) + (outDiffuse.rgb * 1.0)); //���˓x + ���x
        //outDiffuse.a *= In.Diffuse.a;//toon(���C�e�B���O�Ȃ�)
        //outDiffuse *= In.Diffuse * 0.8 + 0.2; //���C�e�B���O����(0.2�`1.0)
    }

    
    //�f�B�]���u
    float3 dissolve = float3(0.0, 0.0, 0.0);
    {
        float dissolveValue = g_DissolveTexture.Sample(g_SamplerState, In.TexCoord);
        float threshold = dissolveThreshold * (1.0f + dissolveRange) - dissolveRange;
        float rate = saturate((dissolveValue - threshold) / dissolveRange);
        outDiffuse.a = rate + dissolveColor.a;
        //outDiffuse.rgb = lerp(outDiffuse.rgb, dissolveColor.rgb, 1 - pow(rate, 5));
        dissolve = lerp(dissolve, dissolveColor.rgb, 1 - pow(rate, 5));
        // �I�����W:1.0f, 0.3f, 0.0f
    }

    
    //����
    outDiffuse.rgb = saturate(lambert + env + dissolve + specular);
    
    
    
    
    
    //�f�v�X�V���h�E
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
