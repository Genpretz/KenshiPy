# KenshiPy

KenshiPy is a DLL extension for [Kenshi](https://store.steampowered.com/app/233860/Kenshi/) that embeds a Python runtime and exposes selected portions of KenshiLib to Python via SWIG-generated bindings.

The goal is to provide a Python scripting workflow to mod Kenshi and, hopefully, lowering the barrier to entry for utilizing KenshiLib through rapid iteration and scripting.

---

## Overview

KenshiLib allows developers to write C++ DLLs that integrate directly with Kenshi’s runtime. KenshiPy follows the same architectural pattern:

- Compiled DLL is loaded into Kenshi at runtime
- Embeds a **Python interpreter**
- Exposes KenshiLib APIs through **SWIG** generated bindings

## How much of KenshiLib can be used from Python?

It's safe to assume that basically all functions that use standard types (ints, floats, bools, etc) should be usable.
- For example, calling functions like GameWorld::userPause(bool p) that return void and use standard data types should work.
- The same should apply to any member variables of standard data types.
- Ogre, MyGui, and Boost libraries have not been wrapped and so any parts of KenshiLib requiring the use of them should be assumed non-functional.

## Toolchain & Version Constraints

KenshiPy intentionally targets the same legacy constraints as Kenshi. We target the following because they are the latest releases supporting MSVC 2010:
- **SWIG 3.0.12**
- **Python 3.4**


## Installation

1. Ensure RE_Kenshi v0.3.3 or Later is installed.
2. Download the latest release and extract the archive, placing the contents in Kenshi's `mods` directory.
3. Activate the mod using the games launcher like you would any other mod.

## Example Usage

After installing KenshiPy, any active mods will automatically loads Python scripts declared in:
```
   .\Kenshi\mods\<modname>\KenshiPy.json
```
```json
{
    "Scripts": ["example.py"]
}
```

```
   .\Kenshi\mods\<modname>\example.py
```
Example Python Script
```python
import KenshiPy

KenshiPy.DebugLog("Hello from Python")
```

Pressing ``Ctrl + ` `` when in-game, open KenshiPy's Script Editor GUI. From here you can open, edit, save, and run scripts using the buttons on the Script Editor's toolbar.

Python's print function will print in the in-game editor's output box as well as be copied to the `KenshiPy.log` and any scripts using KenshiLib's DebugLog or ErrorLog functions can find their logs in `KenshiLib-py.log`. Both files can be found in `.\Kenshi\mods\KenshiPython`.
