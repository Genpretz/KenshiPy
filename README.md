# KenshiPy

KenshiPy is a native DLL extension for [Kenshi](https://store.steampowered.com/app/233860/Kenshi/) that embeds a Python runtime and exposes selected portions of KenshiLib to Python via SWIG-generated bindings.

The goal is to provide a Python scripting workflow comparable to KenshiLib’s C++ DLL modding model—while lowering the barrier to entry for rapid iteration and scripting.

---

## Overview

KenshiLib allows developers to write C++ DLLs that integrate directly with Kenshi’s runtime. KenshiPy follows the same architectural pattern:

- Compiled as an **x64 DLL**
- Loaded into Kenshi at runtime
- Embeds a **Python 3.4 interpreter**
- Exposes KenshiLib APIs through **SWIG 3.0.12** bindings
- Automatically loads Python scripts from:
```
   .\Kenshi\mods\<modname>\KenshiPy.json
```

Any Python script declared in that file is discovered and executed when the interpreter initializes.

> **Note:** Not all KenshiLib functions are currently wrapped or exposed.

---

## Toolchain & Version Constraints

This project intentionally targets the same legacy constraints as Kenshi/KenshiLib:

- **MSVC 2010**
- **x64 only**
- **Limited C++11 support**
- **SWIG 3.0.12**
- **KenshiLib 0.1.5**
- **Python 3.4**

These constraints are deliberate to maintain ABI compatibility with Kenshi’s runtime and KenshiLib.

---

## Architecture

### 1. Native Layer

- KenshiPy is built as a DLL.
- It links against KenshiLib.
- It initializes an embedded Python interpreter.
- It exposes wrapped C++ symbols through a generated `_KenshiPy` module.

### 2. SWIG Binding Layer

- SWIG generates:
  - C++ wrapper glue code
  - A Python extension module
- Explicit template instantiations are used where required.
- Only a curated subset of KenshiLib headers are included to control API surface.

### 3. Script Discovery & Execution

On initialization:

1. The interpreter scans the list of active mods for: 
   .\Kenshi\mods\<modname>\scripts\KenshiPy.json
2. Each `.py` file listed in that JSON file is executed within the embedded runtime.
3. Scripts interact with Kenshi through the exposed wrapped API.

This mirrors the C++ DLL injection model but substitutes Python as the scripting layer.

---

## Project Goals

- Provide a lightweight Python modding workflow.
- Preserve KenshiLib’s programming model.
- Maintain strict binary compatibility with Kenshi.
- Avoid modifying KenshiLib function names or signatures.
- Keep namespace-to-file mapping consistent with KenshiLib structure.

---

## Limitations

- Not all KenshiLib APIs are wrapped.
- Python version is locked to 3.4 due to toolchain constraints.
- MSVC 2010 limits modern C++ features.
- SWIG 3.0.12 lacks newer Python/C++ binding capabilities.

This project prioritizes compatibility and stability over modern language features.

---

## Example Usage

After installing KenshiPy in Kenshi's root directory, you can create a mod with the following structure:
```
Kenshi/mods/MyMod/scripts/example.py
```

```python
import KenshiPy

KenshiPy.Debug.DebugLog("Hello from Python")
```