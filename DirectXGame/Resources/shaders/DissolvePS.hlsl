#include"Test.hlsli"

struct Threshold
{
    float threshold;
};

struct randomTime
{
    float32_t time;
};


Texture2D<float32_t4> gTexture : register(t0); //SRV register=>t
SamplerState gSampler : register(s0); //Sampler register=>s

Texture2D<float32_t4> gMaskTexture : register(t1); //SRV register=>t


ConstantBuffer<randomTime> gRandomTime : register(b1);

ConstantBuffer<Threshold>gThreshold : register(b2);

struct PixelShaderOutPut
{
    float32_t4 color : SV_TARGET0;
};

float32_t rand(float32_t2 uv)
{
    return frac(sin(dot(uv, float32_t2(12.9898, 78.233))) * 43758.5453);
}

PixelShaderOutPut main(VertexShaderOutput input)
{
    
   // 固定ランダム値（ピクセルごとに異なる）
    float32_t mask = gMaskTexture.Sample(gSampler, input.texcoord).r; // 100.0でパターンの密度を調整
    
    //float32_t mask = gMaskTexture.Sample(gSampler, uv);
    //float32_t3 maskColor = float32_t3(1.0f, 1.0f, 1.0f);
    
    //maskColor.rgb *= mask;
    
    // ピクセルごとにランダム性を追加
    mask += rand(input.texcoord * 100.0) * 0.2; // 0.2でランダム強度を調整
    
    if (mask <= gThreshold.threshold)
    {
        discard;
    }
    
    PixelShaderOutPut output;
    
    //float32_t edge = 1.0f - smoothstep(0.5f, 0.53f, mask);
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    //output.color.rgb += edge * float32_t3(1.0f, 0.4f, 0.3f);
    return output;
}