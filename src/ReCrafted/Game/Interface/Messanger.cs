// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using System.Linq;
using ReCrafted.API.Common;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.API.UI.Controls;

namespace ReCrafted.Game.Interface
{
    /// <summary>
    /// Type of message.
    /// </summary>
    public enum MessangerType
    {
        Info,
        Question,
        Warning,
        Error
    }

    /// <summary>
    /// Buttons in message.
    /// </summary>
    public enum MessagerButtons
    {
        None,
        Ok,
        OkNo,
        OkNoCancel,
        YesNo,
        YesNoCancel
    }


    /// <summary>
    /// Button type.
    /// </summary>
    public enum MessagerButton
    {
        None,
        Ok,
        No,
        Yes,
        Cancel
    }

    public class Messanger : Script
    {
        private class QueueItem
        {
            public string Text;
            public string Title;
            public double Duration;

            public MessangerType Type;
            public MessagerButtons Buttons;

            public Action<MessagerButton> OnClick;

            public double _time;
            public bool _done;
        }

        // actual list of center messages
        private static List<QueueItem> _centerMessageQueue = new List<QueueItem>();

        /// <summary>
        /// Instance of messenger.
        /// </summary>
        private static Messanger _instance;

        // actual size of center message
        private Vector2 _centerMessageSize;

        // actual velocity of center message animation
        private Vector2 _centerMessageVelocity;

        // panel of center message
        private UIPanel _centerMessagePanel;

        // icon of center message panel
        private UIBox _centerMessageIcon;

        // title of center message
        private UIButton _centerMessageTitle;

        // text of center message
        private UIText _centerMessageText;

        private UIButton _okButton;

        private UIButton _yesButton;

        private UIButton _noButton;

        private UIButton _cancelButton;


        /// <summary>
        /// Start size of displayed center message.
        /// </summary>
        public Vector2 CenterMessageSizeMin => new Vector2(400, 200);

        /// <summary>
        /// Size of displayed center message.
        /// </summary>
        public Vector2 CenterMessageSizeMax => new Vector2(500, 250);

        /// <summary>
        /// Region of displayed center message.
        /// </summary>
        public RectangleF CenterMessageRegion => new RectangleF(Display.Width / 2f - _centerMessageSize.X / 2f,
            Display.Height / 2f - _centerMessageSize.Y / 2f, _centerMessageSize.X, _centerMessageSize.Y);

        /// <summary>
        /// Color of center message.
        /// </summary>
        public Color CenterMessageColor = new Color(0.8f, 0.8f, 0.8f, 1f);

        // on create
        protected internal override void OnCreate()
        {
            _instance = this;
            _centerMessageSize = CenterMessageSizeMin;

            _centerMessagePanel = UIPanel.Create(CenterMessageRegion, UILayoutType.Vertical, "Messenger-Center");
            _centerMessagePanel.Layout.ForceExpandWidth = true;
            _centerMessagePanel.Layout.ForceExpandHeight = true;

            _centerMessagePanel.Enabled = false;

            var header = _centerMessagePanel.Add(UILayout.Create(new RectangleF(), UILayoutType.Horizontal, UILayoutAlignment.LeftMiddle));
            header.PreferredSize = new Vector2(0, 30);
            header.ForceExpandHeight = true;
            header.Space = 10;

            _centerMessageIcon = header.Add(new UIBox());
            _centerMessageIcon.PreferredSize = new Vector2(30, 30);
            _centerMessageIcon.Sprite = UIControl.DefaultInfo;
            _centerMessageIcon.Enabled = false;

            _centerMessageTitle = header.Add(new UIButton());
            _centerMessageTitle.PreferredSize = new Vector2(0, 30);
            _centerMessageTitle.FixedTextPosition = new Vector2(0, UIControl.DefaultFont.Size / 2f + 2f);
            _centerMessageTitle.Colors = UIControlColors.Create(CenterMessageColor * 0.8f);
            _centerMessageTitle.SpringAnimation = false;
            _centerMessageTitle.SmoothColors = false;
            _centerMessageTitle.CenterText = false;

            _centerMessagePanel.AddSpace(5);

            _centerMessageText = _centerMessagePanel.Add(new UIText());
            _centerMessageText.FixedTextPosition = new Vector2(5, 0);
            _centerMessageText.TextColor = Color.Black;

            _centerMessagePanel.AddFlexSpace(1);
            var footer = _centerMessagePanel.Add(UILayout.Create(new RectangleF(), UILayoutType.Horizontal, UILayoutAlignment.Middle));
            footer.PreferredSize = new Vector2(0, 30);
            footer.ForceExpandHeight = true;
            footer.Space = 10;

            _okButton = footer.Add(new UIButton("Ok"));
            _okButton.PreferredSize = new Vector2(100, 0);
            _okButton.Enabled = false;
            _okButton.OnClick += () =>
            {
                OnCenterMesssageButton(MessagerButton.Ok);
            };
            _yesButton = footer.Add(new UIButton("Yes"));
            _yesButton.PreferredSize = new Vector2(100, 0);
            _yesButton.Enabled = false;
            _yesButton.OnClick += () =>
            {
                OnCenterMesssageButton(MessagerButton.Yes);
            };
            _noButton = footer.Add(new UIButton("No"));
            _noButton.PreferredSize = new Vector2(100, 0);
            _noButton.Enabled = false;
            _noButton.OnClick += () =>
            {
                OnCenterMesssageButton(MessagerButton.No);
            };
            _cancelButton = footer.Add(new UIButton("Cancel"));
            _cancelButton.PreferredSize = new Vector2(100, 0);
            _cancelButton.Enabled = false;
            _cancelButton.OnClick += () =>
            {
                OnCenterMesssageButton(MessagerButton.Cancel);
            };
        }

