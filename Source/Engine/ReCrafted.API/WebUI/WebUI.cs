// ReCrafted (c) 2016-2019 Always Too Late

using System.Collections.Generic;
using ReCrafted.API.Common;
using ReCrafted.API.Core;

namespace ReCrafted.API.WebUI
{
    /// <inheritdoc />
    /// <summary>
    /// WebUIManager system. Manages all WebUI panels.
    /// </summary>
    public sealed class WebUI : GameSystem
    {
        private static WebUI _instance;
        private readonly List<WebUIPanel> _panels = new List<WebUIPanel>();

        protected override void OnCreate()
        {
            _instance = this;
        }

        protected override void OnDestroy()
        {
            _panels.Clear();
        }

        protected override void OnUpdate()
        {
            foreach (var panel in _panels)
            {
                if (panel.IsLoaded)
                {
                    panel.Update();
                }
            }
        }

        /// <summary>
        /// Adds new UI panel of given type.
        /// </summary>
        /// <typeparam name="TPanel">Type of the panel.</typeparam>
        public static TPanel AddPanel<TPanel>() where TPanel : WebUIPanel, new()
        {
            Logger.Log($"Added WebUIPanel of type {nameof(TPanel)}");
            var panel = new TPanel();
            panel.Create();
            _instance._panels.Add(panel);
            return panel;
        }

        /// <summary>
        /// Removes given WebUI panel.
        /// </summary>
        public static void RemovePanel(WebUIPanel panel)
        {
            Logger.Log($"Removed WebUIPanel of type {nameof(panel)}");
            panel.Destroy();
            _instance._panels.Remove(panel);
        }
    }
}
