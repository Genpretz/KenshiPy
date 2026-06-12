import KenshiPy

KC_V = 47

def toggle_prone():
    world = KenshiPy.getGameWorld()

    if not world or not world.player:
        return
    c = world.player.selectedCharacter.getCharacter()
    if not c:
        return

    state = c.getProneState()
    if state == KenshiPy.PS_NORMAL:
        c.setStealthMode(True)
        c.setProneState(KenshiPy.PS_STAYING_LOW)
    elif state == KenshiPy.PS_STAYING_LOW:
        c.setProneState(KenshiPy.PS_NORMAL)
        c.setStealthMode(False)

def on_key_down(key_code):
    if key_code == KC_V:
        toggle_prone()

# RegisterKeyDownCallback expects a function that takes a single argument
KenshiPy.RegisterKeyDownCallback(on_key_down)
KenshiPy.DebugLog("[Go Prone] Callback registered successfully")