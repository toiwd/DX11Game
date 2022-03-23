#pragma once




// 頂点構造体
struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};


// マテリアル構造体
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];
};


// ライト構造体
struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;
	D3DXVECTOR4	Position;
	D3DXVECTOR4	Angle;
};


// 描画関係のクラス
class Renderer
{
private:

	// メンバ変数またはプロパティ、フィールド
	// m_でメンバ変数
	// static
	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*			m_CameraBuffer;
	static ID3D11Buffer*			m_TimeBuffer;


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;




public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	// セッター←値をセットする関数
	static void SetDepthEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetCameraPosition(D3DXVECTOR3 CameraPosition);
	static void SetTime(D3DXVECTOR4 Time);

	// ゲッター←値を取得する関数
	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }



	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);


};
