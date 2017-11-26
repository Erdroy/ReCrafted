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
        private readonly List<UIControl> _controls = new List<UIControl>();

        /// <summary>
        /// Draws all controls.
        /// </summary>
        public override void Draw()
        {
            foreach (var control in _controls)
                control.Draw();
        }

        /// <summary>
        /// Adds new control to the container and then returns it.
        /// </summary>
        /// <typeparam name="T">Control class which must inherit from UIControl.</typeparam>
        /// <param name="instance">The control instance.</param>
        /// <returns>The control instance, allows to modify preferences easily.</returns>
        /// <exception cref="System.ArgumentNullException">Exception is thrown when given instance of control is null.</exception>
        /// <exception cref="ReCraftedException">Exception is thrown when given instance of control already exists in container.</exception>
        public T Add<T>(T instance) where T : UIControl
        {
            if (instance == null) throw new ArgumentNullException(nameof(instance));
            if (_controls.Contains(instance)) throw new ReCraftedException("Unable to add new control in to container. Controls can by added in to container only once.");
            instance.Parent = this;
            _controls.Add(instance);
            return instance;
        }

        /// <summary>
        /// Removes given control from the container.
        /// </summary>
        /// <typeparam name="T">Control class which must inherit from UIControl.</typeparam>
        /// <param name="instance">The control instance.</param>
        /// <exception cref="ArgumentNullException">Exception is thrown when givent instance of controll is null.</exception>
        public void Remove<T>(T instance) where T : UIControl
        {
            if (instance == null) throw new ArgumentNullException(nameof(instance));
            if (!_controls.Contains(instance)) return;
            _controls.Remove(instance);
        }

        /// <summary>
        /// Contains all the controls
        /// </summary>
        public IReadOnlyList<UIControl> Controls => _controls;

        //internal
        internal bool LookForMouseCollision()
        {
            if (!Enabled) return true;
            UIControl mouseControlCollision = null;
            var mousePoint = Input.CursorPosition;
            var reversed = Controls.Reverse();
            foreach (var control in reversed)
            {
                if (!control.OnMouseCollision()) continue;
                if (control.IgnoreMouseCollision) continue;
                if (!control.Region.Contains(mousePoint) || mouseControlCollision != null && mouseControlCollision != control)
                {
                    if (!control.IsMouseOver) continue;
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
                        if (Input.IsKeyDown(Keys.Mouse0))
                            control.OnMouseClick();
                    }

                    mouseControlCollision = control;
                }
            }
            return mouseControlCollision == null;
        }
    }
}
