using System;
using System.Diagnostics;
using ReCrafted.Core.Localization;
using ReCrafted.Graphics;
using ReCrafted.Logic;
using SharpDX.Windows;

namespace ReCrafted.Core
{
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

        private readonly Stopwatch _stopwatch;

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
                Text = string.Format(Locale.Strings.GameTitle, GameInfo.Current.BuildName, GameInfo.Current.BuildNumber, "dev")
            };

            Form.ResizeEnd += Form_ResizeEnd;
            Form.Resize += Form_ResizeEnd;

            Input.Initialize();

            // create renderer
            Renderer.CreateRenderer(RendererApi.D3D11);
            Renderer = Renderer.Instance;
            
            Time.StartupTime = DateTime.Now;
            Time.SimulationDeltaTime = 1.0f / 60.0f; // 60 sps

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

            _stopwatch.Start();
            
            Scene.Instance.Tick();
            Scene.Instance.Simulate(); // TODO: Fixed timestep for physics

            Renderer.Draw();
            
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
        }
    }
}