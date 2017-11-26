// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.VoxelEditor.Graphics;
using ReCrafted.VoxelEditor.Rendering;

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

            UI = new UI();
            UI.Init();
        }

        protected override void OnLoad()
        {
        }

        protected override void OnUnload()
        {
            UI.Dispose();
            Renderer.Dispose();
        }

        protected override void OnUpdate()
        {
            // begin new frame
            Renderer.BeginFrame();

            // update ??

            // draw UI
            UI.Draw();

            // end current frame
            Renderer.EndFrame();
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
        /// The current voxel editor application instance.
        /// </summary>
        public new static VoxelEditorApp Current { get; private set; }
    }
}
