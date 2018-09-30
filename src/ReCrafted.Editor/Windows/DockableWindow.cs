// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Drawing;
using System.Numerics;
using ImGuiNET;
using ReCrafted.Editor.Windows.Docking;

namespace ReCrafted.Editor.Windows
{
    public abstract class DockableWindow : DockPanelBase, IWindow
    {
        private bool _focus;

        public abstract void Initialize();
        public abstract void Dispose();

        public virtual void OnUpdate() { }
        public virtual void OnRender() { }

        protected internal override void OnDock()
        {
            Floating = false;
        }

        protected override void OnUndock()
        {
            Floating = true;
        }

        protected virtual void OnFocus() { }

        public void Update()
        {
            OnUpdate();
        }

        public void Render()
        {
            if (_focus)
            {
                ImGui.SetNextWindowFocus();
                OnFocus();
                _focus = false;
            }

            if (!Floating)
            {
                RenderDocked();
            }
            else
            {
                RenderFloating();
            }
        }

        /// <summary>
        /// Sets focus for this window.
        /// </summary>
        public void Focus()
        {
            _focus = true;
        }

        private void RenderDocked()
        {
            ImGui.PushStyleVar(StyleVar.WindowBorderSize, 1.0f);
            ImGui.PushStyleVar(StyleVar.WindowRounding, 0.0f);

            ImGui.SetNextWindowPos(new Vector2(Rect.X, Rect.Y), Condition.Always);
            ImGui.SetNextWindowSize(new Vector2(Rect.Width, Rect.Height), Condition.Always);
            
            if (ImGui.BeginWindow($"{WindowName}###{WindowId}", WindowSettings | WindowFlags.NoResize | WindowFlags.NoCollapse | WindowFlags.NoBringToFrontOnFocus))
            {
                ProcessDocking();
                OnRender();
                ImGui.EndWindow();
            }

            ImGui.PopStyleVar(2);
        }

        private void RenderFloating()
        {
            ImGui.PushStyleVar(StyleVar.WindowBorderSize, 1.0f);
            ImGui.PushStyleVar(StyleVar.WindowRounding, 3.0f);

            if (ImGui.BeginWindow($"{WindowName}###{WindowId}", WindowSettings | WindowFlags.NoCollapse))
            {
                ProcessDocking();
                OnRender();
                ImGui.EndWindow();
            }

            ImGui.PopStyleVar(2);
        }

        private void ProcessDocking()
        {
            if (ImGui.IsLastItemActive() && ImGui.IsMouseDragging(0, 20.0f))
            {
                if (!Floating)
                {
                    Undock();
                }
                else
                {
                    // TODO: Docking implementation
                }
            }
        }

        public bool Floating { get; set; } = true;
        public int WindowId { get; internal set; }
        public Rectangle WindowRect => Rect;
        public abstract string WindowName { get; }
        public virtual WindowFlags WindowSettings => WindowFlags.Default;
    }
}
