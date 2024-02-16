
#include "common.hlsl"



void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp; //matrix型の変数wvpを定義する

	//wvp = ワールド変換行列×ビュー変換行列
    wvp = mul(World, View);
	//wvp = wvp × プロジェクション変換行列
    wvp = mul(wvp, Projection);

	//入力された頂点座標を変換して出力先へ代入する
    Out.Position = mul(In.Position, wvp);

	//入力されたUV座標を出力先へ代入する
    Out.TexCoord = In.TexCoord;

	//頂点の法線をワールド行列で変換する
	float4 worldNormal, normal;

	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;
    Out.Tangent = float4(0.0, 0.0, 0.0, 0.0);
    Out.Binormal = float4(0.0, 0.0, 0.0, 0.0);

	//光源処理
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);
	//頂点カラーはそのままコピー
	Out.Diffuse = light;
    Out.Diffuse.a = In.Diffuse.a;

    Out.WorldPosition = mul(In.Position, World);

	//ライトカメラから見た頂点座標を作成
	matrix lightwvp;
	lightwvp = mul(World, Light.View);
	lightwvp = mul(lightwvp, Light.Projection);

	Out.ShadowPosition = mul(In.Position, lightwvp);
}
