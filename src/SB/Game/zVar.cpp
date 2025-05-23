#include "zVar.h"

#include <types.h>
#include <stdio.h>
#include <string.h>

#include "iTime.h"

#include "xString.h"
#include "xFont.h"
#include "xSnd.h"
#include "xsavegame.h"
#include "xutil.h"

#include "zEntPlayer.h"
#include "zGlobals.h"
#include "zUI.h"
#include "zMenu.h"
#include "zScene.h"
#include "zSaveLoad.h"

extern var_type vars[39];
/*
vars = {
    {"ActivePad", 9, &var_text_ActivePad},
    {"BadCard", 7, &var_text_BadCard},
    ...
}

Full `vars` Array Contents
Notes:
* Looking up the address in the callback column will yield the first
  instruction of a call in zVar.s, that's the function in question.
* This means the function signature of the callbacks is `char* Callback();`.

NameStr  | NameLen  | Callback
------------------------------
8025FDEF | 00000009 | "ActivePad" -> var_text_ActivePad__Fv
8025FDF9 | 00000007 | "BadCard" -> var_text_BadCard__Fv
8025FE01 | 00000010 | 800BD26C -> TODO: Translate these if you need them
8025FE12 | 0000000D | 800BD2AC
8025FE20 | 0000000F | 800BD2EC
8025FE30 | 0000000B | 800BD380
8025FE3C | 0000000B | 800BD3B0
8025FE48 | 00000017 | 800BD3E0
8025FE60 | 00000015 | 800BD428
8025FE76 | 0000000C | 800BD470
8025FE83 | 0000000B | 800BD4CC
8025FE8F | 00000009 | 800BD7B8
8025FE99 | 00000009 | 800BD7E8
8025FEA3 | 00000009 | 800BD818
8025FEAD | 00000009 | 800BD848
8025FEB7 | 00000009 | 800BD878
8025FEC1 | 00000009 | 800BD8A8
8025FECB | 00000009 | 800BD8D8
8025FED5 | 00000009 | 800BD908
8025FEDF | 0000000C | 800BD938
8025FEEC | 0000000E | 800BD968
8025FEFB | 00000006 | 800BDE68
8025FF02 | 0000000D | 800BD9B4
8025FF10 | 0000000D | 800BD9F4
8025FF1E | 0000000E | 800BDA34
8025FF2D | 0000000E | 800BDA80
8025FF3C | 0000000B | 800BDAC8
8025FF48 | 0000000A | 800BDAE0
8025FF53 | 0000000E | 800BDAF8
8025FF62 | 0000000C | 800BDB74
8025FF6F | 0000000A | 800BDB98
8025FF7A | 0000000E | 800BDBE0
8025FF89 | 0000000D | 800BDC74
8025FF97 | 00000010 | 800BDCBC
8025FFA8 | 0000000E | 800BDD04
8025FFB7 | 00000014 | 800BDD48
8025FFCC | 0000000B | 800BDDDC
8025FFCC | 0000000B | 800BDDDC
8025FFD8 | 0000000E | 800BDE20
8025FFE7 | 00000003 | 800BDF80
*/

char* var_text(const substr& str);
U32 zVarEntryCB_SndFXVol(void*);
U32 zVarEntryCB_SndMusicVol(void*);

extern const char zVar_strings[];

// Buffers for var_text callbacks. These should all be changed to:
// static char buffer[...]
// inside the respective functions in the final version.
extern char zVar_printf_buffer1[];
extern char zVar_printf_buffer2[];
extern char zVar_printf_buffer3[];
extern char zVar_printf_buffer4[];
extern char zVar_printf_buffer5[];
extern char zVar_printf_buffer6[];
extern char zVar_printf_buffer7[];
extern char zVar_printf_buffer8[];
extern char zVar_printf_buffer9[];
extern char zVar_printf_buffer10[];
extern char zVar_buffer11[0x40];
extern char zVar_buffer12[0x40];
extern char zVar_buffer13[0x40];
extern char zVar_buffer14[0x40];
extern char zVar_buffer15[0x40];
extern char zVar_buffer16[0x40];
extern char zVar_buffer17[0x40];
extern char zVar_buffer18[0x40];
extern char zVar_buffer19[];
extern char zVar_buffer20[];
extern char zVar_buffer21[];
extern char zVar_buffer22[];
extern char zVar_buffer23[];
extern char zVar_buffer24[0x30];
extern char zVar_buffer25[];
extern char zVar_buffer26[];
extern char zVar_buffer27[];
extern char zVar_buffer28[];
extern char zVar_buffer29[];
extern char zVar_buffer30[];
extern char zVar_buffer31[];
extern char zVar_buffer32[];
extern char* playername_text[3]; // originally called "text"
extern char* heshe_text[3]; // also originally called "text"
extern char lbl_80291708[];
extern S32 selSceneID;
extern char* state_text[4];

