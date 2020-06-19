#include "../CmdKeyboardCommon.h"
#define OFF_PLAYER_INV 0x6BD0

register volatile u32 *global_ptr1 asm ("lr");
register volatile u32 *global_ptr2 asm ("r1");

/* Text 2 Item function */
void Text2Item(const wchar_t* KeyboardStr) {
    u32 itemID = (wcstoul(KeyboardStr, NULL, 16)&0xFFFF7FFF);
    u32 playerOffset = Get_Base_Player_Address();

    if (playerOffset == 0 || itemID == 0) return;
    else if (((itemID&0x7FFF) >= 0xf9 && (itemID&0x7FFF) <= 0x2000) || (itemID&0x7FFF) > 0x7ffe) return;

    playerOffset+=OFF_PLAYER_INV;
    WRITEU32(playerOffset, itemID);
}