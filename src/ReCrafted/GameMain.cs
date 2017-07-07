// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.UI;

namespace ReCrafted.Game
{
    using Game = API.Core.Game;

    internal class GameMain : Game
    {
        protected override void Initialize()
        {
            AppDomain.CurrentDomain.UnhandledException += (sender, args) =>
            {
                var exception = args.ExceptionObject as Exception;

                if(exception != null)
                    Logger.Write($"Unhandled Exception: {exception}", LogLevel.Error);
            };

            Logger.Write("Hello, World from C#!", LogLevel.Debug);

            var main = UIPanel.Create(new RectangleF(100.0f, 200.0f, 150.0f, 200.0f), UILayoutType.Vertical);
            
        }

        protected override void Update()
        {

        }

        protected override void Simulate()
        {

        }

        protected override void Draw()
        {
            
        }

        protected override void DrawUI()
        {
            UIInternal.Color = Color.DarkViolet;
            UIInternal.DrawBox(new RectangleF(Display.Width / 2.0f - 8.0f, Display.Height / 2.0f - 8.0f, 16.0f, 16.0f));

            UIPanel.DrawAll();
        }

        protected override void Shutdown()
        {

        }
    }
}