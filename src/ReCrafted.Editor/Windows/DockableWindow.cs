// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Diagnostics;
using System.Drawing;
using System.Numerics;
using ImGuiNET;
using ReCrafted.Editor.Windows.Docking;

namespace ReCrafted.Editor.Windows
{
    public abstract class DockableWindow : DockPanelBase, IWindow
    {
        private bool _focus;
        private bool _beenActiveAndDragging;

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
            if (ImGui.IsLastItemActive() || _beenActiveAndDragging)
            {
                if (!Floating && ImGui.IsMouseDragging(0, 20.0f))
                {
                    Undock();
                    _beenActiveAndDragging = false;
                    MainWindow.Instance.DockPane.RecalculateLayout();
                }
                else if(Floating)
                {
                    TryDock();
                }
            }
        }

        private void TryDock()
        {
            var mousePos = ImGui.GetMousePos();

            // Try to find drop surface
            var panel = MainWindow.Instance.DockPane.FindIntersecting(mousePos);

            // Check if panel exists or it's parent is null
            // When panels parent is null, it is the ROOT!
            if (panel?.Parent == null)
                return;

            Debug.Assert(!(panel is DockSplitter));

            var drawList = ImGui.GetOverlayDrawList();
            var rect = panel.Rect;

            var center = new Vector2(rect.X, rect.Y) + new Vector2(rect.Width, rect.Height) * 0.5f;
            
            var innerWidth = (int)(rect.Width * 0.25f);
            var innerHeight = (int)(rect.Height * 0.35f);
            
            // Check left
            var left = new Rectangle(rect.Left, rect.Top, innerWidth, rect.Height);
            if (left.Contains((int)mousePos.X, (int)mousePos.Y))
            {
                drawList.AddRectFilled(
                    new Vector2(left.Left, left.Top),
                    new Vector2(left.Right, left.Bottom),
                    0x4FFF6900, 0.0f);
            }

            // Check right
            var right = new Rectangle(rect.Right - innerWidth, rect.Top, innerWidth, rect.Height);
            if (right.Contains((int)mousePos.X, (int)mousePos.Y))
            {
                drawList.AddRectFilled(
                    new Vector2(right.Left, right.Top),
                    new Vector2(right.Right, right.Bottom),
                    0x4FFF6900, 0.0f);
            }

            // Check bottom
            var bottom = new Rectangle(rect.Left + innerWidth, rect.Bottom - innerHeight, 
                rect.Width - innerWidth * 2, innerHeight);
            if (bottom.Contains((int)mousePos.X, (int)mousePos.Y))
            {
                drawList.AddRectFilled(
                    new Vector2(rect.Left, rect.Bottom - innerHeight),
                    new Vector2(rect.Right, rect.Bottom),
                    0x4FFF6900, 0.0f);
            }

            if (_beenActiveAndDragging && ImGui.IsMouseReleased(0))
            {
                panel.Dock(this, DockType.Vertical, DockDirection.Right);
                MainWindow.Instance.DockPane.RecalculateLayout();
                _beenActiveAndDragging = false; // TODO: Fix
            }
            else
            {
                _beenActiveAndDragging = true;
            }
        }

        public bool Floating { get; set; } = true;
        public int WindowId { get; internal set; }
        public Rectangle WindowRect => Rect;
        public abstract string WindowName { get; }
        public virtual WindowFlags WindowSettings => WindowFlags.Default;
    }
}
