# GLHook API Documentation

## Overview

GLHook provides a simple and efficient API for intercepting OpenGL function calls. This document describes the public interface and how to use it.

## Core Structures

### HookInfo

Stores metadata about a hooked function.

```cpp
struct HookInfo {
    void* target;                    // Original function address
    byte original_bytes[HOOK_SIZE];  // Original instruction bytes
    void* trampoline;                // Trampoline function address
    bool patchAbove;                 // Whether patch is above target
};
```

**Usage:**
```cpp
HookInfo hook_info = {};
install_hook(target_func, hook_func, &original_addr, &hook_info);
```

### TRAMPOLINE

Internal structure for trampoline generation (not for direct use).

```cpp
typedef struct _TRAMPOLINE {
    LPVOID pTarget;           // Target function address
    LPVOID pDetour;           // Hook function address
    LPVOID pTrampoline;       // Trampoline code location
    BOOL patchAbove;          // Patch above flag
    UINT nIP;                 // Instruction pointer count
    UINT8 oldIPs[8];          // Original instruction positions
    UINT8 newIPs[8];          // New instruction positions
} TRAMPOLINE, * PTRAMPOLINE;
```

## Function Reference

### install_hook()

Installs a hook on target function.

```cpp
bool install_hook(
    void* target,           // [in] Target function address to hook
    void* hook_func,        // [in] Your hook function address
    void** original,        // [out] Pointer to original function (trampoline)
    HookInfo* info          // [out] Hook information structure
);
```

**Parameters:**
- `target`: Address of function to hook
- `hook_func`: Address of your replacement function
- `original`: Will receive trampoline address to call original
- `info`: Will receive hook metadata

**Returns:**
- `true` on success
- `false` on failure (invalid addresses, insufficient memory, etc.)

**Example:**
```cpp
void* original_glOrtho = nullptr;
HookInfo hook_info = {};

bool success = install_hook(
    GetProcAddress(GetModuleHandleA("opengl32.dll"), "glOrtho"),
    hooked_glOrtho,
    &original_glOrtho,
    &hook_info
);

if (success) {
    // Now call original through trampoline
    ((glOrtho_t)original_glOrtho)(left, right, bottom, top, zNear, zFar);
}
```

### remove_hook()

Removes a previously installed hook.

```cpp
void remove_hook(HookInfo* info);
```

**Parameters:**
- `info`: Hook information from install_hook()

**Example:**
```cpp
remove_hook(&hook_info);
// Original function is restored
```

### CreateTrampolineFunction()

Generates trampoline code (internal use).

```cpp
BOOL CreateTrampolineFunction(PTRAMPOLINE ct);
```

**Note:** This is called internally by `install_hook()`. Usually not called directly.

## Hook Function Signatures

### OpenGL Functions

All current hooks follow these signatures:

```cpp
using glOrtho_t = void(WINAPI*)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
using glScalef_t = void(WINAPI*)(GLfloat, GLfloat, GLfloat);
using glTranslatef_t = void(WINAPI*)(GLfloat, GLfloat, GLfloat);
using glRotatef_t = void(WINAPI*)(GLfloat, GLfloat, GLfloat, GLfloat);
using wglSwapBuffers_t = BOOL(WINAPI*)(HDC);
```

## Complete Example

```cpp
#include <Windows.h>
#include <gl/GL.h>
#include <iostream>

// Forward declarations
void* original_glScalef_addr = nullptr;
HookInfo hook_scalef = {};

using glScalef_t = void(WINAPI*)(GLfloat, GLfloat, GLfloat);

// Hook function
void WINAPI hooked_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    // Custom logic
    std::cout << "glScalef called: x=" << x << " y=" << y << " z=" << z << std::endl;
    
    // Call original function via trampoline
    ((glScalef_t)original_glScalef_addr)(x, y, z);
}

// Installation
bool SetupHooks()
{
    HMODULE opengl = GetModuleHandleA("opengl32.dll");
    if (!opengl) return false;
    
    void* glScalef_addr = GetProcAddress(opengl, "glScalef");
    
    return install_hook(
        glScalef_addr,
        hooked_glScalef,
        &original_glScalef_addr,
        &hook_scalef
    );
}

// Cleanup
void RemoveHooks()
{
    remove_hook(&hook_scalef);
}
```

## Adding New Hooks

To hook additional OpenGL functions:

1. **Add typedef for function signature:**
   ```cpp
   using glGetError_t = GLenum(WINAPI*)();
   ```

2. **Create hook function:**
   ```cpp
   GLenum WINAPI hooked_glGetError()
   {
       std::cout << "glGetError called" << std::endl;
       return ((glGetError_t)original_glGetError_addr)();
   }
   ```

3. **Declare globals:**
   ```cpp
   void* original_glGetError_addr = nullptr;
   HookInfo hook_glGetError = {};
   ```

4. **Install in DllMain:**
   ```cpp
   install_hook(
       GetProcAddress(opengl, "glGetError"),
       hooked_glGetError,
       &original_glGetError_addr,
       &hook_glGetError
   );
   ```

5. **Clean up on detach:**
   ```cpp
   remove_hook(&hook_glGetError);
   ```

## Constants

```cpp
const size_t HOOK_SIZE = 14;      // x64: 14 bytes, x86: 5 bytes
#define MIN_COPY_SIZE 14          // Minimum function size to hook
```

## Error Handling

Check return values from `install_hook()`:

```cpp
if (!install_hook(target, hook, &original, &info)) {
    std::cerr << "Failed to install hook!" << std::endl;
    // Handle error appropriately
    return false;
}
```

Common failure reasons:
- Invalid function address
- Insufficient memory for trampoline
- Function too small to hook (< MIN_COPY_SIZE)
- Memory allocation failed

## Thread Safety

**Current implementation is NOT thread-safe.** Consider these best practices:

- Install all hooks before rendering starts
- Remove hooks only when no rendering is occurring
- Use synchronization primitives if needed
- Avoid modifying hook state while hooked functions are executing

## Performance Considerations

- Hook function call adds ~2-3 CPU cycles overhead
- Minimize logic in hook functions
- Avoid allocations in hot paths
- Use thread-local storage if logging from hooks

## Limitations

- Only works on Windows (x86 and x64)
- Requires executable memory access
- May be blocked by anti-cheat systems
- Limited to functions >= MIN_COPY_SIZE bytes
- Does not survive DLL reload without re-installation

---

For more information, see the main [README.md](../README.md).
