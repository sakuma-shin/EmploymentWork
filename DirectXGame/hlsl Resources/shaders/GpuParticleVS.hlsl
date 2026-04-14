cbuffer ViewProjection : register(b0) {
	matrix view;
	matrix projection;
	float3 cameraPos;
};

struct VSInput {
	float3 pos : POSITION; // vertex
	float2 uv : TEXCOORD;  // vertex

	// instance (slot 1)
	float4 instanceWorld0 : INSTANCEWORLD0;
	float4 instanceWorld1 : INSTANCEWORLD1;
	float4 instanceWorld2 : INSTANCEWORLD2;
	float4 instanceWorld3 : INSTANCEWORLD3;

	float4 instanceColor : INSTANCECOLOR;
	float4 instanceParam : INSTANCEPARAM; // x=billboard(1/0), y=size.x, z=size.y
};

struct VSOutput {
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};

VSOutput main(VSInput input) {
	VSOutput output;
	float billboard = input.instanceParam.x;
	float sizeX = input.instanceParam.y;
	float sizeY = input.instanceParam.z;

	if (billboard >= 0.5f) {
		// カメラ基底を view 行列から取り出す（ビューは行列なので右/up は逆行列にあたるが簡便法で行う）
		// view 行列が row_major で渡されている前提。ここでは view の逆回転成分を利用。
		// 右ベクトル： view の行0 の xyz の逆（簡便化）
		float3 right = float3(view._11, view._21, view._31);
		float3 up = float3(view._12, view._22, view._32);
		// translation from instanceWorld3 (row-major)
		float3 trans = float3(input.instanceWorld3.x, input.instanceWorld3.y, input.instanceWorld3.z);

		float3 worldPos = trans + right * (input.pos.x * sizeX) + up * (input.pos.y * sizeY);
		float4 viewPos = mul(float4(worldPos, 1.0f), view);
		output.svpos = mul(viewPos, projection);
	} else {
		// 普通のワールド変換（row-major）
		float4 local = float4(input.pos, 1.0f);
		float4 worldPos;
		worldPos.x = dot(input.instanceWorld0, local);
		worldPos.y = dot(input.instanceWorld1, local);
		worldPos.z = dot(input.instanceWorld2, local);
		worldPos.w = dot(input.instanceWorld3, local);
		float4 viewPos = mul(worldPos, view);
		output.svpos = mul(viewPos, projection);
	}

	output.uv = input.uv;
	output.color = input.instanceColor;
	return output;
}