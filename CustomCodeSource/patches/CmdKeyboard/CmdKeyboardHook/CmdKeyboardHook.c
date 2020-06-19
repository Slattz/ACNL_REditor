#include "../CmdKeyboardCommon.h"

/*
TODO: ClearInv, etc

*/

#define CMD_AMOUNT 12

#define FUNC_BOUND ((void *)0xFFFFFFFF)
#define CMD_SIZE_BOUND -1

//Don't use the following registers (some game funcs seem to use them so let's avoid it)
register volatile u32 *global_ptr1 asm ("lr");
register volatile u32 *global_ptr2 asm ("r3");
register volatile u32 *global_ptr3 asm ("r2");
register volatile u32 *global_ptr4 asm ("r1");
//register volatile u32 *global_ptr5 asm ("r0");

//returns false if success; true if not success
bool CmdKeyboardHook(wchar_t* KeyboardStr) { //Used to check if user used a command
    static const void (*func_ptrs[CMD_AMOUNT])(const wchar_t *KeyboardStr) = {Text2Item, ChangeTime, DupeAll, FUNC_BOUND, FUNC_BOUND, FUNC_BOUND,
                                                                              FUNC_BOUND, FUNC_BOUND, FUNC_BOUND, FUNC_BOUND, FUNC_BOUND, FUNC_BOUND};

    static const wchar_t* func_cmds[CMD_AMOUNT] = {L"t2i ", L"time ", L"dupeall", L"GarBAgE2", L"111", L"222", 
                                                   L"333", L"444", L"555", L"666", L"777", L"ENDFUNCCMDS"};
    static const s8 func_cmd_size[CMD_AMOUNT] = {5, 7, 7, CMD_SIZE_BOUND, CMD_SIZE_BOUND, CMD_SIZE_BOUND, CMD_SIZE_BOUND,
                                                 CMD_SIZE_BOUND, CMD_SIZE_BOUND, CMD_SIZE_BOUND, CMD_SIZE_BOUND, CMD_SIZE_BOUND};

    int len = wcslen(KeyboardStr);
    if (len > 0 && *KeyboardStr == '/') {
        KeyboardStr++; //Go past the "/" as we know it's a command
        len--; //take 1 from length as a result

        int i = 0;

        //Use a while until we reach bound instead of a static 'for loop'; allows to be dynamic & add functions easily
        while (func_ptrs[i] != FUNC_BOUND) {
            s8 cmd_size = func_cmd_size[i];
            int cmd_len = wcslen(func_cmds[i]);

            if ((cmd_size != CMD_SIZE_BOUND) && (cmd_size > 0 && len >= cmd_size) && (wcsncmp(KeyboardStr, func_cmds[i], cmd_len) == 0)) {
                KeyboardStr+=cmd_len;
                (*func_ptrs[i])(KeyboardStr);
                return false;
            }
            i++;
        }
    }

    return true;
}