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

        private JSValue TestCallback(JSObject thisObject, params JSValue[] args)
        {
            var f = args.First().ToString();
            var ss = f.ToString();

            return new JSValue(thisObject.GetContext());
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
            var ctx = View.Context;

            var global = JSObject.GetGlobalObject(ctx);
            var obj1 = global.GetPropertyValue(new JSString("TestFunction"));
            var func = obj1.ToFunction();

            global.SetPropertyValue(new JSString("TestCallback"), new JSValue(ctx, TestCallback));

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
