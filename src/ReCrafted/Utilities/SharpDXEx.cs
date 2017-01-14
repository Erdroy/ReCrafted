// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

namespace ReCrafted.Utilities
{
    // ReSharper disable once InconsistentNaming
    /// <summary>
    /// Sharpdx utilites metods
    /// </summary>
    public static class SharpDXEx
    {
        public static OpenTK.Vector2 ToOpenTk(this SharpDX.Vector2 vec)
        {
            return new OpenTK.Vector2(vec.X, vec.Y);
        }

        public static OpenTK.Vector3 ToOpenTk(this SharpDX.Vector3 vec)
        {
            return new OpenTK.Vector3(vec.X, vec.Y, vec.Z);
        }

        public static OpenTK.Vector4 ToOpenTk(this SharpDX.Vector4 vec)
        {
            return new OpenTK.Vector4(vec.X, vec.Y, vec.Z, vec.W);
        }

        public static OpenTK.Quaternion ToOpenTk(this SharpDX.Quaternion vec)
        {
            return new OpenTK.Quaternion(vec.X, vec.Y, vec.Z, vec.W);
        }

        public static OpenTK.Matrix4 ToOpenTk(this SharpDX.Matrix mat)
        {
            return new OpenTK.Matrix4(mat.Column1.ToOpenTk(), mat.Column2.ToOpenTk(), mat.Column3.ToOpenTk(), mat.Column4.ToOpenTk());
        }

        public static OpenTK.Matrix3 ToOpenTk(this SharpDX.Matrix3x3 mat)
        {
            return new OpenTK.Matrix3(mat.Column1.ToOpenTk(), mat.Column2.ToOpenTk(), mat.Column3.ToOpenTk());
        }




        public static SharpDX.Vector4 ToSharpDx(this OpenTK.Vector4 vec)
        {
            return new SharpDX.Vector4(vec.X, vec.Y, vec.Z, vec.W);
        }

        public static SharpDX.Matrix ToSharpDx(this OpenTK.Matrix4 mat)
        {
            return new SharpDX.Matrix
            {
                Column1 = mat.Column0.ToSharpDx(),
                Column2 = mat.Column1.ToSharpDx(),
                Column3 = mat.Column2.ToSharpDx(),
                Column4 = mat.Column3.ToSharpDx()
            };
        }
    }
}