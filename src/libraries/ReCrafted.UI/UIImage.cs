// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.UI
{
    public class UIImage : IDisposable
    {

        public void Dispose()
        {
        }

        public static UIImage FromFile(string fileName)
        {
            return UIContext.Current.ImageLoader.LoadImageFromFile(fileName);
        }

        public int Width { get; internal set; }

        public int Height { get; internal set; }
    }
}
