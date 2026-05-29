import KenshiPy

print(KenshiPy)

world = KenshiPy.getGameWorld()
chars = world.getCharacterUpdateList()

print(chars)
print(len(chars))
print(dir(chars))
help(chars)


