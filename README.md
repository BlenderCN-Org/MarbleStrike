# MarbleStrike
Development source code and assets for the mobile game Marble Strike on iOS, Android and Windows Phone.

## Synopsis

- assets
  - effects: Render states and shader effects.
  - font: Bitmap texture fonts.
  - fx: Particle effects.
  - game: Game configuration and level assets.
  - shaders: GLSL and HLSL shader code.
  - wxgui: wxFormBuilder layouts for editor tools.
- build
  - Custom assets and scripts used to build the Win32 version of the game using DirectX 9 and OpenGL. With the ability to swap between the two at runtime.
- buildandroid
  - Custom assets and scripts used to build the Android version of the game.
- buildd3d11
  - Custom assets and scripts used to build the Win32 version of the game using DirectX 11.
- buildios
  - Custom assets and scripts used to build the iOS version of the game.
- buildgles1
  - Custom assets and scripts used to build the Win32 version of the game using OpenGL ES 1.0.
- buildgles2
  - Custom assets and scripts used to build the Win32 version of the game using OpenGL ES 2.0.
- buildwin8
  - Custom assets and scripts used to build the Windows 8 version of the game.
- buildwp
  - Custom assets and scripts used to build the Windows Phone version of the game.
- code
  - Source code for all platforms.
- sdk
  - External SDKs used for the development of the game. See readme file for a list of SDKs and their version used.
- tools
  - blenderscripts: Scripts for Blender (blender.org) to export geometry and object data into custom game format.
  - d3dshadertool: Used to generate description (.desc) files for mapping shader constant names and texture resources for material sharing.
  - gametweak: Tool used to connect to the game at runtime to allow for real time updates on shader changes.
  - guieditor: Editor used to build UI assets for in-game use. Uses wxWidgets framework.
  - particleeditor: Editor used to build particle effects for in-game use. Uses wxWidgets framework.
  - scripts: Contains random tooling scripts.

**External tools used:**
- Codehead's Bitmap Font Generator for generating bitmap fonts for in-game use.
- PVRTexTool for generating PVR textures for iOS.
- SpriteSheetPakcer for generating sprite sheets, especially for UI textures.
- wxFormBuilder for generating wxForm GUI layouts.
- nvdxt for generating dds textures for DirectX.

## License

MIT License