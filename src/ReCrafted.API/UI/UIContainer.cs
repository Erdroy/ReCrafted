// ReCrafted © 2016-2017 Always Too Late

using System;
using System.CodeDom;
using System.Collections.Generic;
using System.Linq;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI.Controls;

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
        public IReadOnlyList<UIControl> Controls => _controls;

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
        /// <exception cref="ArgumentNullException">Exception is thrown when given instance of control is null.</exception>
        /// <exception cref="ReCraftedException">Exception is thrown when given instance of control already exists in container.</exception>
        public T Add<T>(T instance) where T : UIControl
        {
            if (instance == null) throw new ArgumentNullException(nameof(instance));
            if (_controls.Contains(instance)) throw new ReCraftedException("Unable to add new control in to container. Controls can by added in to container only once.");
            instance.Parent = this;
            instance.Depth = Depth + _controls.Count;
            _controls.Add(instance);
            OnControlsChanged?.Invoke();
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
            OnControlsChanged?.Invoke();
        }

        #region SIMPLIFY OF ADDING CONTROLS

        #region BOX
        /// <summary>
        /// Adds new box to the container.
        /// </summary>
        /// <returns>Our newly created box.</returns>
        public UIBox AddBox() 
        {
            var instance = UIBox.Create();
            Add(instance);
            return instance;
        }

        /// <summary>
        /// Adds new box to the container.
        /// </summary>
        /// <param name="texture">Texture of box.</param>
        /// <returns>Our newly created box.</returns>
        public UIBox AddBox(Texture2D texture)
        {
            var instance = UIBox.Create(new RectangleF(), texture);
            Add(instance);
            return instance;
        }

        /// <summary>
        /// Adds new box to the container.
        /// </summary>
        /// <param name="region">Region of the box.</param>
        /// <returns>Our newly created box.</returns>
        public UIBox AddBox(RectangleF region)
        {
            var instance = UIBox.Create(region);
            Add(instance);
            return instance;
        }

        /// <summary>
        /// Adds new box to the container.
        /// </summary>
        /// <param name="region">Region of the box.</param>
        /// <param name="texture">Texture of box.</param>
        /// <returns>Our newly created box.</returns>
        public UIBox AddBox(RectangleF region, Texture2D texture)
        {
            var instance = UIBox.Create(region, texture);
            Add(instance);
            return instance;
        }

        /// <summary>
        /// Adds new box to the container.
        /// </summary>
        /// <param name="color">Color of box.</param>
        /// <returns>Our newly created box.</returns>
        public UIBox AddBox(Color color)
        {
            var instance = UIBox.Create(new RectangleF(), color);
            Add(instance);
            return instance;
        }

        /// <summary>
        /// Adds new box to the container.
        /// </summary>
        /// <param name="texture">Texture of box.</param>
        /// <param name="color">Color of box.</param>
        /// <returns>Our newly created box.</returns>
        public UIBox AddBox(Texture2D texture, Color color)
        {
            var instance = UIBox.Create(new RectangleF(), texture, color);
            Add(instance);
            return instance;
        }

        /// <summary>
        /// Adds new box to the container.
        /// </summary>
        /// <param name="region">Region of the box/</param>
        /// <param name="color">Color of box.</param>
        /// <returns>Our newly created box.</returns>
        public UIBox AddBox(RectangleF region, Color color)
        {
            var instance = UIBox.Create(region, color);
            Add(instance);
            return instance;
        }

        /// <summary>
        /// Adds new box to the container.
        /// </summary>
        /// <param name="region">Region of the box/</param>
        /// <param name="texture">Texture of box.</param>
        /// <param name="color">Color of box.</param>
        /// <returns>Our newly created box.</returns>
        public UIBox AddBox(RectangleF region, Texture2D texture, Color color)
        {
            var instance = UIBox.Create(region, texture, color);
            Add(instance);
            return instance;
        }
        #endregion

        #region TEXT
        /// <summary>
        /// Adds new text to the container.
        /// </summary>
        /// <param name="text">Text of the uitext.</param>
        /// <returns>Our newly created text..</returns>
        public UIText AddText(string text)
        {
            var instance = UIText.Create(text);
            Add(instance);
            return instance;
        }

        /// <summary>
        /// Adds new text to the container.
        /// </summary>
        /// <param name="text">Text of the uitext.</param>
        /// <param name="color">Color of the text.</param>
        /// <returns>Our newly created text.</returns>
        public UIText AddText(string text, Color color)
        {
            var instance = UIText.Create(new RectangleF(), text, color);
            Add(instance);
            return instance;
        }

        /// <summary>
        /// Adds new text to the container.
        /// </summary>
        /// <param name="region">Region of the text.</param>
        /// <param name="text">Text of the uitext.</param>
        /// <returns>Our newly created text.</returns>
        public UIText AddText(RectangleF region, string text)
        {
            var instance = UIText.Create(region, text);
            Add(instance);
            return instance;
        }

        /// <summary>
        /// Adds new text to the container.
        /// </summary>
        /// <param name="region">Region of the text.</param>
        /// <param name="text">Text of the uitext.</param>
        /// <param name="color">Color of the text.</param>
        /// <returns>Our newly created text.</returns>
        public UIText AddText(RectangleF region, string text, Color color)
        {
            var instance = UIText.Create(region, text, color);
            Add(instance);
            return instance;
        }
        #endregion

        #region BUTTON
        /// <summary>
        /// Adds new button to the container.
        /// </summary>
        /// <param name="text">Text of the button.</param>
        /// <param name="onClick">When button has been clicked.</param>
        /// <returns>Our newly created button.</returns>
        public UIButton AddButton(string text, ButtonClick onClick)
        {
            var instance = UIButton.Create(text);
            Add(instance);
            instance.OnClick = onClick;
            return instance;
        }

        /// <summary>
        /// Adds new button to the container.
        /// </summary>
        /// <param name="text">Text of the button.</param>
        /// <param name="colors">Colors of button.</param>
        /// <param name="onClick">When button has been clicked.</param>
        /// <returns>Our newly created button.</returns>
        public UIButton AddButton(string text, UIButtonColors colors, ButtonClick onClick)
        {
            var instance = UIButton.Create(new RectangleF(), text, Color.Black, colors);
            Add(instance);
            instance.OnClick = onClick;
            return instance;
        }

        /// <summary>
        /// Adds new button to the container.
        /// </summary>
        /// <param name="region">Region of button.</param>
        /// <param name="text">Text of the button.</param>
        /// <param name="onClick">When button has been clicked.</param>
        /// <returns>Our newly created button.</returns>
        public UIButton AddButton(RectangleF region, string text, ButtonClick onClick)
        {
            var instance = UIButton.Create(region, text);
            Add(instance);
            instance.OnClick = onClick;
            return instance;
        }

        /// <summary>
        /// Adds new button to the container.
        /// </summary>
        /// <param name="region">Region of button.</param>
        /// <param name="text">Text of the button.</param>
        /// <param name="colors">Colors of button.</param>
        /// <param name="onClick">When button has been clicked.</param>
        /// <returns>Our newly created button.</returns>
        public UIButton AddButton(RectangleF region, string text, UIButtonColors colors, ButtonClick onClick)
        {
            var instance = UIButton.Create(region, text, Color.Black, colors);
            Add(instance);
            instance.OnClick = onClick;
            return instance;
        }

        /// <summary>
        /// Adds new button to the container.
        /// </summary>
        /// <param name="region">Region of button.</param>
        /// <param name="text">Text of the button.</param>
        /// <param name="textColor">Color of button text.</param>
        /// <param name="colors">Colors of button.</param>
        /// <param name="onClick">When button has been clicked.</param>
        /// <returns>Our newly created button.</returns>
        public UIButton AddButton(RectangleF region, string text, Color textColor, UIButtonColors colors, ButtonClick onClick)
        {
            var instance = UIButton.Create(region, text, textColor, colors);
            Add(instance);
            instance.OnClick = onClick;
            return instance;
        }
        #endregion

        #endregion

        // recalculate depth of all children controls
        internal void RecalculateDepth()
        {
            for (var index = 0; index < _controls.Count; index++)
            {
                var control = _controls[index];
                control.Depth = Depth + index;
            }
        }

        // some stuff with mouse collision on controls
        internal bool LookForMouseCollision()
        {
            if (!Enabled) return true;
            UIControl mouseControlCollision = null;
            var mousePoint = Input.CursorPosition;
            var reversed = Controls.Reverse();
            foreach (var control in reversed)
            {
                if (!control.OnMouseCollision()) continue;
                if (control.IgnoreMouseCollision || !control.Enabled) continue;
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
