#include <Windows.h>
#include <vector>
#include <gl/GL.h>
#pragma comment(lib, "opengl32")
#include <iostream>
#include "help/hde64.h"
#include "help/buffer.c"

#ifdef _WIN64
const size_t HOOK_SIZE = 14;
#define MIN_COPY_SIZE 14
#else
const size_t HOOK_SIZE = 5;
#define MIN_COPY_SIZE 5
#endif

#pragma pack(push, 1)

typedef struct _JMP_REL_SHORT
{
    UINT8  opcode;
    UINT8  operand;
} JMP_REL_SHORT, * PJMP_REL_SHORT;

typedef struct _JMP_REL
{
    UINT8  opcode;
    UINT32 operand;
} JMP_REL, * PJMP_REL, CALL_REL;

typedef struct _JMP_ABS
{
    UINT8  opcode0;
    UINT8  opcode1;
    UINT32 dummy;
    UINT64 address;
} JMP_ABS, * PJMP_ABS;

typedef struct _CALL_ABS
{
    UINT8  opcode0;
    UINT8  opcode1;
    UINT32 dummy0;
    UINT8  dummy1;
    UINT8  dummy2;
    UINT64 address;
} CALL_ABS;

typedef struct _JCC_REL
{
    UINT8  opcode0;
    UINT8  opcode1;
    UINT32 operand;
} JCC_REL;

typedef struct _JCC_ABS
{
    UINT8  opcode;
    UINT8  dummy0;
    UINT8  dummy1;
    UINT8  dummy2;
    UINT32 dummy3;
    UINT64 address;
} JCC_ABS;

#pragma pack(pop)

typedef struct _TRAMPOLINE
{
    LPVOID pTarget;
    LPVOID pDetour;
    LPVOID pTrampoline;

    BOOL   patchAbove;
    UINT   nIP;
    UINT8  oldIPs[8];
    UINT8  newIPs[8];
} TRAMPOLINE, * PTRAMPOLINE;

static BOOL IsCodePadding(LPVOID pAddress, UINT size) {
    LPBYTE p = (LPBYTE)pAddress;
    while (size--) {
        if (*p != 0x90 && *p != 0xCC) return FALSE;
        p++;
    }
    return TRUE;
}

