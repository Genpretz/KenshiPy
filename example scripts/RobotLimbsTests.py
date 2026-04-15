# RobotLimbsTests.py
import KenshiPy as kenshi

def test_robot_limbs(character):
    print("Creating RobotLimbs...")
    rl = kenshi.RobotLimbs(character)

    limbs = [
        kenshi.RobotLimbs.LEFT_ARM,
        kenshi.RobotLimbs.RIGHT_ARM,
        kenshi.RobotLimbs.LEFT_LEG,
        kenshi.RobotLimbs.RIGHT_LEG
    ]

    print("\n--- Initial States ---")
    for limb in limbs:
        state = rl.getState(limb)
        print("Limb:", limb, "State:", state)

    print("\n--- Setting States ---")
    print("Setting RobotLimbs.LEFT_ARM to LIMB_STUMP")
    rl.setState(kenshi.RobotLimbs.LEFT_ARM, kenshi.LIMB_STUMP)
    print("Setting RobotLimbs.RIGHT_ARM to LIMB_REPLACED")
    rl.setState(kenshi.RobotLimbs.RIGHT_ARM, kenshi.LIMB_REPLACED)
    print("Setting RobotLimbs.LEFT_LEG to LIMB_CRUSHED")
    rl.setState(kenshi.RobotLimbs.LEFT_LEG, kenshi.LIMB_CRUSHED)
    print("Setting RobotLimbs.RIGHT_LEG to LIMB_ORIGINAL")
    rl.setState(kenshi.RobotLimbs.RIGHT_LEG, kenshi.LIMB_ORIGINAL)

    print("\n--- Updated States ---")
    for limb in limbs:
        state = rl.getState(limb)
        print("Limb:", limb, "State:", state)

    print("\n--- Setting Items (None for test) ---")
    rl.setLimb(kenshi.RobotLimbs.RIGHT_ARM, kenshi.LIMB_STUMP, None)
    rl.setLimb(kenshi.RobotLimbs.LEFT_LEG, kenshi.LIMB_STUMP, None)

    print("\n--- Getting Items ---")
    for limb in limbs:
        item = rl.getLimb(limb)
        print("Limb:", limb, "Item:", item)


# Entry point depending on how KenshiPy runs scripts
def main():
    print("RobotLimbs test starting...")

    world = kenshi.getGameWorld()

    try:
        character = world.player.selectedCharacter.getCharacter()
    except:
        print("Failed to get selected character.")
        print("Make sure you've selected a player character")
        return

    test_robot_limbs(character)


if __name__ == "__main__":
    main()