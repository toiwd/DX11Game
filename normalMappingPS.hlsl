
#include "common.hlsl"

// �e�N�X�`���ϐ��̐錾�@�@���}�b�v�p�̕ϐ���ǉ��i�e�N�X�`���P�ԁj
Texture2D g_Texture		: register(t0);// ���Ƃ��Ƃ̃e�N�X�`���@�e�N�X�`��0��
Texture2D g_NormalTexture		: register(t1);// �@���}�b�v�p�̃e�N�X�`�� �e�N�X�`��1��

// �T���v���[�̐錾
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//���̃s�N�Z���̖@���}�b�v�̃f�[�^RGBA���擾
	float2 texcoord = In.TexCoord;
	texcoord.x += Time.x;
	float4 color = g_NormalTexture.Sample(g_SamplerState, texcoord);

	//�擾����RGBA���x�N�g�����XYZW�Ƃ��ēW�J����
	color = color * 2.0f - 1.0f;

	//�W�J�����e�N�X�`���f�[�^��@���p�ϐ��֓���ւ�
	float4 normal;
	normal.x = -color.r;
	normal.y = color.b;
	normal.z = color.g;
	normal.w = 0.0;
	normal = normalize(normal);

	//��������͂���܂Œʂ�̌����v�Z
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);


	float light = -dot(normal.xyz, Light.Direction.xyz);

	light = saturate(light);
	outDiffuse.rgb *= In.Diffuse.rgb * light;
	outDiffuse.a *= In.Diffuse.a;


	//�X�y�L�����i�t�H���j
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;	// �����x�N�g���쐬
	eyev = normalize(eyev);										// ���K��
	float3 halfv = eyev + Light.Direction.xyz;					// �n�[�t�x�N�g���쐬
	halfv = normalize(halfv);									// ���K��
	float	specular = -dot(halfv, normal.xyz);					// �����ƃn�[�t�x�N�g���̓���
	specular = saturate(specular);								// �l���T�`�����[�g


	specular = pow(specular, 30);								// 30��
	outDiffuse.rgb += specular;									// �X�y�L�����l�����Z����
}

