// ReCrafted (c) 2016-2018 Always Too Late

#ifndef COMMON_HLSLI
#define COMMON_HLSLI


// ==== Macros ====

#define EncodeNormal(n) normalize(n) * 0.5f + 0.5f
#define DecodeNormal(n) n * 2.0f + 1.0f

#define TransformPosition(pos) mul(ViewProjection, float4(pos, 1.0f)) // TODO: Use ModelViewProjection when possible

#define NearPlane ViewInfo.x
#define FarPlane ViewInfo.y

// ==== Constant Buffers ====

/// <summary>
/// Default constant buffer, this is being applied for every surface shader.
/// </summary>
cbuffer DefaultConstants : register(b0)
{
    /// <summary>
    /// The 'View * Projection' matrix.
    /// </summary>
    float4x4 ViewProjection;

    /// <summary>
    /// The inverted view matrix.
    /// </summary>
    float4x4 InvertedView;

    /// <summary>
    /// Contains view information.
    /// X (near clip plane distance)
    /// Y (far clip plane distance)
    /// Z (1.0f / ProjectionMatrix.M11)
    /// W (1.0f / ProjectionMatrix.M22)
    /// </summary>
    float4 ViewInfo;

    /// <summary>
    /// The camera position (also view position in world space).
    /// </summary>
    float3 CameraPosition;

    /// <summary>
    /// The directional light direction.
    /// </summary>
    float3 LightDirection;
}


// ==== Functions ====

/// <summary>
/// Gets view position from given depth and screen coords.
/// </summary>
float3 GetViewPosition(in float2 uv, in float depth)
{
    uv.x = uv.x * -2.0f + 1.0f;
    uv.y = uv.y * 2.0f + -1.0f;
    return float3(uv * ViewInfo.zw * depth, depth);
}

/// <summary>
/// Gets world position from given depth and screen coords.
/// </summary>
float3 GetWorldPosition(in float2 uv, in float depth)
{
    float3 viewPos = GetViewPosition(uv, depth);
    return mul(float4(viewPos, 1.0f), InvertedView).xyz;
}

/// <summary>
/// Gets world position from given view position.
/// </summary>
float3 GetWorldPosition(float3 viewPos)
{
    return mul(float4(viewPos, 1.0f), InvertedView).xyz;
}

#endif // COMMON_HLSLI
