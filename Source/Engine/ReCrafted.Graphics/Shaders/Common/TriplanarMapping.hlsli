// ReCrafted (c) 2016-2019 Always Too Late

#ifndef TRIPLANAR_MAPPING_HLSLI
#define TRIPLANAR_MAPPING_HLSLI

float3 TriplanarCoords(float3 position)
{
    return position;
}

float3 TriplanarWeights(float3 normal) 
{
    float3 blendWeights = pow(normal, 8); // TODO: Improve blend function
    blendWeights /= dot(blendWeights, float3(1, 1, 1));
    return blendWeights;
}

float3 TriplanarSample(Texture2D tex, sampler smp, float3 position, float3 blendWeights, float textureScale) 
{
    // Triplanar mapping
    const float2 tx = position.zy * textureScale;
    const float2 ty = position.xz * textureScale;
    const float2 tz = position.xy * textureScale;

    return 
        tex.Sample(smp, tx).rgb * blendWeights.x + 
        tex.Sample(smp, ty).rgb * blendWeights.y +
        tex.Sample(smp, tz).rgb * blendWeights.z;
}

#endif // TRIPLANAR_MAPPING_HLSLI
