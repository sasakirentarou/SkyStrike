
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureDepthShadow : register(t1);
TextureCube g_TextureEnvCube : register(t2); //�L���[�u�}�b�v�󂯎��
Texture2D g_DissolveTexture : register(t3);

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
    
    
    float4 baseColor = g_Texture.Sample(g_SamplerState, In.TexCoord);


    //���}�b�s���O
    float3 env;
    {
        float4 envColor = g_TextureEnvCube.Sample(g_SamplerState, refv);

        env = baseColor.rgb;
        env = saturate((envColor.rgb * reflectivity) + (env.rgb * brightness)); //���˓x + ���x
        
        //outDiffuse = baseColor;
        //outDiffuse.rgb = saturate((envColor.rgb * 0.05) + (outDiffuse.rgb * 1.0)); //���˓x + ���x
        //outDiffuse.a *= In.Diffuse.a;//toon(���C�e�B���O�Ȃ�)
        //outDiffuse *= In.Diffuse * 0.8 + 0.2; //���C�e�B���O����(0.2�`1.0)
    }
    
    //�n�[�t�����o�[�g
    float3 lambert;
    {
        lambert = saturate(dot(-Light.Direction.xyz, normal.xyz) + 0.5);
        lambert *= baseColor.rgb;
    }
    
    //�X�y�L����
    float3 specular;
    {
        //�t�H��
        //specular = saturate(dot(-Light.Direction.xyz, refv.xyz)); //���ς��v�Z
        //specular = pow(specular, 100); //n�悵�ĉs���𒲐�
        
        //outDiffuse.rgb += specular;
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

    outDiffuse.rgb = saturate(lambert + env + dissolve) - 0.3;
    
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
