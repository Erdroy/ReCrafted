// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.Numerics;
using ImGuiNET;
using ReCrafted.Editor.Common;

namespace ReCrafted.Editor.Windows
{
    public class ConsoleWindow : DockableWindow
    {
        private class Log
        {
            public string Message;
            public Vector4 Color;
        }
        
        private readonly List<Log> _logs = new List<Log>();

        public bool ShowInfo;
        public bool ShowWarnings;
        public bool ShowErrors;

        private bool _scrollToBottom;

        public override void Initialize()
        {
            Logger.Instance.LogWritten += AddLog;
        }

        public override void Dispose()
        {
            Logger.Instance.LogWritten -= AddLog;
        }
        
        public override void OnRender()
        {
            ImGui.PushStyleColor(ImGuiCol.Button, new Vector4(0.40f, 0.40f, 0.40f, 0.25f));
            ImGui.PushStyleColor(ImGuiCol.ButtonActive, new Vector4(0.40f, 0.40f, 0.40f, 0.38f));
            ImGui.PushStyleColor(ImGuiCol.ButtonHovered, new Vector4(0.40f, 0.40f, 0.40f, 0.42f));

            ImGui.PushStyleColor(ImGuiCol.FrameBg, new Vector4(0.40f, 0.40f, 0.40f, 0.25f));
            ImGui.PushStyleColor(ImGuiCol.FrameBgActive, new Vector4(0.40f, 0.40f, 0.40f, 0.38f));
            ImGui.PushStyleColor(ImGuiCol.FrameBgHovered, new Vector4(0.40f, 0.40f, 0.40f, 0.42f));

            ImGui.PushStyleColor(ImGuiCol.CheckMark, new Vector4(0.6f, 0.6f, 0.6f, 1.0f));

            ImGui.PushStyleColor(ImGuiCol.ChildBg, new Vector4(0.40f, 0.40f, 0.40f, 0.25f));

            if (ImGui.Button("Clear"))
                Clear();

            ImGui.SameLine();
            ImGui.Checkbox("Show Info", ref ShowInfo);

            ImGui.SameLine();
            ImGui.Checkbox("Show Warnings", ref ShowWarnings);

            ImGui.SameLine();
            ImGui.Checkbox("Show Errors", ref ShowErrors);

            ImGui.BeginChild("##scrolling", new Vector2(0.0f, 0.0f), false, ImGuiWindowFlags.HorizontalScrollbar | ImGuiWindowFlags.AlwaysUseWindowPadding);
            {
                foreach (var log in _logs)
                {
                    ImGui.TextColored(log.Color, log.Message);
                }
                
                if (_scrollToBottom)
                {
                    ImGui.SetScrollHere(1.0f);
                    _scrollToBottom = false;
                }
            }
            ImGui.EndChild();
            ImGui.PopStyleColor(8);
        }

        public void Clear()
        {
            _logs.Clear();
        }

        public void AddLog(string message, Logger.LogType logType)
        {
            Vector4 color;
            switch (logType)
            {
                case Logger.LogType.Info:
                    color = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
                    break;
                case Logger.LogType.Warning:
                    color = new Vector4(1.0f, 0.5f, 0.0f, 1.0f);
                    break;
                case Logger.LogType.Error:
                    color = new Vector4(1.0f, 0.0f, 0.0f, 1.0f);
                    break;
                case Logger.LogType.Exception:
                    color = new Vector4(0.6f, 0.15f, 0.15f, 1.0f);
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(logType), logType, null);
            }

            _logs.Add(new Log
            {
                Message = message,
                Color = color
            });

            while(_logs.Count > 100)
                _logs.RemoveAt(0);

            _scrollToBottom = true;
        }

        public override string WindowName => "Console";
    }
}
