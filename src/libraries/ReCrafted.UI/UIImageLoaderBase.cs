// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.UI
{
    public abstract class UIImageLoaderBase : IDisposable
    {
        public abstract UIImage LoadImageFromFile(string fileName);

        public virtual void Dispose() { }
    }
}
