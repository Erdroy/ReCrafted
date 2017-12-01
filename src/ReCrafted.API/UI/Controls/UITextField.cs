// ReCrafted © 2016-2017 Always Too Late

using System;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// UITextField control.
    /// </summary>
    public class UITextField : UIControl
    {
        // current text field color
        private Color _color;

        // current text field text;
        private string _text;

        // color of textField edit box.
        private Color _textFieldEditBoxColor;

        // textField edit box size for spring animation
        private Vector2 _textFieldEditBoxSize;

        // velocity of textField edit box spring animation
        private Vector2 _textFieldEditBoxVelocity;

        // class that adds text selection algorithms
        private UISelectableText _selectableText;

        // is text field using text selection algorithms
        private bool _usingSelectableText;

        // color of text field position beam
        private Color _textFieldPositionColor;

        // position of text editing
        private int _textFieldPosition;
        private double _textFieldWaitForNextKey1;
        private double _textFieldWaitForNextKey2;

        /// <summary>
        /// Creates new UITextField.
        /// </summary>
        public UITextField()
        {
            _applyDefaults(new RectangleF(), string.Empty, Color.Black, UIControlColors.Defaults);
        }

        /// <summary>
        /// Creates new UITextField.
        /// </summary>
        /// <param name="region">Region of new UITextField.</param>
        public UITextField(RectangleF region)
        {
            _applyDefaults(region, string.Empty, Color.Black, UIControlColors.Defaults);
        }

        public override void OnMouseEnter()
        {
            if (!SmoothColors)
                _color = Colors.OverColor;
            Cursor.Icon = CursorIcon.Beam;
        }

        public override void OnMouseOver()
        {
            if (SmoothColors)
            {
                if (Input.IsKeyUp(Keys.Mouse0) && !IsFocused)
                    _color = Colors.ClickColor;
            }
            else
            {
                _color = Input.IsKey(Keys.Mouse0) ? Colors.ClickColor : Colors.OverColor;
            }
        }

        public override void OnMouseExit()
        {
            if (!SmoothColors)
                _color = Colors.NormalColor;
            Cursor.Icon = CursorIcon.Arrow;
        }

        public override void OnControlFocused(bool value)
        {
            if (!value)
                _selectableText.ResetSelection();
            else
            {
                if (string.IsNullOrEmpty(Text))
                    _textFieldPosition = 0;
            }

            if (!SmoothColors)
                _textFieldEditBoxColor = value ? Colors.ClickColor : Color.Transparent;
            AnyTextFieldFocused = value;
        }

        // draw text field
        public override void Draw()
        {
            if (!Enabled) return;

            #region DRAW

            if (!string.IsNullOrEmpty(_selectableText.SelectedText) && Input.IsKey(Keys.Control) &&
                Input.IsKeyDown(Keys.C))
                Clipboard.SetText(_selectableText.SelectedText);

            if (SmoothColors)
                _color = Color.Lerp(_color, IsMouseOver ? Colors.OverColor : Colors.NormalColor,
                    (float) Time.DeltaTime * SmoothTranslation);
            UIInternal.Depth = Depth;
            UIInternal.Color = _color;
            UIInternal.DrawBox(Region);

            var buttonRegion = Region;
            var target = new Vector2(buttonRegion.Width, buttonRegion.Height) * (IsFocused ? 1f : 0.5f);
            UIAnimation.SpringVector2(ref _textFieldEditBoxSize, ref _textFieldEditBoxVelocity, target,
                (float) Time.DeltaTime);
            buttonRegion = new RectangleF(buttonRegion.X - (_textFieldEditBoxSize.X / 2f - buttonRegion.Width / 2f),
                buttonRegion.Y - (_textFieldEditBoxSize.Y / 2f - buttonRegion.Height / 2f),
                _textFieldEditBoxSize.X, _textFieldEditBoxSize.Y);

            if (SmoothColors)
                _textFieldEditBoxColor = Color.Lerp(_textFieldEditBoxColor,
                    IsFocused ? Colors.ClickColor : Color.Transparent,
                    (float) Time.DeltaTime * SmoothTranslation * (IsFocused ? 1f : 2f));
            UIInternal.Depth = Depth + 0.1f;
            UIInternal.Color = _textFieldEditBoxColor;
            UIInternal.DrawBox(buttonRegion);

            TextPosition = new Vector2(Region.X + 5, Region.Y + 5);
            var pos = TextPosition;

            UIInternal.Depth = Depth + 0.2f;
            UIInternal.Color = TextColor;
            UIInternal.DrawString(TextFont.NativePtr, _text, ref pos);

            _selectableText.Draw(UsingSelectableText, _text, TextFont, TextPosition, Depth + 0.2f, IsMouseOver);

            #endregion

            #region INPUT         

            /*
            if (!string.IsNullOrEmpty(_text))
            {
                var keys = KeyboardBuffer.Read();
                foreach (var key in keys)
                {
                    _removeSelectedText();
                }
            }
            */

            if (_textFieldPosition != -1)
            {
                if (!string.IsNullOrEmpty(_text) && (Input.IsKeyDown(Keys.Backspace) || Input.IsKey(Keys.Backspace)))
                {
                    _textFieldWaitForNextKey1 += Time.DeltaTime;
                    if (Input.IsKeyDown(Keys.Backspace) ||
                        _textFieldWaitForNextKey1 > 0.5f && _textFieldWaitForNextKey2 < 0f)
                    {
                        _text = _text.Remove(_textFieldPosition, 1);
                        _textFieldPosition--;
                        _textFieldWaitForNextKey2 = 0.05f;
                    }
                    else
                    {
                        _textFieldWaitForNextKey2 -= Time.DeltaTime;
                    }
                }
                else if (Input.IsKeyDown(Keys.Enter) || Input.IsKey(Keys.Enter))
                {
                    _textFieldWaitForNextKey1 += Time.DeltaTime;
                    if (Input.IsKeyDown(Keys.Enter) ||
                        _textFieldWaitForNextKey1 > 0.5f && _textFieldWaitForNextKey2 < 0f)
                    {
                        _text = string.IsNullOrEmpty(_text)
                            ? '\n'.ToString()
                            : _text.Insert(_textFieldPosition + 1, '\n'.ToString());
                        _textFieldPosition++;
                        _textFieldWaitForNextKey2 = 0.05f;
                    }
                    else
                    {
                        _textFieldWaitForNextKey2 -= Time.DeltaTime;
                    }
                }
                else if (Input.IsKeyDown(Keys.Left) || Input.IsKey(Keys.Left))
                {
                    _textFieldWaitForNextKey1 += Time.DeltaTime;
                    if (Input.IsKeyDown(Keys.Left) ||
                        _textFieldWaitForNextKey1 > 0.5f && _textFieldWaitForNextKey2 < 0f)
                    {
                        _movePositionLeft();
                        _textFieldWaitForNextKey2 = 0.1f;
                    }
                    else
                    {
                        _textFieldWaitForNextKey2 -= Time.DeltaTime;
                    }
                }
                else if (Input.IsKeyDown(Keys.Right) || Input.IsKey(Keys.Right))
                {
                    _textFieldWaitForNextKey1 += Time.DeltaTime;
                    if (Input.IsKeyDown(Keys.Right) ||
                        _textFieldWaitForNextKey1 > 0.5f && _textFieldWaitForNextKey2 < 0f)
                    {
                        _movePositionRight();
                        _textFieldWaitForNextKey2 = 0.1f;
                    }
                    else
                    {
                        _textFieldWaitForNextKey2 -= Time.DeltaTime;
                    }
                }
                else if (Input.IsKeyDown(Keys.Up) || Input.IsKey(Keys.Up))
                {
                    _textFieldWaitForNextKey1 += Time.DeltaTime;
                    if (Input.IsKeyDown(Keys.Up) ||
                        _textFieldWaitForNextKey1 > 0.5f && _textFieldWaitForNextKey2 < 0f)
                    {
                        _movePositionUp();
                        _textFieldWaitForNextKey2 = 0.1f;
                    }
                    else
                    {
                        _textFieldWaitForNextKey2 -= Time.DeltaTime;
                    }
                }
                else if (Input.IsKeyDown(Keys.Down) || Input.IsKey(Keys.Down))
                {
                    _textFieldWaitForNextKey1 += Time.DeltaTime;
                    if (Input.IsKeyDown(Keys.Down) ||
                        _textFieldWaitForNextKey1 > 0.5f && _textFieldWaitForNextKey2 < 0f)
                    {
                        _movePositionDown();
                        _textFieldWaitForNextKey2 = 0.1f;
                    }
                    else
                    {
                        _textFieldWaitForNextKey2 -= Time.DeltaTime;
                    }
                }
                else
                {
                    var keys = KeyboardBuffer.Read();
                    foreach (var key in keys)
                    {
                        if (string.IsNullOrEmpty(_text))
                            _text = key.ToString();
                        else
                        {
                            try
                            {
                                _text = _text.Insert(_textFieldPosition + 1, key.ToString());
                                _textFieldPosition++;
                            }
                            catch (Exception)
                            {
                                _text = _text.Insert(_textFieldPosition, key.ToString());
                                _textFieldPosition += 2;
                            }
                        }
                    }
                }

                var charPosition = _selectableText.GetPointFromCharIndex(_textFieldPosition);
                var beamPosition = new RectangleF(charPosition.X, charPosition.Y - 0.1f, 2, TextFont.Size + 0.2f);
                _textFieldPositionColor = Color.Lerp(_textFieldPositionColor,
                    new Color(14 / 255f, 80 / 255f, 186 / 255f, 100 / 255f), (float) Time.DeltaTime * 3f);
                if (_textFieldPositionColor.A < 110f)
                    _textFieldPositionColor.A = 255;
                UIInternal.Depth = Depth + 0.3f;
                UIInternal.Color = _textFieldPositionColor;
                UIInternal.DrawBox(beamPosition);
            }

            #endregion
        }

        public override void Reset()
        {
            _selectableText.ResetSelection();
        }

        /// <summary>
        /// Sets active text selection algorithm.
        /// </summary>
        public void SetActiveTextSelection(bool value)
        {
            _usingSelectableText = value;
            _selectableText.ResetSelection();
        }

        // move text field position to left
        private void _movePositionLeft()
        {
            if (_textFieldPosition == -1) return;
            if (_textFieldPosition - 1 <= 0) return;
            _textFieldPosition--;
        }

        // move text field position to right
        private void _movePositionRight()
        {
            if (_textFieldPosition == -1) return;
            if (_textFieldPosition + 1 >= _text.Length) return;
            _textFieldPosition++;
        }

        // move text field position up
        private void _movePositionUp()
        {
            if (_textFieldPosition == -1) return;
            var charPoint = _selectableText.GetPointFromCharIndex(_textFieldPosition);
            var point = new Vector2(charPoint.X, charPoint.Y - TextFont.Size / 2f);
            bool exist;
            var newPosition = _selectableText.GetCharIndexFromPoint(point, out exist);
            if (exist)
                _textFieldPosition = newPosition;
            else
            {
                var line = _selectableText.GetLineFromCharIndex(_textFieldPosition);
                if (line == 0)
                    return;
                _textFieldPosition = _selectableText.GetLastCharIndexOfLine(line - 1) - 1;
            }
        }

        // move text field position down
        private void _movePositionDown()
        {
            if (_textFieldPosition == -1) return;
            var charPoint = _selectableText.GetPointFromCharIndex(_textFieldPosition);
            var point = new Vector2(charPoint.X, charPoint.Y + TextFont.Size * 1.5f);
            bool exist;
            var newPosition = _selectableText.GetCharIndexFromPoint(point, out exist);
            if (exist)
                _textFieldPosition = newPosition;
            else
            {
                var lines = _selectableText.GetLines();
                var line = _selectableText.GetLineFromCharIndex(_textFieldPosition);
                if (line == lines.Count - 1)
                    return;
                _textFieldPosition = _selectableText.GetLastCharIndexOfLine(line + 1) + 1;
            }
        }

        // removes selected text
        private void _removeSelectedText()
        {
            if (string.IsNullOrEmpty(_selectableText.SelectedText)) return;
            _text = _text.Remove(_selectableText.SelectedTextStart, _selectableText.SelectedTextEnd);
            _textFieldPosition = _selectableText.SelectedTextStart;
            _selectableText.ResetSelection();
        }

        // set default properties
        private void _applyDefaults(RectangleF region, string text, Color textColor, UIControlColors colors)
        {
            Region = region;
            TextFont = DefaultFont; //set default font
            Text = text;
            TextColor = textColor;
            Colors = colors;
            SmoothColors = true;
            SmoothTranslation = 10f;
            Enabled = true;
            IgnoreMouseCollision = false;
            IsMouseOver = false;
            Parent = null;

            _color = Colors.NormalColor;
            _textFieldEditBoxColor = Color.Transparent;
            _textFieldEditBoxSize = new Vector2(region.Width, region.Height);
            _textFieldEditBoxVelocity = Vector2.One;
            _selectableText = new UISelectableText();
            _selectableText.OnCharacterClick += position =>
            {
                _textFieldPosition = position;
            };
            _selectableText.OnTextSelected += s =>
            {
                _textFieldPosition = -1;
            };
            _selectableText.OnResetSelection += () =>
            {
                _textFieldPosition = -1;
            };

            _usingSelectableText = true;
            _textFieldPosition = -1;
        }

        /// <summary>
        /// Colors of text field.
        /// </summary>
        public UIControlColors Colors { get; set; }

        /// <summary>
        /// Colors on text field will be changed smoothly.
        /// </summary>
        public bool SmoothColors { get; set; }

        /// <summary>
        /// Speed of smooth color translation.
        /// </summary>
        public float SmoothTranslation { get; set; }

        /// <summary>
        /// Color of the text.
        /// </summary>
        public Color TextColor { get; set; }

        /// <summary>
        /// Limit of characters in text that user can input.
        /// </summary>
        public int CharactersLimit { get; set; }

        /// <summary>
        /// Limit of characters in text will automatically fit to current region size and font size.
        /// </summary>
        public bool FitCharactersLimitToSize { get; set; }

        /// <summary>
        /// Text of this control.
        /// </summary>     
        public string Text
        {
            get { return _text; }
            set
            {
                _text = value;
                TextSize = TextFont.MeasureString(_text);
            }
        }

        /// <summary>
        /// Size of current text.
        /// </summary>
        public Vector2 TextSize { get; private set; }

        /// <summary>
        /// Fixed text position.
        /// </summary>
        public Vector2 TextPosition { get; private set; }

        /// <summary>
        /// Loaded font of this text.
        /// </summary>
        public Font TextFont { get; private set; }

        /// <summary>
        /// Is this text field currently using text selection.
        /// </summary>
        public bool UsingSelectableText => _usingSelectableText;

        /// <summary>
        /// Is any text field currently focused?
        /// </summary>
        public static bool AnyTextFieldFocused { get; private set; }
    }
}
