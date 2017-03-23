// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#ifndef R3D_D3D11_H
#define R3D_D3D11_H

// includes
#include "r3d_renderer.h"

class r3d_d3d11 : public r3d_renderer
{
private:

public:
	void init(void* window_handle) override;
	void destroy() override;
	void execute_commandlist(r3d_commandlist* cmdListPtr) override;
};

#endif // R3D_D3D11_H
