// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#include "../r3d_shader.utils.h"

void generate_d3d11(
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

	// TODO: if vertex shader
	method_head += "out float4 out_position : SV_POSITION, ";

	for (auto i = 0u; i < output_fields.size(); i++)
	{
		auto field = output_fields[i];

		if (field.length() > 4) // ignore new line or some other invalid cases
		{
			compiler_utils::replace(field, ";", "");

			method_head += "out " + field; // if not last
			if (i + 1 < output_fields.size())
				method_head += ", ";
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

		buffers_source += "cbuffer " + bufferName + " : register(b" + std::to_string(i) + ") { \n";

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

	// inject r3d api
	// optimize
}
