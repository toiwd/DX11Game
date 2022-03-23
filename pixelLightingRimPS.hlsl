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
	//float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;	// 視線ベクトル
	//eyev = normalize(eyev);										// 正規化する
	//float3 refv = reflect(Light.Direction.xyz, normal.xyz);		// 光の反射ベクトル
	//refv = normalize(refv);										// 正規化する
	//float	specular = -dot(eyev, refv);						// 視線と反射ベクトルの内積
	//specular = saturate(specular);								// 値をサチュレート


	// スペキュラー（ブリンフォン）
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;	// 視線ベクトル作成
	eyev = normalize(eyev);										// 正規化
	float3 halfv = eyev + Light.Direction.xyz;					// ハーフベクトル作成
	halfv = normalize(halfv);									// 正規化
	float	specular = -dot(halfv, normal.xyz);					// 視線とハーフベクトルの内積
	specular = saturate(specular);								// 値をサチュレート


	specular = pow(specular, 30);								// 30乗
	outDiffuse.rgb += specular;									// スペキュラ値を加算する


	// リムライト
	float rim = dot(eyev, normal.xyz) + 1.0f;					// 直角時が1.0になる
	rim = saturate(rim);										// サチュレート
	rim = pow(rim, 3) * 2.0f;									// 見た感じで値を決める

	outDiffuse.rgb += rim;										// リムライトを加算する
}