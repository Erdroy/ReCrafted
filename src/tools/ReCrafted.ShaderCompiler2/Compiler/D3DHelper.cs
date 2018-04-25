// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.ShaderCompiler.Compiler
{
    public static class D3DHelper
    {
        public static int TypeToSize(string typeName)
        {
            switch (typeName)
            {
                case "bool":        // bool
                    return 1;

                case "half":        // half
                    return 2;

                case "int":         // int
                case "uint":        // uint
                case "dword":       // dword
                case "float":       // float
                case "half2":       // half2
                    return 4;

                case "double":      // double
                case "int2":        // int2
                case "uint2":       // uint2
                case "float2":      // float2
                case "half4":       // half4
                    return 8;

                case "int3":        // int3
                case "uint3":       // uint3
                case "float3":      // float3
                    return 12;
                    
                case "int4":        // int4
                case "uint4":       // uint4
                case "dword4":      // dword4
                case "float4":      // float4
                case "float2x2":    // float2x2
                    return 16;

                case "float2x3":    // float2x3
                case "float3x2":    // float3x2
                    return 24;

                case "float4x2":    // float4x2
                case "float2x4":    // float2x4
                    return 32;

                case "float3x3":    // float3x3
                    return 36;

                case "float4x3":    // float4x3
                case "float3x4":    // float3x4
                    return 48;

                case "matrix":      // matrix
                case "float4x4":    // float4x4
                    return 64;

                default:
                    return 0;
            }
        }
    }
}
