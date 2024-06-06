#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    

    matrix wvp; //いつもの処理だが
    wvp = mul(World, View); //1パス目はライトカメラの行列
    wvp = mul(wvp, Projection); //2パス目は通常カメラの行列が設定される
    Out.Position = mul(In.Position, wvp); //頂点座標をワールド変換して出力
    float4 worldNormal, normal; //法線のワールド変換
    
    //Normal
    normal = float4(In.Normal.xyz, 0.0);
    worldNormal = mul(normal, World);
    Out.Normal = normalize(worldNormal); //法線を出力
   
    
    //Tangent
    normal = float4(In.Tangent.xyz, 0.0);
    worldNormal = mul(normal, World);
    Out.Tangent = normalize(worldNormal); //法線を出力
    
    
    //Binormal
    normal = float4(In.Binormal.xyz, 0.0);
	//法線をワールド変換する
    worldNormal = mul(normal, World);
	//ワールド変換した法線を正規化する（拡大縮小の情報を削除する）
    Out.Binormal = normalize(worldNormal);
    
//光源処理
    float light = -dot(Light.Direction.xyz, worldNormal.xyz);
    light = saturate(light);
    Out.Diffuse = light;
    Out.Diffuse.a = In.Diffuse.a;
    Out.TexCoord = In.TexCoord; //テクスチャ座標を出力
    matrix lightwvp;
    lightwvp = mul(World, Light.View); //ワールド行列＊ライトビュー行列
    lightwvp = mul(lightwvp, Light.Projection); //さらに＊ライトプロジェクション行列
    Out.ShadowPosition = mul(In.Position, lightwvp); //ライトカメラからみた頂点座標出力
    
    //頂点カラーはそのままコピー
    Out.Diffuse = In.Diffuse;

	//ワールド変換した頂点座標を出力
    Out.WorldPosition = mul(In.Position, World);
    
}


