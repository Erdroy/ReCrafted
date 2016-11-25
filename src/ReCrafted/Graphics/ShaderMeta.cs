// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.IO;
using System.Linq;
using Newtonsoft.Json;
using ReCrafted.Utilities;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// ShaderMeta class
    /// </summary>
    public struct ShaderMeta
    {
        /// <summary>
        /// ConstantBuffer structure.
        /// </summary>
        public struct ConstantBuffer
        {
            /// <summary>
            /// Field structure.
            /// </summary>
            public struct Field
            {
                public string FieldName;
                public string FieldType;
            }

            public string BufferName;
            public Field[] Fields;

            public int GetOffset(string fieldName)
            {
                var offset = 0;
                foreach (var field in Fields)
                {
                    if (field.FieldName == fieldName)
                        return offset;
                    offset += Shader.GetTypeSize(field.FieldType);
                }

                return -1;
            }

            public int Size => Fields.Sum(field => Shader.GetTypeSize(field.FieldType));
        }

        /// <summary>
        /// Pass structure.
        /// </summary>
        public struct Pass
        {
            /// <summary>
            /// VertexLayoutInput structure.
            /// </summary>
            public struct VertexLayoutInput
            {
                public string InputName;
                public string InputType;
                public string InputSignature;
            }

            public string PassName;
            public string VertexShader;
            public string PixelShader;
            public string ComputeShader;
            public string Profile;

            public VertexLayoutInput[] VertexLayoutInputs;
        }

        /// <summary>
        /// The shader name.
        /// </summary>
        public string ShaderName;

        /// <summary>
        /// The shader description.
        /// </summary>
        public string ShaderDesc;

        /// <summary>
        /// Constant buffers.
        /// </summary>
        public ConstantBuffer[] ConstantBuffers;

        /// <summary>
        /// Passes.
        /// </summary>
        public Pass[] Passes;
        
        /// <summary>
        /// Loads shader meta from json file.
        /// </summary>
        /// <param name="fileName">The json/shader meta file name.</param>
        /// <returns>The loaded shader meta.</returns>
        public static ShaderMeta FromFile(string fileName)
        {
            if(!File.Exists(fileName))
                throw new ReCraftedException($"The '{fileName}'shader metafile not found!");

            var metadata = File.ReadAllText(fileName);

            return JsonConvert.DeserializeObject<ShaderMeta>(metadata);
        }
    }
}