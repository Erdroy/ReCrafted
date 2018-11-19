using ReCrafted.Editor.Common;
using Veldrid;

namespace ReCrafted.Editor.Core
{
    public class Input
    {
        private EditorApplication EditorApplication { get; }

        public Input(EditorApplication editorApplication)
        {
            EditorApplication = editorApplication;
            EditorApplication.SdlWindow.KeyDown += @event =>
            {
                _lastKey = @event;
                _lastKeyDown = @event;
                _nextKeyDownTime = Time.CurrentTime + 0.02f;
            };
            EditorApplication.SdlWindow.KeyUp += @event =>
            {
                _lastKey = default(KeyEvent);
                _lastKeyUp = @event;
            };
        }

        public void Update()
        {
            if (!_lastKeyDown.Equals(default(KeyEvent)) && Time.CurrentTime >=_nextKeyDownTime)
            {
                _lastKeyDown = default(KeyEvent);
            }

            if (!_lastKeyUp.Equals(default(KeyEvent)) && Time.CurrentTime >= _nextKeyUpTime)
            {
                _lastKeyUp = default(KeyEvent);
            }
        }

        private static float _nextKeyDownTime;
        private static float _nextKeyUpTime;
        private static KeyEvent _lastKey;
        private static KeyEvent _lastKeyDown;
        private static KeyEvent _lastKeyUp;

        public static bool IsKey(Key key)
        {
            return _lastKey.Down && _lastKey.Key == key;
        }

        public static bool IsKeyDown(Key key)
        {
            return _lastKeyDown.Down && _lastKeyDown.Key == key;
        }

        public static bool IsKeyUp(Key key)
        {
            return _lastKeyUp.Down && _lastKeyUp.Key == key;
        }
    }
}
