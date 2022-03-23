#include "main.h"
#include "renderer.h"
#include "animationModel.h"


void AnimationModel::Load(const char *FileName)
{
	const std::string modelPath(FileName);

	// assimpのメソッド
	m_AiScene = aiImportFile(FileName,
		aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];

	// 変形後頂点配列生成
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

	// 再帰的にボーン生成
	CreateBone(m_AiScene->mRootNode);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// 頂点バッファ生成
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;	// 頂点バッファを動かす際はDYNAMIC
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // 頂点バッファを動かす際はWRITE

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

			delete[] vertex;	// &m_VertexBuffer[m]に入っているので消す
		}

		// インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;	// インデックスバッファはDEFAULT
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3; // インデックスは3頂点
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER; // 頂点バッファと違うところ
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

			delete[] index;
		}

		// 変形後頂点データ初期化
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX deformVertex;
			deformVertex.Position = mesh->mVertices[v];
			deformVertex.Normal = mesh->mNormals[v];
			deformVertex.BoneNum = 0;

			for (unsigned int b = 0; b < 4; b++)
			{
				deformVertex.BoneName[b] = "";
				deformVertex.BoneWeight[b] = 0.0f;
			}

			m_DeformVertex[m].push_back(deformVertex);
		}

		// ボーンデータ初期化
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_Bone[bone->mName.C_Str()].OffsetMarix = bone->mOffsetMatrix;

			// 変形後頂点にボーンデータ格納
			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];

				int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

				m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
				m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
				m_DeformVertex[m][weight.mVertexId].BoneNum++;

				assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);
			}
		}
	}

	// テクスチャ読み込み
	{
		for (unsigned int m = 0; m < m_AiScene->mNumMaterials; m++)
		{
			aiString path;

			if (m_AiScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				if (path.data[0] == '*')
				{
					//FBXファイル内から読み込み
					if (m_Texture[path.data] == NULL)
					{
						ID3D11ShaderResourceView* texture;
						int id = atoi(&path.data[1]);

						D3DX11CreateShaderResourceViewFromMemory(
							Renderer::GetDevice(),
							(const unsigned char*)m_AiScene->mTextures[id]->pcData,
							m_AiScene->mTextures[id]->mWidth,
							NULL, NULL, &texture, NULL);

						m_Texture[path.data] = texture;
					}
				}
				else
				{
					//外部ファイルから読み込み
				}
			}
			else
			{
				m_Texture[path.data] = NULL;
			}
		}
	}
}

void AnimationModel::LoadAnimation(const char *FileName, const char *AnimationName)
{
	m_Animation[AnimationName] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[AnimationName]);
}

void AnimationModel::CreateBone(aiNode* node)
{
	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

void AnimationModel::Unload()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	// unordererd_mapのfor文を回すためにはpairが必要
	for (std::pair<std::string, ID3D11ShaderResourceView*>pair : m_Texture)
	{
		pair.second->Release();
	}

	aiReleaseImport(m_AiScene);

	for (std::pair<const std::string, const aiScene*>pair : m_Animation)
	{
		aiReleaseImport(pair.second);
	}
}

