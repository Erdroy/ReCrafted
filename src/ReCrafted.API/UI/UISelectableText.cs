// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI
{
    /// <summary>
    /// Delegate for UISelectableText text select event.
    /// </summary>
    /// <param name="text">Selected text.</param>
    public delegate void TextSelected(string text);

    /// <summary>
    /// Delegate for UISelectableText character click event.
    /// </summary>
    /// <param name="position">Index of character that was clicked.</param>
    public delegate void OnCharacterClick(int position);

    /// <summary>
    /// Selectable text add-on for UIControls.
    /// </summary>
    public class UISelectableText
    {
        public UISelectableText()
        {
            ResetSelection();
        }

        // region of selection carpet
        private RectangleF[] _carpetRegions;

        // is text currently under carpet selection
        private bool _selectingText;

        // index of character where selection carpet starts
        private int _selectingTextStartIndex;

        // index of character where selection carpet ends
        private int _selectingTextEndIndex;

        // last selected character
        private int _lastSelectedIndex = -1;

        // last received text
        private string _text;

        // last received font
        private Font _textFont;

        // last received text position
        private Vector2 _textPosition;

        /// <summary>
        /// When selected text was changed.
        /// </summary>
        public TextSelected OnTextSelected;

        /// <summary>
        /// When character in text was clicked.
        /// </summary>
        public OnCharacterClick OnCharacterClick;

        /// <summary>
        /// When selection has been reset.
        /// </summary>
        public Action OnResetSelection;

        /// <summary>
        /// Resets current selection.
        /// </summary>
        public void ResetSelection()
        {
            SelectedText = null;
            SelectedTextStart = 0;
            SelectedTextEnd = 0;

            _carpetRegions = new RectangleF[0];
            _selectingText = false;
            _selectingTextStartIndex = 0;
            _selectingTextEndIndex = 0;
            _lastSelectedIndex = -1;

            OnResetSelection?.Invoke();
        }

        /// <summary>
        /// Calculates and draws selected text.
        /// </summary>
        /// <param name="calculate">Is algorithm have to calculate the selected text?</param>
        /// <param name="text">Text.</param>
        /// <param name="font">Text font.</param>
        /// <param name="textPosition">Text screen position.</param>
        /// <param name="carpetDepth">Text screen depth.</param>
        /// <param name="isMouseOver">Is mouse over text.</param>
        public void Draw(bool calculate, string text, Font font, Vector2 textPosition, float carpetDepth,
            bool isMouseOver)
        {
            _text = text;
            _textFont = font;
            _textPosition = textPosition;

            // good if you needs some of UISelectableText functions but don't want to use text selection.
            if (!calculate)
                return;
            if (string.IsNullOrEmpty(text))
                return;

            if (_selectingText)
            {
                if (Input.IsKey(Keys.Mouse0))
                {
                    _selectingTextEndIndex = GetCharIndexFromPoint(Input.CursorPosition);
                    var startIndex = _selectingTextStartIndex;
                    var endIndex = _selectingTextEndIndex;
                    if (startIndex > endIndex) //swap
                    {
                        var s = startIndex;
                        startIndex = endIndex;
                        endIndex = s;
                    }

                    var lines = GetLines();

                    var startPoint = GetPointFromCharIndex(startIndex);
                    var startLine = GetLineFromCharIndex(startIndex);
                    var startCharSize = _textFont.MeasureString(_text[startIndex].ToString());

                    var endPoint = GetPointFromCharIndex(endIndex);
                    var endLine = GetLineFromCharIndex(endIndex);

                    //var p = new Vector2(0, 0);
                    //UIInternal.DrawString(TextFont.NativePtr, "From -> " + startIndex + ", To -> " + endIndex, ref p);

                    _carpetRegions = new RectangleF[lines.Count];
                    for (int line = startLine; line < lines.Count; line++)
                    {
                        if (line > startLine && line < endLine)
                        {
                            var lineSize = GetLineSize(line);
                            _carpetRegions[line] = new RectangleF(_textPosition.X,
                                _textPosition.Y + _textFont.Size * line,
                                lineSize.X, lineSize.Y);
                        }
                        else if (line < startLine && line > endLine)
                        {
                            var lineSize = GetLineSize(line);
                            _carpetRegions[line] = new RectangleF(_textPosition.X,
                                _textPosition.Y + _textFont.Size * line,
                                lineSize.X, lineSize.Y);
                        }
                        else
                        {
                            if (startLine == line)
                            {
                                if (startIndex == 0)
                                    startCharSize.X = 0;

                                var endOfLine = GetLastCharIndexOfLine(startLine) - 1;
                                if (endIndex > endOfLine)
                                {
                                    if (startLine < endLine)
                                    {
                                        var pointOfEndOfLine = GetPointFromCharIndex(endOfLine);
                                        _carpetRegions[line] = new RectangleF(startPoint.X - startCharSize.X,
                                            startPoint.Y,
                                            pointOfEndOfLine.X - startPoint.X + startCharSize.X,
                                            pointOfEndOfLine.Y - startPoint.Y + _textFont.Size);
                                    }
                                }
                                else
                                {
                                    _carpetRegions[line] = new RectangleF(startPoint.X - startCharSize.X,
                                        startPoint.Y,
                                        endPoint.X - startPoint.X + startCharSize.X,
                                        endPoint.Y - startPoint.Y + _textFont.Size);
                                }
                            }
                            else if (endLine == line)
                            {
                                _carpetRegions[line] = new RectangleF(_textPosition.X,
                                    _textPosition.Y + _textFont.Size * line,
                                    endPoint.X - _textPosition.X,
                                    _textFont.Size);
                            }
                        }

                    }
                }

                if (Input.IsKeyUp(Keys.Mouse0))
                {
                    // some stuff with selected text
                    if (_selectingTextEndIndex != _selectingTextStartIndex)
                    {
                        if (_selectingTextStartIndex < _selectingTextEndIndex)
                        {
                            SelectedText = _text.Substring(_selectingTextStartIndex, _selectingTextEndIndex - _selectingTextStartIndex + 1);
                            SelectedTextStart = _selectingTextStartIndex;
                            SelectedTextEnd = _selectingTextEndIndex + 1;
                        }
                        else
                        {
                            SelectedText = _text.Substring(_selectingTextEndIndex, _selectingTextStartIndex - _selectingTextEndIndex + 1);
                            SelectedTextStart = _selectingTextEndIndex;
                            SelectedTextEnd = _selectingTextStartIndex + 1;
                        }

                        OnTextSelected?.Invoke(SelectedText);
                    }
                    else
                    {
                        if (_lastSelectedIndex == -1)
                        {
                            var p = _selectingTextStartIndex;
                            ResetSelection();
                            OnCharacterClick?.Invoke(p);
                            _lastSelectedIndex = p;
                        }
                        else
                        {
                            if (_lastSelectedIndex == _selectingTextStartIndex)
                            {
                                SelectedText = _text[_selectingTextStartIndex].ToString();
                                SelectedTextStart = _selectingTextStartIndex;
                                SelectedTextEnd = _selectingTextStartIndex;
                                OnTextSelected?.Invoke(SelectedText);
                            }
                            else
                            {
                                var p = _selectingTextStartIndex;
                                ResetSelection();
                                OnCharacterClick?.Invoke(p);
                                _lastSelectedIndex = p;
                            }
                        }
                    }
                }
            }

            if (isMouseOver)
            {
                if (Input.IsKeyDown(Keys.Mouse0))
                {
                    _selectingText = true;
                    _selectingTextStartIndex = GetCharIndexFromPoint(Input.CursorPosition);
                }
            }

            if (_selectingTextStartIndex == _selectingTextEndIndex && _lastSelectedIndex != _selectingTextStartIndex)
                return;

            UIInternal.Depth = carpetDepth;
            UIInternal.Color = new Color(64 / 255f, 134 / 255f, 247 / 255f, 0.600f);
            if (_carpetRegions != null)
                foreach (var carpet in _carpetRegions)
                    UIInternal.DrawBox(carpet);
        }

        /// <summary>
        /// Returns lines of text in list.
        /// </summary>
        /// <returns>Read only string list.</returns>
        public IReadOnlyList<string> GetLines()
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

        /// <summary>
        /// Gets last character index of line.
        /// </summary>
        /// <param name="line">Our line.</param>
        /// <returns>Character index.</returns>
        public int GetLastCharIndexOfLine(int line)
        {
            var lines = GetLines();
            var totalCharacters = 0;
            for (int index = 0; index < lines.Count; index++)
            {
                totalCharacters += lines[index].Length == 0 ? 1 : lines[index].Length;
                if (index == line)
                {
                    return totalCharacters;
                }
            }

            return 0;
        }


        /// <summary>
        /// Gets first character index of line.
        /// </summary>
        /// <param name="line">Our line.</param>
        /// <returns>Character index.</returns>
        public int GetFirstCharIndexOfLine(int line)
        {
            var lines = GetLines();
            var totalCharacters = 0;
            for (int index = 0; index < lines.Count; index++)
            {
                if (index == line)
                {
                    return totalCharacters;
                }
                totalCharacters += lines[index].Length;
            }

            return 0;
        }

        /// <summary>
        /// Gets total size of region from line to line.
        /// </summary>
        /// <param name="fromLine">From line.</param>
        /// <param name="toLine">To line.</param>
        /// <returns>Calculated region size.</returns>
        public Vector2 GetRegionSizeOfLines(int fromLine, int toLine)
        {
            int line = 0;
            var size = new Vector2(0, _textFont.Size);
            string lastLine = string.Empty;
            foreach (char t in _text)
            {
                if (t == '\n')
                {
                    line++;
                    size.Y += _textFont.Size;
                    var lastLineSize = _textFont.MeasureString(lastLine);
                    if (size.X < lastLineSize.X)
                        size.X = lastLineSize.X;
                    lastLine = string.Empty;
                    if (line == toLine)
                        return size;
                }
                else
                {
                    if (line >= fromLine)
                        lastLine += t;
                }
            }
            if (!string.IsNullOrEmpty(lastLine))
            {
                var lineSize = _textFont.MeasureString(lastLine);
                if (size.X < lineSize.X)
                    size.X = lineSize.X;
            }
            return size;
        }

        /// <summary>
        /// Gets line size.
        /// </summary>
        /// <param name="line">Our line.</param>
        /// <returns>Region size.</returns>
        public Vector2 GetLineSize(int line)
        {
            var lines = GetLines();
            for (int index = 0; index < lines.Count; index++)
            {
                if (index == line)
                    return _textFont.MeasureString(lines[index]);
            }
            return new Vector2(0, 0);
        }

        /// <summary>
        /// Gets character under point
        /// </summary>
        /// <param name="point">Screen point.</param>
        /// <returns>Character index.</returns>
        public int GetCharIndexFromPoint(Vector2 point)
        {
            bool e;
            return GetCharIndexFromPoint(point, out e);
        }

        /// <summary>
        /// Gets character under point
        /// </summary>
        /// <param name="point">Screen point.</param>
        /// <param name="exist">True if character was successfully resolved.</param>
        /// <returns>Character index.</returns>
        public int GetCharIndexFromPoint(Vector2 point, out bool exist)
        {
            exist = false;
            if (string.IsNullOrEmpty(_text)) return 0;
            var lines = GetLines();
            int total = 0;
            for (var line = 0; line < lines.Count; line++)
            {
                var lastSize = new Vector2(0, 0);
                var lineH = _textPosition.Y + line * _textFont.Size;
                var lineX = _textPosition.X;
                for (int index = 0; index < lines[line].Length; index++)
                {
                    var cChar = lines[line][index];
                    var cRect = new RectangleF(lineX, lineH, 0, 0);
                    lineX = cRect.X += lastSize.X;
                    lastSize = _textFont.MeasureString(cChar.ToString());
                    cRect.Width = lastSize.X;
                    cRect.Height = lastSize.Y;
                    if (cRect.Contains(point))
                    {
                        exist = true;
                        return total + index;
                    }
                }
                total += lines[line].Length + 1;
            }

            return point.X > _textPosition.X ? _text.Length - 1 : 0;
        }

        /// <summary>
        /// Gets screen position of character.
        /// </summary>
        /// <param name="index">Index of character.</param>
        /// <returns>Screen point.</returns>
        public Vector2 GetPointFromCharIndex(int index)
        {
            if (index == 0) return _textPosition;
            var sizeH = 0f;
            var sizeW = 0f;
            for (var i = 0; i < _text.Length; i++)
            {
                if (_text[i] == '\n')
                {
                    sizeW = 0;
                    sizeH += _textFont.Size;
                }
                else
                {
                    var charSize = _textFont.MeasureString(_text[i].ToString());
                    sizeW += charSize.X;
                }

                if (i == index)
                {
                    return new Vector2(_textPosition.X + sizeW, _textPosition.Y + sizeH);
                }
            }
            return _textPosition;
        }

        /// <summary>
        /// Gets line of character index.
        /// </summary>
        /// <param name="index">Index of character.</param>
        /// <returns>Index of line.</returns>
        public int GetLineFromCharIndex(int index)
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
                total++; // n/
            }
            return total;
        }

        /// <summary>
        /// Currently selected text.
        /// </summary>
        public string SelectedText { get; private set; }

        /// <summary>
        /// Start character index of selected text.
        /// </summary>
        public int SelectedTextStart { get; private set; }

        /// <summary>
        /// End character index of selected text.
        /// </summary>
        public int SelectedTextEnd { get; private set; }
    }
}
