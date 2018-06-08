// ReCrafted (c) 2016-2018 Always Too Late

using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using AForge.Imaging.Filters;
using ReCrafted.VoxelEditor.Graphics;
using ReCrafted.VoxelEditor.Rendering;
using ReCrafted.VoxelEditor.Voxels;
using SharpDX;
using Color = SharpDX.Color;
using RectangleF = SharpDX.RectangleF;

namespace ReCrafted.VoxelEditor.Core
{
    /// <summary>
    /// Main voxel editor class.
    /// </summary>
    public class VoxelEditorApp : ApplicationBase
    {
        public VoxelEditorApp()
        {
            Current = this;
        }

        protected override void OnInit()
        {
            Renderer = new Renderer();
            Renderer.Init();

            Renderer.RenderForm.Closed += (sender, args) =>
            {
                Shutdown();
            };

            // initialize input
            Input = new Input();
            Input.Init();

            // initialize UI
            UI = new UI();
            UI.Init();
        }

        protected override void OnLoad()
        {
            // Temporary, create CHMTexture
            var texture = CHMBitmap.Create(2048, 2048, 8);
            
            for (var i = 0; i < 6; i++)
            {
                var faceType = (CHMBitmap.FaceType)i;

                // Read bitmap
                var fileName = "../assets/spacebodies/moon/" + faceType.ToString().ToLower() + ".bmp";
                var bitmap = new Bitmap(fileName);
                
                var mipWidth = bitmap.Width / 2;
                var mipHeight = bitmap.Width / 2;

                for (var j = 0; j < 8; j++)
                {
                    if (j > 0)
                    {
                        // Generate mip
                        var mipBitmap = ResizeImage(bitmap, mipWidth, mipHeight);

                        var mipBitmapDataRaw = BitmapToByte(mipBitmap);
                        texture.AddFaceMip(faceType, j, mipBitmapDataRaw, mipWidth, mipHeight);

                        //mipBitmap.Save("CHMBitmap-lod" + j + ".bmp");

                        // Release mip bitmap
                        mipBitmap.Dispose();

                        // Set next mip size
                        mipWidth = mipWidth / 2;
                        mipHeight = mipHeight / 2;
                    }
                    else
                    {
                        // NOTE: We want to 'resize' the image, to hold the gray-scale level, not optimal but needed. TODO: Try to get better way to generate mip maps gray-scale
                        var bitmapDataRaw = BitmapToByte(bitmap);
                        //bitmap.Save("CHMBitmap-lod0.bmp");
                        texture.AddFaceMip(faceType, j, bitmapDataRaw, bitmap.Width, bitmap.Height);
                    }
                }

                // Release bitmap
                bitmap.Dispose();
            }

            texture.Save("../assets/voxeldata/moon.chm");
            texture.Dispose();
            
            // Shutdown
            Shutdown();
        }

        protected override void OnUnload()
        {
            Input.Dispose();
            UI.Dispose();
            Renderer.Dispose();
        }

        protected override void OnUpdate()
        {
            Display.Update();
            Input.Update();

            // do application events
            Application.DoEvents();

            // begin new frame
            Renderer.BeginFrame();

            // begin UI drawing
            UI.Begin();

            DrawUI();

            // end and push the UI draw commands
            UI.End();

            // end current frame
            Renderer.EndFrame();
        }

        private bool _toggle1;
        private void DrawUI()
        {
            // update ??
            UI.Box(new RectangleF(0.0f, 0.0f, Display.Width, 30.0f), Color.Gray);

            if (UI.Button(new RectangleF(2.0f, 2.0f, 100.0f, 26.0f), "Hello!", new Color(255, 105, 0, 255)))
            {
                
            }

            if (UI.Button(new RectangleF(104.0f, 2.0f, 100.0f, 26.0f), "Hello!", new Color(255, 105, 0, 255)))
            {

            }

            _toggle1 = UI.Toggle(_toggle1, new Vector2(10.0f, 100.0f), Color.Gray);
        }

        private static Bitmap ResizeImage(Image image, int width, int height)
        {
            // source: https://stackoverflow.com/questions/10442269/scaling-a-system-drawing-bitmap-to-a-given-size-while-maintaining-aspect-ratio

            var dstRect = new System.Drawing.Rectangle(0, 0, width, height);
            var dstImage = new Bitmap(width, height);

            dstImage.SetResolution(image.HorizontalResolution, image.VerticalResolution);

            using (var graphics = System.Drawing.Graphics.FromImage(dstImage))
            {
                graphics.CompositingMode = CompositingMode.SourceCopy;
                graphics.CompositingQuality = CompositingQuality.HighQuality;
                graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
                graphics.SmoothingMode = SmoothingMode.HighQuality;
                graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

                using (var wrapMode = new ImageAttributes())
                {
                    wrapMode.SetWrapMode(WrapMode.TileFlipXY);
                    graphics.DrawImage(image, dstRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, wrapMode);
                }
            }
            
            return Grayscale.CommonAlgorithms.BT709.Apply(dstImage);
        }

        private static byte[] BitmapToByte(Bitmap img)
        {
            var rect = new System.Drawing.Rectangle(new System.Drawing.Point(0), img.Size);
            var data = img.LockBits(rect, ImageLockMode.ReadOnly, PixelFormat.Format8bppIndexed);
            var size = data.Stride * data.Height;
            var bytes = new byte[size];
            Marshal.Copy(data.Scan0, bytes, 0, size);
            img.UnlockBits(data);
            return bytes;
        }

        /// <summary>
        /// The renderer instance.
        /// </summary>
        public Renderer Renderer { get; private set; }

        /// <summary>
        /// The UI instance.
        /// </summary>
        public UI UI { get; private set; }

        /// <summary>
        /// The Input instance.
        /// </summary>
        public Input Input { get; private set; }

        /// <summary>
        /// The current voxel editor application instance.
        /// </summary>
        public new static VoxelEditorApp Current { get; private set; }
    }
}
