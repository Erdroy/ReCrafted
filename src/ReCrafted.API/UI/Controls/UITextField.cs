// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Linq;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;

//TODO: Fix white space problems in TextField and SelectableText.
//TODO: Line selection based on mouse Y when mouse X is out of text region. (currently selection is set always to end of the text)

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
        /// On submit event of text field. (Available only when MultipleLines is disabled)
        /// </summary>
        public Action OnSubmit;

        /// <summary>
        /// On value changed of text field.
        /// </summary>
        public Action<string> OnValueChanged;

        /// <summary>
        /// Creates new UITextField.
        /// </summary>
        public UITextField()
        {
            ApplyDefaults(new RectangleF(), string.Empty, Color.Black, UIControlColors.Default);
        }

        /// <summary>
        /// Creates new UITextField.
        /// </summary>
        /// <param name="region">Region of new UITextField.</param>
        public UITextField(RectangleF region)
        {
            ApplyDefaults(region, string.Empty, Color.Black, UIControlColors.Default);
        }

        /// <summary>
        /// Creates new UITextField.
        /// </summary>
        /// <param name="text">Text of new UITextField.</param>
        public UITextField(string text)
        {
            ApplyDefaults(new RectangleF(), text, Color.Black, UIControlColors.Default);
        }

        /// <summary>
        /// Creates new UITextField.
        /// </summary>
        /// <param name="region">Region of new UITextField.</param>
        /// <param name="text">Text of new UITextField.</param>
        public UITextField(RectangleF region, string text)
        {
            ApplyDefaults(region, text, Color.Black, UIControlColors.Default);
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
            AnyTextFieldFocused = value;

            // if focus of input field is set to false, restart selection
            if (!value)
            {
                _selectableText.ResetSelection();
                _textFieldPosition = 0;
            }
            else
            {
                // if current text is empty, fix position beam
                if (string.IsNullOrEmpty(_text))
                {
                    _selectableText.ResetSelection();
                    _textFieldPosition = 0;
                }
            }

            if (!SmoothColors)
                _textFieldEditBoxColor = value ? Colors.ClickColor : Color.Transparent;
        }

        // draw text field
        public override void Draw()
        {
            if (!Enabled)
                return;

            if (EnableClipping)
                UIInternal.BeginViewRect(Region);

            #region DRAW

            if (!string.IsNullOrEmpty(_selectableText.SelectedText) && Input.IsKey(Keys.Control) &&
                Input.IsKeyDown(Keys.C))
            {
                Clipboard.SetText(_selectableText.SelectedText);
            }

            if (SmoothColors)
            {
                _color = Color.Lerp(_color, IsMouseOver ? Colors.OverColor : Colors.NormalColor,
                    (float) Time.DeltaTime * SmoothTranslation);
            }

            UIInternal.Depth = Depth;
            UIInternal.Color = _color;
            UIInternal.DrawBox(Region);

            var buttonRegion = Region;
            if (SpringAnimation)
            {
                var target = new Vector2(buttonRegion.Width, buttonRegion.Height) * (IsFocused ? 1f : 0.5f);
                UIAnimation.SpringVector2(ref _textFieldEditBoxSize, ref _textFieldEditBoxVelocity, target,
                    (float) Time.DeltaTime);
                buttonRegion = new RectangleF(buttonRegion.X - (_textFieldEditBoxSize.X / 2f - buttonRegion.Width / 2f),
                    buttonRegion.Y - (_textFieldEditBoxSize.Y / 2f - buttonRegion.Height / 2f),
                    _textFieldEditBoxSize.X, _textFieldEditBoxSize.Y);
            }

            if (SmoothColors)
            {
                _textFieldEditBoxColor = Color.Lerp(_textFieldEditBoxColor,
                    IsFocused ? Colors.ClickColor : Color.Transparent,
                    (float) Time.DeltaTime * SmoothTranslation * (IsFocused ? 1f : 2f));
            }

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

            // if input field is focused, handle input
            if (!IsFocused)
                return;

            if (!string.IsNullOrEmpty(_text))
            {
                var keys = KeyboardBuffer.Read();
                foreach (var key in keys)
                {
                    if (RemoveSelectedText())
                        return;
                }
            }

            // if input field text is empty and position beam isn't zero, fix!
            if (string.IsNullOrEmpty(_text) && _textFieldPosition != 0)
            {
                _selectableText.ResetSelection();
                _textFieldPosition = 0;
            }

            var type = false;
            if (!string.IsNullOrEmpty(_text) && (Input.IsKeyDown(Keys.Backspace) || Input.IsKey(Keys.Backspace))) // handle backspace
            {
                if (_text.Length >= 0)
                {
                    _textFieldWaitForNextKey1 += Time.DeltaTime;
                    if (Input.IsKeyDown(Keys.Backspace) ||
                        _textFieldWaitForNextKey1 > 0.5f && _textFieldWaitForNextKey2 < 0f)
                    {
                        _text = _text.Remove(_textFieldPosition, 1);
                        _textFieldPosition--;
                        _textFieldWaitForNextKey2 = 0.05f;
                        OnValueChanged?.Invoke(_text);
                    }
                    else
                    {
                        _textFieldWaitForNextKey2 -= Time.DeltaTime;
                    }
                }
            }
            else if (Input.IsKeyDown(Keys.Enter) || Input.IsKey(Keys.Enter)) // handle enter
            {
                if (MultipleLine)
                {
                    if (CharactersLimit == 0 || _text.Length <= CharactersLimit)
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
                            OnValueChanged?.Invoke(_text);
                        }
                        else
                        {
                            _textFieldWaitForNextKey2 -= Time.DeltaTime;
                        }
                    }
                }
                else
                {
                    if (Input.IsKeyDown(Keys.Enter))
                    {
                        OnSubmit?.Invoke();
                    }
                }
            }
            else if (Input.IsKeyDown(Keys.Left) || Input.IsKey(Keys.Left)) // handle left arrow
            {
                _textFieldWaitForNextKey1 += Time.DeltaTime;
                if (Input.IsKeyDown(Keys.Left) ||
                    _textFieldWaitForNextKey1 > 0.5f && _textFieldWaitForNextKey2 < 0f)
                {
                    MovePositionLeft();
                    _textFieldWaitForNextKey2 = 0.1f;
                }
                else
                {
                    _textFieldWaitForNextKey2 -= Time.DeltaTime;
                }
            }
            else if (Input.IsKeyDown(Keys.Right) || Input.IsKey(Keys.Right)) // handle right arrow
            {
                _textFieldWaitForNextKey1 += Time.DeltaTime;
                if (Input.IsKeyDown(Keys.Right) ||
                    _textFieldWaitForNextKey1 > 0.5f && _textFieldWaitForNextKey2 < 0f)
                {
                    MovePositionRight();
                    _textFieldWaitForNextKey2 = 0.1f;
                }
                else
                {
                    _textFieldWaitForNextKey2 -= Time.DeltaTime;
                }
            }
            else if (Input.IsKeyDown(Keys.Up) || Input.IsKey(Keys.Up)) // handle up arrow
            {
                _textFieldWaitForNextKey1 += Time.DeltaTime;
                if (Input.IsKeyDown(Keys.Up) ||
                    _textFieldWaitForNextKey1 > 0.5f && _textFieldWaitForNextKey2 < 0f)
                {
                    MovePositionUp();
                    _textFieldWaitForNextKey2 = 0.1f;
                }
                else
                {
                    _textFieldWaitForNextKey2 -= Time.DeltaTime;
                }
            }
            else if (Input.IsKeyDown(Keys.Down) || Input.IsKey(Keys.Down)) // handle down arrow
            {
                _textFieldWaitForNextKey1 += Time.DeltaTime;
                if (Input.IsKeyDown(Keys.Down) ||
                    _textFieldWaitForNextKey1 > 0.5f && _textFieldWaitForNextKey2 < 0f)
                {
                    MovePositionDown();
                    _textFieldWaitForNextKey2 = 0.1f;
                }
                else
                {
                    _textFieldWaitForNextKey2 -= Time.DeltaTime;
                }
            }
            else
            {
                if (CharactersLimit == 0 || _text.Length <= CharactersLimit)
                {
                    _textFieldWaitForNextKey1 = 0f;
                    // read input
                    var keys = KeyboardBuffer.Read();
                    foreach (var c in keys)
                    {
                        if (CharacterBlackList.Contains(c))
                            break;

                        if (_text.Length == 0) // if text is empty, set received character as text.
                        {
                            _text = c.ToString();
                            _textFieldPosition = -1;
                        }
                        else
                        {
                            if (_textFieldPosition == _text.Length ) // if position beam is at end of text, just add new character
                                _text += c.ToString();
                            else if (_textFieldPosition == -1) // if position is at start of the text, insert from zero index
                                _text = _text.Insert(0, c.ToString());
                            else // default text insert
                            {
                                _text = _text.Insert(_textFieldPosition + 1, c.ToString());
                            }
                        }
                        // fix position of beam to be always before last added character
                        _textFieldPosition++;
                        type = true;
                        OnValueChanged?.Invoke(_text);
                    }
                }
            }

            /*
            // some debug stuff
            UIInternal.Color = Color.White;
            pos.Y -= TextFont.Size * 2;
            UIInternal.DrawString(TextFont.NativePtr, "TextFieldPosition -> " + _textFieldPosition, ref pos);
            pos.Y -= TextFont.Size * 2;
            UIInternal.DrawString(TextFont.NativePtr, "TextFieldLine -> " + _selectableText.GetLineFromCharIndex(_textFieldPosition), ref pos);
            pos.Y -= TextFont.Size * 2;
            UIInternal.DrawString(TextFont.NativePtr, "TextFieldSize -> " + Text.Length, ref pos);

            // get edited line
            var nextLine = _selectableText.GetLineFromCharIndex(_textFieldPosition + 1);

            // move to next line
            nextLine++;

            pos.Y -= TextFont.Size * 2;
            UIInternal.DrawString(TextFont.NativePtr, "NextLine -> " + nextLine, ref pos);

            // fix resolved line to avoid any exceptions
            _selectableText.IsLineExist(ref nextLine);

            pos.Y -= TextFont.Size * 2;
            UIInternal.DrawString(TextFont.NativePtr, "FixedNextLine -> " + nextLine, ref pos);

            // get last character of resolved line
            bool endOfLineStatex;
            int endOfLinex = _selectableText.GetLastCharIndexOfLine(nextLine, out endOfLineStatex) - 2;

            pos.Y -= TextFont.Size * 2;
            UIInternal.DrawString(TextFont.NativePtr, "EndOfNextLine -> " + endOfLinex, ref pos);

            UIInternal.Color = Color.White;
            var lines = _selectableText.GetLines();
            for (int lineIndex = 0; lineIndex < lines.Count; lineIndex++)
            {
                bool endOfLineState;
                int endOfLine = _selectableText.GetLastCharIndexOfLine(lineIndex, out endOfLineState);
                pos = _selectableText.GetPointFromCharIndex(endOfLine);
                pos.X -= 270;

                bool success;
                UIInternal.DrawString(TextFont.NativePtr, $"L{lineIndex},  F-{Text[_selectableText.GetFirstCharIndexOfLine(lineIndex)]},  SIZ-{lines[lineIndex].Length},  TOT-{endOfLine}({endOfLineState})", ref pos);
            }
            UIInternal.Color = Color.Black;
            */

            // resolve current beam position
            var charPosition = FixedCharacterPoint(_textFieldPosition);

            // beam position
            var beamPosition = new RectangleF(charPosition.X, charPosition.Y - 0.1f, 2, TextFont.Size + 0.2f);

            // animate beam color
            _textFieldPositionColor = Color.Lerp(_textFieldPositionColor,
                new Color(14 / 255f, 80 / 255f, 186 / 255f, 100 / 255f), (float) Time.DeltaTime * 3f);
            if (_textFieldPositionColor.A < 110f || type)
                _textFieldPositionColor.A = 255;

            // draw position beam
            UIInternal.Depth = Depth + 0.3f;
            UIInternal.Color = _textFieldPositionColor;
            UIInternal.DrawBox(beamPosition);

            #endregion

            if (EnableClipping)
                UIInternal.EndViewRect();
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

        private Vector2 FixedCharacterPoint(int characterPosition)
        {
            return _text.Length == 0
                ? _selectableText.GetPointFromCharIndex(0)
                : (characterPosition == 0
                    ? _selectableText.GetPointFromCharIndex(0) +
                      new Vector2(TextFont.MeasureString(_text[0].ToString()).X, 0)
                    : _selectableText.GetPointFromCharIndex(characterPosition));
        }

        // move text field position to left
        private void MovePositionLeft()
        {
            if (_textFieldPosition - 1 < -1) return;
            _textFieldPosition--;
        }

        // move text field position to right
        private void MovePositionRight()
        {
            if (_textFieldPosition + 1 >= _text.Length) return;
            _textFieldPosition++;
        }

        // move text field position up
        private void MovePositionUp()
        {
            var charPoint = _selectableText.GetPointFromCharIndex(_textFieldPosition);
            var point = new Vector2(charPoint.X, charPoint.Y - TextFont.Size / 2f);
            bool exist;
            var newPosition = _selectableText.GetCharIndexFromPoint(point, out exist);
            if (exist) // if character above current exist, just set new position
                _textFieldPosition = newPosition;
            else
            {
                // get edited line
                var nextLine = _selectableText.GetLineFromCharIndex(_textFieldPosition + 1);

                // move to previous line
                nextLine--;

                // fix resolved line to avoid any exceptions
                _selectableText.IsLineExist(ref nextLine);

                // get last character of resolved line
                bool endOfLineState;
                int endOfLine = _selectableText.GetLastCharIndexOfLine(nextLine, out endOfLineState) - 2;

                // apply new position of IBeam
                _textFieldPosition = endOfLine;
            }
        }

        // move text field position down
        private void MovePositionDown()
        {
            var charPoint = _selectableText.GetPointFromCharIndex(_textFieldPosition);
            var point = new Vector2(charPoint.X, charPoint.Y + TextFont.Size * 1.5f);
            bool exist;
            var newPosition = _selectableText.GetCharIndexFromPoint(point, out exist);
            if (exist) // if character below current exist, just set new position
                _textFieldPosition = newPosition;
            else
            {
                // get edited line
                var nextLine = _selectableText.GetLineFromCharIndex(_textFieldPosition + 1);

                // move to next line
                nextLine++;

                // fix resolved line to avoid any exceptions
                _selectableText.IsLineExist(ref nextLine);

                // get last character of resolved line
                bool endOfLineState;
                int endOfLine = _selectableText.GetLastCharIndexOfLine(nextLine, out endOfLineState) - 2;

                // apply new position of IBeam
                _textFieldPosition = endOfLine;
            }
        }

        // removes selected text
        private bool RemoveSelectedText()
        {
            if (string.IsNullOrEmpty(_selectableText.SelectedText)) return false;
            //TODO: Remove selected text.

            return false;
        }

        // set default properties
        private void ApplyDefaults(RectangleF region, string text, Color textColor, UIControlColors colors)
        {
            Region = region;
            Enabled = true;
            IgnoreMouseCollision = false;
            IsMouseOver = false;
            Parent = null;

            Colors = colors;
            SmoothColors = true;
            SmoothTranslation = 10f;
            SpringAnimation = true;
            TextColor = textColor;
            CharactersLimit = 0;
            FitCharactersLimitToSize = false;
            MultipleLine = true;
            CharacterBlackList = new[] { '`' };
            EnableClipping = false;
            TextFont = DefaultFont; //set default font
            Text = text;

            _color = Colors.NormalColor;
            _textFieldEditBoxColor = Color.Transparent;
            _textFieldEditBoxSize = new Vector2(region.Width, region.Height);
            _textFieldEditBoxVelocity = Vector2.One;
            _selectableText = new UISelectableText();
            _selectableText.OnCharacterClick += position =>
            {
                _textFieldPosition = position;
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
        /// Is text field currently using spring animation.
        /// </summary>
        public bool SpringAnimation { get; set; }

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
        /// Is text field are able to use multiple lines? 
        /// </summary>
        public bool MultipleLine { get; set; }

        /// <summary>
        /// List of character that can't be used.
        /// </summary>
        public char[] CharacterBlackList { get; set; } = new char[0];

        /// <summary>
        /// Enable text field clipping.
        /// </summary>
        public bool EnableClipping { get; set; }

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