// var_text callbacks
namespace
{
    char* var_text_ActivePad()
    {
        sprintf(zVar_printf_buffer1, &zVar_strings[0] /*"%d"*/, globals.currentActivePad + 1);
        return zVar_printf_buffer1;
    }

    char* var_text_BadCard()
    {
        S32 badCardCount = zMenuGetBadCard();
        if (badCardCount > 0)
        {
            sprintf(zVar_printf_buffer2, &zVar_strings[3] /*"%c"*/, 'A' + (badCardCount - 1));
        }
        else
        {
            strcpy(zVar_printf_buffer2, &zVar_strings[6] /*"A or B"*/);
        }
        return zVar_printf_buffer2;
    }

    char* var_text_BadCardAvailable()
    {
        sprintf(zVar_printf_buffer3, &zVar_strings[0] /*"%d"*/, bad_card_available);
        return zVar_printf_buffer3;
    }

    // var_text_BadCardNeeded__18_esc__2_unnamed_esc__2_zVar_cpp_esc__2_Fv
    char* var_text_BadCardNeeded()
    {
        sprintf(zVar_printf_buffer4, &zVar_strings[0] /*"%d"*/, bad_card_needed);
        return zVar_printf_buffer4;
    }

    // Indexing into zVar_strings didn't get pulled out of the loop in the original
    // code for some reason.
    char* var_text_CorruptFileName()
    {
        char corruptedNames[3][64];
        U32 corruptCount = zMenuGetCorruptFiles(corruptedNames);
        strcpy(zVar_printf_buffer5, &zVar_strings[0xD] /*""*/);
        for (U32 i = 0; i < corruptCount; ++i)
        {
            strcat(zVar_printf_buffer5, corruptedNames[i]);
            strcat(zVar_printf_buffer5, &zVar_strings[0xE] /*" "*/);
        }
        return zVar_printf_buffer5;
    }

    const char* var_text_CurrentArea()
    {
        return zSceneGetAreaname(globals.sceneCur->sceneID);
    }

    char* var_text_CurrentDate()
    {
        iGetCurrFormattedDate(zVar_printf_buffer6);
        return zVar_printf_buffer6;
    }

    char* var_text_CurrentLevelCollectable()
    {
        sprintf(zVar_printf_buffer7, zVar_strings /*"%d"*/,
                globals.player.Inv_LevelPickups_CurrentLevel);
        return zVar_printf_buffer7;
    }

    char* var_text_CurrentLevelPatsSocks()
    {
        sprintf(zVar_printf_buffer8, zVar_strings /*"%d"*/,
                globals.player.Inv_PatsSock_CurrentLevel);
        return zVar_printf_buffer8;
    }

    char* var_text_CurrentScene()
    {
        sprintf(zVar_printf_buffer9, &zVar_strings[0x10] /*"%s"*/,
                xUtil_idtag2string(globals.sceneCur->sceneID, 0));
        return zVar_printf_buffer9;
    }

    char* var_text_CurrentTime()
    {
        iGetCurrFormattedTime(zVar_printf_buffer10);
        return zVar_printf_buffer10;
    }

} // namespace

// Note: zVarGameSlotInfo should be in the anonymous namespace, need the
// anomymous namespace symbol formatting fix from Seil to move it in though.

