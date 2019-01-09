// ReCrafted (c) 2016-2019 Always Too Late

using ReCrafted.API.Common.Actors;
using ReCrafted.API.Core;

namespace ReCrafted.API.WebUI
{
    /// <inheritdoc />
    /// <summary>
    /// WebUIManager system. Manages all WebUI panels.
    /// </summary>
    public sealed class WebUIManager : GameSystem
    {
        private static WebUIManager _instance;
        private EmptyActor _uiManagerActor;
        
        protected override void OnCreate()
        {
            _instance = this;
            _uiManagerActor = EmptyActor.Create();
        }

        protected override void OnDestroy()
        {
            _uiManagerActor.Destroy();
        }
        
        /// <summary>
        /// Adds new UI panel of given type.
        /// </summary>
        /// <typeparam name="TPanel">Type of the panel.</typeparam>
        public static TPanel AddPanel<TPanel>() where TPanel : WebUIPanel, new()
        {
            Logger.Log($"Added WebUIPanel of type {nameof(TPanel)}");
            return _instance._uiManagerActor.AddScript<TPanel>();
        }

        /// <summary>
        /// Removes given WebUI panel.
        /// </summary>
        public static void RemovePanel(WebUIPanel panel)
        {
            Logger.Log($"Removed WebUIPanel of type {nameof(panel)}");
            _instance._uiManagerActor.RemoveScript(panel);
        }
    }
}
