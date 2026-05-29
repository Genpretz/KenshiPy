import KenshiPy

ou = KenshiPy.getGameWorld()
if ou:
     if ou.paused:
          print("Game is paused. Unpausing game...")
          ou.userPause(False)
     else:
          print("Game is not paused. Pausing game...")
          ou.userPause(True)
else:
     print("GameWorld not initialized yet")
