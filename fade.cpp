#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "title.h"
#include "game.h"
#include "result.h"

#define	FADE_RATE	(0.02f)		// フェード係数（明るくまたは暗くなる速度）

ID3D11VertexShader*			Fade::m_VertexShader = NULL;
ID3D11PixelShader*			Fade::m_PixelShader = NULL;
ID3D11InputLayout*			Fade::m_VertexLayout = NULL;

ID3D11Buffer*				Fade::m_VertexBuffer = NULL;
ID3D11ShaderResourceView*	Fade::m_Texture = NULL;

int		Fade::m_Fade = 0;
int		Fade::m_NextScene = 0;
float	Fade::m_FadeLight = 0.0f;

void Fade::Init()
{
	VERTEX_3D vertex[4];

	//左上
	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//描画座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);	//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	//右上
	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	//左下
	vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	//右下
	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;	//サイズの設定
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//VERTEX_BUFFERという説明
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};//初期値の設定
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/fadeblack.png",//ファイル指定
		NULL,
		NULL,
		&m_Texture,					//出来上がったテクスチャ
		NULL);
	assert(m_Texture);	//エラーチェック(NULLだと例外スロー)

	//コンパイル済みのシェーダープログラムを読み込んで、VertexShaderに入れ込む
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,"unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}


void Fade::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Fade::Update()
{
	if (m_Fade == 1) // フェードアウト
	{
		// ここで暗くする
		m_FadeLight += FADE_RATE;

		// 頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//左上
		vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//描画座標
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);	//色
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

		//右上
		vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//左下
		vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//右下
		vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}
	else if (m_Fade == 2) // フェードイン
	{
		// ここで明るくする
		m_FadeLight -= FADE_RATE;

		// 頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//左上
		vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//描画座標
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);	//色
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

		//右上
		vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//左下
		vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//右下
		vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_FadeLight);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		// 完全に明るくなったら処理を止める
		if (m_FadeLight < 0.0f)
		{
			m_Fade = 0;
		}
	}

	// 完全に暗くなったら次のシーンへ移行
	if (m_FadeLight > 1.0f)
	{
		switch (m_NextScene)
		{
		case 1:
			Manager::SetScene<Title>();
			m_Fade = 2;
			break;
		case 2:
			Manager::SetScene<Game>();
			m_Fade = 2;
			break;
		case 3:
			Manager::SetScene<Result>();
			m_Fade = 2;
			break;
		default:
			break;
		}
	}
}

void Fade::Draw()
{
	// フェードが終わっていたら描画しない
	if (m_Fade == 0) return

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);	//FVFの代わり

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);	//マテリアルによってシェーダが変わる
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();	//2D用のマトリクス

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);//頂点数
}
