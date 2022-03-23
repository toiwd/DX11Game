#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment(lib,"assimp.lib")



// 変形後頂点構造体
struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D Normal;

	int			BoneNum;
	std::string	BoneName[4]; // 本来はボーンインデックスで管理するべき
	float		BoneWeight[4];
};

// ボーン構造体
struct BONE
{
	aiMatrix4x4	Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMarix;
};


class AnimationModel
{
private:
	const aiScene* m_AiScene = NULL;	// assimpのモデルデータ
	std::unordered_map<std::string, const aiScene*>m_Animation;

	ID3D11Buffer** m_VertexBuffer;		// 頂点バッファのポインタの配列が必要
	ID3D11Buffer** m_IndexBuffer;		// ↑ダブルポインタ

	// unordered_map 配列と同じようにできる 好きな型を指定できる
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;
	

	std::vector<DEFORM_VERTEX>* m_DeformVertex;  // 変形後頂点データ
	std::unordered_map<std::string, BONE>m_Bone; // ボーンデータ（名前で参照）

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);


public:
	void Load(const char *FileName);	// FBXファイルを読み込む関数
	void LoadAnimation(const char *FileName, const char*AnimationName);
	void Unload();
	void Update(const char *FileName, int Frame);
	void Draw();
};