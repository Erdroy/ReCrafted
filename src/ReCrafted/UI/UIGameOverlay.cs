// ReCrafted (c) 2016-2019 Always Too Late

using System;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.WebUI;
using ReCrafted.Common;

namespace ReCrafted.Game.UI
{
    public class UIGameOverlay : WebUIPanel
    {
        private float _minFrame;
        private float _maxFrame;
        private float _frameTimeTotal;
        private int _frames;

        private float _lastUpdate;

        public UIGameOverlay() : base("file:///game/overlay.html") { }

        protected override void OnCreate()
        {
            _lastUpdate = (float)Time.CurrentTime;
        }

        protected override void OnViewFinishLoading()
        {
            // Update game version
            View.Call("UpdateGameVersion", GameInfo.Current.BuildName, GameInfo.Current.BuildNumber);
        }

        protected override void OnUpdate()
        {
            _minFrame = Math.Min((float)Time.DeltaTime, _minFrame);
            _maxFrame = Math.Max((float)Time.DeltaTime, _maxFrame);
            _frameTimeTotal += (float)Time.DeltaTime;
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
            View.Call("UpdatePerf", 1.0f / avg, _minFrame, avg, _maxFrame);
        }
    }
}
