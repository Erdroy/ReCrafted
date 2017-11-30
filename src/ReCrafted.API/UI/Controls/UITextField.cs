// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
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

        // region of selection carpet
        private RectangleF[] _carpetRegions;
        // is text currently under carpet selection
        private bool _selectingText;
        // index of character where carpet selection starts
        private int _selectingTextStartIndex;

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
                if (Input.IsKeyUp(Keys.Mouse0))
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
                _resetCarpet();
            if (!SmoothColors)
                _textFieldEditBoxColor = value ? Colors.ClickColor : Color.Transparent;
            AnyTextFieldFocused = value;         
        }

        // draw text field
        public override void Draw()
        {
            if (!Enabled) return;
            #region INPUT
            if (_selectingText)
            {
                var startIndex = _selectingTextStartIndex;
                var endIndex = GetCharIndexFromCursor();
                if (startIndex > endIndex)//swap
                {
                    var s = startIndex;
                    startIndex = endIndex;
                    endIndex = s;
                }

                if (Input.IsKey(Keys.Mouse0))
                {
                    var lines = GetLines();

                    var startPoint = GetPointFromCharIndex(startIndex);
                    var startLine = GetLineFromCharIndex(startIndex);

                    var endPoint = GetPointFromCharIndex(endIndex);
                    var endLine = GetLineFromCharIndex(endIndex);
                    var endCharSize = TextFont.MeasureString(_text[endIndex].ToString());

                    _carpetRegions = new RectangleF[lines.Count];
                    var p = new Vector2(0, 0);
                    UIInternal.DrawString(TextFont.NativePtr, "Regions -> " + _carpetRegions.Length + " StartLine -> " + startLine, ref p);
                    for (int line = startLine; line < lines.Count; line++)
                    {
                        if (line > startLine && line < endLine) // full line
                        {
                            var lineSize = GetLineSize(line);
                            _carpetRegions[line] = new RectangleF(TextPosition.X, TextPosition.Y + TextFont.Size * line, lineSize.X, lineSize.Y);
                        }
                        else if (line < startLine && line > endLine)// full line
                        {
                            var lineSize = GetLineSize(line);
                            _carpetRegions[line] = new RectangleF(TextPosition.X, TextPosition.Y + TextFont.Size * line, lineSize.X, lineSize.Y);
                        }
                        else
                        {
                            if (startLine == line)
                            {
                                var endOfLine = GetEndCharIndexOfLine(startLine) - 1;
                                if (endIndex > endOfLine)
                                {
                                    if (startLine < endLine)
                                    {
                                        var pointOfEndOfLine = GetPointFromCharIndex(endOfLine);
                                        _carpetRegions[line] = new RectangleF(startPoint.X, startPoint.Y,
                                            pointOfEndOfLine.X - startPoint.X,
                                            pointOfEndOfLine.Y - startPoint.Y + TextFont.Size);
                                    }
                                }
                                else
                                {
                                    _carpetRegions[line] = new RectangleF(startPoint.X, startPoint.Y,
                                        endPoint.X - startPoint.X, endPoint.Y - startPoint.Y + TextFont.Size);
                                }
                            }
                            else if (endLine == line)
                            {
                                _carpetRegions[line] = new RectangleF(TextPosition.X,
                                    TextPosition.Y + TextFont.Size * line, endPoint.X - TextPosition.X, TextFont.Size);
                            }
                        }

                        p = new Vector2(0, (line+1) * TextFont.Size);
                        UIInternal.DrawString(TextFont.NativePtr, "Start -> " + startLine + " Line -> " + line + " EndLine -> " + endLine, ref p);
                    }
                }
                else
                {
                    _selectingText = false;
                    if (endIndex != startIndex)
                    {
                        // some stuff with selected text
                    }
                }
            }

            if (IsMouseOver)
            {
                if (Input.IsKeyDown(Keys.Mouse0))
                {
                    _selectingText = true;
                    _selectingTextStartIndex = GetCharIndexFromCursor();
                }
            }

            #endregion

            #region DRAW
            if (SmoothColors) _color = Color.Lerp(_color, IsMouseOver ? Colors.OverColor : Colors.NormalColor, (float)Time.DeltaTime * SmoothTranslation);
            UIInternal.Color = _color;
            UIInternal.Depth = Depth;
            UIInternal.DrawBox(Region);

            var buttonRegion = Region;    
            var target = new Vector2(buttonRegion.Width, buttonRegion.Height) * (IsFocused ? 1f : 0.5f);
            UIAnimation.SpringVector2(ref _textFieldEditBoxSize, ref _textFieldEditBoxVelocity, target, (float)Time.DeltaTime);
            buttonRegion = new RectangleF(buttonRegion.X - (_textFieldEditBoxSize.X / 2f - buttonRegion.Width / 2f),
                                          buttonRegion.Y - (_textFieldEditBoxSize.Y / 2f - buttonRegion.Height / 2f),
                                          _textFieldEditBoxSize.X, _textFieldEditBoxSize.Y);

            if (SmoothColors) _textFieldEditBoxColor = Color.Lerp(_textFieldEditBoxColor, IsFocused ? Colors.ClickColor : Color.Transparent, (float) Time.DeltaTime * SmoothTranslation * (IsFocused ? 1f : 2f));
            UIInternal.Color = _textFieldEditBoxColor;
            UIInternal.DrawBox(buttonRegion);

            UIInternal.Depth = Depth + 0.1f;
            UIInternal.Color = TextColor;
            TextPosition = new Vector2(Region.X + Region.Width / 2 - TextSize.X / 2, Region.Y + Region.Height / 2 - TextSize.Y / 2);
            var pos = TextPosition;
            UIInternal.DrawString(TextFont.NativePtr, _text, ref pos);
            pos.Y += 40;
            UIInternal.Color = Color.White;
            UIInternal.DrawString(TextFont.NativePtr, GetPointFromCharIndex(GetCharIndexFromCursor()).ToString(), ref pos);
            pos.Y += 20;
            UIInternal.Color = Color.White;
            UIInternal.DrawString(TextFont.NativePtr, "From -> " + _selectingTextStartIndex + " To " + GetCharIndexFromCursor(), ref pos);
            pos.Y += 20;
            UIInternal.Color = Color.White;
            UIInternal.DrawString(TextFont.NativePtr, _text[(int)GetCharIndexFromCursor()].ToString(), ref pos);

            if (_carpetRegions != null)
                foreach (var carpet in _carpetRegions)
                {
                    UIInternal.Color = new Color(64 / 255f, 134 / 255f, 247 / 255f, 0.600f);
                    UIInternal.DrawBox(carpet);

                    pos.Y += 20;
                    UIInternal.Color = Color.White;
                    UIInternal.DrawString(TextFont.NativePtr, $"Carpet {carpet}", ref pos);
                }
            #endregion
        }

        // returns lines of text in list.
        private IReadOnlyList<string> GetLines()
        {
            var lines = new List<string> {string.Empty};
            int line = 0;
            foreach (char t in _text)
            {
                if (t == '\n')
                {
                    line++;
                    lines.Add(string.Empty);
                }
                else
                {
                    lines[line] += t;
                }
            }

            return lines;
        }

        private int GetEndCharIndexOfLine(int line)
        {
            var lines = GetLines();
            var totalCharacters = 0;
            for (int index = 0; index < lines.Count; index++)
            {
                totalCharacters += lines[index].Length;
                if (index == line)
                {
                    return totalCharacters;
                }
            }
            
            return 0;
        }

        // gets size of text to given line
        private Vector2 GetRegionSizeToLine(int fromLine, int toLine)
        {
            int line = 0;
            var size = new Vector2(0, TextFont.Size);
            string lastLine = string.Empty;
            foreach (char t in _text)
            {
                if (t == '\n')
                {
                    line++;
                    size.Y += TextFont.Size;
                    var lastLineSize = TextFont.MeasureString(lastLine);
                    if (size.X < lastLineSize.X)
                        size.X = lastLineSize.X;
                    lastLine = string.Empty;
                    if (line == toLine)
                        return size;
                }
                else
                {
                    if (line >= fromLine)
                    lastLine+= t;
                }
            }
            if (!string.IsNullOrEmpty(lastLine))
            {
                var lineSize = TextFont.MeasureString(lastLine);
                if (size.X < lineSize.X)
                    size.X = lineSize.X;
            }
            return size;
        }

        private Vector2 GetLineSize(int line)
        {
            var lines = GetLines();
            for (int index = 0; index < lines.Count; index++)
            {
                if (index == line)
                    return TextFont.MeasureString(lines[index]);
            }
            return new Vector2(0, 0);
        }

        // gets character from text under cursor
        private int GetCharIndexFromCursor()
        {
            if (string.IsNullOrEmpty(_text)) return 0;
            var point = Input.CursorPosition;
            var lines = GetLines();
            int total = 0;    
            for (var line = 0; line < lines.Count; line++)
            {
                var lastSize = new Vector2(0, 0);
                var lineH = TextPosition.Y + line * TextFont.Size;
                var lineX = TextPosition.X;
                for (int index = 0; index < lines[line].Length; index++)
                {
                    var cChar = lines[line][index];
                    var cRect = new RectangleF(lineX, lineH, 0, 0);
                    lineX = cRect.X += lastSize.X;
                    lastSize = TextFont.MeasureString(cChar.ToString());
                    cRect.Width = lastSize.X;
                    cRect.Height = lastSize.Y;
                    if (cRect.Contains(point))
                    {
                        return total + index;
                    }
                }
                total += lines[line].Length + 1;
            }

            return point.X > TextPosition.X ? _text.Length - 1 : 0;
        }

        // gets character screen point
        private Vector2 GetPointFromCharIndex(int index)
        {
            if (index == 0) return TextPosition;
            var sizeH = 0f;
            var sizeW = 0f;
            for (var i = 0; i < _text.Length; i++)
            {
                if (_text[i] == '\n')
                {
                    sizeW = 0;
                    sizeH += TextFont.Size;
                }
                else
                {
                    var charSize = TextFont.MeasureString(_text[i].ToString());
                    sizeW += charSize.X;
                }

                if (i == index)
                {
                    return new Vector2(TextPosition.X + sizeW, TextPosition.Y + sizeH);
                }
            }
            return TextPosition;
        }

        // gets line from given character index
        private int GetLineFromCharIndex(int index)
        {
            var lines = GetLines();
            var total = 0;
            for (int i = 0; i < lines.Count; i++)
            {
                for (int l = 0; l < lines[i].Length; l++)
                {
                    if (total == index)
                        return i;
                    total++;
                }
                total++;// n/
            }
            return total;
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
            _carpetRegions = new RectangleF[0];
        }

        // resets current selection carpet
        private void _resetCarpet()
        {
            _carpetRegions = new RectangleF[0];
            _selectingText = false;
            _selectingTextStartIndex = 0;
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
        /// Is any text field currently focused?
        /// </summary>
        public static bool AnyTextFieldFocused { get; private set; }
    }
}
