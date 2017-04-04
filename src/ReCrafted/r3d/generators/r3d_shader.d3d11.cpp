// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#include "../r3d_shader.utils.h"
#include "../r3d.h"

auto r3d_hlsl_api = "\
#define HLSL 1				\n\
#define vec4 float4			\n\
#define vec3 float3			\n\
#define vec2 float2			\n\
#define mat4 matrix			\n\
#define mat3 float3x4		\n\
";

void generate_d3d11(
	r3d_shadertype::Enum type,
	std::string& source,
	std::string& input,
	std::string& output,
	std::vector<std::string>& buffers,
	std::vector<std::string>& buffer_names)
{
	// inject input and output (replace 'void main()')

	auto input_fields = compiler_utils::splitString(input, '\n');
	auto output_fields = compiler_utils::splitString(output, '\n');

	std::string method_head = "void main( ";

	for (auto i = 0u; i < input_fields.size(); i++)
	{
		auto field = input_fields[i];

		if (field.length() > 4) // ignore new line or some other invalid cases
		{
			compiler_utils::replace(field, ";", "");

			method_head += "in " + field + ", ";
		}
	}

	if(type == r3d_shadertype::vertexshader)
		method_head += "out float4 out_position : SV_POSITION";

	if (type == r3d_shadertype::pixelshader)
		method_head += "out float4 out_color : COLOR0";

	for (auto i = 0u; i < output_fields.size(); i++)
	{
		auto field = output_fields[i];

		if (field.length() > 4) // ignore new line or some other invalid cases
		{
			compiler_utils::replace(field, ";", "");
			method_head += ", out " + field;
		}
	}

	method_head += " )";
	compiler_utils::replace(source, "void main()", method_head);

	// inject buffers
	std::string buffers_source = "";
	for (auto i = 0u; i < buffers.size(); i++)
	{
		auto buffer = buffers[i];
		auto bufferName = buffer_names[i];
		auto fields = compiler_utils::splitString(buffer, '\n');

		buffers_source += "cbuffer " + bufferName + " : register(b" + std::to_string(i) + ")\n{\n";

		for (auto j = 0u; j < fields.size(); j++)
		{
			auto field = fields[j];

			if (field.length() > 4) // ignore new line or some other invalid cases
			{
				buffers_source += field + "\n";
			}
		}
		buffers_source += "}";
	}

	// inject buffers and r3d api
	std::string api = r3d_hlsl_api;
	auto tmp = source;
	source = api + "\n" + buffers_source + "\n" + tmp;

	// TODO: optimize
}
