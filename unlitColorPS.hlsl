
//�s�N�Z���V�F�[�_�[

#include "common.hlsl"	//�K���C���N���[�h

//
//in�͓��͂���Ă���f�[�^�[
//out�͏o�͂���f�[�^
//
void main( in PS_IN In, out float4 outDiffuse : SV_Target )
{
	//1�s�N�Z�����̐F�����肵�ďo�͂���


	//���͂��ꂽ�s�N�Z���̐F�����̂܂܏o��
	outDiffuse = In.Diffuse;

}

//////////////////EOF//////////////////////