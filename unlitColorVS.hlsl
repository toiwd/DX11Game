
//���_�V�F�[�_�[

#include "common.hlsl"	//�K���C���N���[�h

//
//in�͓��͂���Ă���f�[�^�[
//out�͏o�͂���f�[�^
//
void main( in VS_IN In, out PS_IN Out )
{

	//�P���_���̃f�[�^���󂯎��A�������āA�o�͂���

	//���_�ϊ����� �K���K�v
	matrix	wvp;						//�s��ϐ����쐬
	wvp = mul(World, View);				//wvp = ���[���h�s�񁖃J�����s��
//	wvp = World * View;
	wvp = mul(wvp, Projection);			//wvp = wvp *�v���W�F�N�V�����s��

	//���_���W���s��ŕϊ����ďo�́@�K���K�v
	Out.Position = mul( In.Position, wvp );


	//���_�̃f�t���[�Y�����̂܂܏o��
	Out.Diffuse = In.Diffuse;

}

/////////////EOF//////////////////////////////////













//Out.Position = In.Position * wvp;	//���������
//	Out.Position.x += 1.0f; //�V��ł݂悤

