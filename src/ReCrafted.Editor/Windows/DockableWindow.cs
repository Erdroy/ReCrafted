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

        private DockType _dockType;
        private DockDirection _dockDirection;
        private DockPanelBase _dockTarget;

        public abstract void Initialize();
        public abstract void Dispose();
        
        public virtual void OnRender() { }
        public virtual void OnOpen() { }
        public virtual void OnHide() { }
        public virtual void OnClose() { }

        protected internal override void OnDock()
        {
            Floating = false;
        }

        protected override void OnUndock()
        {
            Floating = true;
        }

        protected virtual void OnFocus() { }

        public void Open()
        {
            OnOpen();
            WindowVisible = true;
        }

        public void Hide()
        {
            OnHide();
            WindowVisible = false;
        }

        public void Close()
        {
            OnClose();
            WindowClosed = true;
            WindowVisible = false;
            Dispose();
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
            if (DockSplitter.IsAnySplitterDragging)
                return;

            if (Floating)
            {
                if (_dockTarget != null && ImGui.IsMouseReleased(0))
                {
                    _dockTarget.Dock(this, _dockType, _dockDirection, 0.30f);
                    MainWindow.Instance.DockPane.RecalculateLayout();
                    _dockTarget = null;
                    return;
                }
            }

            if (ImGui.IsLastItemActive())
            {
                if (!Floating && ImGui.IsMouseDragging(0, 20.0f))
                {
                    Undock();
                    MainWindow.Instance.DockPane.RecalculateLayout();
                }
                else if(Floating)
                {
                    _dockTarget = DockingHelpers.TryFindDockTarget(ref _dockType, ref _dockDirection);
                }
            }
        }

        public bool Floating { get; set; } = true;
        public bool WindowClosed { get; private set; }
        public bool WindowVisible { get; private set; }
        public int WindowId { get; internal set; }
        public Rectangle WindowRect => Rect;
        public abstract string WindowName { get; }
        public virtual WindowFlags WindowSettings => WindowFlags.Default;
    }
}
