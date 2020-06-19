#include "../CmdKeyboardCommon.h"
#define OFF_PLAYER_INV 0x6BD0

/* ClearInv function */
void ClearInv(const wchar_t* KeyboardStr) {
    u32 playerOffset = Get_Base_Player_Address();
    if (playerOffset == 0) return;

    playerOffset+=OFF_PLAYER_INV;
    for (u32 i = 0; i < 16; i++)
        WRITEU32(playerOffset+(i*4), 0x00007ffe);
}