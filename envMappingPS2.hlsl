#include "common.hlsl" // �K���C���N���[�h

// DirectX�̃e�N�X�`���̐ݒ���󂯌p��
Texture2D g_Texture : register(t0); // �e�N�X�`���[0��
Texture2D g_TextureEnv : register(t1); // �e�N�X�`���[1��
// DirectX�̃T���v���[�X�e�[�g�ݒ���󂯌p��
SamplerState g_SamplerState : register(s0); // �e�N�X�`���[�T���v���[0��


//
//Texture X650 Y650
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal);//�@��

	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;//�����x�N�g��
	eyev = normalize(eyev);

	float3 refv = reflect(eyev, normal.xyz); //���˃x�N�g��
	refv = normalize(refv);

	//���}�b�s���O
	float2 envTexCoord;//	-1.0�`1.0 => 0.0�`1.0
	envTexCoord.x = -refv.x * 0.5 + 0.5;
	envTexCoord.y = -refv.y * 0.5 + 0.5;



	//�e�N�X�`���̐F���o��
	outDiffuse.rgb = g_TextureEnv.SampleBias(g_SamplerState, envTexCoord, 1.0).rgb;
	outDiffuse += g_Texture.Sample(g_SamplerState, In.TexCoord);//�F���Â��Ȃ�̂łƂ��Ă���
	outDiffuse.a = 1.0f;

	/*
	float4		normal = normalize(In.Normal);	// �s�N�Z���̖@���𐳋K��

	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;	// �����x�N�g��
	eyev = normalize(eyev);										// ���K������

	float3 refv = reflect(Light.Direction.xyz, normal.xyz);		// ���̔��˃x�N�g��
	refv = normalize(refv);

	float2 envTexCoord;											// ���˃x�N�g��������}�b�s���O�p�̃e�N�X�`�����W���쐬
	envTexCoord.x = -refv.x * 0.5f + 0.5f;							// ��������Ȃ��I�I
	envTexCoord.y = -refv.y * 0.5f + 0.5f;

	// �e�N�X�`���o�C�A�X�w��t���ł̃e�N�X�`���̎擾
	float4 EnvTex = g_TextureEnv.SampleBias(g_SamplerState, envTexCoord, 0.0f);
	float4 BaseTex = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb = (1.0f * EnvTex.rgb) + (1.0f * BaseTex.rgb);	// �e�N�X�`�������邳�����_�F

	outDiffuse.a = BaseTex.a * In.Diffuse.a;				// �x�[�X�̃��ƈ����s�N�Z���̃�����Z�i�����������j


	float light = -dot(normal.xyz, Light.Direction.xyz);	// ���邳���v�Z
	*/





}