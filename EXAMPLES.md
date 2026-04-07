# GLHook Examples

This document contains practical examples of using GLHook for various scenarios.

## Example 1: Basic Hooking and Logging

Log all `glScalef` calls with their parameters.

```cpp
#include <iostream>
#include <fstream>

std::ofstream log_file;
void* original_glScalef_addr = nullptr;
HookInfo hook_scalef = {};

using glScalef_t = void(WINAPI*)(GLfloat, GLfloat, GLfloat);

void WINAPI hooked_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    // Log the call
    log_file << "glScalef(" << x << ", " << y << ", " << z << ")" << std::endl;
    log_file.flush();
    
    // Call original function
    ((glScalef_t)original_glScalef_addr)(x, y, z);
}

bool SetupLogging()
{
    log_file.open("opengl_calls.log", std::ios::app);
    
    HMODULE opengl = GetModuleHandleA("opengl32.dll");
    if (!opengl) return false;
    
    return install_hook(
        GetProcAddress(opengl, "glScalef"),
        hooked_glScalef,
        &original_glScalef_addr,
        &hook_scalef
    );
}

void CleanupLogging()
{
    remove_hook(&hook_scalef);
    if (log_file.is_open())
        log_file.close();
}
```

## Example 2: Modifying Function Behavior

Modify object scale to always be at least 1.0 on all axes.

```cpp
void* original_glScalef_addr = nullptr;
HookInfo hook_scalef = {};

using glScalef_t = void(WINAPI*)(GLfloat, GLfloat, GLfloat);

void WINAPI hooked_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    // Enforce minimum scale
    if (x < 1.0f) x = 1.0f;
    if (y < 1.0f) y = 1.0f;
    if (z < 1.0f) z = 1.0f;
    
    // Call original with modified values
    ((glScalef_t)original_glScalef_addr)(x, y, z);
}

bool SetupScaleHook()
{
    HMODULE opengl = GetModuleHandleA("opengl32.dll");
    if (!opengl) return false;
    
    return install_hook(
        GetProcAddress(opengl, "glScalef"),
        hooked_glScalef,
        &original_glScalef_addr,
        &hook_scalef
    );
}
```

## Example 3: Performance Monitoring

Track rendering performance by measuring frame time using `wglSwapBuffers`.

```cpp
#include <chrono>

void* original_wglSwapBuffers_addr = nullptr;
HookInfo hook_swapbuffers = {};

using wglSwapBuffers_t = BOOL(WINAPI*)(HDC);

static int frame_count = 0;
static auto last_time = std::chrono::high_resolution_clock::now();

BOOL WINAPI hooked_wglSwapBuffers(HDC hdc)
{
    BOOL result = ((wglSwapBuffers_t)original_wglSwapBuffers_addr)(hdc);
    
    frame_count++;
    
    // Calculate FPS every 60 frames
    if (frame_count >= 60) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            current_time - last_time
        ).count();
        
        float fps = (60.0f / elapsed) * 1000.0f;
        
        // Output FPS (could write to log, console, overlay, etc.)
        std::cout << "FPS: " << fps << std::endl;
        
        frame_count = 0;
        last_time = current_time;
    }
    
    return result;
}

bool SetupPerformanceMonitoring()
{
    HMODULE opengl = GetModuleHandleA("opengl32.dll");
    if (!opengl) return false;
    
    return install_hook(
        GetProcAddress(opengl, "wglSwapBuffers"),
        hooked_wglSwapBuffers,
        &original_wglSwapBuffers_addr,
        &hook_swapbuffers
    );
}
```

## Example 4: Parameter Statistics

Collect statistics about OpenGL calls.

