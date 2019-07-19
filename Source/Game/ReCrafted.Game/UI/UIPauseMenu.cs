// ReCrafted (c) 2016-2019 Always Too Late

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
            //View.BindCallback("ResumeGame", () => { Show(false); });
            //View.BindCallback("ExitGame", Application.Quit);
            var ctx = View.Context;

            var global = JSObject.GetGlobalObject(ctx);
            var obj1 = global.GetPropertyValue(new JSString("TestFunction"));
            var func = obj1.ToFunction();
            func.Invoke(global);
        }

        private void Show(bool show)
        {
            View.Active = show;
            InputManager.ShowCursor = show;
            InputManager.LockCursor = !show;
        }
    }
}