// I don't understand this function. The behavior perfectly matches... but
// it never ends up doing anything with the buffer it makes up?? It just returns
// the same buffer it takes in, throwing away all the work it just did.
// Still needs the strings to be pulled out
char* zVarGameSlotInfo(S32 i, char* buffer, size_t something)
{
    char date[0x20] = "";
    char time[0x20] = "";
    S32 hour;
    char ampm[0x20] = "";
    char anotherBuffer[0x100] = "";

    // Get the date
    strncpy(date, zSaveLoadGameTable[i].date, 5);
    date[2] = '/';

    sprintf(anotherBuffer, &zVar_strings[0x13] /*"%s/%c%c"*/, date, zSaveLoadGameTable[i].date[0x8],
            zSaveLoadGameTable[i].date[0x9]);
    strncpy(date, anotherBuffer, 0x20);
    date[0x20 - 1] = '\0';

    // Get the hour
    sprintf(time, &zVar_strings[0x1B] /*"%c%c"*/, zSaveLoadGameTable[i].date[0xB],
            zSaveLoadGameTable[i].date[0xC]);

    // Get the AM/PM
    hour = atoi(time);
    if (hour >= 12)
    {
        strncpy(ampm, &zVar_strings[0x20] /*"PM"*/, 3);
        if (hour != 12)
        {
            hour -= 12;
        }
    }
    else
    {
        strncpy(ampm, &zVar_strings[0x23] /*"AM"*/, 3);
        if (hour == 0)
        {
            hour = 12;
        }
    }

    // Construct final date
    sprintf(time, &zVar_strings[0x26] /*"%d:%c%c %s"*/, hour, zSaveLoadGameTable[i].date[0xE],
            zSaveLoadGameTable[i].date[0xF], ampm);
    sprintf(date, &zVar_strings[0x31] /*"%s %s"*/, date, time);
    time[0x20 - 1] = '\0';
    date[0x20 - 1] = '\0';

    if (xStricmp(zSaveLoadGameTable[i].label, &zVar_strings[0x37] /*"Empty"*/) == 0)
    {
        sprintf(buffer, &zVar_strings[0x37] /*"Empty"*/);
    }
    else
    {
        zSaveLoad_BuildName(buffer, i);
    }

    return buffer;
}

namespace
{

    char* var_text_GameSlot0()
    {
        return zVarGameSlotInfo(0, zVar_buffer11, 0x3f);
    }

    char* var_text_GameSlot1()
    {
        return zVarGameSlotInfo(1, zVar_buffer12, 0x3f);
    }

    char* var_text_GameSlot2()
    {
        return zVarGameSlotInfo(2, zVar_buffer13, 0x3f);
    }

    char* var_text_GameSlot3()
    {
        return zVarGameSlotInfo(3, zVar_buffer14, 0x3f);
    }

    char* var_text_GameSlot4()
    {
        return zVarGameSlotInfo(4, zVar_buffer15, 0x3f);
    }

    char* var_text_GameSlot5()
    {
        return zVarGameSlotInfo(5, zVar_buffer16, 0x3f);
    }

    char* var_text_GameSlot6()
    {
        return zVarGameSlotInfo(6, zVar_buffer17, 0x3f);
    }

    char* var_text_GameSlot7()
    {
        return zVarGameSlotInfo(7, zVar_buffer18, 0x3f);
    }

    char* var_text_MCAccessType()
    {
        return state_text[zSaveLoad_getMCAccessType()];
    }

    char* var_text_MCAutoSaveCard()
    {
        sprintf(zVar_buffer19, &zVar_strings[0x3] /*"%c"*/, 'A' + zSaveLoadGetAutoSaveCard());
        return zVar_buffer19;
    }

    char* var_text_MCPS2MaxSpace()
    {
        // Yes... this matches. Probably just stripped out during porting since it
        // was a PS2 thing.
        sprintf(zVar_buffer20, &zVar_strings[0] /*"%d*/, 0);
        return zVar_buffer20;
    }

    char* var_text_MCPS2MinSpace()
    {
        sprintf(zVar_buffer21, &zVar_strings[0] /*"%d*/, 0);
        return zVar_buffer21;
    }

    char* var_text_MCSelectedCard()
    {
        sprintf(zVar_buffer22, &zVar_strings[0x3] /*"%c"*/, 'A' + zSaveLoad_getcard());
        return zVar_buffer22;
    }

    char* var_text_MCSelectedGame()
    {
        sprintf(zVar_buffer23, &zVar_strings[0] /*"%d"*/, zSaveLoad_getgame() + 1);
        return zVar_buffer23;
    }

    char* var_text_PlayerHeShe()
    {
        return heshe_text[gCurrentPlayer];
    }

    char* var_text_PlayerName()
    {
        return playername_text[gCurrentPlayer];
    }

    char* var_text_PlayerPosition()
    {
        xEntFrame* frame = globals.player.ent.frame;
        sprintf(zVar_buffer24, &zVar_strings[0xE0] /*"%d,%d,%d"*/, S32(frame->mat.pos.x),
                S32(frame->mat.pos.y), S32(frame->mat.pos.z));
        return zVar_buffer24;
    }

    const char* var_text_SelectedArea()
    {
        // Looks like this variable was actually declared in this function given
        // the context, it needs to be here to show up in the right order.
        return zSceneGetAreaname(selSceneID);
    }

