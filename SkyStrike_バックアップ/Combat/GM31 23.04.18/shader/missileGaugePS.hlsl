
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

    //横HPゲージ
   {
        float nowHp = hp.x;
        float maxHp = hp.y;
    
        //UV値からピクセルへのスケール変換
        float gage = In.TexCoord.x * maxHp;
        //HPゲージの緑部分を作成する
        outDiffuse.rgb *= baseColor.rgb * (1.0f - step(nowHp, gage));
        //HPゲージの赤色部分を作成する
        outDiffuse.r += lostColor.a * (1.0f - step(maxHp, gage)) * step(nowHp, gage);
        //全体を半透明
        outDiffuse.a -= 0.4 * (1.0f - step(maxHp, gage));
    }

}