        private void OnCenterMesssageButton(MessagerButton ok)
        {
            if (_centerMessageQueue.Count == 0)
                return;
            _centerMessageQueue[0].OnClick?.Invoke(ok);
            _centerMessageQueue[0]._done = true;
            CenterMessageMoveNext();
        }

        // on update
        protected override void OnUpdate()
        {
            var first = _centerMessageQueue.Count == 0 ? null : _centerMessageQueue.First();
            if (first != null && _centerMessagePanel.Enabled)
            {
                var target = CenterMessageSizeMax;
                UIAnimation.SpringVector2(ref _centerMessageSize, ref _centerMessageVelocity, target,
                    (float) Time.DeltaTime, 0.6f);
                _centerMessagePanel.Region = CenterMessageRegion;
                _centerMessagePanel.PanelColor = Color.Lerp(_centerMessagePanel.PanelColor, CenterMessageColor,
                    (float) Time.DeltaTime * 15f);

                _centerMessageIcon.BoxColor = _centerMessagePanel.PanelColor;

                if (first.Duration <= -2f)
                {
                }
                else
                {
                    if (first._time < first.Duration)
                        first._time += Time.DeltaTime;
                    else
                    {
                        first._done = true;
                        CenterMessageMoveNext();
                    }
                }
            }
        }

        // move to next item
        private static void CenterMessageMoveNext()
        {
            if (_centerMessageQueue.Count == 0 || !_centerMessageQueue[0]._done)
                return;
            _centerMessageQueue.RemoveAt(0);
            CenterMessageRestartTransform();
        }

        // restart displayed center message to default
        private static void CenterMessageRestartTransform()
        {
            _instance._centerMessageSize = _instance.CenterMessageSizeMin;
            _instance._centerMessagePanel.PanelColor = Color.Transparent;
            _instance._centerMessagePanel.Enabled = _centerMessageQueue.Count != 0;
            _instance._centerMessageIcon.Enabled = _instance._centerMessagePanel.Enabled;

            if (_centerMessageQueue.Count != 0)
            {
                _instance._centerMessageTitle.Text = _centerMessageQueue[0].Title;
                _instance._centerMessageText.Text = _centerMessageQueue[0].Text;

                switch (_centerMessageQueue[0].Type)
                {
                    case MessangerType.Info:
                        _instance._centerMessageIcon.Sprite = UIControl.DefaultInfo;
                        break;
                    case MessangerType.Question:
                        _instance._centerMessageIcon.Sprite = UIControl.DefaultQuestion;
                        break;
                    case MessangerType.Warning:
                        _instance._centerMessageIcon.Sprite = UIControl.DefaultWarning;
                        break;
                    case MessangerType.Error:
                        _instance._centerMessageIcon.Sprite = UIControl.DefaultError;
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }

                var b = _centerMessageQueue[0].Buttons;
                _instance._okButton.Enabled = b == MessagerButtons.Ok || b == MessagerButtons.OkNoCancel;
                _instance._yesButton.Enabled = b == MessagerButtons.YesNo || b == MessagerButtons.YesNoCancel;
                _instance._noButton.Enabled = b == MessagerButtons.OkNoCancel || b == MessagerButtons.YesNo ||
                                              b == MessagerButtons.YesNoCancel;
                _instance._cancelButton.Enabled = b == MessagerButtons.OkNoCancel || b == MessagerButtons.YesNoCancel;

            }
        }

        /// <summary>
        /// Show center message.
        /// </summary>
        /// <param name="text">Text of message.</param>
        /// <param name="title">Title message.</param>
        /// <param name="duration">How long this message will be displayed.</param>
        /// <param name="onClick">On click callback.</param>
        /// <param name="type">Type of message.</param>
        /// <param name="buttons">Buttons in message.</param>
        public static void ShowCenterMessage(string text, string title, double duration, Action<MessagerButton> onClick,
            MessangerType type = MessangerType.Info, MessagerButtons buttons = MessagerButtons.Ok)
        {
            if (duration <= 0f)
                duration = -2f;

            _centerMessageQueue.Add(new QueueItem
            {
                Text = text,
                Title = title,
                Duration = duration,

                Type = type,
                Buttons = buttons,

                OnClick = onClick
            });

            CenterMessageMoveNext();
            if (_centerMessageQueue.Count == 1)
                CenterMessageRestartTransform();
        }
    }
}
