#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "mouse.h"
#include "option.h"
#include "optionbar.h"
#include "player.h"
#include "Audio.h"

float	OptionBar::m_BarPointBGM = 600.0f;
float	OptionBar::m_BarPointSE = 600.0f;
bool	OptionBar::m_PointChange = false;

Mouse_State obMouse;

void OptionBar::Init(int Num)
{
	VERTEX_3D vertex[4];

	m_Position.x = 237.5f + 600.0f;
	// 2のときだけ真ん中にセットする
	if (Num == 2)
	{
		m_Position.x = 237.5f + 300.0f;
	}
	m_Position.y = 75.0f + Num * 100.0f;

	//左上
	vertex[0].Position = D3DXVECTOR3(0.0f + m_Position.x, 0.0f + m_Position.y, 0.0f);			//描画座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	//右上
	vertex[1].Position = D3DXVECTOR3(25.0f + m_Position.x, 0.0f + m_Position.y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	//左下
	vertex[2].Position = D3DXVECTOR3(0.0f + m_Position.x, 50.0f + m_Position.y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	//右下
	vertex[3].Position = D3DXVECTOR3(25.0f + m_Position.x, 50.0f + m_Position.y, 0.0f);
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
		"asset/texture/optionbar.png",//ファイル指定
		NULL,
		NULL,
		&m_Texture,					//出来上がったテクスチャ
		NULL);
	assert(m_Texture);	//エラーチェック(NULLだと例外スロー)

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/optionbar2.png",//ファイル指定
		NULL,
		NULL,
		&m_Texture2,					//出来上がったテクスチャ
		NULL);
	assert(m_Texture2);	//エラーチェック(NULLだと例外スロー)


	//DirectXの関数ではない
	//コンパイル済みのシェーダープログラムを読み込んで、VertexShaderに入れ込む
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"unlitTextureVS.cso");
	//DirectXの関数ではない
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	//unlitとは→光の計算をしないという意味
}


void OptionBar::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void OptionBar::Update(int Num)
{
	// マウスの現在の状態を保持
	Mouse_GetState(&obMouse);

	// テクスチャ上にマウスがあるか
	if (m_Position.x - 10.0f <= obMouse.x && m_Position.x + 30.0f >= obMouse.x && m_Position.y-5.0f <= obMouse.y && m_Position.y + 55.0f >= obMouse.y)
	{
		// 既に他のバーが掴まれていない場合
		if (!Input::GetKeyPress(VK_LBUTTON))
		{
			m_OnMouse = true;
		}
		
		if (Input::GetKeyTrigger(VK_LBUTTON))
		{
			m_HoldMouse = true;
		}
	}
	else
	{
		m_OnMouse = false;
	}

	// 掴んだバーを離した時
	if (!Input::GetKeyPress(VK_LBUTTON))
	{
		m_HoldMouse = false;
		
	}

	// 掴んでいたら動かせるようにする
	if (m_HoldMouse == true)
	{
		m_Position.x = obMouse.x;
		if (m_Position.x >= 600.0f + 237.5f)
		{
			m_Position.x = 600.0f + 237.5f;
		}
		else if (m_Position.x <= 237.5f)
		{
			m_Position.x = 237.5f;
			// 0にならないようにする
			if (Num == 2)
			{
				m_Position.x = 238.5f;
			}
		}

		// 頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//左上
		vertex[0].Position = D3DXVECTOR3(0.0f + m_Position.x, 0.0f + m_Position.y, 0.0f);			//描画座標
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//色
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

		//右上
		vertex[1].Position = D3DXVECTOR3(25.0f + m_Position.x, 0.0f + m_Position.y, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//左下
		vertex[2].Position = D3DXVECTOR3(0.0f + m_Position.x, 50.0f + m_Position.y, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//右下
		vertex[3].Position = D3DXVECTOR3(25.0f + m_Position.x, 50.0f + m_Position.y, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);



		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		m_PointChange = true;
	}

	// バーの位置に応じた数値
	m_BarPoint = m_Position.x - 237.5f;

	// Numの数値に応じた変更
	if (Num == 0)
	{
		m_BarPointBGM = m_Position.x - 237.5f;
	}
	else if (Num == 1)
	{
		m_BarPointSE = m_Position.x - 237.5f;
	}
	else
	{
		Player::SetPlayerSens(m_BarPoint);
	}
}

void OptionBar::Draw()
{
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

	if (m_OnMouse == true || m_HoldMouse == true)
	{
		// テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	}
	else
	{
		// テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture2);
	}
	
	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);//頂点数
}
