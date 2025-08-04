#include"Test.hlsli"

Texture2D<float32_t4> gTexture : register(t0); //SRV register=>t
SamplerState gSampler : register(s0); //Sampler register=>s

struct PixelShaderOutPut
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutPut main(VertexShaderOutput input)
{
    PixelShaderOutPut output;
    
    float32_t2 uv = input.texcoord;
    
    float32_t4 textureColor = gTexture.Sample(gSampler, uv);
    output.color = textureColor;
    
    return output;
}