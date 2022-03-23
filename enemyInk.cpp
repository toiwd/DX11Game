#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "enemyInk.h"
#include "camera.h"

ID3D11Buffer*				EnemyInk::m_VertexBuffer = NULL;
ID3D11ShaderResourceView*	EnemyInk::m_Texture = NULL;
ID3D11ShaderResourceView*	EnemyInk::m_NormalTexture = NULL;

void EnemyInk::Load()
{
	VERTEX_3D vertex[4];

	//左上
	vertex[0].Position = D3DXVECTOR3(-0.75f, 0.01f, 0.75f);			//描画座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//ライティング
	vertex[0].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);	//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	//右上
	vertex[1].Position = D3DXVECTOR3(0.75f, 0.01f, 0.75f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	//左下
	vertex[2].Position = D3DXVECTOR3(-0.75f, 0.01f, -0.75f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	//右下
	vertex[3].Position = D3DXVECTOR3(0.75f, 0.01f, -0.75f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;//テクスチャアニメーションでは動的のDYNAMICにする
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;	//サイズの設定
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//VERTEX_BUFFERという説明
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//テクスチャアニメーションでは書き換え可能のWRITEにする

	D3D11_SUBRESOURCE_DATA sd{};//初期値の設定
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/ink.png",//ファイル指定
		NULL,
		NULL,
		&m_Texture,					//出来上がったテクスチャ
		NULL);
	assert(m_Texture);	//エラーチェック(NULLだと例外スロー)

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/normalmap.png", // 法線マップテクスチャ
		NULL,
		NULL,
		&m_NormalTexture,	// 法線マップ用インターフェース
		NULL);

	assert(m_NormalTexture);
}

void EnemyInk::Unload()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
	m_NormalTexture->Release();
}

void EnemyInk::Init()
{
	//コンパイル済みのシェーダープログラムを読み込んで、VertexShaderに入れ込む
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "pixelLightingBlinnVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "pixelLightingBlinnPS.cso");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Time.x = 0.0f;
}

void EnemyInk::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void EnemyInk::Update()
{
	m_Time.x += 0.001f;
}

void EnemyInk::Draw()
{
	Renderer::SetTime(m_Time);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);	//FVFの代わり

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);	//マテリアルによってシェーダが変わる
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_NormalTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);//頂点数
}

void EnemyInk::Paint(D3DXVECTOR3 position)
{
	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//左上
	vertex[0].Position = D3DXVECTOR3(position.x - 1.0f, 0.01f, position.z + 1.0f);			//描画座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//ライティング
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);	//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	//右上
	vertex[1].Position = D3DXVECTOR3(position.x + 1.0f, 0.01f, position.z + 1.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	//左下
	vertex[2].Position = D3DXVECTOR3(position.x - 1.0f, 0.01f, position.z - 1.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	//右下
	vertex[3].Position = D3DXVECTOR3(position.x + 1.0f, 0.01f, position.z - 1.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}