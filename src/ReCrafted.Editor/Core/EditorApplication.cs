// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Collections.Generic;
using ReCrafted.Editor.Common;
using ReCrafted.Editor.Content;
using ReCrafted.Editor.Graphics;
using ReCrafted.Editor.Windows;
using Veldrid;
using Veldrid.Sdl2;
using Veldrid.StartupUtilities;

namespace ReCrafted.Editor.Core
{
    /// <summary>
    /// Main voxel editor class.
    /// </summary>
    public class EditorApplication : ApplicationBase
    {
        private readonly List<IWindow> _applicationWindows = new List<IWindow>();

        private CommandList _commandList;
        private ImGuiRenderer _guiController;
        private ContentManager _contentManager;

        public EditorApplication()
        {
            Current = this;
        }

        protected override void OnInit()
        {
            SdlWindow = VeldridStartup.CreateWindow(new WindowCreateInfo
            {
                WindowWidth = 1280,
                WindowHeight = 720,
                WindowInitialState = WindowState.Maximized,
                WindowTitle = "ReCrafted Editor"
            });

            // Shutdown on window close
            SdlWindow.Closing += Shutdown;

            GraphicsDevice = VeldridStartup.CreateGraphicsDevice(SdlWindow, new GraphicsDeviceOptions(true, null, true));
            
            SdlWindow.Resized += () =>
            {
                GraphicsDevice.MainSwapchain.Resize((uint)SdlWindow.Width, (uint)SdlWindow.Height);
                _guiController.WindowResized(SdlWindow.Width, SdlWindow.Height);
            };
        }

        protected override void OnLoad()
        {
            // Create main command list
            _commandList = GraphicsDevice.ResourceFactory.CreateCommandList();

            // Create GUI controller
            _guiController = new ImGuiRenderer(GraphicsDevice, GraphicsDevice.MainSwapchain.Framebuffer.OutputDescription, SdlWindow.Width, SdlWindow.Height);
            
            // Create content manager
            _contentManager = new ContentManager();
            _contentManager.Initialize();

            // Create main editor panel
            MainWindow = new MainWindow();
            MainWindow.Initialize();
            _applicationWindows.Add(MainWindow);
            
            // Initialize time
            Time.Init();
        }

        protected override void OnUnload()
        {
            GraphicsDevice.WaitForIdle();

            foreach(var window in _applicationWindows)
                window?.Dispose();

            _contentManager.Dispose();
            _guiController.Dispose();
            _commandList.Dispose();
            GraphicsDevice.Dispose();
        }

        protected override void OnUpdate()
        {
            // Pump window events
            var inputSnapshot = SdlWindow.PumpEvents();

            if (!SdlWindow.Exists)
            {
                Shutdown();
                return;
            }

            Time.Update();

            // Update gui
            _guiController.Update(Time.DeltaTime, inputSnapshot);
            
            // Begin rendering
            _commandList.Begin();
            _commandList.SetFramebuffer(GraphicsDevice.MainSwapchain.Framebuffer);
            _commandList.ClearColorTarget(0, new RgbaFloat(0.10f, 0.10f, 0.10f, 1.0f));

            // Render windows
            foreach (var window in _applicationWindows)
            {
                window.Render();
            }

            // Render ImGui
            _guiController.Render(GraphicsDevice, _commandList);

            // Finish rendering
            _commandList.End();
            GraphicsDevice.SubmitCommands(_commandList);
            GraphicsDevice.SwapBuffers();
        }

        public GraphicsDevice GraphicsDevice { get; private set; }
        public MainWindow MainWindow { get; private set; }
        public Sdl2Window SdlWindow { get; private set; }

        /// <summary>
        /// The current voxel editor application instance.
        /// </summary>
        public new static EditorApplication Current { get; private set; }
    }
}
