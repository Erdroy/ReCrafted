// ReCrafted (c) 2016-2018 Always Too Late

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
    /// Type and style of message.
    /// </summary>
    public enum MessageType
    {
        Info,
        Question,
        Warning,
        Error
    }

    /// <summary>
    /// Type of buttons in message.
    /// </summary>
    public enum MessageButtons
    {
        None,
        Ok,
        OkNo,
        OkNoCancel,
        YesNo,
        YesNoCancel
    }


    /// <summary>
    /// Button type of message.
    /// </summary>
    public enum MessageButton
    {
        None,
        Ok,
        No,
        Yes,
        Cancel
    }

    public enum MessagePosition
    {
        Left,
        Right,
        Middle,
        Top,
        Bottom
    }

    /// <summary>
    /// Messenger on click event.
    /// </summary>
    /// <param name="button">Source button of this event.</param>
    public delegate void MessengerClick(MessageButton button);

    /// <summary>
    /// UI Messenger class.
    /// </summary>
    public class Messenger : Script
    {
        /// <summary>
        /// Messanger queue item.
        /// </summary>
        private class QueueItem
        {
            /// <summary>
            /// Text of item.
            /// </summary>
            public string Text;

            /// <summary>
            /// Title of item.
            /// </summary>
            public string Title;

            /// <summary>
            /// Duration of item.
            /// </summary>
            public double Duration;

            /// <summary>
            /// Position of item.
            /// </summary>
            public MessagePosition Position;

            /// <summary>
            /// Type of item.
            /// </summary>
            public MessageType Type;

            /// <summary>
            /// Buttons of item.
            /// </summary>
            public MessageButtons Buttons;

            /// <summary>
            /// On click callback.
            /// </summary>
            public MessengerClick OnClick;

            /// <summary>
            /// Time, how long this item is displayed.
            /// </summary>
            internal double Time;

            /// <summary>
            /// Is time of this item done?
            /// </summary>
            internal bool Done;
        }

        /// <summary>
        /// Instance of messenger.
        /// </summary>
        private static Messenger _instance;

        // actual list of center messages
        private static readonly List<QueueItem> CenterMessageQueue = new List<QueueItem>();

        // actual size of center message
        private Vector2 _centerMessageSize;

        private Vector2 _centerMessageVelocity;

        private UIPanel _centerMessagePanel;
        private UIBox _centerMessageIcon;
        private UIButton _centerMessageTitle;
        private UIText _centerMessageText;

        private UIButton _centerMessageOkButton;
        private UIButton _centerMessageYesButton;
        private UIButton _centerMessageNoButton;
        private UIButton _centerMessageCancelButton;

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

            // create center messsage panel
            _centerMessageSize = CenterMessageSizeMin;
            _centerMessagePanel = UIPanel.Create(CenterMessageRegion, UILayoutType.Vertical, "Messenger-Center");
            _centerMessagePanel.Layout.ForceExpandWidth = true;
            _centerMessagePanel.Layout.ForceExpandHeight = true;

            _centerMessagePanel.Enabled = false;

            var header = _centerMessagePanel.Add(UILayout.Create(new RectangleF(), UILayoutType.Horizontal,
                UILayoutAlignment.LeftMiddle));
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
            var footer = _centerMessagePanel.Add(UILayout.Create(new RectangleF(), UILayoutType.Horizontal,
                UILayoutAlignment.Middle));
            footer.PreferredSize = new Vector2(0, 30);
            footer.ForceExpandHeight = true;
            footer.Space = 10;

            _centerMessageOkButton = footer.Add(new UIButton("Ok"));
            _centerMessageOkButton.PreferredSize = new Vector2(100, 0);
            _centerMessageOkButton.Enabled = false;
            _centerMessageOkButton.OnClick += () =>
            {
                OnCenterMessengeButton(MessageButton.Ok);
            };
            _centerMessageYesButton = footer.Add(new UIButton("Yes"));
            _centerMessageYesButton.PreferredSize = new Vector2(100, 0);
            _centerMessageYesButton.Enabled = false;
            _centerMessageYesButton.OnClick += () =>
            {
                OnCenterMessengeButton(MessageButton.Yes);
            };
            _centerMessageNoButton = footer.Add(new UIButton("No"));
            _centerMessageNoButton.PreferredSize = new Vector2(100, 0);
            _centerMessageNoButton.Enabled = false;
            _centerMessageNoButton.OnClick += () =>
            {
                OnCenterMessengeButton(MessageButton.No);
            };
            _centerMessageCancelButton = footer.Add(new UIButton("Cancel"));
            _centerMessageCancelButton.PreferredSize = new Vector2(100, 0);
            _centerMessageCancelButton.Enabled = false;
            _centerMessageCancelButton.OnClick += () =>
            {
                OnCenterMessengeButton(MessageButton.Cancel);
            };
        }

        // center message button on click event
        private void OnCenterMessengeButton(MessageButton ok)
        {
            if (CenterMessageQueue.Count == 0)
                return;

            CenterMessageQueue[0].OnClick?.Invoke(ok);
            CenterMessageQueue[0].Done = true;
            CenterMessageMoveNext();
        }

        // on update
        protected override void OnUpdate()
        {
            var first = CenterMessageQueue.Count == 0 ? null : CenterMessageQueue.First();
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
                    if (first.Time < first.Duration)
                        first.Time += Time.DeltaTime;
                    else
                    {
                        first.Done = true;
                        CenterMessageMoveNext();
                    }
                }
            }
        }

        // move to next item
        private static void CenterMessageMoveNext()
        {
            if (CenterMessageQueue.Count == 0 || !CenterMessageQueue[0].Done)
                return;
            CenterMessageQueue.RemoveAt(0);
            CenterMessageRestartTransform();
        }

        // restart displayed center message to default
        private static void CenterMessageRestartTransform()
        {
            _instance._centerMessageSize = _instance.CenterMessageSizeMin;
            _instance._centerMessagePanel.PanelColor = Color.Transparent;
            _instance._centerMessagePanel.Enabled = CenterMessageQueue.Count != 0;
            _instance._centerMessageIcon.Enabled = _instance._centerMessagePanel.Enabled;

            if (CenterMessageQueue.Count != 0)
            {
                _instance._centerMessageTitle.Text = CenterMessageQueue[0].Title;
                _instance._centerMessageText.Text = CenterMessageQueue[0].Text;

                switch (CenterMessageQueue[0].Type)
                {
                    case MessageType.Info:
                        _instance._centerMessageIcon.Sprite = UIControl.DefaultInfo;
                        break;
                    case MessageType.Question:
                        _instance._centerMessageIcon.Sprite = UIControl.DefaultQuestion;
                        break;
                    case MessageType.Warning:
                        _instance._centerMessageIcon.Sprite = UIControl.DefaultWarning;
                        break;
                    case MessageType.Error:
                        _instance._centerMessageIcon.Sprite = UIControl.DefaultError;
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }

                var b = CenterMessageQueue[0].Buttons;
                _instance._centerMessageOkButton.Enabled = b == MessageButtons.Ok || b == MessageButtons.OkNoCancel;
                _instance._centerMessageYesButton.Enabled =
                    b == MessageButtons.YesNo || b == MessageButtons.YesNoCancel;
                _instance._centerMessageNoButton.Enabled =
                    b == MessageButtons.OkNoCancel || b == MessageButtons.YesNo ||
                    b == MessageButtons.YesNoCancel;
                _instance._centerMessageCancelButton.Enabled =
                    b == MessageButtons.OkNoCancel || b == MessageButtons.YesNoCancel;

            }
        }

        /// <summary>
        /// Show center message on screen.
        /// </summary>
        /// <param name="text">Text of message.</param>
        /// <param name="title">Title message.</param>
        /// <param name="duration">How long this message will be displayed.</param>
        /// <param name="onClick">On click callback.</param>
        /// <param name="type">Type of message.</param>
        /// <param name="buttons">Buttons in message.</param>
        public static void ShowCenterMessage(string text, string title, double duration, MessengerClick onClick,
            MessageType type = MessageType.Info, MessageButtons buttons = MessageButtons.Ok)
        {
            if (duration <= 0f)
                duration = -2f;

            CenterMessageQueue.Add(new QueueItem
            {
                Text = text,
                Title = title,
                Duration = duration,

                Position = MessagePosition.Middle,
                Type = type,
                Buttons = buttons,

                OnClick = onClick
            });

            CenterMessageMoveNext();
            if (CenterMessageQueue.Count == 1)
                CenterMessageRestartTransform();
        }

        /// <summary>
        /// Show small notify on screen.
        /// </summary>
        /// <param name="position">Position of notify.</param>
        /// <param name="text">Text of notify.</param>
        /// <param name="title">Title of notify.</param>
        /// <param name="duration">Duration of notify.</param>
        /// <param name="type">Type of notify.</param>
        public static void ShowNotify(MessagePosition position, string text, string title, double duration,
            MessageType type = MessageType.Info)
        {
            throw new NotImplementedException("ShowNotify is not yet implemented.");
        }
    }
}
