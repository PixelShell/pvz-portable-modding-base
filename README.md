![alt text](https://github.com/PixelShell/pvz-portable-modding-base/blob/main/icon-readme.png)
A fork of [PvZ Portable](https://github.com/wszqkzqk/PvZ-Portable) to make it easier to make multi-platform PvZ mods. Supports Windows, Linux, Android and Web builds.

Features:
- A custom resources folder that's separate from main.pak and properties. You can add your mod's own assets here without modifying main.pak at all. Resources here override the resources with the same name
in main.pak or properties so you can replace assets without having to distribute a modified main.pak or have to patch it. You'll have to place this folder along with your own main.pak and properties in the
same folder as the executable. For Android and Web builds the resources folder is automatically added into the executable so you don't need to distribute it separately.
- Basic QoTL features such as a speed-up button as well as an extra options menu with a debug toggle, healthbars, and sun & coin auto-collection.
- Seed packet hotkeys are available at all times.
- Some minor bug fixes.
