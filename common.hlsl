


cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}




struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	float3 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}




struct LIGHT
{
	bool Enable;
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
	float4 Position;
	float4 Angle;
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}

cbuffer CameraBuffer : register(b5) // �o�b�t�@��5�ԂƂ���
{
	float4 CameraPosition;	// �J�������W���󂯎��ϐ�
}

cbuffer TimeBuffer : register(b6)
{
	float4	Time;
}



struct VS_IN
{
	float4 Position			: POSITION0;
	float4 Normal			: NORMAL0;
	float4 Diffuse			: COLOR0;
	float2 TexCoord			: TEXCOORD0;
};


struct PS_IN
{
	float4 Position			: SV_POSITION;
	float4 WorldPosition	: POSITION0;
	float4 Normal			: NORMAL0;
	float4 Diffuse			: COLOR0;
	float2 TexCoord			: TEXCOORD0;
};
