struct VSOut
{
	float3 color : Color;
	float4 pos : SV_Position;
};

cbuffer CBuf
{
	// 在GPU中，会将行向量和矩阵相乘，意味着GPU想要的是列向量的矩阵，但是我们在Graphics.cpp中定义的是行主序的，这里加上row_major可以告诉hlsl
	// 正确的列向量。
	// 注意： 列主序的矩阵会更快，后面会进行优化，这里只是简单的告诉hlsl，避免使用错误的列向量。
	row_major matrix transform;
};

VSOut main( float2 pos : Position,float3 color : Color )
{
	VSOut vso;
	vso.pos = mul( float4(pos.x,pos.y,0.0f,1.0f),transform );
	vso.color = color;
	return vso;
}