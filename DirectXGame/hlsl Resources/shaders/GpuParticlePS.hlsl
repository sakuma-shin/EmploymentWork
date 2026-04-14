struct PSInput {
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};

float4 main(PSInput input) : SV_TARGET {
	// 現在は単色。テクスチャを使う場合はここで Sample する実装へ拡張
	return input.color;
}