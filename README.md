# ReCrafted

## External tools
- [VisualStudio 2015/2017](https://www.visualstudio.com/) (IDE, Windows)
- [VisualStudio Code](https://code.visualstudio.com/) with C++ extensions (IDE, Linux)
- XCode (IDE, OSX)
- [TexturePacker](https://www.codeandweb.com/texturepacker) for creating atlases

## Building
1. Clone ReCrafted repo into suitable directory: `X:/ReCrafted/` (should contain: licenses, src, tools etc.)
2. Clone ReCraftedBinaries repo into suitable directory: `X:/ReCrafted/build/` (should contain: assets, bin64, mono etc.)
3. Download precompiled libs:
https://mega.nz/#!Xh4nFD4T!8ZLfnz27OBBz0sEcSWZqLITuhnyQtEbancWE08TBuHg
4. Unzip all files from downloaded archive - `libs_prebuilt.zip/libs_temp/` into `X:/ReCrafted/libs/`.
5. Create `X:/ReCrafted/username.txt` file with your nickname.
6. Run generate_project_X.(sh/bat) file in the root directory to build needed projects.
7. Done! For example: open .projects/vs2015/ReCrafted.sln for C++ project.

Warning: If you pull newly created files for C++ project, you must run generate_project_X.(sh/bat) again and then reload project.

## Links
- [Trello](https://trello.com/b/xGap0YZQ/recrafted)
- [Mind42](https://mind42.com/mindmap/4a8e42f6-a7e8-49a2-832c-3595e9a25301)

*ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski*
