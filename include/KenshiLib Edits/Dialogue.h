#pragma once

#include "Enums.h"
#include "util/hand.h"
#include "util/OgreUnordered.h"
#include "util/lektor.h"
#include "util/TimeOfDay.h"
#include "FitnessSelector.h"
#include "TitleScreen.h"

#include <mygui/MyGUI_Widget.h>
#include <boost/unordered_set.hpp>
#include <ogre/OgreMemoryAllocatorConfig.h>

#include <map>
#include <set>


enum TalkerEnum
{
    T_ME,
    T_TARGET,
    T_TARGET_IF_PLAYER,
    T_INTERJECTOR1,
    T_INTERJECTOR2,
    T_INTERJECTOR3,
    T_WHOLE_SQUAD,
    T_TARGET_WITH_RACE
};

enum DialogActionEnum
{
    DA_NONE,
    DA_TRADE,
    DA_TALK_TO_LEADER,
    DA_JOIN_SQUAD_WITH_EDIT,
    DA_AFFECT_RELATIONS,
    DA_AFFECT_REPUTATION,
    DA_ATTACK_CHASE_FOREVER,
    DA_GO_HOME,
    DA_TAKE_MONEY,
    DA_GIVE_MONEY,
    DA_PAY_BOUNTY,
    DA_CHARACTER_EDITOR,
    DA_FORCE_SPEECH_TIMER,
    DA_DECLARE_WAR,
    DA_END_WAR,
    DA_CLEAR_AI,
    DA_FOLLOW_WHILE_TALKING,
    DA_THUG_HUNTER,
    DA_JOIN_SQUAD_FAST,
    DA_REMEMBER_CHARACTER,
    DA_FLAG_TEMP_ALLY,
    DA_FLAG_TEMP_ENEMY,
    DA_MATES_KILL_ME,
    DA_MAKE_TARGET_RUN_FASTER,
    DA_GIVE_TARGET_MY_SLAVES,
    DA_TAG_ESCAPED_SLAVE,
    DA_FREE_TARGET_SLAVE,
    DA_MERGE_WITH_SIMILAR_SQUADS,
    DA_SEPARATE_TO_MY_OWN_SQUAD,
    DA_ARREST_TARGET,
    DA_ARREST_TARGETS_CARRIED_PERSON,
    DA_ATTACK_TOWN,
    DA_ASSIGN_BOUNTY,
    DA_CRIME_ALARM,
    DA_RUN_AWAY,
    DA_INCREASE_FACTION_RANK,
    DA_LOCK_THIS_DIALOG,
    DA_ASSAULT_PHASE,
    DA_RETREAT_PHASE,
    DA_VICTORY_PHASE,
    DA_ENSLAVE_TARGETS_CARRIED_PERSON,
    CHOOSE_SLAVES_SELLING,
    CHOOSE_SLAVES_BUYING,
    CHOOSE_PRISONER_BAIL,
    CHOOSE_CONSCRIPTION,
    CHOOSE_RECRUITING,
    CHOOSE_HIRING_CONTRACT,
    SURRENDER_NON_HUMANS,
    CHOOSE_ANIMALS_BUYING,
    DA_CLEAR_BOUNTY,
    DA_PLAYER_SELL_PRISONERS,
    DA_PLAYER_SURRENDER_MEMBER_DIFFERENT_RACE,
    DA_SUMMON_MY_SQUAD,
    DA_REMOVE_SLAVE_STATUS,
    DA_OPEN_NEAREST_GATE,
    DA_ATTACK_STAY_NEAR_HOME,
    DA_MASSIVE_ALARM,
    DA_ATTACK_IF_NO_COEXIST,
    DA_KNOCKOUT,
    DA_END
};

