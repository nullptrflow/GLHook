# Quick Start Guide

Get up and running with GLHook in 5 minutes.

## Prerequisites

- **Visual Studio 2022** or later
- **Windows SDK**
- Basic C++ knowledge

## Step 1: Build the DLL

1. Open the project in Visual Studio
2. Select **Release x64** configuration
3. Press **Ctrl+Shift+B** to build
4. Output file: `GLHook.dll`

## Step 2: Inject into Target Application

There are several ways to inject the DLL:

### Method A: Using a DLL Injector

```cpp
// Simple injector example
HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetPID);
LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, dllPath.size(), 
                                 MEM_COMMIT, PAGE_READWRITE);
WriteProcessMemory(hProcess, pDllPath, dllPath.c_str(), dllPath.size(), NULL);

HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, 
                  (LPTHREAD_START_ROUTINE)GetProcAddress(
                      GetModuleHandleA("kernel32.dll"), "LoadLibraryA"),
                  pDllPath, 0, NULL);

WaitForSingleObject(hThread, INFINITE);
CloseHandle(hThread);
CloseHandle(hProcess);
```

### Method B: Using SetWindowsHookEx

```cpp
// Hook method example
HHOOK hHook = SetWindowsHookEx(WH_GETMESSAGE, 
                               (HOOKPROC)GetProcAddress(
                                   LoadLibraryA("GLHook.dll"), 
                                   "DllMain"),
                               hModule, 0);
```

### Method C: Modify Target Application

If you have access to the target application's source:

```cpp
#pragma comment(lib, "GLHook.lib")
// Link against GLHook.lib at build time
```

## Step 3: Verify Hook Installation

Check that hooks are installed by monitoring the application:

1. **Check for FPS drop** - The hook adds minimal overhead
2. **Check for errors** - Look for crash indicators
3. **Add logging** - Modify hook functions to output to console/file

Example logging:

```cpp
void WINAPI hooked_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    OutputDebugStringW(L"glScalef called\n");
    ((glScalef_t)original_glScalef_addr)(x, y, z);
}
```

## Step 4: Customize Hooks

Modify the hook functions in `dllmain.cpp` to add your custom logic:

```cpp
void WINAPI hooked_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    // YOUR CUSTOM CODE HERE
    
    // Call original function
    ((glScalef_t)original_glScalef_addr)(x, y, z);
}
```

## Common Patterns

### Pattern 1: Condition-Based Modification

```cpp
void WINAPI hooked_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    if (x > 2.0f) x = 2.0f;  // Cap scale at 2.0
    ((glScalef_t)original_glScalef_addr)(x, y, z);
}
```

### Pattern 2: Logging to File

```cpp
#include <fstream>
static std::ofstream log_file("hooks.log");

void WINAPI hooked_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    log_file << "Scale: " << x << "," << y << "," << z << "\n";
    ((glScalef_t)original_glScalef_addr)(x, y, z);
}
```

### Pattern 3: Counting Calls

```cpp
static int call_count = 0;

void WINAPI hooked_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    call_count++;
    ((glScalef_t)original_glScalef_addr)(x, y, z);
}
```

## Troubleshooting

### DLL Won't Load

- Check Windows version compatibility
- Verify Visual C++ Redistributable is installed
- Run as Administrator

### Hooks Not Firing

- Verify DLL is injected (use Process Explorer)
- Check if target uses different OpenGL library
- Ensure hooking happens before first OpenGL call

### Application Crashes

- Check trampoline memory is executable
- Verify hook function signature matches original
- Add exception handling in hook functions
- Ensure original function is called correctly

### Performance Issues

- Minimize logic in hook functions
- Avoid memory allocations in hot paths
- Consider caching results
- Use profiler to identify bottlenecks

## Next Steps

1. Review [API.md](API.md) for full API reference
2. Check [EXAMPLES.md](EXAMPLES.md) for practical examples
3. Read [../README.md](../README.md) for technical details
4. Experiment with modifying different functions

## Additional Resources

- [OpenGL Documentation](https://www.khronos.org/opengl/)
- [Windows API Reference](https://docs.microsoft.com/windows/win32/)
- [Visual Studio Documentation](https://docs.microsoft.com/visualstudio/)

## Support

- Check existing [Issues](../../issues)
- Review [Documentation](../README.md)
- Create new [Issue](../../issues/new) with detailed information

---

Happy hooking! 🎣
