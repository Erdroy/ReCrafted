// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Utilities;
using ReCrafted.Voxels;
using SharpDX;
using Vector2 = SharpDX.Vector2;
using Vector3 = SharpDX.Vector3;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// ShadowRenderer class.
    /// </summary>
    public sealed class ShadowRenderer : IDisposable
    {
        /// <summary>
        /// The ShadowMap resolution.
        /// </summary>
        public const int ShadowMapSize = 2048;

        private RenderTarget _depth;
        private Mesh _quad;
        private Shader _shadowShader;
        private Sampler _sampler;

        /// <summary>
        /// Initialize shadow renderer
        /// </summary>
        public void Init()
        {
            ShadowMap = RenderTarget.Create(ShadowMapSize, ShadowMapSize, RenderTarget.TextureFormat.R32_Float);
            _depth = RenderTarget.Create(ShadowMapSize, ShadowMapSize, RenderTarget.TextureFormat.Depth);

            _sampler = Sampler.Create(Sampler.Type.PointClamped);
            ShadowOcculusion = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight, RenderTarget.TextureFormat.R32_Float);
            _shadowShader = Shader.FromFile("lighting/shadows");

            _quad = Mesh.Create();
            _quad.SetVertices(new []
            {
                new Vector3(1.0f, 1.0f, 0.0f),
                new Vector3(1.0f, -1.0f, 0.0f),
                new Vector3(-1.0f, 1.0f, 0.0f),
                new Vector3(-1.0f, -1.0f, 0.0f),
            });
            _quad.SetUVs(new []
            {
                new Vector2(1.0f, 0.0f),
                new Vector2(1.0f, 1.0f),
                new Vector2(0.0f, 0.0f),
                new Vector2(0.0f, 1.0f),
            });
            _quad.PrimitiveType = PrimitiveType.TriangleStrip;

            _quad.ApplyChanges();
        }

        /// <summary>
        /// Resize all render targets
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        public void Resize(int width, int height)
        {
            ShadowOcculusion.Resize(width, height);
        }

        /// <summary>
        /// Render the depth into shadow map
        /// </summary>
        public void RenderShadowMap()
        {
            //ShadowMap.Clear(Color.Black);
            //ShadowOcculusion.Clear(Color.Black);
            _depth.Clear(Color.Transparent);

            Renderer.Instance.SetRenderTargetsDepth(_depth, ShadowMap);
            Renderer.Instance.SetDepthTestState(true);

            Renderer.Instance.SetViewportSize(ShadowMapSize, ShadowMapSize);

            var lightPos = new Vector3(50, 50, 50);
            var view = Matrix.LookAtLH(lightPos, lightPos + LightDir, Vector3.Up);
            var proj = Matrix.OrthoLH(100, 100, -100, 100);
            
            var vp = view*proj;

            VoxelWorld.Instance.DrawShadowMap(vp);
            
            Renderer.Instance.SetViewportSize(Display.ClientWidth, Display.ClientHeight);

            // draw
            Renderer.Instance.SetRenderTargets(ShadowOcculusion);
            Renderer.Instance.SetDepthTestState(false);

            _shadowShader.Apply();

            _shadowShader.SetRenderTexture(ShaderType.PS, 0, ShadowMap);
            _shadowShader.SetRenderTexture(ShaderType.PS, 1, Renderer.Instance.DepthRenderTarget);
            _shadowShader.SetSampler(0, _sampler);

            _shadowShader.SetValue("g_matInvView", Matrix.Transpose(Matrix.Invert(Camera.Current.ViewMatrix)));
            _shadowShader.SetValue("g_matInvProj", Matrix.Transpose(Matrix.Invert(Camera.Current.ProjectionMatrix)));
            _shadowShader.SetValue("g_matLightViewProj", Matrix.Transpose(vp));
            _shadowShader.SetValue("g_vShadowMapSize", new Vector2(ShadowMapSize, ShadowMapSize));
            _shadowShader.SetValue("g_vOcclusionTextureSize", new Vector2(Display.ClientWidth, Display.ClientHeight));

            _shadowShader.ApplyChanges();
            
            _shadowShader.Draw(_quad);

            _shadowShader.UnsetRenderTexture(ShaderType.PS, 0);
            _shadowShader.UnsetRenderTexture(ShaderType.PS, 1);

            Renderer.Instance.SetDepthTestState(true);
        }

        /// <summary>
        /// Dispose the shadow renderer.
        /// </summary>
        public void Dispose()
        {
            ShadowMap?.Dispose();
            ShadowOcculusion?.Dispose();
            _depth?.Dispose();
            _quad?.Dispose();
            _sampler?.Dispose();
            _shadowShader?.Dispose();
        }

        /// <summary>
        /// ShadowMap render target.
        /// </summary>
        public RenderTarget ShadowMap { get; private set; }

        /// <summary>
        /// Shadow occulusion.
        /// </summary>
        public RenderTarget ShadowOcculusion { get; private set; }

        /// <summary>
        /// Directional light direction
        /// </summary>
        public Vector3 LightDir { get; set; }
    }
}