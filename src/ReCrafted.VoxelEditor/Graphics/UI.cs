// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Windows.Forms;
using ReCrafted.VoxelEditor.Core;
using ReCrafted.VoxelEditor.Rendering;
using SharpDX;
using Keys = ReCrafted.VoxelEditor.Core.Keys;

namespace ReCrafted.VoxelEditor.Graphics
{
    /// <summary>
    /// UI class. Provides basic user interface rendering.
    /// </summary>
    public class UI : IDisposable
    {
        private Render2D _render2D;
        private Vector2 _mousePos;

        /// <summary>
        /// Initializes UI.
        /// </summary>
        public void Init()
        {
            _render2D = new Render2D();
            _render2D.Init();
        }

        /// <summary>
        /// Begins UI rendering.
        /// </summary>
        public void Begin()
        {
            if (VoxelEditorApp.Current.Renderer.RenderForm.IsDisposed)
                return;

            var mousePos = VoxelEditorApp.Current.Renderer.RenderForm.PointToClient(Cursor.Position);
            _mousePos = new Vector2(mousePos.X, mousePos.Y);
        }

        /// <summary>
        /// Ends UI rendering.
        /// </summary>
        public void End()
        {
            _render2D.Draw();
        }

        /// <summary>
        /// Disposes the UI.
        /// </summary>
        public void Dispose()
        {
            _render2D.Dispose();
        }

        /// <summary>
        /// Draws box.
        /// </summary>
        public void Box(RectangleF rect, Color color)
        {
            DrawBox(rect, color);
        }

        /// <summary>
        /// Draws interactive button.
        /// </summary>
        public bool Button(RectangleF rect, string text, Color color, float hoverMul = 0.75f, float clickMul = 0.5f)
        {
            var state = false;

            if (rect.Contains(_mousePos))
            {
                var a = color.A;
                if (Input.IsKey(Keys.Mouse0))
                {
                    color *= clickMul;

                    if(Input.IsKeyUp(Keys.Mouse0))
                        state = true;
                }
                else
                {
                    color *= hoverMul;
                }
                color.A = a;
            }

            DrawBox(rect, color);
            return state;
        }

        /// <summary>
        /// Draws interactive toggle button.
        /// </summary>
        public bool Toggle(bool state, Vector2 position, Color color)
        {
            var rect = new RectangleF(position.X, position.Y, 20.0f, 20.0f);

            if (rect.Contains(_mousePos) && Input.IsKeyUp(Keys.Mouse0))
            {
                state = !state;
            }

            DrawBox(rect, color);

            if(state)
                DrawBox(new RectangleF(rect.X + 4.0f, rect.Y + 4.0f, rect.Width - 8.0f, rect.Height - 8.0f), color * 0.6f);

            return state;
        }

        private void DrawBox(RectangleF rect, Color color)
        {
            var size = VoxelEditorApp.Current.Renderer.RenderForm.ClientSize;

            if (size.Height % 2 != 0) // small fix
                size.Height += 1;

            // Width
            var width = rect.Width / (size.Width * 0.5f);

            // Height
            var height = rect.Height / (size.Height * 0.5f);

            // X (Top-left)
            var x = rect.X / (size.Width * 0.5f) - 1.0f;

            // Y (Top-left)
            var y = 1.0f - rect.Y / (size.Height * 0.5f) - height;

            _render2D.Add(new Render2D.DrawCmd
            {
                ZOrder = 0,
                Vertices = new []
                {
                    new Render2D.Vertex
                    {
                        Position = new Vector3(x, y + height, 0.0f),
                        Color = color.ToColor4(),
                        TexCoord = -Vector2.One
                    },
                    new Render2D.Vertex
                    {
                        Position = new Vector3(x + width, y + height, 0.0f),
                        Color = color.ToColor4(),
                        TexCoord = -Vector2.One
                    },
                    new Render2D.Vertex
                    {
                        Position = new Vector3(x + width, y, 0.0f),
                        Color = color.ToColor4(),
                        TexCoord = -Vector2.One
                    },
                    new Render2D.Vertex
                    {
                        Position = new Vector3(x, y, 0.0f),
                        Color = color.ToColor4(),
                        TexCoord = -Vector2.One
                    }
                }
            });
        }
    }
}