```cpp
#include <map>

struct ScaleStats {
    float min_x, max_x, avg_x;
    float min_y, max_y, avg_y;
    float min_z, max_z, avg_z;
    int call_count;
};

ScaleStats scale_stats = {
    FLT_MAX, FLT_MIN, 0,
    FLT_MAX, FLT_MIN, 0,
    FLT_MAX, FLT_MIN, 0,
    0
};

void* original_glScalef_addr = nullptr;
HookInfo hook_scalef = {};

using glScalef_t = void(WINAPI*)(GLfloat, GLfloat, GLfloat);

void WINAPI hooked_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    // Update statistics
    scale_stats.min_x = std::min(scale_stats.min_x, x);
    scale_stats.max_x = std::max(scale_stats.max_x, x);
    scale_stats.avg_x += x;
    
    scale_stats.min_y = std::min(scale_stats.min_y, y);
    scale_stats.max_y = std::max(scale_stats.max_y, y);
    scale_stats.avg_y += y;
    
    scale_stats.min_z = std::min(scale_stats.min_z, z);
    scale_stats.max_z = std::max(scale_stats.max_z, z);
    scale_stats.avg_z += z;
    
    scale_stats.call_count++;
    
    // Call original
    ((glScalef_t)original_glScalef_addr)(x, y, z);
}

void PrintStatistics()
{
    if (scale_stats.call_count == 0) return;
    
    scale_stats.avg_x /= scale_stats.call_count;
    scale_stats.avg_y /= scale_stats.call_count;
    scale_stats.avg_z /= scale_stats.call_count;
    
    std::cout << "glScalef Statistics (call count: " 
              << scale_stats.call_count << ")" << std::endl;
    std::cout << "X: min=" << scale_stats.min_x 
              << " max=" << scale_stats.max_x 
              << " avg=" << scale_stats.avg_x << std::endl;
    std::cout << "Y: min=" << scale_stats.min_y 
              << " max=" << scale_stats.max_y 
              << " avg=" << scale_stats.avg_y << std::endl;
    std::cout << "Z: min=" << scale_stats.min_z 
              << " max=" << scale_stats.max_z 
              << " avg=" << scale_stats.avg_z << std::endl;
}
```

## Example 5: Conditional Hooking

Only hook specific functions based on certain conditions.

```cpp
struct HookConfig {
    bool hook_ortho = false;
    bool hook_scale = true;
    bool hook_translate = true;
    bool hook_rotate = false;
    bool hook_swapbuffers = false;
};

HookConfig config;
HookInfo hooks[5] = {};

bool SetupConditionalHooks(const HookConfig& cfg)
{
    HMODULE opengl = GetModuleHandleA("opengl32.dll");
    if (!opengl) return false;
    
    config = cfg;
    
    if (config.hook_scale) {
        if (!install_hook(
            GetProcAddress(opengl, "glScalef"),
            hooked_glScalef,
            &original_glScalef_addr,
            &hooks[0]
        )) return false;
    }
    
    if (config.hook_translate) {
        if (!install_hook(
            GetProcAddress(opengl, "glTranslatef"),
            hooked_glTranslatef,
            &original_glTranslatef_addr,
            &hooks[1]
        )) return false;
    }
    
    // ... similar for other functions
    
    return true;
}

void CleanupConditionalHooks()
{
    for (int i = 0; i < 5; i++) {
        if (hooks[i].target) {
            remove_hook(&hooks[i]);
        }
    }
}
```

## Example 6: Error Handling

Proper error handling for hook installation.

```cpp
#include <stdexcept>
#include <string>

class HookException : public std::runtime_error {
public:
    HookException(const std::string& msg) : std::runtime_error(msg) {}
};

bool SafeInstallHook(const std::string& func_name, 
                    void* hook_func, 
                    void** original,
                    HookInfo* info)
{
    HMODULE opengl = GetModuleHandleA("opengl32.dll");
    if (!opengl) {
        throw HookException("Failed to load opengl32.dll");
    }
    
    void* target = GetProcAddress(opengl, func_name.c_str());
    if (!target) {
        throw HookException("Function not found: " + func_name);
    }
    
    if (!install_hook(target, hook_func, original, info)) {
        throw HookException("Failed to hook: " + func_name);
    }
    
    return true;
}

// Usage:
try {
    SafeInstallHook("glScalef", hooked_glScalef, &original_glScalef_addr, &hook_scalef);
} catch (const HookException& e) {
    std::cerr << "Hook error: " << e.what() << std::endl;
    // Handle error
}
```

---

For more information, see [API.md](API.md) and [README.md](README.md).
