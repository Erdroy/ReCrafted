// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using ReCrafted.API.Core;
using ReCrafted.API.WebUI;
using ReCrafted.Tools.Common;

namespace ReCrafted.Game.UI
{
    public class UIGameOverlay : WebUIPanel
    {
        private float _minFrame;
        private float _maxFrame;
        private float _frameTimeTotal;
        private int _frames;

        private float _lastUpdate;

        private WebUIViewFunction _updatePerf;

        public UIGameOverlay() : base("file:///Game/overlay.html") { }

        protected override void OnCreate()
        {
            _lastUpdate = (float) Time.CurrentTime + 1.0f;
        }

        protected override void RegisterBindings()
        {
            _updatePerf = View.BindFunction("UpdatePerf");

            var updateVersion = View.BindFunction("UpdateGameVersion");
            updateVersion(View.Object, View.ToValue(GameInfo.Current.BuildName), View.ToValue(GameInfo.Current.BuildNumber));
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
            _updatePerf(View.Object,
                View.ToValue(1.0f / avg),
                View.ToValue(_minFrame * 1000.0f),
                View.ToValue(avg * 1000.0f),
                View.ToValue(_maxFrame * 1000.0f)
            );
        }
    }
}
