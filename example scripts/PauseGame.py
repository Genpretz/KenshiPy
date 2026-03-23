import KenshiPy

ou = KenshiPy.ou()
if ou:
    ou.userPause(True)
else:
    print("GameWorld not initialized yet")