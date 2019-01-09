// ReCrafted (c) 2016-2019 Always Too Late

using ReCrafted.API.Input;
using ReCrafted.API.WebUI;

namespace ReCrafted.Game.UI
{
    public class UIGameHud : WebUIPanel
    {
        private bool _cursorHide;

        public UIGameHud() : base("file:///game/hud.html") { }

        protected override void OnUpdate()
        {
            if (InputManager.IsKeyDown(Key.Tab) && Cursor.Lock)
            {
                _cursorHide = true;
                Cursor.Lock = false;
                Cursor.Show = true;
            }

            if (InputManager.IsKeyUp(Key.Tab) && !Cursor.Lock && _cursorHide)
            {
                _cursorHide = false;
                Cursor.Lock = true;
                Cursor.Show = false;
            }
        }
    }
}
