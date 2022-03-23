#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "mouse.h"
#include "option.h"
#include "optionFrame.h"


ID3D11VertexShader*			OptionFrame::m_VertexShader = NULL;
ID3D11PixelShader*			OptionFrame::m_PixelShader = NULL;
ID3D11InputLayout*			OptionFrame::m_VertexLayout = NULL;

ID3D11Buffer*				OptionFrame::m_VertexBuffer = NULL;
ID3D11ShaderResourceView*	OptionFrame::m_Texture = NULL;

int							OptionFrame::m_SelectFrame = 0;

Mouse_State ofMouse;

void OptionFrame::Init()
{
	VERTEX_3D vertex[4];

	//左上
	vertex[0].Position = D3DXVECTOR3(30.0f, 345.0f, 0.0f);		//描画座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	//右上
	vertex[1].Position = D3DXVECTOR3(310.0f, 345.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	//左下
	vertex[2].Position = D3DXVECTOR3(30.0f, 525.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	//右下
	vertex[3].Position = D3DXVECTOR3(310.0f, 525.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
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
		"asset/texture/optionframe.png",//ファイル指定
		NULL,
		NULL,
		&m_Texture,					//出来上がったテクスチャ
		NULL);
	assert(m_Texture);	//エラーチェック(NULLだと例外スロー)

	//コンパイル済みのシェーダープログラムを読み込んで、VertexShaderに入れ込む
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,"unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}


void OptionFrame::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void OptionFrame::Update()
{
	// マウスの現在の状態を保持
	Mouse_GetState(&ofMouse);

	if (30.0f <= (float)ofMouse.x && (float)ofMouse.x <= 310.0f &&
		345.0f <= (float)ofMouse.y && (float)ofMouse.y <= 525.0f)
	{
		// 頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//左上
		vertex[0].Position = D3DXVECTOR3(30.0f, 345.0f, 0.0f);		//描画座標
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//色
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

		//右上
		vertex[1].Position = D3DXVECTOR3(310.0f, 345.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//左下
		vertex[2].Position = D3DXVECTOR3(30.0f, 525.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//右下
		vertex[3].Position = D3DXVECTOR3(310.0f, 525.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
		
		// タイトルへ戻る
		m_SelectFrame = 1;
	}
	else if (30.0f + 310.0f <= (float)ofMouse.x && (float)ofMouse.x <= 310.0f + 310.0f && 345.0f <= (float)ofMouse.y && (float)ofMouse.y <= 525.0f)
	{
		// 頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//左上
		vertex[0].Position = D3DXVECTOR3(30.0f+310.0f, 345.0f, 0.0f);	//描画座標
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);		//色
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);					//テクスチャ座標

		//右上
		vertex[1].Position = D3DXVECTOR3(310.0f + 310.0f, 345.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//左下
		vertex[2].Position = D3DXVECTOR3(30.0f + 310.0f, 525.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//右下
		vertex[3].Position = D3DXVECTOR3(310.0f + 310.0f, 525.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		// ゲームをリセットする
		m_SelectFrame = 2;
	}
	else if (30.0f + 620.0f <= (float)ofMouse.x && (float)ofMouse.x <= 310.0f + 620.0f && 345.0f <= (float)ofMouse.y && (float)ofMouse.y <= 525.0f)
	{
		// 頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//左上
		vertex[0].Position = D3DXVECTOR3(30.0f + 620.0f, 345.0f, 0.0f);			//描画座標
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//色
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

		//右上
		vertex[1].Position = D3DXVECTOR3(310.0f + 620.0f, 345.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//左下
		vertex[2].Position = D3DXVECTOR3(30.0f + 620.0f, 525.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//右下
		vertex[3].Position = D3DXVECTOR3(310.0f + 620.0f, 525.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		// ゲームに戻る
		m_SelectFrame = 3;
	}
	else
	{
		m_SelectFrame = 0;
	}

	if (Input::GetKeyTrigger(VK_LBUTTON))
	{
		switch (m_SelectFrame)
		{
		case 1:	// タイトルに戻る
			Fade::SetFade(1, 1);
			Option::SetOptionMode(false);
			break;
		case 2: // ゲームをリセットする
			Fade::SetFade(1, 2);
			Option::SetOptionMode(false);
			break;
		case 3: // ゲームに戻る
			Option::SetOptionMode(false);
			break;
		default:
			break;
		}
	}
	
}

void OptionFrame::Draw()
{
	// オプション状態でなければ描画しない
	if (m_SelectFrame == 0) return;

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