class DialogueSpeechBubble : public wraps::BaseLayout, public Ogre::GeneralAllocatedObject
{
public:
    // wraps::BaseLayout offset = 0x0, length = 0xA0
    // Ogre::GeneralAllocatedObject offset = 0xA1, length = 0x1
    // no_addr void DialogueSpeechBubble(const class DialogueSpeechBubble & _a1);// public missing arg names
    DialogueSpeechBubble(bool shout, bool importnt);// public RVA = 0x724D60
    DialogueSpeechBubble* _CONSTRUCTOR(bool shout, bool importnt);// public RVA = 0x724D60
    virtual ~DialogueSpeechBubble();// public RVA = 0x722060 vtable offset = 0x0
    void _DESTRUCTOR();// public RVA = 0x722060 vtable offset = 0x0
    void setText(const std::string& text);// public RVA = 0x720ED0
    void setAlpha(float value);// public RVA = 0x720FD0
    void setPosition(int x, int y);// public RVA = 0x7219D0
    void setPosition(const Ogre::Vector3& position);// public RVA = 0x720FF0
    void reset();// public RVA = 0x720FB0
    float getAlpha() const;// public RVA = 0x721120
    MyGUI::types::TRect<int> getRect() const;// public RVA = 0x721140
    bool stayOnScreen; // 0xA1 Member
    bool shout; // 0xA2 Member
    int marginW; // 0xA4 Member
    int marginH; // 0xA8 Member
    MyGUI::EditBox* textBox; // 0xB0 Member
    MyGUI::types::TSize<int> baseSize; // 0xB8 Member
    static MyGUI::Colour TextColour; // RVA = 0x212FB70 Static Member
    // no_addr class DialogueSpeechBubble & operator=(const class DialogueSpeechBubble & _a1);// public missing arg names
    // virtual void * __vecDelDtor(unsigned int _a1) = 0;// public vtable offset = 0x0 missing arg names
};

class CampaignTriggerData
{
public:
    GameData* what; // 0x0 Member
    int minTime; // 0x8 Member
    int maxTime; // 0xC Member
    float chance; // 0x10 Member
};

class Faction;
class WorldEventStateQueryList;
class DialogChoiceList;
class Dialogue;

// TODO move?
class GameDataValuePair
{
public:
    GameDataValuePair(GameData* d, int v);// public RVA = 0x670A50
    GameDataValuePair* _CONSTRUCTOR(GameData* d, int v);// public RVA = 0x670A50
    GameDataValuePair();// public RVA = 0x670A40
    GameDataValuePair* _CONSTRUCTOR();// public RVA = 0x670A40
    GameData* data; // 0x0 Member
    int val0; // 0x8 Member
};

