
//ピクセルシェーダー

#include "common.hlsl"	//必ずインクルード

//
//inは入力されてくるデーター
//outは出力するデータ
//
void main( in PS_IN In, out float4 outDiffuse : SV_Target )
{
	//1ピクセル分の色を決定して出力する


	//入力されたピクセルの色をそのまま出力
	outDiffuse = In.Diffuse;

}

//////////////////EOF//////////////////////