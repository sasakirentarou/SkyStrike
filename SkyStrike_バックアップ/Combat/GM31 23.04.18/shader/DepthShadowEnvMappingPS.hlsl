
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

    float4 envColor = g_TextureEnvCube.Sample(g_SamplerState, refv);

    //���}�b�s���O����
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb = saturate((envColor.rgb * 0.2) + (outDiffuse.rgb * 1.0)); //���˓x + ���x
    //outDiffuse.a *= In.Diffuse.a;//toon(���C�e�B���O�Ȃ�)
    outDiffuse *= In.Diffuse * 0.8 + 0.2;//���C�e�B���O����(0.2�`1.0)

    //�f�B�]���u����
    {
        float dissolveValue = g_DissolveTexture.Sample(g_SamplerState, In.TexCoord);
        float threshold = dissolveThreshold * (1.0f + dissolveRange) - dissolveRange;
        float rate = saturate((dissolveValue - threshold) / dissolveRange);
        outDiffuse.a = rate + 0.3;
        outDiffuse.rgb = lerp(outDiffuse.rgb, float3(1.0f, 1.0f, 1.0f), 1 - pow(rate, 5));
        // �I�����W:1.0f, 0.3f, 0.0f
    }

    //Shadow����
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