BOOL CreateTrampolineFunction(PTRAMPOLINE ct)
{
#if defined(_M_X64) || defined(__x86_64__)
    CALL_ABS call = {
        0xFF, 0x15, 0x00000002,
        0xEB, 0x08,
        0x0000000000000000ULL
    };
    JMP_ABS jmp = {
        0xFF, 0x25, 0x00000000,
        0x0000000000000000ULL
    };
    JCC_ABS jcc = {
        0x70, 0x0E,
        0xFF, 0x25, 0x00000000,
        0x0000000000000000ULL
    };
#else
    CALL_REL call = {
        0xE8,
        0x00000000
    };
    JMP_REL jmp = {
        0xE9,
        0x00000000
    };
    JCC_REL jcc = {
        0x0F, 0x80,
        0x00000000
    };
#endif

    UINT8     oldPos = 0;
    UINT8     newPos = 0;
    ULONG_PTR jmpDest = 0;
    BOOL      finished = FALSE;
#if defined(_M_X64) || defined(__x86_64__)
    UINT8     instBuf[16];
#endif

    ct->patchAbove = FALSE;
    ct->nIP = 0;

    do
    {
        hde64s    hs;
        UINT      copySize;
        LPVOID    pCopySrc;
        ULONG_PTR pOldInst = (ULONG_PTR)ct->pTarget + oldPos;
        ULONG_PTR pNewInst = (ULONG_PTR)ct->pTrampoline + newPos;

        copySize = hde64_disasm((LPVOID)pOldInst, &hs);
        if (hs.flags & F_ERROR)
            return FALSE;

        pCopySrc = (LPVOID)pOldInst;
        if (oldPos >= MIN_COPY_SIZE)
        {
#if defined(_M_X64) || defined(__x86_64__)
            jmp.address = pOldInst;
#else
            jmp.operand = (UINT32)(pOldInst - (pNewInst + sizeof(jmp)));
#endif
            pCopySrc = &jmp;
            copySize = sizeof(jmp);

            finished = TRUE;
        }
#if defined(_M_X64) || defined(__x86_64__)
        else if ((hs.modrm & 0xC7) == 0x05)
        {
            PUINT32 pRelAddr;

            memcpy(instBuf, (LPBYTE)pOldInst, copySize);
            pCopySrc = instBuf;

            pRelAddr = (PUINT32)(instBuf + hs.len - ((hs.flags & 0x3C) >> 2) - 4);
            *pRelAddr
                = (UINT32)((pOldInst + hs.len + (INT32)hs.disp.disp32) - (pNewInst + hs.len));

            if (hs.opcode == 0xFF && hs.modrm_reg == 4)
                finished = TRUE;
        }
#endif
        else if (hs.opcode == 0xE8)
        {
            ULONG_PTR dest = pOldInst + hs.len + (INT32)hs.imm.imm32;
#if defined(_M_X64) || defined(__x86_64__)
            call.address = dest;
#else
            call.operand = (UINT32)(dest - (pNewInst + sizeof(call)));
#endif
            pCopySrc = &call;
            copySize = sizeof(call);
        }
        else if ((hs.opcode & 0xFD) == 0xE9)
        {
            ULONG_PTR dest = pOldInst + hs.len;

            if (hs.opcode == 0xEB)
                dest += (INT8)hs.imm.imm8;
            else
                dest += (INT32)hs.imm.imm32;

            if ((ULONG_PTR)ct->pTarget <= dest
                && dest < ((ULONG_PTR)ct->pTarget + MIN_COPY_SIZE))
            {
                if (jmpDest < dest)
                    jmpDest = dest;
            }
            else
            {
#if defined(_M_X64) || defined(__x86_64__)
                jmp.address = dest;
#else
                jmp.operand = (UINT32)(dest - (pNewInst + sizeof(jmp)));
#endif
                pCopySrc = &jmp;
                copySize = sizeof(jmp);

                finished = (pOldInst >= jmpDest);
            }
        }
        else if ((hs.opcode & 0xF0) == 0x70
            || (hs.opcode & 0xFC) == 0xE0
            || (hs.opcode2 & 0xF0) == 0x80)
        {
            ULONG_PTR dest = pOldInst + hs.len;

            if ((hs.opcode & 0xF0) == 0x70
                || (hs.opcode & 0xFC) == 0xE0)
                dest += (INT8)hs.imm.imm8;
            else
                dest += (INT32)hs.imm.imm32;

            if ((ULONG_PTR)ct->pTarget <= dest
                && dest < ((ULONG_PTR)ct->pTarget + MIN_COPY_SIZE))
            {
                if (jmpDest < dest)
                    jmpDest = dest;
            }
            else if ((hs.opcode & 0xFC) == 0xE0)
            {
                return FALSE;
            }
            else
            {
                UINT8 cond = ((hs.opcode != 0x0F ? hs.opcode : hs.opcode2) & 0x0F);
#if defined(_M_X64) || defined(__x86_64__)
                jcc.opcode = 0x71 ^ cond;
                jcc.address = dest;
#else
                jcc.opcode1 = 0x80 | cond;
                jcc.operand = (UINT32)(dest - (pNewInst + sizeof(jcc)));
#endif
                pCopySrc = &jcc;
                copySize = sizeof(jcc);
            }
        }
        else if ((hs.opcode & 0xFE) == 0xC2)
        {
            finished = (pOldInst >= jmpDest);
        }

        if (pOldInst < jmpDest && copySize != hs.len)
            return FALSE;

        if ((newPos + copySize) > MEMORY_SLOT_SIZE)
            return FALSE;

        if (ct->nIP >= ARRAYSIZE(ct->oldIPs))
            return FALSE;

        ct->oldIPs[ct->nIP] = oldPos;
        ct->newIPs[ct->nIP] = newPos;
        ct->nIP++;

        memcpy((LPBYTE)ct->pTrampoline + newPos, pCopySrc, copySize);
        newPos += copySize;
        oldPos += hs.len;
    } while (!finished);

    if (oldPos < MIN_COPY_SIZE
        && !IsCodePadding((LPBYTE)ct->pTarget + oldPos, MIN_COPY_SIZE - oldPos))
    {
#if defined(_M_X64) || defined(__x86_64__)
#else
        if (oldPos < sizeof(JMP_REL_SHORT)
            && !IsCodePadding((LPBYTE)ct->pTarget + oldPos, sizeof(JMP_REL_SHORT) - oldPos))
        {
            return FALSE;
        }
#endif
        if (!IsExecutableAddress((LPBYTE)ct->pTarget - MIN_COPY_SIZE))
            return FALSE;

        if (!IsCodePadding((LPBYTE)ct->pTarget - MIN_COPY_SIZE, MIN_COPY_SIZE))
            return FALSE;

        ct->patchAbove = TRUE;
    }
    return TRUE;
}

struct HookInfo {
    void* target = nullptr;
    byte original_bytes[HOOK_SIZE];
    void* trampoline = nullptr;
    bool patchAbove = false;
};

