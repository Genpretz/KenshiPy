# KenshiPy

KenshiPy is a DLL extension for [Kenshi](https://store.steampowered.com/app/233860/Kenshi/) that embeds a Python runtime and exposes selected portions of KenshiLib to Python via SWIG-generated bindings.

The goal is to provide a Python scripting workflow to mod Kenshi and lowers the barrier to entry for utilizing KenshiLib through rapid iteration and scripting.

## Overview

[RE_Kenshi](https://www.nexusmods.com/kenshi/mods/847) ([GitHub](https://github.com/BFrizzleFoShizzle/RE_Kenshi)) is a Kenshi mod that adds improvements that can't be achieved through traditional mod tools, including a C++ plugin system for modifying the game's code. KenshiLib builds on this to allow developers to write C++ DLLs that integrate directly with Kenshi's runtime. KenshiPy follows the same architectural pattern:

- Compiled DLL is loaded into Kenshi at runtime
- Embeds a **Python interpreter**
- Exposes KenshiLib APIs through **SWIG** generated bindings

## How much of KenshiLib can be used from Python?

It's safe to assume that basically all functions that use standard types (ints, floats, bools, etc) should be usable.
- For example, C++ functions that return void and use standard data types should be available to call from Python.

```c++
class GameWorld
{
    void userPause(bool p);
}
```

should be useable like so

```python
# Get pointer to GameWorld object
world = getGameWorld()

world.userPause(True)
# Game should now be paused
```

- The same should apply to any member variables of standard data types.

The parts of KenshiLib that explicitly do not work:
- Ogre, MyGui, and Boost libraries have not been wrapped and so any parts of KenshiLib requiring the use of them should be assumed non-functional.
- KenshiLib's use of MinHook for hooking functionality. Hooks need to be defined within the KenshiPy as part of a callback system in order for Python to use them. See [Callbacks](#callbacks) below for what is currently available.

### Things I'd like to Add
- Support for lektor<T>, ogre_unordered_set, and ogre_unordered_map types
- Bindings allowing MyGUI widgets to be created from Python
- More callbacks


## Installation

1. Ensure [RE_Kenshi](https://www.nexusmods.com/kenshi/mods/847) v0.3.3 or later is installed.
2. Download the latest release of KenshiPy and extract the archive, placing the contents in Kenshi's `mods` directory.
3. Activate the mod using the game's launcher like you would any other mod.

## Usage

### Script Editor GUI

Pressing `Ctrl` + `` ` `` in-game will open KenshiPy's Script Editor GUI. From here you can load, edit, save, and run scripts using the buttons on the Script Editor's toolbar.

### Loading Python Scripts

When the game starts, KenshiPy will check whether any active mods contain a file, `KenshiPy.json`, and if found will load the Python scripts declared within.

#### Example

Place **KenshiPy.json** within a given mod's directory
```
   .\Kenshi\mods\<modname>\KenshiPy.json
```
**KenshiPy.json** should be formatted like so
```json
{
    "Scripts": ["example.py"]
}
```
Assuming the file **example.py** exists at this location.
```
   .\Kenshi\mods\<modname>\example.py
```

### Example Python Script

**Example.py**
```python
import KenshiPy

KenshiPy.DebugLog("Hello from Python")
```

## Compiling from Source
 
**Quick Guide**
1. Use SWIG 3.0.12 to generate the Python binding wrapper, `GeneratedWrappper.cpp`, and KenshiPy Python module, `KenshiPy.py`, from KenshiLib's headers and KenshiPy's interface files
2. Compile the generated wrapper alongside the rest of KenshiPy's source into `KenshiPy.dll`
3. Place the `KenshiPy.dll` and `KenshiPy.py` along with `python34.dll`, `Kenshi_ScriptEditor_EditBox.xml`, `RE_Kenshi.json` into a mod folder named `KenshiPython`
### Prerequisites
 
- **Visual Studio 2010 Professional/Ultimate**
- **Visual Studio 2022 Community**
- **SWIG 3.0.12**
- **Python 3.4**
- **Kenshi**

The project's include paths, library paths, and build events use environment variables to account for varying installation locations, avoiding the need to hardcode paths in the project file. You will need to set the variables listed below before building.
 
1. **SWIG 3.0.12** — set environment variable `SWIG_INSTALL_DIR` to SWIG's root directory (e.g. `C:\swigwin-3.0.12\`)
2. **Python 3.4** — set environment variable `PYTHON34_INSTALL_DIR` to Python 3.4's root directory (e.g. `C:\Python34\`)
3. **Kenshi** — set environment variable `KENSHI_INSTALL_DIR` to Kenshi's root directory
   - Steam: `C:\Program Files (x86)\Steam\steamapps\common\Kenshi\`
   - GOG: `C:\Program Files (x86)\GOG Galaxy\Games\Kenshi\`
### Building
 
`KenshiPy.vcxproj` can be built using Visual Studio's standard build option and includes two custom build events to help streamline the process. These events are disabled by default. If you choose to enable them, you can find them described below. If you prefer not to use them, manual alternatives are provided.
 
#### Pre-Build Event:
When this event is enabled, SWIG parses KenshiLib's headers and KenshiPy's interface files in order to generate the Python bindings, writing the output to `.\src\GeneratedWrapper.cpp` and `$(SolutionDir)bin\$(Configuration)\KenshiPython\KenshiPy.py`

When this event is disabled, either use a pre-existing copy of `GeneratedWrapper.cpp` from the source repository and move on, or run SWIG manually to generate the required files:
```
set SWIG_INSTALL_DIR=C:\swigwin-3.0.12\
set PYTHON_INSTALL_DIR=C:\Python34\
set OUTPUT_DIR=%SolutionDir%bin\%Configuration%\KenshiPython

"%SWIG_INSTALL_DIR%swig.exe" -Wall -v -c++ -python -threads -I"%SolutionDir%include" -I"%SolutionDir%interfaces" -I"%SolutionDir%extern\KenshiLib\Include" -outdir "%OUTPUT_DIR%" -o "%SolutionDir%src\GeneratedWrapper.cpp" "%SolutionDir%interfaces\KenshiPy.i"
```
**NOTE:** The large size of `GeneratedWrapper.cpp` and therefore `GenerateWrapper.obj` leads to long build times. In my experience they can be upwards of of around **1 hour** when building with Optimizations enabled (which they are by default).
 
#### Post-Build Event:
This event copies `python34.dll`, `Kenshi_ScriptEditor_EditBox.xml`, `RE_Kenshi.json` into the build output directory alongside `KenshiPy.dll`, then copies the entire output directory into Kenshi's mods folder (`<KENSHI_INSTALL_DIR>\mods\KenshiPython\`), making the mod immediately available to test in-game.
 
Choosing not to enable the Post_Build Event requires manually copying the build output, `python34.dll`, `Kenshi_ScriptEditor_EditBox.xml`, and `RE_Kensh.json` into Kenshi's mods folder.

## Callbacks

KenshiPy exposes a set of callbacks that allow Python scripts to respond to in-game events. Each callback type has a corresponding `Register` and `Unregister` function.

### Key Down
Fired every time a key is pressed. The callback receives the key code as an `int`.
```python
def on_key_down(key_code):
    KenshiPy.DebugLog("Key pressed: " + str(key_code))

KenshiPy.RegisterKeyDownCallback(on_key_down)
```

### Frame
Fired once per rendered frame via Ogre's frame listener. The callback receives the time since the last frame as a `float` (in seconds).
```python
def on_frame(delta_time):
    pass  # called every frame

KenshiPy.RegisterFrameCallback(on_frame)
```

### Character Say
Fired when a `Character` speaks a line of dialogue. The callback receives the `Character` and the message as a `str`.
```python
def on_character_say(character, message):
    KenshiPy.DebugLog(message)

KenshiPy.RegisterCharacterSayCallback(on_character_say)
```

### Character Select
Fired when the player selects a `Character`. The callback receives the `Character`.
```python
def on_character_select(character):
    pass

KenshiPy.RegisterCharacterSelectCallback(on_character_select)
```

### Character Unselect
Fired when the player deselects a `Character`. The callback receives the `Character`.
```python
def on_character_unselect(character):
    pass

KenshiPy.RegisterCharacterUnselectCallback(on_character_unselect)
```

### Character Declare Dead
Fired when a `Character` is declared dead. The callback receives the `Character`.
```python
def on_character_declare_dead(character):
    KenshiPy.DebugLog("A character has died.")

KenshiPy.RegisterCharacterDeclareDeadCallback(on_character_declare_dead)
```