class DialogLineData
{
public:
    // VTable         : (none)
    class DialogAction
    {
    public:
        DialogActionEnum key; // 0x0 Member
        int value; // 0x4 Member
    };
    class DialogCondition
    {
    public:
        DialogConditionEnum key; // 0x0 Member
        ComparisonEnum compareBy; // 0x4 Member
        TalkerEnum who; // 0x8 Member
        int value; // 0xC Member
    };
    class FlagCondition
    {
    public:
        // no_addr void FlagCondition();// public
        DialogConditionEnum key; // 0x0 Member
        bool want; // 0x4 Member
        unsigned int flags; // 0x8 Member
    };
    unsigned int targetFlagsNeeded; // 0x8 Member
    unsigned int targetFlagsNotWanted; // 0xC Member
    unsigned int personalityNeeded; // 0x10 Member
    unsigned int personalityNotWanted; // 0x14 Member
    FitnessSelector<CampaignTriggerData*> campaignTriggers; // 0x18 Member
    lektor<GameData*> isTargetRace; // 0x78 Member
    lektor<GameData*> isTargetSubRace_specificallyTheTarget; // 0x90 Member
    DialogLineData* getParent() const;// public RVA = 0x670A60
    lektor<GameDataValuePair> givesItem; // 0xA8 Member
    bool isForSpecificCharacter(GameData* who);// public RVA = 0x6712C0
    bool hasSpecificCharacterRequirement() const;// public RVA = 0x68EB30
    std::set<Faction*, std::less<Faction*>, Ogre::STLAllocator<Faction*, Ogre::GeneralAllocPolicy > > inTownOf; // 0xC0 Member
    std::set<Faction*, std::less<Faction*>, Ogre::STLAllocator<Faction*, Ogre::GeneralAllocPolicy > > isTargetFaction; // 0xE8 Member
    std::set<Faction*, std::less<Faction*>, Ogre::STLAllocator<Faction*, Ogre::GeneralAllocPolicy > > isMyFaction; // 0x110 Member
    lektor<GameData*> isCharacter; // 0x138 Member
    lektor<GameData*> isTargetCarryingCharacter; // 0x150 Member
    lektor<GameData*> _hasPackage; // 0x168 Member
    lektor<GameData*> isMyRace; // 0x180 Member
    lektor<GameData*> isMySubRace; // 0x198 Member
    ItemFunction hasItemType; // 0x1B0 Member
    lektor<GameData*> hasItem; // 0x1B8 Member
    WorldEventStateQueryList* worldState; // 0x1D0 Member
    void _initialiseAList(const std::string& listname, lektor<GameDataValuePair>* list);// protected RVA = 0x67A7B0
    void _initialiseAList(const std::string& listname, lektor<GameData*>* list);// protected RVA = 0x67A6C0
    // no_addr void DialogLineData(const class DialogLineData & _a1);// public missing arg names
    DialogLineData(GameData* dat);// protected RVA = 0x67A8F0
    DialogLineData* _CONSTRUCTOR(GameData* dat);// protected RVA = 0x67A8F0
    virtual ~DialogLineData();// protected RVA = 0x672C90 vtable offset = 0x0
    void _DESTRUCTOR();// protected RVA = 0x672C90 vtable offset = 0x0
    void setupChildren();// protected RVA = 0x67CAE0
    bool checkRepeatLimits();// protected RVA = 0x670EE0
    bool checkTags(Character* me, Character* target);// protected RVA = 0x6744D0
    GameData* data; // 0x1D8 Member
    bool onceOnly; // 0x1E0 Member
    bool isMonologue; // 0x1E1 Member
    CharacterTypeEnum forCertainType; // 0x1E4 Member
    DialogChoiceList* children; // 0x1E8 Member
    lektor<DialogLineData::DialogCondition*> conditions; // 0x1F0 Member
    lektor<DialogLineData::DialogAction*> actions; // 0x208 Member
    int lineCount; // 0x220 Member
    std::string* texts; // 0x228 Member
    DialogLineData* parent; // 0x230 Member
    float chancePermanent; // 0x238 Member
    float chanceTemporary; // 0x23C Member
    bool unique; // 0x240 Member
    hand uniqueOwner; // 0x248 Member
    float dialogRepeatMinTimeInHours; // 0x268 Member
    TimeOfDay lastTimeSaid; // 0x270 Member
    int score; // 0x278 Member
    bool oneAtATime; // 0x27C Member
    bool isLocked; // 0x27D Member
    lektor<DialogLineData*> locks; // 0x280 Member
    lektor<DialogLineData*> unlocks_lockMe; // 0x298 Member
    lektor<DialogLineData*> unlocks_dontLockMe; // 0x2B0 Member
    DialogLineData* crowdTrigger; // 0x2C8 Member
    std::string getName() const;// public RVA = 0x5E7100
    bool saidItBefore();// public RVA = 0x670A70
    bool willTalkToEnemies();// public RVA = 0x670A90
    virtual bool isEmptyNode() const;// public RVA = 0x6A58D0 vtable offset = 0x8
    bool _NV_isEmptyNode() const;// public RVA = 0x6A58D0 vtable offset = 0x8
    bool isAnnouncement();// public RVA = 0x670AA0
    void stampLastTimeSaid();// public RVA = 0x670FF0
    ogre_unordered_map<GameData*, int>::type factionRelationEffects; // 0x2D0 Member
    DialogLineData* playerInterruptionDialog; // 0x310 Member
    bool isInterjection; // 0x318 Member
    TalkerEnum speaker; // 0x31C Member
    int getScore(Character* target) const;// public RVA = 0x671430
    int getScorePlusChildrenIfEmpty(Character* target);// public RVA = 0x6715D0
    bool hasChildren() const;// public RVA = 0x670AB0
    bool checkConditions(Dialogue* dialog, Character* target, bool isWordswap);// public RVA = 0x6787B0
    DialogLineData* chooseAChild(Dialogue* who, Character* target, bool isForWordswaps);// public RVA = 0x670F50
    std::string getText(bool _stampTime);// public RVA = 0x674EB0
    void getText(std::string& out, bool _stampTime);// public RVA = 0x674BA0
    std::string getStringID() const;// public RVA = 0x6A12E0
    GameData* getGameData();// public RVA = 0x670AC0
    DialogLineData* getChildByStringID(const std::string& sid);// public RVA = 0x671A10
    void getPlayerReplies(lektor<DialogLineData*>& out, Dialogue* who, Character* target);// public RVA = 0x679380
    void setParent(DialogLineData* p);// public RVA = 0x670AD0
    int getMoneyCostForLine() const;// public RVA = 0x671390
    const lektor<DialogLineData::DialogAction*>* getActions() const;// public RVA = 0x670AE0
    // no_addr class DialogLineData & operator=(const class DialogLineData & _a1);// public missing arg names
    // virtual void * __vecDelDtor(unsigned int _a1) = 0;// protected vtable offset = 0x0 missing arg names
};

