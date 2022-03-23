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
	float4 normal = normalize(In.Normal);

	//ランバート拡散照明
	float light = -dot(normal.xyz, Light.Direction.xyz);
	light = saturate(light);

	//通常のテクスチャの色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb * light;
	outDiffuse.a *= In.Diffuse.a;


	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;//視線ベクトル
	eyev = normalize(eyev);

	float3 refv = reflect(eyev, normal.xyz); //視線と法線との反射ベクトル
	refv = normalize(refv);



	//環境マッピング
	float2 envTexCoord;
	envTexCoord.x = -refv.x * 0.3 + 0.5;	//反射ベクトルをテクスチャ座標へ変換
	envTexCoord.y = -refv.y * 0.3 + 0.5;


	//outDiffuse.rgb = g_TextureEnv.Sample(g_SamplerState, envTexCoord).rgb;
	outDiffuse.rgb = g_TextureEnv.SampleBias(g_SamplerState, envTexCoord, 0.0);//バイアス付き
	
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