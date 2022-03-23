// �Ȃ�
#include "common.hlsl" // �K���C���N���[�h

// DirectX�̃e�N�X�`���̐ݒ���󂯌p��
Texture2D g_Texture : register(t0); // �e�N�X�`���[0��
// DirectX�̃T���v���[�X�e�[�g�ݒ���󂯌p��
SamplerState g_SamplerState : register(s0); // �e�N�X�`���[�T���v���[0��


//
//in �͓��͂���Ă���f�[�^
//out �͏o�͂���f�[�^
//Texture X650 Y650
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4		normal = normalize(In.Normal);	// �s�N�Z���̖@���𐳋K��
	float		light = -dot(normal.xyz, Light.Direction.xyz);	// ���邳���v�Z

	// ���̃s�N�Z���Ɏg����e�N�X�`���̐F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= (light * In.Diffuse.rgb);	// �e�N�X�`�������邳�����_�F
	outDiffuse.a *= In.Diffuse.a;				// ���_�F�̃������̂܂܎g��

	// �X�y�L�����[�i�t�H�����ʔ��ˁj
	//float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;	// �����x�N�g��
	//eyev = normalize(eyev);										// ���K������
	//float3 refv = reflect(Light.Direction.xyz, normal.xyz);		// ���̔��˃x�N�g��
	//refv = normalize(refv);										// ���K������
	//float	specular = -dot(eyev, refv);						// �����Ɣ��˃x�N�g���̓���
	//specular = saturate(specular);								// �l���T�`�����[�g


	// �X�y�L�����[�i�u�����t�H���j
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;	// �����x�N�g���쐬
	eyev = normalize(eyev);										// ���K��
	float3 halfv = eyev + Light.Direction.xyz;					// �n�[�t�x�N�g���쐬
	halfv = normalize(halfv);									// ���K��
	float	specular = -dot(halfv, normal.xyz);					// �����ƃn�[�t�x�N�g���̓���
	specular = saturate(specular);								// �l���T�`�����[�g


	specular = pow(specular, 30);								// 30��
	outDiffuse.rgb += specular;									// �X�y�L�����l�����Z����


	// �������C�g
	float rim = dot(eyev, normal.xyz) + 1.0f;					// ���p����1.0�ɂȂ�
	rim = saturate(rim);										// �T�`�����[�g
	rim = pow(rim, 3) * 2.0f;									// ���������Œl�����߂�

	outDiffuse.rgb += rim;										// �������C�g�����Z����
}