# KenshiPy

KenshiPy is a DLL extension for [Kenshi](https://store.steampowered.com/app/233860/Kenshi/) that embeds a Python runtime and exposes selected portions of KenshiLib to Python via SWIG-generated bindings.

The goal is to provide a Python scripting workflow to mod Kenshi and, hopefully, lowering the barrier to entry for utilizing KenshiLib through rapid iteration and scripting.

---

## Overview

KenshiLib allows developers to write C++ DLLs that integrate directly with Kenshi’s runtime. KenshiPy follows the same architectural pattern:

- Compiled and loaded into Kenshi at runtime
- Embeds a **Python 3.4 interpreter**
- Exposes KenshiLib APIs through **SWIG 3.0.12** bindings

## How much of KenshiLib can be used from Python?

It's safe to assume that basically all functions that use standard types (ints, floats, bools, etc) should be usable.
- For example, calling functions like GameWorld::userPause(bool p) that don't return anything and use standard types should work.
- The same should apply to similar member variables of standard types.
- If they don't work, it more likely that I didn't include their headers properly or they initially didn't work and are stil being suppressed.

## Toolchain & Version Constraints

KenshiLib intentionally targets the same legacy constraints as Kenshi,
- **KenshiLib 0.2.4**
- **MSVC 2010**
- **x64**
- **Limited C++11 support**
- **Boost 1.60.0**
and because of these contraints we are target the following:
- **SWIG 3.0.12**
- **Python 3.4**

## Installation

1. Download the current release
2. Extract and place /KenshiPython/ inside of Kenshi's root directory.
3. Append to the end of Plugin_x64.cfg (which is located in Kenshi's root directory)
```
Plugin=KenshiPython\KenshiPy
```

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

Pressing ``Alt + ` `` when in-game, opens a console that can also be used to access a python interpreter console. This expects a single line so use semicolons for multiline input, like so:
```python
import KenshiPy; KenshiPy,DebugLog("Hello from the in-game console");
```
or you can run any python scripts by using run(). It will check the Kenshi's root directory by default, but you can also just use the path to your script if stored elsewhere.
Just make sure to either use double backslashes `\\` 
```python
run("path\\to\\example.py")
```
or prefix the `"path\to\example.py"` with a `r` like so
```python
run(r"path\to\example.py")
```
