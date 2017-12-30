// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using System.Linq;
using ReCrafted.API.Common;
using ReCrafted.API.Core;

namespace ReCrafted.API.UI
{
    /// <summary>
    /// UIContainer class, implements support for containing child UIControls.
    /// </summary>
    public class UIContainer : UIControl
    {
        // actual list of control
        private readonly List<UIControl> _controls = new List<UIControl>();

        // when control has been added or removed from container.
        internal Action OnControlsChanged;

        /// <summary>
        /// Contains all the controls
        /// </summary>
        public IReadOnlyList<UIControl> Controls
        {
            get
            {
                if (ReverseContainer)
                {
                    var controls = new List<UIControl>(_controls);
                    controls.Reverse();
                    return controls;
                }
                return _controls;
            }
        }

        /// <summary>
        /// Draws all controls.
        /// </summary>
        public override void Draw()
        {
            Profiler.BeginProfile("UIContainer.Draw");

            foreach (var control in Controls)
                control.Draw();
            Profiler.EndProfile();
        }

        // reset controls
        public override void Reset()
        {
            foreach (var control in Controls)
                control.Reset();
        }

        // on depth changed
        public override void OnDepthChanged()
        {
            RecalculateDepth();
        }

        /// <summary>
        /// Adds new control to the container and then returns it.
        /// </summary>
        /// <typeparam name="T">Control class which must inherit from UIControl.</typeparam>
        /// <param name="instance">The control instance.</param>
        /// <returns>The control instance, allows to modify preferences easily.</returns>
        /// <exception cref="ArgumentNullException">Exception is thrown when given instance of control is null.</exception>
        /// <exception cref="ReCraftedException">Exception is thrown when given instance of control already exists in container.</exception>
        public T Add<T>(T instance) where T : UIControl
        {
            if (instance == null) throw new ArgumentNullException(nameof(instance));
            if (instance == this) throw new NotSupportedException("You can't add container as control of container");
            if (instance is UIPanel) throw new NotSupportedException("You can't add UIPanel to container.");
            if (_controls.Contains(instance))
                throw new ReCraftedException(
                    "Unable to add new control in to container. Controls can by added in to container only once.");
            instance.Parent = this;
            instance.Depth = Depth + _controls.Count + instance.BaseDepth;
            instance.OnRegionChanged();
            _controls.Add(instance);
            OnControlsChanged?.Invoke();
            return instance;
        }

        /// <summary>
        /// Removes given control from the container.
        /// </summary>
        /// <typeparam name="T">Control class which must inherit from UIControl.</typeparam>
        /// <param name="instance">The control instance.</param>
        /// <exception cref="ArgumentNullException">Exception is thrown when given instance of control is null.</exception>
        public void Remove<T>(T instance) where T : UIControl
        {
            if (instance == null) throw new ArgumentNullException(nameof(instance));
            if (!_controls.Contains(instance)) return;
            instance.Parent = null;
            instance.OnRegionChanged();
            _controls.Remove(instance);
            OnControlsChanged?.Invoke();
        }

        // recalculate depth of all children controls
        internal void RecalculateDepth()
        {
            for (var index = 0; index < Controls.Count; index++)
            {
                var control = Controls[index];
                control.Depth = Depth + control.BaseDepth + index + 1;
                control.OnDepthChanged();
                control.OnDepthChangedAction?.Invoke();
            }
        }

        // some stuff with mouse collision on controls
        internal UIControl LookForMouseCollision()
        {
            if (!Enabled)
                return null;

            Profiler.BeginProfile("UIContainer.LookForMouseCollision");
            UIControl mouseControlCollision = null;
            var mousePoint = Input.CursorPosition;
            var reversed = Controls.Reverse();
            foreach (var control in reversed)
            {
                if (control.OnMouseCollision() != null)
                    continue;
                if (control.IgnoreMouseCollision || !control.Enabled)
                    continue;

                if (!control.Region.Contains(mousePoint) ||
                    mouseControlCollision != null && mouseControlCollision != control)
                {
                    if (!control.IsMouseOver)
                        continue;

                    control.IsMouseOver = false;
                    control.OnMouseExit();
                }
                else
                {
                    if (!control.IsMouseOver)
                    {
                        control.IsMouseOver = true;
                        control.OnMouseEnter();
                    }
                    else
                    {
                        control.OnMouseOver();
                    }

                    mouseControlCollision = control;
                }
            }

            Profiler.EndProfile();
            return mouseControlCollision;
        }

        /// <summary>
        /// Id true Container content will be displayed from end to start.
        /// </summary>
        public bool ReverseContainer { get; set; }

        /// <summary>
        /// Is mouse over on any control added to container?
        /// </summary>
        public bool IsMouseOverContent
        {
            get
            {
                if (IsMouseOver)
                    return true;

                var v = false;
                foreach (var c in _controls)
                {
                    if (c.IsMouseOver)
                        v = true;
                }
                return v;
            }
        }
    }
}
