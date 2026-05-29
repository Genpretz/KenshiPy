import KenshiPy

KC_V = 47

def _toggle_prone():
    KenshiPy.DebugLog("[Go Prone] _toggle_prone called")

    world = KenshiPy.getGameWorld()
    KenshiPy.DebugLog("[Go Prone] Got world: {}".format(world))

    if not world or not world.player:
        KenshiPy.DebugLog("[Go Prone] No world or no player")
        return

    c = world.player.selectedCharacter.getCharacter()
    KenshiPy.DebugLog("[Go Prone] Got character: {}".format(c))

    if not c:
        KenshiPy.ErrorLog("[Go Prone] No selected character.")
        return

    state = c.getProneState()
    KenshiPy.DebugLog("[Go Prone] Current prone state: {}".format(state))

    if state == KenshiPy.PS_NORMAL:
        KenshiPy.DebugLog("[Go Prone] Switching to STAYING_LOW")
        c.setStealthMode(True)
        c.setProneState(KenshiPy.PS_STAYING_LOW)
    elif state == KenshiPy.PS_STAYING_LOW:
        KenshiPy.DebugLog("[Go Prone] Switching to NORMAL")
        c.setProneState(KenshiPy.PS_NORMAL)
        c.setStealthMode(False)
    else:
        KenshiPy.DebugLog("[Go Prone] Unknown state: {}".format(state))
        return

def on_key_down(key_code):
    KenshiPy.DebugLog("[Go Prone] Key pressed: {}".format(key_code))
    if key_code == KC_V:
        KenshiPy.DebugLog("[Go Prone] V key detected, toggling prone")
        _toggle_prone()

KenshiPy.DebugLog("[Go Prone] Registering key callback")

# RegisterKeyDownCallback expects a function that takes a single argument
KenshiPy.RegisterKeyDownCallback(on_key_down)
KenshiPy.DebugLog("[Go Prone] Callback registered successfully")