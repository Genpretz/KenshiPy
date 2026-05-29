import KenshiPy

KenshiPy.DebugLog("=== BountyManager Test Start ===")

world = KenshiPy.getGameWorld()

KenshiPy.DebugLog("world = {}".format(world))

player = world.player
KenshiPy.DebugLog("player = {}".format(player))

selected = player.selectedCharacter
KenshiPy.DebugLog("selectedCharacter = {}".format(selected))

character = selected.getCharacter()
KenshiPy.DebugLog("character = {}".format(character))

bountyManager = character.crimes
KenshiPy.DebugLog("bountyManager = {}".format(bountyManager))

## Test ordinary wrapped methods

try:
    total = bountyManager.getTotalBounty()
    KenshiPy.DebugLog("getTotalBounty OK: {}".format(total))
except Exception as e:
    KenshiPy.ErrorLog("getTotalBounty FAILED: {}".format(e))

try:
    committing = bountyManager.isCommittingCrime()
    KenshiPy.DebugLog("isCommittingCrime OK: {}".format(committing))
except Exception as e:
    KenshiPy.ErrorLog("isCommittingCrime FAILED: {}".format(e))

try:
    threshold = bountyManager.getBountyRecognitionThreshold()
    KenshiPy.DebugLog("getBountyRecognitionThreshold OK: {}".format(threshold))
except Exception as e:
    KenshiPy.ErrorLog("getBountyRecognitionThreshold FAILED: {}".format(e))

## Test new wrapper

try:
    bounties = bountyManager.getBounties()
    KenshiPy.DebugLog("getBounties OK: {}".format(bounties))
except Exception as e:
    KenshiPy.ErrorLog("getBounties FAILED: {}".format(e))
    bounties = None

## Test keys()

if bounties:

    try:
        keys = bounties.keys()

        KenshiPy.DebugLog("keys() OK")
        KenshiPy.DebugLog("Faction count: {}".format(len(keys)))

        for faction in keys:

            KenshiPy.DebugLog("Faction: {}".format(faction))

            bounty = bounties.get(faction)

            if bounty:
                KenshiPy.DebugLog("Bounty object OK: {}".format(bounty))

                try:
                    KenshiPy.DebugLog("Current amount: {}".format(bounty.amount))
                except Exception as e:
                    KenshiPy.ErrorLog("Reading bounty.amount FAILED: {}".format(e))

    except Exception as e:
        KenshiPy.ErrorLog("keys/get FAILED: {}".format(e))

KenshiPy.DebugLog("=== BountyManager Test End ===")