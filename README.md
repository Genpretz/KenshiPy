# KenshiPy

KenshiPy is a DLL extension for [Kenshi](https://store.steampowered.com/app/233860/Kenshi/) that embeds a Python runtime and exposes selected portions of KenshiLib to Python via SWIG-generated bindings.

The goal is to provide a Python scripting workflow to mod Kenshi and, hopefully, lowering the barrier to entry for utilizing KenshiLib through rapid iteration and scripting.

---

## Overview

KenshiLib allows developers to write C++ DLLs that integrate directly with Kenshi’s runtime. KenshiPy follows the same architectural pattern:

- Compiled and loaded into Kenshi at runtime
- Embeds a **Python 3.4 interpreter**
- Exposes KenshiLib APIs through **SWIG 3.0.12** bindings

---

## Toolchain & Version Constraints

This project intentionally targets the same legacy constraints as Kenshi/KenshiLib:

- **MSVC 2010**
- **x64**
- **Limited C++11 support**
- **SWIG 3.0.12**
- **KenshiLib 0.1.5**
- **Python 3.4**

These constraints are deliberate to maintain ABI compatibility with Kenshi’s runtime and KenshiLib.

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

```python
import KenshiPy

KenshiPy.DebugLog("Hello from Python")
```

Using the ` key when in-game, opens a console that can also be used to run python, like so:
```python
import KenshiPy; KenshiPy,DebugLog("Hello from the in-game console");
```
or you can run any script available in Kenshi's root directory
```python
run("example.py")
```
