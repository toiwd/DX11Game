#include "common.hlsl" // 必ずインクルード

// DirectXのテクスチャの設定を受け継ぐ
Texture2D g_Texture : register(t0); // テクスチャー0番
Texture2D g_TextureEnv : register(t1); // テクスチャー1番
// DirectXのサンプラーステート設定を受け継ぐ
SamplerState g_SamplerState : register(s0); // テクスチャーサンプラー0番


//
//Texture X650 Y650
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal);//法線

	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;//視線ベクトル
	eyev = normalize(eyev);

	float3 refv = reflect(eyev, normal.xyz); //反射ベクトル
	refv = normalize(refv);

	//環境マッピング
	float2 envTexCoord;//	-1.0～1.0 => 0.0～1.0
	envTexCoord.x = -refv.x * 0.5 + 0.5;
	envTexCoord.y = -refv.y * 0.5 + 0.5;



	//テクスチャの色を出力
	outDiffuse.rgb = g_TextureEnv.SampleBias(g_SamplerState, envTexCoord, 1.0).rgb;
	outDiffuse += g_Texture.Sample(g_SamplerState, In.TexCoord);//色が暗くなるのでとっておく
	outDiffuse.a = 1.0f;

	/*
	float4		normal = normalize(In.Normal);	// ピクセルの法線を正規化

	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;	// 視線ベクトル
	eyev = normalize(eyev);										// 正規化する

	float3 refv = reflect(Light.Direction.xyz, normal.xyz);		// 光の反射ベクトル
	refv = normalize(refv);

	float2 envTexCoord;											// 反射ベクトルから環境マッピング用のテクスチャ座標を作成
	envTexCoord.x = -refv.x * 0.5f + 0.5f;							// ここ足りない！！
	envTexCoord.y = -refv.y * 0.5f + 0.5f;

	// テクスチャバイアス指定付きでのテクスチャの取得
	float4 EnvTex = g_TextureEnv.SampleBias(g_SamplerState, envTexCoord, 0.0f);
	float4 BaseTex = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb = (1.0f * EnvTex.rgb) + (1.0f * BaseTex.rgb);	// テクスチャ＊明るさ＊頂点色

	outDiffuse.a = BaseTex.a * In.Diffuse.a;				// ベースのαと引数ピクセルのαを乗算（ここも足りん）


	float light = -dot(normal.xyz, Light.Direction.xyz);	// 明るさを計算
	*/





}