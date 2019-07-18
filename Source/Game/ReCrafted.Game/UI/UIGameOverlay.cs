// ReCrafted (c) 2016-2019 Always Too Late

using System;
using ReCrafted.API.Core;
using ReCrafted.API.WebUI;

namespace ReCrafted.Game.UI
{
    public class UIGameOverlay : WebUIPanel
    {
        private float _minFrame;
        private float _maxFrame;
        private float _frameTimeTotal;
        private int _frames;

        private float _lastUpdate;

        public UIGameOverlay() : base("file:///Game/overlay.html") { }

        protected override void OnCreate()
        {
            _lastUpdate = (float) Time.CurrentTime + 1.0f;
        }

        protected override void OnViewDOMReady()
        {
            // Update game version
            //View.Call("UpdateGameVersion", GameInfo.Current.BuildName, GameInfo.Current.BuildNumber);
        }

        protected override void OnUpdate()
        {
            var deltaTime = (float) Time.DeltaTime;

            _minFrame = Math.Min(deltaTime, _minFrame);
            _maxFrame = Math.Max(deltaTime, _maxFrame);
            _frameTimeTotal += deltaTime;
            _frames++;

            if (_lastUpdate < Time.CurrentTime)
            {
                UpdateFps();
                
                _frames = 0;
                _minFrame = float.MaxValue;
                _maxFrame = 0.0f;
                _frameTimeTotal = 0.0f;
                _lastUpdate = (float)Time.CurrentTime + 1.0f;
            }
        }

        private void UpdateFps()
        {
            var avg = _frameTimeTotal / _frames;

            // Update FPS
            //View.Call("UpdatePerf", 1.0f / avg, _minFrame * 1000.0f, avg * 1000.0f, _maxFrame * 1000.0f);
        }
    }
}
