// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Diagnostics;
using System.Windows.Forms;
using ReCrafted.Core.Localization;
using ReCrafted.Graphics;
using ReCrafted.Logic;
using SharpDX.Windows;

namespace ReCrafted.Core
{
    /// <summary>
    /// OnResize
    /// </summary>
    /// <param name="width">The width.</param>
    /// <param name="height">The height.</param>
    public delegate void OnResize(int width, int height);

    public sealed class Game : IDisposable, IRunnable
    {
        /// <summary>
        /// The instance of game class.
        /// </summary>
        public static Game Instance;

        /// <summary>
        /// The target render form/game window.
        /// </summary>
        public RenderForm Form;

        /// <summary>
        /// The current renderer instance.
        /// </summary>
        public Renderer Renderer;

        /// <summary>
        /// OnResize event.
        /// Called when the game window has been resized.
        /// </summary>
        public event OnResize OnResize;

        private readonly Stopwatch _stopwatch;

        private GameManager _gameManager;
        private float _nextTimeUpdate;
        private int _frames;
        private float _framesTime;

        /// <summary>
        /// Game class constructor.
        /// </summary>
        public Game()
        {
            Instance = this;

            _stopwatch = new Stopwatch();
        }

        /// <summary>
        /// Runs the game.
        /// </summary>
        public void Run()
        {
            Locale.SetLanguage("polski");
            
            GameInfo.Load();

            Form = new RenderForm
            {
                Text = string.Format(Locale.Strings.GameTitle, GameInfo.Current.BuildName, GameInfo.Current.BuildNumber)
            };

            Form.ResizeEnd += Form_ResizeEnd;
            Form.Resize += Form_ResizeEnd;

            Input.Initialize();

            // create renderer
            Renderer.CreateRenderer(RendererApi.D3D11, new DeferredRendering());
            Renderer = Renderer.Instance;
            
            Time.StartupTime = DateTime.Now;
            Time.SimulationDeltaTime = 1.0f / 60.0f; // 60 sps

            Form.WindowState = FormWindowState.Maximized;
            
            // initialize scene
            Scene.Instance.Init();

            _gameManager = new GameManager();

            // run the game
            RenderLoop.Run(Form, Loop, true);
        }

        /// <summary>
        /// Shutdowns the game.
        /// </summary>
        public void Shutdown()
        {
            Dispose();
        }

        /// <summary>
        /// Disposes the game, please call Shutdown() instead.
        /// </summary>
        public void Dispose()
        {
            Form.Close();
            Form.Dispose();
        }

        // private
        private void Loop()
        {
            // loop
            if (_stopwatch.IsRunning)
            {
                Time.DeltaTime = _stopwatch.ElapsedMilliseconds/1000.0f;
                _stopwatch.Stop();
                _stopwatch.Reset();
                
                if (_nextTimeUpdate <= Time.TimeSinceStartup)
                {
                    Time.FramesPerSecond = 1.0f / Time.DeltaTime;
                    Time.SmoothDeltaTime = _framesTime / _frames;
                    Time.SmoothFramesPerSecond = 1.0f / Time.SmoothDeltaTime;

                    _frames = 0;
                    _framesTime = 0.0f;

                    _nextTimeUpdate = Time.TimeSinceStartup + 1.0f;
                }
                else
                {
                    _frames++;
                    _framesTime += Time.DeltaTime;
                }
            }

            if (Input.IsKeyDown(KeyCode.Escape))
            {
                Environment.Exit(0);
            }

            _stopwatch.Start();
            
            Tick();
            Simulate();
            Draw();
            
            // update time
            Time.Frame++;
            Time.SimulationFrame++;
            Time.TimeSinceStartup += Time.DeltaTime;
        }

        // private
        private void Form_ResizeEnd(object sender, EventArgs e)
        {
            // resized
            Renderer.Resize(Form.ClientSize.Width, Form.ClientSize.Height);
            Renderer.Draw();

            // call onresize event
            OnResize?.Invoke(Form.ClientSize.Width, Form.ClientSize.Height);
        }

        // private
        private void Tick()
        {
            Scene.Instance.Tick();
            _gameManager.Tick();
        }

        // private
        private void Simulate()
        {
            Scene.Instance.Simulate(); // TODO: Fixed timestep for physics
        }

        // private
        private void Draw()
        {
            Renderer.Draw();
        }
    }
}