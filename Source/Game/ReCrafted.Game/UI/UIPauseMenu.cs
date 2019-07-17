// ReCrafted (c) 2016-2019 Always Too Late

using ReCrafted.API.Input;
using ReCrafted.API.WebUI;

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
            // function ExitGame();
            //View.Bind("ResumeGame", () => { Show(false); });
            //View.Bind("ExitGame", Application.Exit);
        }

        private void Show(bool show)
        {
            View.Active = show;
            InputManager.ShowCursor = show;
            InputManager.LockCursor = !show;
        }
    }
}
