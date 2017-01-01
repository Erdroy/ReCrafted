// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace ReCrafted.Core
{
    /// <summary>
    /// Input class.
    /// </summary>
    public sealed class Input
    {
        private struct KeyState
        {
            public bool Down;
            public int Frame;
        }

        /// <summary>
        /// Input instance.
        /// </summary>
        public static Input Instance;

        private readonly Dictionary<KeyCode, KeyState> _keyTable = new Dictionary<KeyCode, KeyState>();

        private readonly Dictionary<ButtonCode, KeyState> _buttonTable = new Dictionary<ButtonCode, KeyState>();

        private Input()
        {
            Game.Instance.Form.KeyDown += OnKeyDown;
            Game.Instance.Form.KeyUp += OnKeyUp;
            Game.Instance.Form.MouseDown += OnButtonDown;
            Game.Instance.Form.MouseUp += OnButtonUp;

            Game.Instance.Form.LostFocus += OnFocusLost;
        }

        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            lock (_keyTable)
            {
                if (_keyTable.ContainsKey((KeyCode)e.KeyCode) && _keyTable[(KeyCode)e.KeyCode].Down)
                    return;
                
                _keyTable[(KeyCode) e.KeyCode] = new KeyState
                {
                    Down = true,
                    Frame = Time.Frame
                };
            }
        }

        private void OnKeyUp(object sender, KeyEventArgs e)
        {
            lock (_keyTable)
            {
                _keyTable[(KeyCode)e.KeyCode] = new KeyState
                {
                    Down = false,
                    Frame = Time.Frame
                };
            }
        }

        private void OnButtonDown(object sender, MouseEventArgs e)
        {
            if (_buttonTable.ContainsKey((ButtonCode)e.Button) && _buttonTable[(ButtonCode)e.Button].Down)
                return;

            lock (_buttonTable)
            {
                _buttonTable[(ButtonCode)e.Button] = new KeyState
                {
                    Down = true,
                    Frame = Time.Frame
                };
            }
        }

        private void OnButtonUp(object sender, MouseEventArgs e)
        {
            lock (_buttonTable)
            {
                _buttonTable[(ButtonCode)e.Button] = new KeyState
                {
                    Down = false,
                    Frame = Time.Frame
                };
            }
        }

        private void OnFocusLost(object sender, EventArgs e)
        {
            _keyTable.Clear();
            _buttonTable.Clear();
        }

        /// <summary>
        /// Returns true when the key is pressed since some frames.
        /// </summary>
        /// <param name="key">The key code.</param>
        /// <returns>True when key is held.</returns>
        public static bool IsKey(KeyCode key)
        {
            KeyState state;

            if (!Instance._keyTable.TryGetValue(key, out state))
                return false;

            return state.Down && state.Frame != Time.Frame;
        }

        /// <summary>
        /// Returns true when the key was pressed during the current frame.
        /// </summary>
        /// <param name="key">The key code.</param>
        /// <returns>True when pressed.</returns>
        public static bool IsKeyDown(KeyCode key)
        {
            KeyState state;
            if (!Instance._keyTable.TryGetValue(key, out state))
                return false;

            return state.Down && state.Frame == Time.Frame;
        }

        /// <summary>
        /// Returns true when the key was released during the current frame.
        /// </summary>
        /// <param name="key">The key code.</param>
        /// <returns>True when released.</returns>
        public static bool IsKeyUp(KeyCode key)
        {
            KeyState state;
            if (!Instance._keyTable.TryGetValue(key, out state))
                return false;

            return !state.Down && state.Frame == Time.Frame;
        }
        
        /// <summary>
        /// Returns true when the button is pressed since some frames.
        /// </summary>
        /// <param name="button">The button code.</param>
        /// <returns>True when button is held.</returns>
        public static bool IsButton(ButtonCode button)
        {
            KeyState state;

            if (!Instance._buttonTable.TryGetValue(button, out state))
                return false;

            return state.Down && state.Frame != Time.Frame;
        }

        /// <summary>
        /// Returns true when the button was pressed during the current frame.
        /// </summary>
        /// <param name="button">The button code.</param>
        /// <returns>True when pressed.</returns>
        public static bool IsButtonDown(ButtonCode button)
        {
            KeyState state;
            if (!Instance._buttonTable.TryGetValue(button, out state))
                return false;

            return state.Down && state.Frame == Time.Frame;
        }

        /// <summary>
        /// Returns true when the button was released during the current frame.
        /// </summary>
        /// <param name="button">The button code.</param>
        /// <returns>True when released.</returns>
        public static bool IsButtonUp(ButtonCode button)
        {
            KeyState state;
            if (!Instance._buttonTable.TryGetValue(button, out state))
                return false;

            return !state.Down && state.Frame == Time.Frame;
        }

        /// <summary>
        /// Initializes input.
        /// </summary>
        internal static void Initialize()
        {
            Instance = new Input();
        }
    }
}