class DialogChoiceList
{
public:
    lektor<DialogLineData*> conversationChoices; // 0x0 Member
    // no_addr void DialogChoiceList(const class DialogChoiceList & _a1);// public missing arg names
    DialogChoiceList();// public RVA = 0x691FC0
    DialogChoiceList* _CONSTRUCTOR();// public RVA = 0x691FC0
    ~DialogChoiceList();// public RVA = 0x692030
    void _DESTRUCTOR();// public RVA = 0x692030
    void add(GameData* conversation, DialogLineData* parent);// public RVA = 0x6AD250
    // no_addr class DialogChoiceList & operator=(const class DialogChoiceList & _a1);// public missing arg names
    // no_addr void * __vecDelDtor(unsigned int _a1);// public missing arg names
};

class CharStats;
class CharMovement;
class GameData;
class Character;
class DatapanelGUI;
class Faction;

class Dialogue
{
public:
    class RepetitionCounter
    {
    public:
        class DialogState
        {
        public:
            DialogState();// public RVA = 0x670AF0
            DialogState* _CONSTRUCTOR();// public RVA = 0x670AF0
            int count; // 0x0 Member
            float lastTimeStamp; // 0x4 Member
            float resetTime; // 0x8 Member
        };
        std::map<EventTriggerEnum, Dialogue::RepetitionCounter::DialogState, std::less<EventTriggerEnum>, Ogre::STLAllocator<std::pair<EventTriggerEnum const, Dialogue::RepetitionCounter::DialogState>, Ogre::GeneralAllocPolicy > > states; // 0x0 Member
        void setup();// public RVA = 0x672090
        bool count(EventTriggerEnum ev);// public RVA = 0x671FE0
        double getTimeSinceLastTrigger(EventTriggerEnum ev);// public RVA = 0x672040
        int getCount(EventTriggerEnum ev);// public RVA = 0x6A4310
        // no_addr void resetCount(enum EventTriggerEnum _a1);// public missing arg names
        // no_addr void RepetitionCounter(const class Dialogue::RepetitionCounter & _a1);// public missing arg names
        RepetitionCounter();// public RVA = 0x6A57F0
        RepetitionCounter* _CONSTRUCTOR();// public RVA = 0x6A57F0
        ~RepetitionCounter();// public RVA = 0x6A5810
        void _DESTRUCTOR();// public RVA = 0x6A5810
        // no_addr class Dialogue::RepetitionCounter & operator=(const class Dialogue::RepetitionCounter & _a1);// public missing arg names
        // no_addr void * __vecDelDtor(unsigned int _a1);// public missing arg names
    };
    Dialogue::RepetitionCounter repCounter; // 0x0 Member
    std::string getWordSwap(const std::string& key, Character* _target, bool _a3, DialogLineData* _line);// private RVA = 0x679DB0
    void setupWordSwaps();// private RVA = 0x67DB40
    void _insertWordSwapsRecurse(std::string& text, Character* target, bool swapMe, DialogLineData* _line);// private RVA = 0x67DEE0
    bool _needsDynamicAssessments; // 0x28 Member
    std::map<DialogLineData*, bool, std::less<DialogLineData*>, Ogre::STLAllocator<std::pair<DialogLineData* const, bool>, Ogre::GeneralAllocPolicy > > locked; // 0x30 Member
    std::string sayMsg; // 0x58 Member
    enum DT_MSG
    {
        DT_NONE,
        DT_END_DIALOG,
        DT_OPENWINDOW,
        DT_CLOSEWINDOW,
        DT_CLEAR_RESPONSES,
        DT_SET_RESPONSES,
        DT_SET_NPC_REPLY
    };

