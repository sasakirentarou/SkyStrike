
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

    //��HP�Q�[�W
   {
        float nowHp = hp.x;
        float maxHp = hp.y;
    
        //UV�l����s�N�Z���ւ̃X�P�[���ϊ�
        float gage = In.TexCoord.x * maxHp;
        //HP�Q�[�W�̗Ε������쐬����
        outDiffuse.rgb *= baseColor.rgb * (1.0f - step(nowHp, gage));
        //HP�Q�[�W�̐ԐF�������쐬����
        outDiffuse.r += lostColor.a * (1.0f - step(maxHp, gage)) * step(nowHp, gage);
        //�S�̂𔼓���
        outDiffuse.a -= 0.4 * (1.0f - step(maxHp, gage));
    }

}