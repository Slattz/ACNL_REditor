
#ifndef CMDKEYBOARDCOMMON_H
#define CMDKEYBOARDCOMMON_H

#include "common.h"

/* Game Function Declarations */
static inline u32 Get_Base_Player_Address(void) { //EUR 1.5: 0x2FBAC8
    register u32 outreg asm("r0");
    static const u32 offset = 0x2FBAC8;

    __asm__ __volatile__("BLX %[off]" 
                    : "=r" (outreg) //output
                    : [off] "r" (offset)); //input
    return outreg;
}

static inline size_t wcslen(const wchar_t* wcs) { //EUR 1.5: 0x100834 (Thumb so +1)
    register size_t outreg asm("r0");
    register const wchar_t* inreg asm("r0") = wcs;
    static const u32 offset = 0x100834+1; //Tells that it's Thumb

    __asm__ __volatile__("BLX %[off]" //asm template
                        : "=r" (outreg) //output
                        : "r" (inreg), [off] "r" (offset)); //input
    return outreg;
}

static inline int wcsncmp(const wchar_t* wcs1, const wchar_t* wcs2, size_t num) { //EUR 1.5: 0x101864 (Thumb so +1)
    register int outreg asm("r0");
    register const wchar_t* inreg0 asm("r0") = wcs1;
    register const wchar_t* inreg1 asm("r1") = wcs2;
    register size_t inreg2 asm("r2") = num;
    static const u32 offset = 0x101864+1; //Tells that it's Thumb

    __asm__ __volatile__("BLX %[off]"
                         : "=r"(outreg)                                               //output
                         : "r"(inreg0), "r"(inreg1), "r"(inreg2), [off] "r"(offset)); //input
    return outreg;
}

static inline u32 wcstoul(const wchar_t* str, wchar_t** endptr, int base) { //EUR 1.5: 0x1013C8 (Thumb so +1))
    register u32 outreg asm("r0");
    register const wchar_t* inreg0 asm("r0") = str;
    register wchar_t** inreg1 asm("r1") = endptr;
    register int inreg2 asm("r2") = base;
    static const u32 offset = 0x1013C8+1;

    __asm__ __volatile__("BLX %[off]"
                        : "=r"(outreg) //output
                        : "r"(inreg0), "r"(inreg1), "r"(inreg2), [off] "r" (offset)); //input
    return outreg;
}

//char
//size_t (*strlen)(const char *str) = NULL;                            //EUR 1.5: 0x2FAA20
//int (*strcmp)(const char *str1, const char *str2) = NULL;            //EUR 1.5: 0x2FA95C
//int (*strncmp)(const char *str1, const char *str2, size_t n) = NULL; //EUR 1.5: 0x150FB4
//u32 (*strtoul)(const char *str, char **endptr, int base) = NULL;     //EUR 1.5: 0x10134A

//wchar_t
    //size_t (*wcslen)(const wchar_t* wcs) = NULL; //EUR 1.5: 0x100834
//int (*wcscmp)(const wchar_t *wcs1, const wchar_t *wcs2) = NULL;      //EUR 1.5: 0x101848
    //int (*wcsncmp)(const wchar_t* wcs1, const wchar_t* wcs2, size_t num) = NULL; //EUR 1.5: 0x101864
    //u32 (*wcstoul)(const wchar_t* str, wchar_t** endptr, int base) = NULL; //EUR 1.5: 0x1013C8


//Custom Function Declarations
void Text2Item(const wchar_t* KeyboardStr);
void ChangeTime(const wchar_t* KeyboardStr);
void DupeAll(const wchar_t* KeyboardStr);





#endif