// なべ
#include "common.hlsl" // 必ずインクルード

// DirectXのテクスチャの設定を受け継ぐ
Texture2D g_Texture : register(t0); // テクスチャー0番
// DirectXのサンプラーステート設定を受け継ぐ
SamplerState g_SamplerState : register(s0); // テクスチャーサンプラー0番


//
//in は入力されてくるデータ
//out は出力するデータ
//Texture X650 Y650
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4		normal = normalize(In.Normal);	// ピクセルの法線を正規化
	float		light = -dot(normal.xyz, Light.Direction.xyz);	// 明るさを計算

	// このピクセルに使われるテクスチャの色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= (light * In.Diffuse.rgb);	// テクスチャ＊明るさ＊頂点色
	outDiffuse.a *= In.Diffuse.a;				// 頂点色のαをそのまま使う

	// スペキュラー（フォン鏡面反射）
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;	// 視線ベクトル
	eyev = normalize(eyev);										// 正規化する

	float3 refv = reflect(Light.Direction.xyz, normal.xyz);		// 光の反射ベクトル
	refv = normalize(refv);										// 正規化する

	float	specular = -dot(eyev, refv);						// 視線と反射ベクトルの内積
	specular = saturate(specular);								// 値をサチュレート
	specular = pow(specular, 30);								// 30乗

	outDiffuse.rgb += specular;									// スペキュラ値を加算する
}