    Ogre::vector<Dialogue::DT_MSG>::type threadMessages; // 0x80 Member
    // no_addr void Dialogue(const class Dialogue & _a1);// public missing arg names
    Dialogue();// public RVA = 0x6735B0
    Dialogue* _CONSTRUCTOR();// public RVA = 0x6735B0
    ~Dialogue();// public RVA = 0x674760
    void _DESTRUCTOR();// public RVA = 0x674760
    void insertWordSwaps(std::string& text, Character* target, bool swapMeYou, DialogLineData* line);// public RVA = 0x67E130
    void getGUIData(DatapanelGUI* datapanel, int cat);// public RVA = 0x5CCB40
    void create(Character* c, CharStats* s, CharMovement* m);// public RVA = 0x670C00
    hand getHandle() const;// public RVA = 0x671270
    Character* getCharacter();// public RVA = 0x670B10
    void setLineLocked(DialogLineData* line, bool on);// public RVA = 0x6A4330
    bool isLocked(DialogLineData* line);// public RVA = 0x6A4370
    void setInDialog(bool on);// public RVA = 0x673F20
    void clearDialogues();// public RVA = 0x673440
    void clearAnnouncements();// public RVA = 0x6720C0
    std::set<GameData*, std::less<GameData*>, Ogre::STLAllocator<GameData*, Ogre::GeneralAllocPolicy > > pacakgesIHave; // 0xA0 Member
    bool needsDialogAssessmentUpdate() const;// public RVA = 0x6711A0
    void addDialoguePackage(GameData* _con);// public RVA = 0x67F330
    void addConversation(GameData* _con, EventTriggerEnum t);// public RVA = 0x67DAA0
    void clearConversationList(EventTriggerEnum t);// public RVA = 0x671FC0
    bool willTalkToEnemies();// public RVA = 0x672AE0
    DialogLineData* playerInterruptionDialog; // 0xC8 Member
    bool sendEventOverride(Character* who, EventTriggerEnum what, bool forceRepeat);// public RVA = 0x684EE0
    bool sendEvent(Character* who, EventTriggerEnum what);// public RVA = 0x684210
    void stopEvent(EventTriggerEnum what);// public RVA = 0x6746E0
    bool runCustomDialog(GameData* dialog);// public RVA = 0x6845F0
    int scoreCustomDialog(GameData* dialog);// public RVA = 0x67D8E0
    bool conversationHasEnded() const;// public RVA = 0x6714E0
    hand getConversationTarget();// public RVA = 0x85C600
    void changeConversationTarget(Character* c);// public RVA = 0x6711B0
    bool conversationHasEndedPrettyMuch() const;// public RVA = 0x671250
    bool makeAnnouncement(GameData* dat);// public RVA = 0x684FD0
    void update(float frameTime);// public RVA = 0x684910
    void endDialogue(bool definitelyTheEnd);// public RVA = 0x6740B0
    void notifyUnderAttack(Character* attacker);// public RVA = 0x684E40
    bool hasDialogue_Fast();// public RVA = 0x6721A0
    bool hasDialogue_Accurate();// public RVA = 0x67A2D0
    bool hasDialogueEvent(EventTriggerEnum e);// public RVA = 0x6722B0
    void say(DialogLineData* dialogLine);// private RVA = 0x683190
    void say(const std::string& _text, DialogLineData* line);// public RVA = 0x67F600
    bool dialogDelivered(EventTriggerEnum e);// public RVA = 0x6730B0
    bool _checkCondition(DialogConditionEnum conditionName, ComparisonEnum compareBy, int val, Character* target, Character* actualConversationTarget);// public RVA = 0x6761D0
    void dontLetTargetBeMe(Character** target, Character* actualTarget);// public RVA = 0x671040
    bool isAtTownOf(const std::set<Faction*, std::less<Faction*>, Ogre::STLAllocator<Faction*, Ogre::GeneralAllocPolicy > >& f);// public RVA = 0x793DF0
    bool hasThisChanceLine(DialogLineData* line, float chance);// public RVA = 0x6722E0
    DialogLineData* _chooseDialog(DialogChoiceList* list, Character* target, bool isWordswap);// public RVA = 0x679940
    void triggerNextLine(DialogLineData* previousLine);// public RVA = 0x683FA0
    bool targetInTalkingRange(Character* target, float mult);// public RVA = 0x670F70
    void save(GameData* to);// public RVA = 0x679180
    void load(GameData* to);// public RVA = 0x67F000
    void replyClicked(const std::string& index);// private RVA = 0x683480
    void replyClicked(int index);// public RVA = 0x683670
    Character* getSpeaker(TalkerEnum who, DialogLineData* line, bool isForWordswaps);// public RVA = 0x672470
    bool isCurrentConversationRunning(DialogLineData* line) const;// public RVA = 0x692070
    static void resolveOverlappedSpeechBubbles();// public RVA = 0x6717D0
    void _wordSwapCharacterName(std::string& name);// public RVA = 0x620860
    std::map<EventTriggerEnum, float, std::less<EventTriggerEnum>, Ogre::STLAllocator<std::pair<EventTriggerEnum const, float>, Ogre::GeneralAllocPolicy > > eventRepeatTimers; // 0xD0 Member
    std::map<EventTriggerEnum, hand, std::less<EventTriggerEnum>, Ogre::STLAllocator<std::pair<EventTriggerEnum const, hand>, Ogre::GeneralAllocPolicy > > eventDeliveredStates; // 0xF8 Member
    Character* findInterjectionCharacter(DialogLineData* line);// private RVA = 0x679440
    bool isKOExempt(EventTriggerEnum what);// private RVA = 0x670B20
    std::map<DialogLineData*, bool, std::less<DialogLineData*>, Ogre::STLAllocator<std::pair<DialogLineData* const, bool>, Ogre::GeneralAllocPolicy > > _hasChanceLines; // 0x120 Member
    void _endPlayerConversation(bool finished);// private RVA = 0x674750
    bool startPlayerConversation(Character* target, DialogLineData* _talk);// private RVA = 0x683BA0
    bool startConversation(Character* target, DialogLineData* _talk, EventTriggerEnum ev, bool force);// private RVA = 0x683810
    bool _hasEnded; // 0x148 Member
    bool shouting; // 0x149 Member
    bool staysOnScreen; // 0x14A Member
    Character* me; // 0x150 Member
    hand conversationTarget; // 0x158 Member
    CharStats* stats; // 0x178 Member
    CharMovement* movement; // 0x180 Member
    EventTriggerEnum currentConversationType; // 0x188 Member
    DialogLineData* currentConversation; // 0x190 Member
    DialogLineData* currentLine; // 0x198 Member
    DialogChoiceList* getConversationList(EventTriggerEnum t);// private RVA = 0x671F40
    std::map<EventTriggerEnum, DialogChoiceList*, std::less<EventTriggerEnum>, Ogre::STLAllocator<std::pair<EventTriggerEnum const, DialogChoiceList*>, Ogre::GeneralAllocPolicy > > conversationsMain; // 0x1A0 Member
    void _doActions(DialogLineData* dialogLine);// private RVA = 0x67FDE0
    bool sayLine(DialogLineData* line);// private RVA = 0x683220
    void listPlayerReplies();// private RVA = 0x67F9B0
    bool isLastLine(DialogLineData* dialogLine);// private RVA = 0x670D40
    void setSpeaker(TalkerEnum who, Character* c);// private RVA = 0x670CB0
    hand interjector1; // 0x1C8 Member
    hand interjector2; // 0x1E8 Member
    hand interjector3; // 0x208 Member
    void _updateTextPos();// private RVA = 0x671BA0
    void clearSpeechBox();// private RVA = 0x671B50
    void clearRespones();// private RVA = 0x6744A0
    void clearResponesGUI();// private RVA = 0x673800
    void setResponesGUI();// private RVA = 0x6738F0
    void setConversationReplyGUI();// private RVA = 0x6739F0
    DialogueSpeechBubble* speechBubblePanel; // 0x228 Member
    float speechTextTimer; // 0x230 Member
    float speechTextTimer_forced; // 0x234 Member
    Ogre::vector<std::string>::type replyIds; // 0x238 Member
    Ogre::vector<std::string>::type responses; // 0x258 Member
    std::string npcReplyText; // 0x278 Member
    hand conversationMaster; // 0x2A0 Member
    hand waitingForReplyFrom; // 0x2C0 Member
    static ogre_unordered_set<DialogueSpeechBubble*>::type speechBubbleList; // RVA = 0x1EF5CD0 Static Member
    // no_addr class Dialogue & operator=(const class Dialogue & _a1);// public missing arg names
    // no_addr void * __vecDelDtor(unsigned int _a1);// public missing arg names
};

class WorldEventStateQueryList;

class DialogDataManager
{
public:
    static ogre_unordered_map<GameData*, DialogLineData*>::type allDatas; // RVA = 0x1EF5C50 Static Member
    static GameData* saveData; // RVA = 0x212D3E8 Static Member
    static DialogLineData* _createData(GameData* d);// private RVA = 0x6AD080
    static void initialise();// private RVA = 0x67D650
    static void save(GameData* d);// public RVA = 0x387620
    static void load(GameData* d);// public RVA = 0x387730
    static void newGameReset();// public RVA = 0x3852B0
    static DialogLineData* getData(GameData* d);// public RVA = 0x6AD120
};