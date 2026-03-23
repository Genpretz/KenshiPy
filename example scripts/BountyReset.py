import KenshiPy

w = KenshiPy.getGameWorld()
chars = w.player.getAllPlayerCharacters()

uc = w.factionMgr.getFactionByName("United Cities")

for i in range(chars.size()):
    c = chars[i]
    bm = c.crimes
    total = bm.getTotalBounty()
    if total > 0:
        print("Clearing bounty on " + c.getName() + " (was: " + str(total) + ")")
        bm.clearBounty(uc)

print("Done.")
w.showPlayerAMessage("Bounties cleared.", False)