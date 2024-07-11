
#include "common.hlsl"

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = float4(0.0f, 0.0f, 0.0f,0.6f);
   
        //��HP�Q�[�W
   {
        float nowHp = hp.x;
        float maxHp = hp.y;
        float beforHp = hp.z;
    
        //UV�l����s�N�Z���ւ̃X�P�[���ϊ�
        float gage = In.TexCoord.x * maxHp;
        //HP�Q�[�W�̃O���[�������쐬����
        outDiffuse.rgb += lostColor.rgb * step(beforHp, gage);
        //HP�Q�[�W�̗Ε������쐬����
        outDiffuse.rgb += hpBaseColor.rgb * (1.0f - step(nowHp, gage));
        //HP�Q�[�W�̐ԐF�������쐬����
        outDiffuse.rgb += diffColor.rgb * (1.0f - step(beforHp, gage)) * step(nowHp, gage);
    }

}