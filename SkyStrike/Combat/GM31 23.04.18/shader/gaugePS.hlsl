
#include "common.hlsl"

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = float4(0.0f, 0.0f, 0.0f,0.6f);
   
        //横HPゲージ
   {
        float nowHp = hp.x;
        float maxHp = hp.y;
        float beforHp = hp.z;
    
        //UV値からピクセルへのスケール変換
        float gage = In.TexCoord.x * maxHp;
        //HPゲージのグレー部分を作成する
        outDiffuse.rgb += lostColor.rgb * step(beforHp, gage);
        //HPゲージの緑部分を作成する
        outDiffuse.rgb += hpBaseColor.rgb * (1.0f - step(nowHp, gage));
        //HPゲージの赤色部分を作成する
        outDiffuse.rgb += diffColor.rgb * (1.0f - step(beforHp, gage)) * step(nowHp, gage);
    }

}