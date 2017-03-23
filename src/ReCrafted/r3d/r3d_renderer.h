// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#ifndef R3D_RENDERER_H
#define R3D_RENDERER_H

// includes
#include "r3d.h"

class r3d_commandlist;

class r3d_renderer
{
public:
	virtual ~r3d_renderer() { }

	virtual void execute_commandlist(r3d_commandlist* cmdListPtr) = 0;
	virtual void destroy() = 0;
};

#endif // R3D_RENDERER_H