void AnimationModel::Update(const char *AnimationName, int Frame)
{
	if(!m_Animation[AnimationName]->HasAnimations())
	return;

	// アニメーションデータからボーンマトリクス算出
	aiAnimation* animation = m_Animation[AnimationName]->mAnimations[0];

	for (unsigned int c = 0; c < animation->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation->mChannels[c];
		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;
		f = Frame % nodeAnim->mNumRotationKeys; // 簡易実装
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame % nodeAnim->mNumPositionKeys; // 簡易実装
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	// 再帰的にボーンマトリクスを更新
	UpdateBoneMatrix(m_AiScene->mRootNode, aiMatrix4x4());

	// 頂点変換（CPUスキニング）
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		D3D11_MAPPED_SUBRESOURCE ms;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0, 
			D3D11_MAP_WRITE_DISCARD, 0, &ms);

		VERTEX_3D *vertex = (VERTEX_3D*)ms.pData;

		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX *deformVertex = &m_DeformVertex[m][v];

			aiMatrix4x4 matrix[4];
			aiMatrix4x4 outMatrix;
			matrix[0] = m_Bone[deformVertex->BoneName[0]].Matrix;
			matrix[1] = m_Bone[deformVertex->BoneName[1]].Matrix;
			matrix[2] = m_Bone[deformVertex->BoneName[2]].Matrix;
			matrix[3] = m_Bone[deformVertex->BoneName[3]].Matrix;

			{
				outMatrix.a1 = matrix[0].a1*deformVertex->BoneWeight[0]
					+ matrix[1].a1*deformVertex->BoneWeight[1]
					+ matrix[2].a1*deformVertex->BoneWeight[2]
					+ matrix[3].a1*deformVertex->BoneWeight[3];

				outMatrix.a2 = matrix[0].a2*deformVertex->BoneWeight[0]
					+ matrix[1].a2*deformVertex->BoneWeight[1]
					+ matrix[2].a2*deformVertex->BoneWeight[2]
					+ matrix[3].a2*deformVertex->BoneWeight[3];

				outMatrix.a3 = matrix[0].a3*deformVertex->BoneWeight[0]
					+ matrix[1].a3*deformVertex->BoneWeight[1]
					+ matrix[2].a3*deformVertex->BoneWeight[2]
					+ matrix[3].a3*deformVertex->BoneWeight[3];

				outMatrix.a4 = matrix[0].a4*deformVertex->BoneWeight[0]
					+ matrix[1].a4*deformVertex->BoneWeight[1]
					+ matrix[2].a4*deformVertex->BoneWeight[2]
					+ matrix[3].a4*deformVertex->BoneWeight[3];


				outMatrix.b1 = matrix[0].b1*deformVertex->BoneWeight[0]
					+ matrix[1].b1*deformVertex->BoneWeight[1]
					+ matrix[2].b1*deformVertex->BoneWeight[2]
					+ matrix[3].b1*deformVertex->BoneWeight[3];

				outMatrix.b2 = matrix[0].b2*deformVertex->BoneWeight[0]
					+ matrix[1].b2*deformVertex->BoneWeight[1]
					+ matrix[2].b2*deformVertex->BoneWeight[2]
					+ matrix[3].b2*deformVertex->BoneWeight[3];

				outMatrix.b3 = matrix[0].b3*deformVertex->BoneWeight[0]
					+ matrix[1].b3*deformVertex->BoneWeight[1]
					+ matrix[2].b3*deformVertex->BoneWeight[2]
					+ matrix[3].b3*deformVertex->BoneWeight[3];

				outMatrix.b4 = matrix[0].b4*deformVertex->BoneWeight[0]
					+ matrix[1].b4*deformVertex->BoneWeight[1]
					+ matrix[2].b4*deformVertex->BoneWeight[2]
					+ matrix[3].b4*deformVertex->BoneWeight[3];


				outMatrix.c1 = matrix[0].c1*deformVertex->BoneWeight[0]
					+ matrix[1].c1*deformVertex->BoneWeight[1]
					+ matrix[2].c1*deformVertex->BoneWeight[2]
					+ matrix[3].c1*deformVertex->BoneWeight[3];

				outMatrix.c2 = matrix[0].c2*deformVertex->BoneWeight[0]
					+ matrix[1].c2*deformVertex->BoneWeight[1]
					+ matrix[2].c2*deformVertex->BoneWeight[2]
					+ matrix[3].c2*deformVertex->BoneWeight[3];

				outMatrix.c3 = matrix[0].c3*deformVertex->BoneWeight[0]
					+ matrix[1].c3*deformVertex->BoneWeight[1]
					+ matrix[2].c3*deformVertex->BoneWeight[2]
					+ matrix[3].c3*deformVertex->BoneWeight[3];

				outMatrix.c4 = matrix[0].c4*deformVertex->BoneWeight[0]
					+ matrix[1].c4*deformVertex->BoneWeight[1]
					+ matrix[2].c4*deformVertex->BoneWeight[2]
					+ matrix[3].c4*deformVertex->BoneWeight[3];


				outMatrix.d1 = matrix[0].d1*deformVertex->BoneWeight[0]
					+ matrix[1].d1*deformVertex->BoneWeight[1]
					+ matrix[2].d1*deformVertex->BoneWeight[2]
					+ matrix[3].d1*deformVertex->BoneWeight[3];

				outMatrix.d2 = matrix[0].d2*deformVertex->BoneWeight[0]
					+ matrix[1].d2*deformVertex->BoneWeight[1]
					+ matrix[2].d2*deformVertex->BoneWeight[2]
					+ matrix[3].d2*deformVertex->BoneWeight[3];

				outMatrix.d3 = matrix[0].d3*deformVertex->BoneWeight[0]
					+ matrix[1].d3*deformVertex->BoneWeight[1]
					+ matrix[2].d3*deformVertex->BoneWeight[2]
					+ matrix[3].d3*deformVertex->BoneWeight[3];

				outMatrix.d4 = matrix[0].d4*deformVertex->BoneWeight[0]
					+ matrix[1].d4*deformVertex->BoneWeight[1]
					+ matrix[2].d4*deformVertex->BoneWeight[2]
					+ matrix[3].d4*deformVertex->BoneWeight[3];
			}

			deformVertex->Position = mesh->mVertices[v];
			deformVertex->Position *= outMatrix;

			// 法線変換用に移動成分を削除
			outMatrix.a4 = 0.0f;
			outMatrix.b4 = 0.0f;
			outMatrix.c4 = 0.0f;

			deformVertex->Normal = mesh->mNormals[v];
			deformVertex->Normal *= outMatrix;

			// 頂点バッファへ書き込み
			vertex[v].Position.x = deformVertex->Position.x;
			vertex[v].Position.y = deformVertex->Position.y;
			vertex[v].Position.z = deformVertex->Position.z;

			vertex[v].Normal.x = deformVertex->Normal.x;
			vertex[v].Normal.y = deformVertex->Normal.y;
			vertex[v].Normal.z = deformVertex->Normal.z;

			vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
			vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

			vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);

	}

}

void AnimationModel::Draw()
{
	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		aiMaterial* material = m_AiScene->mMaterials[mesh->mMaterialIndex];

		// テクスチャ設定
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[path.data]);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1,
			&m_VertexBuffer[m], &stride, &offset);

		// インデックスバッファ設定
		Renderer::GetDeviceContext()->IASetIndexBuffer(
			m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// ポリゴン描画
		Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void AnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &m_Bone[node->mName.C_Str()];

	// マトリクスの乗算順番に注意
	aiMatrix4x4 worldMatrix;

	worldMatrix = matrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMarix;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}