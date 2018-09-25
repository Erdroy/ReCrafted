// ReCrafted Editor (c) 2016-2018 Always Too Late

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
        private CommandList _commandList;
        private ImGuiRenderer _guiController;

        private MainWindow _mainWindow;
        private ContentWindow _contentPanel;

        private ContentManager _contentManager;

        public EditorApplication()
        {
            Current = this;
        }

        protected override void OnInit()
        {
            Window = VeldridStartup.CreateWindow(new WindowCreateInfo
            {
                WindowWidth = 1280,
                WindowHeight = 720,
                WindowInitialState = WindowState.Maximized,
                WindowTitle = "ReCrafted Editor"
            });

            // Shutdown on window close
            Window.Closing += Shutdown;

            GraphicsDevice = VeldridStartup.CreateGraphicsDevice(Window, new GraphicsDeviceOptions(true, null, true));
            
            Window.Resized += () =>
            {
                GraphicsDevice.MainSwapchain.Resize((uint)Window.Width, (uint)Window.Height);
                _guiController.WindowResized(Window.Width, Window.Height);
            };
        }

        protected override void OnLoad()
        {
            // Create main command list
            _commandList = GraphicsDevice.ResourceFactory.CreateCommandList();

            // Create GUI controller
            _guiController = new ImGuiRenderer(GraphicsDevice, GraphicsDevice.MainSwapchain.Framebuffer.OutputDescription, Window.Width, Window.Height);

            // Create main editor panel
            _mainWindow = new MainWindow();
            _mainWindow.Initialize();

            _contentPanel = new ContentWindow();
            _contentPanel.Initialize();
            _mainWindow.Children.Add(_contentPanel);

            _contentManager = new ContentManager();
            _contentManager.Initialize();
        }

        protected override void OnUnload()
        {
            GraphicsDevice.WaitForIdle();

            _contentManager.Dispose();
            _mainWindow.Dispose();
            _guiController.Dispose();
            _commandList.Dispose();
            GraphicsDevice.Dispose();
        }

        protected override void OnUpdate()
        {
            // Pump window events
            var inputSnapshot = Window.PumpEvents();

            if (!Window.Exists)
            {
                Shutdown();
                return;
            }

            Time.Update();

            // Update gui
            _guiController.Update(Time.DeltaTime, inputSnapshot);

            // Update main panel
            _mainWindow.Update();

            // Begin rendering
            _commandList.Begin();
            _commandList.SetFramebuffer(GraphicsDevice.MainSwapchain.Framebuffer);
            _commandList.ClearColorTarget(0, new RgbaFloat(0.15f, 0.15f, 0.15f, 1.0f));

            // Render main panel
            _mainWindow.Render();

            // Render ImGui
            _guiController.Render(GraphicsDevice, _commandList);

            // Finish rendering
            _commandList.End();
            GraphicsDevice.SubmitCommands(_commandList);
            GraphicsDevice.SwapBuffers();
        }

        public GraphicsDevice GraphicsDevice { get; private set; }

        public Sdl2Window Window { get; private set; }

        /// <summary>
        /// The current voxel editor application instance.
        /// </summary>
        public new static EditorApplication Current { get; private set; }
    }
}
