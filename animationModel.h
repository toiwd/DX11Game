#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment(lib,"assimp.lib")



// �ό`�㒸�_�\����
struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D Normal;

	int			BoneNum;
	std::string	BoneName[4]; // �{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
	float		BoneWeight[4];
};

// �{�[���\����
struct BONE
{
	aiMatrix4x4	Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMarix;
};


class AnimationModel
{
private:
	const aiScene* m_AiScene = NULL;	// assimp�̃��f���f�[�^
	std::unordered_map<std::string, const aiScene*>m_Animation;

	ID3D11Buffer** m_VertexBuffer;		// ���_�o�b�t�@�̃|�C���^�̔z�񂪕K�v
	ID3D11Buffer** m_IndexBuffer;		// ���_�u���|�C���^

	// unordered_map �z��Ɠ����悤�ɂł��� �D���Ȍ^���w��ł���
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;
	

	std::vector<DEFORM_VERTEX>* m_DeformVertex;  // �ό`�㒸�_�f�[�^
	std::unordered_map<std::string, BONE>m_Bone; // �{�[���f�[�^�i���O�ŎQ�Ɓj

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);


public:
	void Load(const char *FileName);	// FBX�t�@�C����ǂݍ��ފ֐�
	void LoadAnimation(const char *FileName, const char*AnimationName);
	void Unload();
	void Update(const char *FileName, int Frame);
	void Draw();
};