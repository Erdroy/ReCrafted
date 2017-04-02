// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#ifndef R3D_HEADERS_H
#define R3D_HEADERS_H

struct r3d_cmdlist_header
{
	enum Enum
	{
		unknown = 0,

		target_window,
		vsync,
		viewport,
		rendermode,

		clearcolor,
		cleardepth,

		create_vertexbuffer,
		create_indexbuffer,

		present,

		endframe = present,
		beginframe,

		count
	};
};

#endif // R3D_HEADERS_H
