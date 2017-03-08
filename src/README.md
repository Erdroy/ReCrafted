# BUILDING - Developer draft

1) Download and instal
http://gnuwin32.sourceforge.net/downlinks/make.php
http://gnuwin32.sourceforge.net/downlinks/coreutils.php
http://gnuwin32.sourceforge.net/downlinks/libiconv.php
http://gnuwin32.sourceforge.net/downlinks/libintl.php

2) Clone repos: https://github.com/bkaradzic/bgfx and https://github.com/bkaradzic/bx
into directories: bgfx and bx(under current directory - src/, should be: src/bgfx, src/bx)

3) Enter bgfx directory and type command 'make' in cmd, then wait.

4) Open VS project, bgfx/.build/projects/vs????/bgfx.sln

5) Delete examples in bgfx, bgfx-shared-lib, and tools: geometryc, texturec, texturev

6) Change BGFX_CONFIG_MAX_VERTEX_BUFFERS and BGFX_CONFIG_MAX_INDEX_BUFFERS in `bgfx/config.h` to: (16 << 10)

should be:
#ifndef BGFX_CONFIG_MAX_INDEX_BUFFERS
#	define BGFX_CONFIG_MAX_INDEX_BUFFERS (16 << 10)
#endif // BGFX_CONFIG_MAX_INDEX_BUFFERS

#ifndef BGFX_CONFIG_MAX_VERTEX_BUFFERS
#	define BGFX_CONFIG_MAX_VERTEX_BUFFERS (16 << 10)
#endif // BGFX_CONFIG_MAX_VERTEX_BUFFERS

7) Compile everything(debug/release for 32bit and 64bit).

8) Done, run copy_binaries64.bat or/and copy_binaries32.bat in the root directory of ReCrafted repo,
after this, you should build the game without any problems.

ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski