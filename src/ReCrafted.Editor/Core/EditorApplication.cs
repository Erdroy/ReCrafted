// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Core
{
    /// <summary>
    /// Main voxel editor class.
    /// </summary>
    public class EditorApplication : ApplicationBase
    {
        public EditorApplication()
        {
            Current = this;
        }

        protected override void OnInit()
        {
            
        }

        protected override void OnLoad()
        {
        }

        protected override void OnUnload()
        {
            
        }

        protected override void OnUpdate()
        {
            
        }
        
        /// <summary>
        /// The current voxel editor application instance.
        /// </summary>
        public new static EditorApplication Current { get; private set; }
    }
}
