using SharpDX.DXGI;

namespace ReCrafted.Graphics.Renderers.D3D11
{
    /// <summary>
    /// D3D11Format class.
    /// </summary>
    internal static class D3D11Format
    {
        /// <summary>
        /// Converts format name to Format.
        /// </summary>
        /// <param name="format">The format name.</param>
        /// <returns>The converted format.</returns>
        public static Format FromString(string format)
        {
            switch (format)
            {
                case "float":
                    return Format.R32_Float;
                case "float2":
                    return Format.R32G32_Float;
                case "float3":
                    return Format.R32G32B32_Float;
                case "float4":
                    return Format.R32G32B32A32_Float;

                default:
                    return Format.Unknown;
            }
        }
    }
}