    char* var_text_ShinyCount()
    {
        sprintf(zVar_buffer25, &zVar_strings[0] /*"%d"*/, globals.player.Inv_Shiny);
        return zVar_buffer25;
    }

    char* var_text_ShinyCountText()
    {
        if (globals.player.Inv_Shiny == 0)
        {
            strcpy(lbl_80291708, &zVar_strings[0xE9] /*"{i:text_noshinies}"*/);
        }
        else if (globals.player.Inv_Shiny == 1)
        {
            strcpy(lbl_80291708, &zVar_strings[0xFC] /*"1 {i:text_shiny}"*/);
        }
        else
        {
            sprintf(lbl_80291708, &zVar_strings[0x10D] /*"%d {i:text_shinies}"*/,
                    globals.player.Inv_Shiny);
        }
        return lbl_80291708;
    }

    char* var_text_SoundFXVolume()
    {
        sprintf(zVar_buffer26, &zVar_strings[0] /*"%d"*/, zVarEntryCB_SndFXVol(NULL));
        return zVar_buffer26;
    }

    char* var_text_SoundMusicVolume()
    {
        sprintf(zVar_buffer27, &zVar_strings[0] /*"%d"*/, zVarEntryCB_SndMusicVol(NULL));
        return zVar_buffer27;
    }

    char* var_text_SpaceAvailable()
    {
        sprintf(zVar_buffer28, &zVar_strings[0], zSaveLoad_getMCavailable());
        return zVar_buffer28;
    }

    char* var_text_SpaceAvailableString()
    {
        // What a wierd dance... they could have just used zVar_buffer29 directly.
        char tmp[0x20];
        S32 available = zSaveLoad_getMCavailable();
        memset(tmp, 0, 0x20);
        memset(zVar_buffer29, 0, 0x40);
        sprintf(tmp, &zVar_strings[0] /*"%d"*/, available);
        sprintf(zVar_buffer29, &zVar_strings[0x10] /*"%s"*/, tmp);
        return zVar_buffer29;
    }

    char* var_text_SpaceNeeded()
    {
        sprintf(zVar_buffer30, &zVar_strings[0] /*"%d"*/, zSaveLoad_getMCneeded());
        return zVar_buffer30;
    }

    char* var_text_TotalPatsSocks()
    {
        sprintf(zVar_buffer31, &zVar_strings[0] /*"%d"*/, globals.player.Inv_PatsSock_Total);
        return zVar_buffer31;
    }

    char* var_text_MCName()
    {
        switch (zSaveLoad_getcard())
        {
        case 0:
            strcpy(zVar_buffer32, &zVar_strings[0x121] /*"{i:LD MC1 TXT}"*/);
            break;
        case 1:
            strcpy(zVar_buffer32, &zVar_strings[0x130] /*"{i:LD MC2 TXT}"*/);
            break;
        default:
            strcpy(zVar_buffer32, &zVar_strings[0xD] /*""*/);
            break;
        }
        return zVar_buffer32;
    }

    var_type* find_var(const substr& str)
    {
        U32 start = 0;
        U32 end = 0x27;
        while (start != end)
        {
            S32 c = (start + end) / 2;
            var_type* t = &vars[c];
            int i = icompare(str, t->name);
            if (i < 0)
            {
                end = c;
            }
            else if (i > 0)
            {
                start = c + 1;
            }
            else
            {
                return t;
            }
        }

        return NULL;
    }

    void parse_tag_var(xtextbox::jot& r31, const xtextbox& r4, const xtextbox& r5,
                       const xtextbox::split_tag& r6)
    {
        if ((r6.action.size != 1) || (r6.action.text[0] != 0x3A) || (r6.value.size <= 1))
            return;

        r31.context = var_text(r6.value);
        r31.context_size = 0;
        if (r31.context != NULL)
        {
            r31.context_size = 0xFC00;
            r31.flag.insert = r31.flag.dynamic = 1;
        }
    }

} // namespace

void var_init()
{
    extern xtextbox::tag_type var_tag[];
    xtextbox::register_tags(var_tag, 1);
}

char* var_text(const substr& str)
{
    var_type* entry = find_var(str);
    if (entry == NULL)
        return NULL;

    return entry->get_text();
}

void zVarInit(zVarEntry* table)
{
    for (S32 i = 0; i < 18; ++i)
    {
        table[i].varNameID = xStrHash(table[i].varName);
    }
}

void zVarNewGame()
{
}

U32 zVarEntryCB_SndMode(void* arg)
{
    return gSnd.stereo;
}

