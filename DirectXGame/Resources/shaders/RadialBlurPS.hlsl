#include"Test.hlsli"

Texture2D<float32_t4> gTexture : register(t0); //SRV register=>t
SamplerState gSampler : register(s0); //Sampler register=>s


struct PixelShaderOutPut
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutPut main(VertexShaderOutput input)
{
    const float32_t2 kCenter = float32_t2(0.5f, 0.5f);
    const int32_t kNumSamples = 10; // サンプリング数。 サンプリング数。多いほど滑らかだが重い
    const float32_t kBlurWidth = 0.01f; // ぼかしの幅。大きいほど大きい
    // 中心から現在のuvに対しての方向を計算。
    // 普段方向といえば、単位ベクトルだが、ここではあえて正規化せず、遠いほどより遠くをサンプリングする
    float32_t2 direction = input.texcoord - kCenter;
    float32_t3 outputColor = float32_t3(0.0f, 0.0f, 0.0f);
    for (int32_t sampleIndex = 0; sampleIndex < kNumSamples; ++sampleIndex)
    {
        // 現在のuvからさきほど計算した方向にサンプリング点を進めながらサンプリングしていく
        float32_t2 texcoord = input.texcoord + direction * kBlurWidth * float32_t(sampleIndex);
        outputColor.rgb += gTexture.Sample(gSampler, texcoord).rgb;
    }
    // 平均化する
    outputColor.rgb *= rcp(kNumSamples);
    
    PixelShaderOutPut output;
    output.color.rgb = outputColor;
    output.color.a = 1.0f;
    return output;
}