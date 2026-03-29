import KenshiPy as kp

ou = KenshiPy.ou()

if ou:
    print("GameWorld object found.")
else:
    print("GameWorld object couldn't be found.")

if ou.initialized:
    print("GameWorld initialized = True.")
else:
    print("GameWorld initialized = False.")

if ou.steamEnabled:
    print("Steam platform = True.")
else:
    print("Steam platform = False.")

ou.showPlayerAMessage("Hello from Python! Player message sent successfully.", False)

ou.setGameSpeed(5.0, False)
print("Game speed set to 5.0.")