import KenshiPy as kp

ou = KenshiPy.ou()

if ou:
    print("GameWorld object found.")
else:
    print("GameWorld object couldn't be found.")

if ou.initialized():
    print("GameWorld initialized = True.")
else:
    print("GameWorld initialized = False.")

if ou.steamEnabled():
    print("Steam platform = True.")
else:
    print("Steam platform = False.")

if ou.showPlayerAMessage("Hello from Python!", False):
    print("Player message sent successfully.")
else:
    print("Failed to send player message.")

if ou.setGameSpeed(1.5, False):
    print("Game speed set successfully.")
else:
    print("Failed to set game speed.")