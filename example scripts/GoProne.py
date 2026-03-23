import KenshiPy

KC_V = 47

def _toggle_prone():
    world = KenshiPy.getGameWorld()
    if not world or not world.player:
        return
    c = world.player.selectedCharacter.getCharacter()
    if not c:
        KenshiPy.ErrorLog("[Go Prone] No selected character.")
        return

    state = c._NV_getProneState()
    if state == KenshiPy.PS_NORMAL:
        c.setStealthMode(True)
        c._NV_setProneState(KenshiPy.PS_STAYING_LOW)
    elif state == KenshiPy.PS_STAYING_LOW:
        c._NV_setProneState(KenshiPy.PS_NORMAL)
        c.setStealthMode(False)

def on_key_down(key_code):
    if key_code == KC_V:
        _toggle_prone()

KenshiPy.RegisterKeyDownCallback(on_key_down)