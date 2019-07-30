// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Linq;
using ReCrafted.API.Core;
using ReCrafted.API.Input;
using ReCrafted.API.WebUI;
using ReCrafted.API.WebUI.JavaScript;

namespace ReCrafted.Game.UI
{
    public class UIPauseMenu : WebUIPanel
    {
        public UIPauseMenu() : base("file:///Game/menu.html", false) { }

        protected override void OnUpdate()
        {
            if (InputManager.IsKeyDown(Key.Escape))
                Show(!View.Active);
        }

        protected override void RegisterBindings()
        {
            View.Name = nameof(UIPauseMenu);
            View.BindCallback("ResumeGame", (thisObject, args) =>
            {
                Show(false);
                return JSValue.Null;
            });
            View.BindCallback("ExitGame", (thisObject, args) =>
            {
                Application.Quit();
                return JSValue.Null;
            });
        }

        private void Show(bool show)
        {
            View.Active = show;
            InputManager.ShowCursor = show;
            InputManager.LockCursor = !show;
        }
    }
}