U32 zVarEntryCB_SndMusicVol(void* arg)
{
    F64 f0 = 10.0;
    F64 f1 = 0.5;
    F64 f2 = gSnd.categoryVolFader[2];

    f2 *= f1;
    f2 = f0 + f2;

    return f2;
}

U32 zVarEntryCB_SndFXVol(void* arg)
{
    F64 f0 = 10.0;
    F64 f1 = 0.5;
    F64 f2 = gSnd.categoryVolFader[0];

    f2 *= f1;
    f2 = f0 + f2;

    return f2;
}

U32 zVarEntryCB_MCAvailable(void* arg)
{
    if (xSGGameIsEmpty(NULL, 0) == 1)
    {
        if (xSGGameIsEmpty(NULL, 1) == 1)
        {
            return 0;
        }
        if (xSGGameIsEmpty(NULL, 1) == 0)
        {
            return 2;
        }
    }
    else if (xSGGameIsEmpty(NULL, 0) == 0)
    {
        if (xSGGameIsEmpty(NULL, 1) == 1)
        {
            return 1;
        }
        if (xSGGameIsEmpty(NULL, 1) == 0)
        {
            return 3;
        }
    }

    return 0;
}

U32 zVarEntryCB_VibrationOn(void* arg)
{
    // I'm pretty sure they actually and'd with a bit mask here, I couldn't find
    // another way (such as casting to bool) to get the right instructions.
    return globals.option_vibration & 0x1;
}

S32 zVarEntryCB_CurrentSceneLetter(void*)
{
    char buffer[16];
    sprintf(buffer, &zVar_strings[0x10] /*"%s"*/, xUtil_idtag2string(globals.sceneCur->sceneID, 0));

    // Convert the most significant char to uppercase
    // The lowercase variable doesn't exist in the ps4 symbols but I can't
    // figure out how to get a match otherwise.
    U32 mostSignificantChar = buffer[0];
    bool lowercase = false;
    if (mostSignificantChar >= 'a' && mostSignificantChar <= 'z')
    {
        lowercase = true;
    }
    if (lowercase)
    {
        mostSignificantChar -= 0x20;
    }

    return (mostSignificantChar - 'A') + 1;
}

S32 zVarEntryCB_CurrentRoom(void*)
{
    char buffer[16];
    sprintf(buffer, &zVar_strings[0x10] /*"%s"*/, xUtil_idtag2string(globals.sceneCur->sceneID, 0));

    S32 room = (buffer[2] - '0') * 10 + (buffer[3] - '0');

    // Convert the most significant char to uppercase
    // The lowercase variable doesn't exist in the ps4 symbols but I can't
    // figure out how to get a match otherwise.
    U32 mostSignificantChar = buffer[0];
    bool lowercase = false;
    if (mostSignificantChar >= 'a' && mostSignificantChar <= 'z')
    {
        lowercase = true;
    }
    if (lowercase)
    {
        mostSignificantChar -= 0x20;
    }

    return room + ((mostSignificantChar - 'A') + 1) * 100;
}

S32 zVarEntryCB_CurrentLevelPickup(void*)
{
    return globals.player.Inv_LevelPickups_CurrentLevel;
}

S32 zVarEntryCB_CurrentLevelPSocks(void*)
{
    return globals.player.Inv_PatsSock_CurrentLevel;
}

S32 zVarEntryCB_TotalPSocks(void*)
{
    return globals.player.Inv_PatsSock_Total;
}

S32 zVarEntryCB_Shinies(void*)
{
    return globals.player.Inv_Shiny;
}

S32 zVarEntryCB_Spatulas(void*)
{
    return globals.player.Inv_Spatula;
}

S32 zVarEntryCB_Date(void*)
{
    return iGetDay() + iGetMonth() * 0x64;
}

S32 zVarEntryCB_Hour(void*)
{
    return iGetHour();
}

S32 zVarEntryCB_Minute(void*)
{
    return iGetMinute();
}

S32 zVarEntryCB_CounterValue(void* arg)
{
    // Given the void argument type, no idea what type this actually is.
    // What we do know that at an offset 5 words in there's a signed S16.
    struct something
    {
        U32 stuff[5];
        S16 theValue;
    };
    return ((something*)arg)->theValue;
}

S32 zVarEntryCB_IsEnabled(void* arg)
{
    return xBaseIsEnabled((xBase*)arg) != 0;
}

S32 zVarEntryCB_IsVisible(void* arg)
{
    return xEntIsVisible((xEnt*)arg) != 0;
}
