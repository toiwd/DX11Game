#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	// 1���_���̃f�[�^���󂯎��A�������āA�o�͂���
	// ���_�ϊ����� ���̏����͕K���K�v
	matrix	wvp;							// �s��ϐ����쐬
	wvp = mul(World, View);					// wvp = ���[���h�s�� * �J�����s��
	wvp = mul(wvp, Projection);				// wvp = wvp * �v���W�F�N�V�����s��
	Out.Position = mul(In.Position, wvp);	// �ϊ����ʂ��o�͂���

	// �@�������[���h�ϊ����ďo��
	float4		worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);	// normal.w��0�ɂ���(���s�ړ��͂��Ȃ�)
	worldNormal = mul(normal, World);		// ���[���h�ϊ�
	worldNormal = normalize(worldNormal);	// ��]��̖@���𐳋K������
	Out.Normal = worldNormal;				// ��]��̖@���o�� In.Normal�łȂ���]��̖@�����o��

	// ���邳�̌v�Z
	// ���x�N�g���Ɩ@���̓���XYZ�v�f�݂̂Ōv�Z
	//float light = -dot(Light.Direction.xyz, worldNormal);
	//light = saturate(light);

	// �󂯎�������̒��_�̃f�t���[�Y�����̂܂܏o��
	Out.Diffuse = In.Diffuse;

	// ���_�̃e�N�X�`�����W���o��
	Out.TexCoord = In.TexCoord;

	Out.WorldPosition = mul(In.Position, World);	// ���[���h�ϊ��������_���W���o��
}