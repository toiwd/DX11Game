
#include "common.hlsl"

// テクスチャ変数の宣言　法線マップ用の変数を追加（テクスチャ１番）
Texture2D g_Texture		: register(t0);// もともとのテクスチャ　テクスチャ0番
Texture2D g_NormalTexture		: register(t1);// 法線マップ用のテクスチャ テクスチャ1番

// サンプラーの宣言
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//このピクセルの法線マップのデータRGBAを取得
	float2 texcoord = In.TexCoord;
	texcoord.x += Time.x;
	float4 color = g_NormalTexture.Sample(g_SamplerState, texcoord);

	//取得したRGBAをベクトル情報XYZWとして展開する
	color = color * 2.0f - 1.0f;

	//展開したテクスチャデータを法線用変数へ入れ替え
	float4 normal;
	normal.x = -color.r;
	normal.y = color.b;
	normal.z = color.g;
	normal.w = 0.0;
	normal = normalize(normal);

	//ここからはこれまで通りの光源計算
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);


	float light = -dot(normal.xyz, Light.Direction.xyz);

	light = saturate(light);
	outDiffuse.rgb *= In.Diffuse.rgb * light;
	outDiffuse.a *= In.Diffuse.a;


	//スペキュラ（フォン）
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;	// 視線ベクトル作成
	eyev = normalize(eyev);										// 正規化
	float3 halfv = eyev + Light.Direction.xyz;					// ハーフベクトル作成
	halfv = normalize(halfv);									// 正規化
	float	specular = -dot(halfv, normal.xyz);					// 視線とハーフベクトルの内積
	specular = saturate(specular);								// 値をサチュレート


	specular = pow(specular, 30);								// 30乗
	outDiffuse.rgb += specular;									// スペキュラ値を加算する
}

