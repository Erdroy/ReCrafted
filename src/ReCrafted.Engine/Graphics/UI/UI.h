// ReCrafted © 2016-2017 Always Too Late

#ifndef UI_H
#define UI_H

// includes
#include "Core/Math/Color.h"
#include "Core/Math/Rectf.h"
#include "Common/ReCraftedAPI.h"
#include "Common/Text.h"
#include "Graphics/Shader.h"
#include "Graphics/Atlas.h"
#include "Core/Containers/Array.h"

struct Text;
class Font;

class UI
{
	API_DEF

private:
	struct vertex
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		float u = -1.0f;
		float v = -1.0f;

		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		float a = 1.0f;
	};

	struct drawcmd
	{
		vertex vertices[4];
		uint16_t indices[6];
		Rectf clipRect = {};
		uint texture = 0;
		float zOrder = 0.0f;
		bool hasClipRect = false;
		bool hasTexture = false;
		bool hasRenderTexture = false;
	};

private:
	const unsigned int m_maxVertexCount = 65535 * 3;
	const unsigned int m_maxIndexCount = 65535;

	const int m_vertexBufferSize = m_maxVertexCount * sizeof vertex;
	const int m_indexBufferSize = m_maxIndexCount * sizeof uint16_t;

private:
	static UI* m_instance;

private:
	Array<drawcmd> m_drawCmds = {};

	bgfx::VertexDecl m_vertexdecl = {};

	const byte* m_vertexBufferData = nullptr;
	const byte* m_indexBufferData = nullptr;

	uint m_vertexBufferDataPos = 0u;
	uint m_indexBufferDataPos = 0u;

	uint m_vertexCount = 0u;
	uint m_indexCount = 0u;

	Color m_color = {};

	float m_color_r = 1.0f;
	float m_color_g = 1.0f;
	float m_color_b = 1.0f;
	float m_color_a = 1.0f;

	float m_depth = 0.0f;

	Ptr<Shader> m_shader = nullptr;

private:
	static bool drawcmd_comparison(drawcmd& cmd1, drawcmd& cmd2);

	void clear();

	void drawnow();

	// drawing
	
	FORCEINLINE void setupVertexData(Rectf& rect, vertex& v0, vertex& v1, vertex& v2, vertex& v3) const;

	FORCEINLINE void finalizeVertexData(vertex& v0, vertex& v1, vertex& v2, vertex& v3, uint texture);

	FORCEINLINE void internal_drawBox(Rectf rect);

	FORCEINLINE void internal_drawBoxTextured(Rectf rect, uint texture, Rectf uvs);

	// internal

	FORCEINLINE void push_drawcmd(drawcmd* cmd, int index);

public:
	UI() { m_instance = this; }

public:
	void init();

	void dispose();
	
	void beginDraw();

	void endDraw();

public:
	// common
	/// <summary>
	/// Sets UI rendering color (boxes)
	/// </summary>
	/// <param name="color">The color.</param>
	static void setColor(Color color);

	/// <summary>
	/// Gets current UI rendering color.
	/// </summary>
	/// <returns>Current color.</returns>
	static Color getColor();
	
	/// <summary>
	/// Sets current depth.
	/// </summary>
	static void setDepth(float depth);

	// drawing
	/// <summary>
	/// Draws colored box on the screen.
	/// Rect X/Y is start coord and W/H is end coord.
	/// </summary>
	/// <param name="rect">The box rectangle in pixels.</param>
	static void drawBox(Rectf rect);

	/// <summary>
	/// Draws text on the screen.
	/// </summary>
	/// <param name="font">The font which will be used for drawing.</param>
	/// <param name="text">The text to be drawn.</param>
	/// <param name="position">The point in pixels where the text will be drawn on the screen.</param>
	static void drawText(Font* font, Text text, Vector2 position);

	/// <summary>
	/// Draws text on the screen.
	/// </summary>
	/// <param name="font">The font which will be used for drawing.</param>
	/// <param name="characters">The characters to be drawn.</param>
	/// <param name="characterCount">The character count.</param>
	/// <param name="position">The point in pixels where the text will be drawn on the screen.</param>
	static void drawText(Font* font, Char* characters, int characterCount, Vector2 position);

	/// <summary>
	/// Draws texture.
	/// Rect X/Y is start coord and W/H is end coord.
	/// </summary>
	/// <param name="texture">The texture which will drawn.</param>
	/// <param name="pos">The screen position (in pixels) where the texture will be drawn.</param>
	/// <param name="pos">The texture uvs.</param>
	static void drawTexture(Texture2D* texture, Rectf rect, Rectf uvs);
};

#endif // UI_H
