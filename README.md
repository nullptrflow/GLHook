# GLHook

A lightweight OpenGL hooking library for intercepting and modifying OpenGL function calls in real-time.

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-green?style=for-the-badge)
![Windows](https://img.shields.io/badge/Platform-Windows-blue?style=for-the-badge&logo=windows&logoColor=white)

> [!NOTE]
> **Educational Purpose:** This project is for educational and research purposes only. Understand the code you're using and use it responsibly.

---

## Features

- **Multi-Function Hooking:** Intercept and modify OpenGL rendering functions
- **Trampoline Generation:** Automatically generates trampolines for hooked functions
- **x86/x64 Support:** Works on both 32-bit and 64-bit architectures
- **Instruction Disassembly:** Uses HDE64 for accurate instruction disassembly
- **Buffer Management:** Custom memory buffer allocation for trampolines
- **Non-Invasive:** Preserves original function behavior while allowing inspection/modification

## Currently Hooked Functions

- **glOrtho** - Projection matrix manipulation
- **glScalef** - Object scaling control
- **glTranslatef** - Object translation/positioning
- **glRotatef** - Object rotation control
- **wglSwapBuffers** - Frame rendering interception

## Use Cases

### Graphics Modification
- Add real-time visual overlays
- Apply shader effects (blur, glow, etc.)
- Modify object colors, sizes, and positions
- Render debugging aids (grids, coordinate axes)

### Monitoring & Debugging
- Log all OpenGL function parameters
- Measure function execution time
- Analyze rendering performance
- Track draw call sequences

### Advanced Applications
- Implement custom rendering pipelines
- Add visualization tools
- Create in-game overlays
- Performance profiling

## Technical Architecture

### Core Components

1. **HookInfo Structure** (lines 293-298)
   - Stores hook metadata and original bytecode
   - Tracks trampoline location and patching position

2. **install_hook()** (lines 300-343)
   - Creates trampoline function
   - Patches target function with jump instruction
   - Saves original bytecode for restoration

3. **remove_hook()** (lines 345-364)
   - Restores original function code
   - Frees allocated trampoline memory
   - Cleans up hook data structures

4. **CreateTrampolineFunction()** (lines 89-291)
   - Disassembles original function instructions
   - Handles position-independent code
   - Manages relative jumps and calls
   - Allocates executable memory for trampoline

### Hooking Mechanism

```
Original Function          Hooked Function
      ↓                          ↓
  [JMP Hook] ──────→ hooked_glOrtho()
      ↓                          ↓
  [Original Code]      [Modified Logic]
  Trampoline                    ↓
      ↓                   original_glOrtho_addr()
   [RET]                        ↓
                            [RET]
```

## How It Works

### Installation Process

1. **Locate Function:** Get address of OpenGL function from `opengl32.dll`
2. **Allocate Trampoline:** Reserve executable memory for original code
3. **Create Trampoline:** Copy and adjust instructions to new location
4. **Patch Function:** Replace first bytes with jump to hook function
5. **Save Original:** Store original bytecode for restoration

### Memory Layout (x64)

```
Target Function:
  [JMP 0xFF 0x25] → Absolute 64-bit jump
  [8 bytes] → Hook function address

Trampoline:
  [Original instructions with adjustments]
  [JMP] → Back to original code after hook
```

### Memory Layout (x86)

```
Target Function:
  [JMP 0xE9] → Relative 32-bit jump
  [4 bytes] → Offset to hook function

Trampoline:
  [Original instructions with adjustments]
  [JMP] → Back to original code after hook
```

## Quick Start

### Prerequisites

- Visual Studio 2022 (or later)
- Windows SDK
- C++17 or later

### Building

1. **Clone the repository:**
   ```bash
   git clone https://github.com/nullptrflow/GLHook.git
   cd GLHook
   ```

2. **Build with Visual Studio:**
   - Open the project in Visual Studio
   - Select **Release x64** configuration
   - Build solution
   - Output: `GLHook.dll`

### Usage

```cpp
#include "dllmain.cpp"

// The DLL automatically hooks OpenGL functions on load
// Add custom logic in the hooked_* functions:

void WINAPI hooked_glOrtho(GLdouble left, GLdouble right, GLdouble bottom, 
                           GLdouble top, GLdouble zNear, GLdouble zFar)
{
    // Your custom code here
    std::cout << "glOrtho called with: " << left << ", " << right << std::endl;
    
    // Call original function
    ((glOrtho_t)original_glOrtho_addr)(left, right, bottom, top, zNear, zFar);
}
```

## Project Structure

```
GLHook/
├── dllmain.cpp           — Main hook implementation
├── help/
│   ├── hde64.h/.c        — x86-64 instruction disassembler
│   ├── buffer.h/.c       — Memory buffer management
│   ├── pstdint.h         — Platform-independent integer types
│   └── table64.h         — HDE64 instruction lookup table
├── README.md             — This file
├── LICENSE               — MIT License
└── CONTRIBUTING.md       — Contribution guidelines
```

## Implementation Details

### Architecture Support

| Feature | x86 | x64 |
|---------|-----|-----|
| Hook Size | 5 bytes | 14 bytes |
| Absolute Jumps | No | Yes |
| RIP-Relative Addressing | N/A | Yes |
| Memory Range | 2GB | 16EB |

### Instruction Handling

The hooking system handles various instruction types:

- **Position-Relative Instructions:** Adjusted based on new location
- **Conditional Jumps:** Converted to absolute jumps with fallthrough
- **Relative Calls:** Preserved with offset recalculation
- **Code Padding:** Detected and utilized for seamless hooking

## Important Notes

### Limitations

1. **OpenGL-Specific:** Only works with OpenGL rendering
2. **Single Process:** Hooks are per-process (DLL must be injected into target)
3. **Thread Safety:** Current implementation is not thread-safe; consider adding synchronization
4. **Anti-Cheat:** May be detected by game anti-cheat systems

### Safety Considerations

- Always test in isolated environments
- Verify game terms of service before use
- Some anti-cheat systems may detect or block hooking
- Use only in single-player or authorized contexts

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Disclaimer

**This software is provided "as is" without warranty of any kind.** The author is not responsible for any misuse, legal issues, or damage resulting from the use of this software. Use at your own risk and responsibility.

## Troubleshooting

### Hook Installation Fails

**Problem:** `install_hook()` returns `false`

**Solutions:**
- Verify target function address is valid
- Check if function size is larger than `MIN_COPY_SIZE`
- Ensure no anti-cheat is actively monitoring

### Crashes After Hooking

**Problem:** Application crashes when calling hooked functions

**Solutions:**
- Check trampoline generation in `CreateTrampolineFunction()`
- Verify original function is called correctly
- Ensure memory protection is properly restored

### Performance Issues

**Problem:** Significant FPS drop or stuttering

**Solutions:**
- Minimize logic in hook functions
- Avoid heavy computations in callbacks
- Use thread-safe logging if needed
- Profile with sampling profiler

## References

- [x86-64 Instruction Set Manual](https://software.intel.com/content/dam/develop/external/us/en/documents/manual/64-ia-32-architectures-software-developer-vol-1-manual.pdf)
- [HDE64 Disassembler](https://github.com/Exodia/HDE64)
- [Windows API Documentation](https://docs.microsoft.com/en-us/windows/win32/)
- [OpenGL Reference](https://www.khronos.org/opengl/)

---

**Created by:** nullptrflow  
**Repository:** https://github.com/nullptrflow/GLHook
