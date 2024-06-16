#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    Out.Position = mul(In.Position, wvp);
    Out.WorldPosition = float4(0.0, 0.0, 0.0, 0.0);
    Out.Normal = float4(0.0, 0.0, 0.0, 0.0);
    Out.Tangent = float4(0.0, 0.0, 0.0, 0.0);
    Out.Binormal = float4(0.0, 0.0, 0.0, 0.0);

    Out.Diffuse = In.Diffuse;
    Out.TexCoord = In.TexCoord;
}