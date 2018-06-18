// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;

namespace ReCrafted.UI
{
    public class UIControl : IDisposable
    {
        private readonly List<UIControl> _controls = new List<UIControl>();
        private readonly UIBuffer _buffer = new UIBuffer();

        internal UIControl() { }

        public UIControl(UIControl parent)
        {
            Parent = parent;
            DirtyFlags = UIDirtyFlags.Layout | UIDirtyFlags.VertexBuffer;
        }

        internal void Update()
        {
            if (DirtyFlags.HasFlag(UIDirtyFlags.Layout))
            {
                // Also rebuild layout of children nodes (recursive -> down)
                foreach (var child in _controls)
                    child.DirtyFlags |= UIDirtyFlags.Layout;

                RebuildLayout();
            }

            if (DirtyFlags.HasFlag(UIDirtyFlags.VertexBuffer))
                RebuildVertexBuffer(_buffer);

            ResetDirtyFlags();

            foreach (var child in _controls)
                child.Update();
        }

        public void Dispose()
        {
            _controls?.Clear();
            _buffer?.Dispose();
        }

        public virtual void RebuildLayout() { }

        public virtual void RebuildVertexBuffer(UIBuffer buffer) { }

        public T AddControl<T>() where T : UIControl, new()
        {
            var control = new T();
            _controls.Add(control);
            return control;
        }

        public void RemoveControl<T>(T control) where T : UIControl
        {
            _controls.Remove(control);
        }

        public void ResetDirtyFlags()
        {
            DirtyFlags = UIDirtyFlags.None;
        }

        public UIDirtyFlags DirtyFlags { get; private set; }

        public UITransform Transform { get; set; }

        public UIControl Parent { get; internal set; }

        public UIBuffer Buffer => _buffer;

        public IReadOnlyList<UIControl> Controls => _controls;
    }
}
