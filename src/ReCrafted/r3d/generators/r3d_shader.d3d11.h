#pragma once

#ifndef R3D_SHADER_D3D11_H
#define R3D_SHADER_D3D11_H

// includes
#include <vector>

void generate_d3d11(
	std::string& source,
	std::string& input,
	std::string& output,
	std::vector<std::string>& buffers,
	std::vector<std::string>& buffer_names);

#endif // R3D_SHADER_D3D11_H
