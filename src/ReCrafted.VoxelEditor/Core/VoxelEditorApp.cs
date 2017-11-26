// ReCrafted © 2016-2017 Always Too Late

using System.Windows.Forms;
using ReCrafted.VoxelEditor.Graphics;
using ReCrafted.VoxelEditor.Rendering;
using SharpDX;

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
