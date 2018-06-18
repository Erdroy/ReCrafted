// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.UI;

namespace ReCrafted.Game
{
    public class UITests2
    {
        private UIContext _context;

        public class Renderer : UIRendererBase
        {

        }

        public class ImageLoader : UIImageLoaderBase
        {
            public override UIImage LoadImageFromFile(string fileName)
            {
                throw new System.NotImplementedException();
            }
        }

        public void Init()
        {
            _context = new UIContext(new Renderer(), new ImageLoader());
        }

        public void Update()
        {

        }
    }
}