bool install_hook(void* target, void* hook_func, void** original, HookInfo* info)
{
    if (!target || !hook_func) return false;

    info->trampoline = AllocateBuffer(target);
    if (!info->trampoline) return false;

    TRAMPOLINE ct = {};
    ct.pTarget = target;
    ct.pDetour = hook_func;
    ct.pTrampoline = info->trampoline;

    if (!CreateTrampolineFunction(&ct)) {
        FreeBuffer(info->trampoline);
        return false;
    }

    info->patchAbove = ct.patchAbove;

    LPVOID patch_target = target;
    if (ct.patchAbove) {
        patch_target = (LPBYTE)target - HOOK_SIZE;
    }

    DWORD old_prot;
    VirtualProtect(patch_target, HOOK_SIZE, PAGE_EXECUTE_READWRITE, &old_prot);
    memcpy(info->original_bytes, patch_target, HOOK_SIZE);

#ifdef _WIN64
    byte hook_inst[HOOK_SIZE] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    *(uint64_t*)(hook_inst + 6) = (uint64_t)hook_func;
#else
    byte hook_inst[HOOK_SIZE] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
    *(uint32_t*)(hook_inst + 1) = (uint32_t)((uintptr_t)hook_func - (uintptr_t)patch_target - HOOK_SIZE);
#endif

    memcpy(patch_target, hook_inst, HOOK_SIZE);
    VirtualProtect(patch_target, HOOK_SIZE, old_prot, &old_prot);

    info->target = target;
    if (original) *original = ct.pTrampoline;

    return true;
}

void remove_hook(HookInfo* info) {
    if (!info->target) return;

    LPVOID patch_target = info->target;
    if (info->patchAbove) {
        patch_target = (LPBYTE)patch_target - HOOK_SIZE;
    }

    DWORD old_prot;
    VirtualProtect(patch_target, HOOK_SIZE, PAGE_EXECUTE_READWRITE, &old_prot);
    memcpy(patch_target, info->original_bytes, HOOK_SIZE);
    VirtualProtect(patch_target, HOOK_SIZE, old_prot, &old_prot);

    if (info->trampoline) {
        FreeBuffer(info->trampoline);
        info->trampoline = nullptr;
    }

    info->target = nullptr;
}

HookInfo hook_ortho = {};
HookInfo hook_scalef = {};
HookInfo hook_translatef = {};
HookInfo hook_rotatef = {};
HookInfo hook_swapbuffers = {};

using glOrtho_t = void(WINAPI*)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
using glScalef_t = void(WINAPI*)(GLfloat, GLfloat, GLfloat);
using glTranslatef_t = void(WINAPI*)(GLfloat, GLfloat, GLfloat);
using glRotatef_t = void(WINAPI*)(GLfloat, GLfloat, GLfloat, GLfloat);
using wglSwapBuffers_t = BOOL(WINAPI*)(HDC);

void* original_glOrtho_addr = nullptr;
void* original_glScalef_addr = nullptr;
void* original_glTranslatef_addr = nullptr;
void* original_glRotatef_addr = nullptr;
void* original_wglSwapBuffers_addr = nullptr;

void WINAPI hooked_glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
    ((glOrtho_t)original_glOrtho_addr)(left, right, bottom, top, zNear, zFar);
}

void WINAPI hooked_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    ((glScalef_t)original_glScalef_addr)(x, y, z);
}

void WINAPI hooked_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    ((glTranslatef_t)original_glTranslatef_addr)(x, y, z);
}

void WINAPI hooked_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    ((glRotatef_t)original_glRotatef_addr)(angle, x, y, z);
}

BOOL WINAPI hooked_wglSwapBuffers(HDC hdc) {
    return ((wglSwapBuffers_t)original_wglSwapBuffers_addr)(hdc);
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        InitializeBuffer();
        HMODULE opengl = GetModuleHandleA("opengl32.dll");
        if (!opengl) return FALSE;
        original_glOrtho_addr = GetProcAddress(opengl, "glOrtho");
        original_glScalef_addr = GetProcAddress(opengl, "glScalef");
        original_glTranslatef_addr = GetProcAddress(opengl, "glTranslatef");
        original_glRotatef_addr = GetProcAddress(opengl, "glRotatef");
        original_wglSwapBuffers_addr = GetProcAddress(opengl, "wglSwapBuffers");
        if (!install_hook(original_glOrtho_addr, hooked_glOrtho, &original_glOrtho_addr, &hook_ortho) ||
            !install_hook(original_glScalef_addr, hooked_glScalef, &original_glScalef_addr, &hook_scalef) ||
            !install_hook(original_glTranslatef_addr, hooked_glTranslatef, &original_glTranslatef_addr, &hook_translatef) ||
            !install_hook(original_glRotatef_addr, hooked_glRotatef, &original_glRotatef_addr, &hook_rotatef) ||
            !install_hook(original_wglSwapBuffers_addr, hooked_wglSwapBuffers, &original_wglSwapBuffers_addr, &hook_swapbuffers)) {
            return FALSE;
        }
    }
    else if (reason == DLL_PROCESS_DETACH) {
        remove_hook(&hook_ortho);
        remove_hook(&hook_scalef);
        remove_hook(&hook_translatef);
        remove_hook(&hook_rotatef);
        remove_hook(&hook_swapbuffers);
        UninitializeBuffer();
    }
    return TRUE;
}