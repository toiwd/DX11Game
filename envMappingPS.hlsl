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
	float4 normal = normalize(In.Normal);

	//�����o�[�g�g�U�Ɩ�
	float light = -dot(normal.xyz, Light.Direction.xyz);
	light = saturate(light);

	//�ʏ�̃e�N�X�`���̐F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb * light;
	outDiffuse.a *= In.Diffuse.a;


	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;//�����x�N�g��
	eyev = normalize(eyev);

	float3 refv = reflect(eyev, normal.xyz); //�����Ɩ@���Ƃ̔��˃x�N�g��
	refv = normalize(refv);



	//���}�b�s���O
	float2 envTexCoord;
	envTexCoord.x = -refv.x * 0.3 + 0.5;	//���˃x�N�g�����e�N�X�`�����W�֕ϊ�
	envTexCoord.y = -refv.y * 0.3 + 0.5;


	//outDiffuse.rgb = g_TextureEnv.Sample(g_SamplerState, envTexCoord).rgb;
	outDiffuse.rgb = g_TextureEnv.SampleBias(g_SamplerState, envTexCoord, 0.0);//�o�C�A�X�t��
	
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