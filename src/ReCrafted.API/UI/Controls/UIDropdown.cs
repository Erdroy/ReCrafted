// ReCrafted © 2016-2017 Always Too Late

using System.Collections.Generic;
using System.Linq;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// UI Drop down class.
    /// </summary>
    public class UIDropdown : UIButton
    {
        // current values of drop down
        private List<string> _values = new List<string>();

        // buttons of current values
        private List<UIButton> _buttons = new List<UIButton>();

        // is list of values displayed?
        private bool _displayList;

        // currently selected item
        private int _selectedItem;

        // layout container of drop down list
        private UIPanel _buttonsContainer;

        private UIBox _rightImage;

        private bool _fixed;

        /// <summary>
        /// Creates new UI Drop down.
        /// </summary>
        public UIDropdown()
        {
            _applyDefaults(new RectangleF(), UIControlColors.Default, Color.Black, UIControlColors.Default);
        }

        /// <summary>
        /// Creates new UI Drop down.
        /// </summary>
        /// <param name="region">Region of drop down.</param>
        public UIDropdown(RectangleF region)
        {
            _applyDefaults(region, UIControlColors.Default, Color.Black, UIControlColors.Default);
        }

        // draw drop down
        public override void Draw()
        {
            base.Draw();

            FixedTextPosition = new Vector2(-Region.Height, 0);
            _rightImage.Depth = Depth + 1;
            _rightImage.Sprite = _displayList ? DefaultArrowUp : DefaultArrowDown;
            _rightImage.Region = new RectangleF(Region.X + Region.Width - Region.Height, Region.Y, Region.Height, Region.Height);
            _rightImage.Draw();

            if (_displayList && _values.Count != 0)
            {
                var height = ButtonHeight * _values.Count;
                if (ListHeight > ButtonHeight && height > ListHeight)
                {
                    _buttonsContainer.EnableScrollBars = true;
                    height = ListHeight;
                }
                else
                {
                    _buttonsContainer.EnableScrollBars = false;
                }

                _buttonsContainer.Region = new RectangleF(Region.X,
                                                          Region.Y + Region.Height,
                                                          Region.Width,
                                                          height);
                _buttonsContainer.Draw();
            }

            // fix
            if (!_fixed)
            {
                Reset();
                _fixed = true;
            }
            else
            {
                if (!IsMouseOver &&
                    !_buttonsContainer.IsMouseOver &&
                    !_buttonsContainer.Layout.IsMouseOverContent &&
                    !_buttonsContainer._internalPanel.Layout.IsMouseOverContent &&
                    _displayList && Input.IsKeyDown(Keys.Mouse0))
                {
                    _displayList = false;
                }
            }
        }

        // reset drop down
        public override void Reset()
        {
            _buttonsContainer?.Reset();
        }

        // set default properties
        private void _applyDefaults(RectangleF region, UIControlColors itemColors, Color itemTextColor,
            UIControlColors backgroundColors)
        {
            Region = region;
            TextFont = DefaultFont; //set default font
            Text = string.Empty;
            TextColor = itemTextColor;
            Colors = itemColors;
            SmoothColors = true;
            SmoothTranslation = 10f;
            SpringAnimation = false;
            Sprite = null;

            Enabled = true;
            IgnoreMouseCollision = false;
            IsMouseOver = false;
            Parent = null;

            _color = Colors.NormalColor;
            _buttonSize = new Vector2(Region.Width, Region.Height);
            _buttonVelocity = Vector2.Zero;

            ItemColors = itemColors;
            ItemTextColor = itemTextColor;
            BackgroundColors = backgroundColors;

            ButtonHeight = 30;
            ListHeight = 120;

            _buttonsContainer = UIPanel.Create(region, UILayoutType.Vertical, $"{Name}-List", Depth);
            _buttonsContainer.Parent = this;
            _buttonsContainer.EnableClipping = true;
            _buttonsContainer.EnableScrollBars = true;

            _buttonsContainer.HorizontalScrollBar = false;
            _buttonsContainer.VerticalScrollBar = true;

            _buttonsContainer.Layout.Space = 0;
            _buttonsContainer.Layout.ForceExpandWidth = true;
            _buttonsContainer.Layout.ForceExpandHeight = true;

            _buttonsContainer.Layout.Alignment = UILayoutAlignment.MiddleBottom;

            _rightImage = new UIBox();

            OnClick += OnValueClick;
        }

        // when current value of drop down was click
        private void OnValueClick()
        {
            _displayList = !_displayList;
        }

        // when value from drop down list was selected
        private void OnValueSelected(int index)
        {
            Logger.Write($"Select {index}");
            _displayList = false;
            Value = index;
        }

        /// <summary>
        /// Colors of button.
        /// </summary>
        public UIControlColors ItemColors { get; set; }

        /// <summary>
        /// Color of checkbox in toggle.
        /// </summary>
        public UIControlColors BackgroundColors { get; set; }

        /// <summary>
        /// Color of the text.
        /// </summary>
        public Color ItemTextColor { get; set; }

        /// <summary>
        /// Values of drop down.
        /// </summary>
        public IReadOnlyList<string> Values
        {
            get { return _values; }
            set
            {
                if (_values.Count != value.Count)
                {
                    if (_values.Count > value.Count)
                    {
                        var index = _values.Count;
                        while (index > value.Count)
                        {
                            var indexOf = _values.IndexOf(_values.Last());

                            if (indexOf == _selectedItem)
                                Value = indexOf - 1;

                            _values.RemoveAt(indexOf);
                            _buttons.RemoveAt(indexOf);
                            if (_buttonsContainer.Layout.Controls.Count < indexOf)
                                _buttonsContainer.Remove(_buttonsContainer.Layout.Controls[indexOf]);
                            
                            index--;
                        }
                    }
                    else
                    {
                        var index = _values.Count;
                        while (index < value.Count)
                        {
                            _values.Add(string.Empty);
                            var button = _buttonsContainer.Add(new UIButton(new RectangleF(), $"Item {index}",
                                ItemTextColor, ItemColors));
                            button.SpringAnimation = false;
                            button.TextFont = TextFont;
                            button.PreferredSize = new Vector2(0, ButtonHeight);
                            var index1 = index;
                            button.OnClick += () =>
                            {
                                OnValueSelected(index1);
                            };
                            _buttons.Add(button);
                            index++;
                        }

                    }
                }

                for (var valueIndex = 0; valueIndex < value.Count; valueIndex++)
                {
                    _values[valueIndex] = value[valueIndex];
                    _buttons[valueIndex].Text = value[valueIndex];
                }
            }
        }

        /// <summary>
        /// Value of drop down.
        /// </summary>
        public int Value {
            get { return _selectedItem ; }
            set
            {
                _selectedItem = value;
                _selectedItem = MathUtil.Clamp(_selectedItem, 0, _values.Count - 1);

                Text = SelectedText;
            }
        }

        /// <summary>
        /// Text of currently selected item.
        /// </summary>
        public string SelectedText => _values.Count == 0 ? string.Empty : _values[_selectedItem];

        /// <summary>
        /// Height of item button.
        /// </summary>
        public int ButtonHeight { get; set; }

        /// <summary>
        /// Height of displayed list.
        /// </summary>
        public int ListHeight { get; set; }
    }
}
