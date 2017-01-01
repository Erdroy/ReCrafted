// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Voxels;
using SharpDX;
using Vector2 = SharpDX.Vector2;
using Vector3 = SharpDX.Vector3;

namespace ReCrafted.Graphics
{
    public class ShadowRenderer : IDisposable
    {
        public const int ShadowMapSize = 2048;

        private RenderTarget _depth;
        private Mesh _quad;
        private Shader _shadowShader;
        private Sampler _sampler;

        public void Init()
        {
            ShadowMap = RenderTarget.Create(ShadowMapSize, ShadowMapSize, RenderTarget.TextureFormat.R32_Float);
            _depth = RenderTarget.Create(ShadowMapSize, ShadowMapSize, RenderTarget.TextureFormat.Depth);
            _sampler = Sampler.Create(Sampler.Type.PointClamped);
            ShadowOcculusion = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight, RenderTarget.TextureFormat.R32_Float);
            _shadowShader = Shader.FromFile("lighting/Shadows");

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

        public void Resize(int width, int height)
        {
            ShadowOcculusion.Resize(width, height);
        }

        public void RenderShadowMap()
        {
            ShadowMap.Clear(Color.Black);
            ShadowOcculusion.Clear(Color.Black);
            _depth.Clear(Color.Transparent);

            Renderer.Instance.SetViewportSize(ShadowMapSize, ShadowMapSize);
            Renderer.Instance.SetRenderTargetsDepth(_depth, ShadowMap);

            var lightPos = new Vector3(50, 50, 50);
            var view = Matrix.LookAtLH(lightPos, lightPos + LightDir, Vector3.Up);
            var proj = Matrix.OrthoLH(100, 100, -100, 100);

            VoxelWorld.Instance.DrawShadowMap(view*proj);
            
            Renderer.Instance.SetRenderTargets(ShadowOcculusion);
            Renderer.Instance.SetDepthTestState(false);
            Renderer.Instance.SetViewportSize(Display.ClientWidth, Display.ClientHeight);

            // draw
            _shadowShader.Apply();

            _shadowShader.SetRenderTexture(ShaderType.PS, 0, ShadowMap);
            _shadowShader.SetRenderTexture(ShaderType.PS, 1, Renderer.Instance.DepthRenderTarget);
            _shadowShader.SetSampler(0, _sampler);

            _shadowShader.SetValue("g_matInvView", Matrix.Transpose(Matrix.Invert(Camera.Current.ViewMatrix)));
            _shadowShader.SetValue("g_matInvProj", Matrix.Transpose(Matrix.Invert(Camera.Current.ProjectionMatrix)));
            _shadowShader.SetValue("g_matLightViewProj", Matrix.Transpose(view*proj));
            _shadowShader.SetValue("g_vShadowMapSize", new Vector2(ShadowMapSize, ShadowMapSize));
            _shadowShader.SetValue("g_vOcclusionTextureSize", new Vector2(Display.ClientWidth, Display.ClientHeight));

            _shadowShader.ApplyChanges();

            _shadowShader.Draw(_quad);
            _shadowShader.UnsetRenderTexture(ShaderType.PS, 0);
            _shadowShader.UnsetRenderTexture(ShaderType.PS, 1);

            Renderer.Instance.SetDepthTestState(true);
        }

        public void Dispose()
        {
            ShadowMap?.Dispose();
            ShadowOcculusion?.Dispose();
            _depth?.Dispose();
            _quad?.Dispose();
            _sampler?.Dispose();
            _shadowShader?.Dispose();
        }

        public RenderTarget ShadowMap { get; private set; }

        public RenderTarget ShadowOcculusion { get; private set; }

        public Vector3 LightDir { get; set; }
    }
}