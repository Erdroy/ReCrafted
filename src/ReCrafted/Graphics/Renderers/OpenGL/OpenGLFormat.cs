// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using SharpDX.DXGI;

namespace ReCrafted.Graphics.Renderers.OpenGLMesh
{
    /// <summary>
    /// OpenGLMeshFormat class.
    /// </summary>
    // ReSharper disable once InconsistentNaming
    internal static class OpenGLMeshFormat
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
                case "uint":
                    return Format.R32_UInt;
                    
                default:
                    return Format.Unknown;
            }
        }
    }
}