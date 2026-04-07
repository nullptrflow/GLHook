# Documentation Index

Complete guide to all documentation files in the GLHook project.

## 📖 Main Documentation

### [README.md](../README.md) - **START HERE**
- Project overview and features
- Architecture and how it works
- Quick start guide
- Installation instructions
- Troubleshooting section
- **Best for:** Understanding what GLHook is and getting started

### [QUICKSTART.md](QUICKSTART.md) - **5-Minute Setup**
- Step-by-step setup guide
- Build instructions
- DLL injection methods
- Common patterns
- **Best for:** Getting the project running quickly

### [API.md](API.md) - **Technical Reference**
- Complete API documentation
- Function signatures
- Structure definitions
- Code examples
- Adding new hooks
- Error handling
- **Best for:** Understanding how to use the library

## 💡 Learning Resources

### [EXAMPLES.md](EXAMPLES.md) - **Code Samples**
1. Basic hooking and logging
2. Modifying function behavior
3. Performance monitoring
4. Parameter statistics
5. Conditional hooking
6. Error handling

**Best for:** Learning by example with real, working code

## 🚀 Development & Deployment

### [CONTRIBUTING.md](CONTRIBUTING.md) - **Contributing Guidelines**
- How to contribute
- Code style guidelines
- Testing procedures
- Pull request process
- Issue reporting
- **Best for:** If you want to contribute to the project

### [GITHUB_UPLOAD.md](GITHUB_UPLOAD.md) - **Deploy to GitHub**
- Step-by-step GitHub upload
- Repository setup
- Authentication troubleshooting
- After-deployment steps
- Promotion strategies
- **Best for:** Publishing your project on GitHub

### [LICENSE](../LICENSE) - **MIT License**
- Legal terms and conditions
- Permissions and limitations
- **Best for:** Understanding legal usage rights

## 📁 Source Code

### [dllmain.cpp](../src/dllmain.cpp)
- Main hooking implementation (434 lines)
- Hook installation and removal
- Trampoline generation
- DLL entry point

### [help/](../src/help/) - **Support Libraries**
- `hde64.c/h` - x86-64 instruction disassembler
- `buffer.c/h` - Memory buffer management
- `pstdint.h` - Platform-independent types
- `table64.h` - Instruction lookup tables

## 🎯 Quick Navigation

**I want to...**

| Goal | Read This |
|------|-----------|
| Understand what this is | [README.md](../README.md) |
| Build and run it | [QUICKSTART.md](QUICKSTART.md) |
| Learn how to use it | [API.md](API.md) |
| See code examples | [EXAMPLES.md](EXAMPLES.md) |
| Upload to GitHub | [GITHUB_UPLOAD.md](GITHUB_UPLOAD.md) |
| Contribute to project | [CONTRIBUTING.md](CONTRIBUTING.md) |
| Understand the license | [LICENSE](../LICENSE) |
| Review the code | [dllmain.cpp](../src/dllmain.cpp) |

## 📊 Documentation Statistics

```
Total Documentation:    ~1,500 lines
API Documentation:       ~300 lines
Examples:               ~400 lines
GitHub Guide:           ~200 lines
This Index:             ~150 lines
```

## 🔍 Key Concepts by Document

### Architecture Deep Dive
- See: [README.md - Technical Architecture](../README.md#technical-architecture)
- Code: [dllmain.cpp:89-291](../src/dllmain.cpp) (CreateTrampolineFunction)

### Hooking Mechanism
- See: [README.md - How It Works](../README.md#how-it-works)
- Diagram: [README.md - Memory Layout](../README.md#memory-layout-x64)

### Adding New Hooks
- See: [API.md - Adding New Hooks](API.md#adding-new-hooks)
- Example: [EXAMPLES.md - Example 1](EXAMPLES.md#example-1-basic-hooking-and-logging)

### Performance Optimization
- See: [README.md - Performance Issues](../README.md#performance-issues)
- Example: [EXAMPLES.md - Example 3](EXAMPLES.md#example-3-performance-monitoring)

## 🏗️ Project Structure

```
GLHook/
├── 📁 src/
│   ├── 💻 dllmain.cpp            ← Main source
│   └── 📁 help/                  ← Support libraries
│       ├── hde64.c/h
│       ├── buffer.c/h
│       ├── pstdint.h
│       └── table64.h
├── 📁 docs/                      ← Documentation
│   ├── 📄 README.md              ← Start here
│   ├── 📄 QUICKSTART.md          ← Setup guide
│   ├── 📄 API.md                 ← Technical reference
│   ├── 📄 EXAMPLES.md            ← Code examples
│   ├── 📄 CONTRIBUTING.md        ← How to contribute
│   ├── 📄 GITHUB_UPLOAD.md       ← Deployment guide
│   ├── 📄 DOCS_INDEX.md          ← This file
│   └── 📄 SETUP_COMPLETE.md      ← Setup info
├── 📄 README.md                  ← Main README (GitHub)
├── 📄 LICENSE                    ← MIT License
├── .gitignore                    ← Git exclusions
└── .gitattributes                ← Line ending config
```

## 🎓 Learning Path

### For Beginners
1. Read [README.md](README.md) - 10 minutes
2. Follow [QUICKSTART.md](QUICKSTART.md) - 5 minutes
3. Study [EXAMPLES.md](EXAMPLES.md) - 15 minutes
4. Try simple examples yourself - 30 minutes
5. Refer to [API.md](API.md) as needed

### For Advanced Users
1. Skim [README.md - Architecture](README.md#technical-architecture)
2. Review [API.md](API.md) completely
3. Study complex [EXAMPLES.md](EXAMPLES.md) patterns
4. Read [dllmain.cpp](dllmain.cpp) source code
5. Review [CONTRIBUTING.md](CONTRIBUTING.md) if adding features

## 📝 Document Quality Checklist

- ✅ README.md - Comprehensive and well-organized
- ✅ QUICKSTART.md - Clear step-by-step instructions
- ✅ API.md - Complete function documentation
- ✅ EXAMPLES.md - Real, tested code examples
- ✅ CONTRIBUTING.md - Clear contribution guidelines
- ✅ GITHUB_UPLOAD.md - Detailed deployment guide
- ✅ CODE - Well-commented and documented

## 🔗 External Resources

### Official Documentation
- [OpenGL Specification](https://www.khronos.org/opengl/)
- [Windows API Reference](https://docs.microsoft.com/windows/win32/)
- [x86-64 Assembly](https://software.intel.com/content/dam/develop/external/us/en/documents/manual/64-ia-32-architectures-software-developer-vol-1-manual.pdf)

### Related Projects
- [HDE64 - Instruction Disassembler](https://github.com/Exodia/HDE64)
- [MinHook - Advanced Hooking Library](https://github.com/TsudaKageyu/minhook)

## ❓ FAQ

**Q: Where do I start?**
A: Read [README.md](README.md) first, then [QUICKSTART.md](QUICKSTART.md)

**Q: How do I hook a new OpenGL function?**
A: See [API.md - Adding New Hooks](API.md#adding-new-hooks)

**Q: Where are the examples?**
A: All examples are in [EXAMPLES.md](EXAMPLES.md)

**Q: How do I upload to GitHub?**
A: Follow [GITHUB_UPLOAD.md](GITHUB_UPLOAD.md)

**Q: Can I modify and redistribute?**
A: Yes! See [LICENSE](LICENSE) - MIT license allows it

**Q: Where's the source code?**
A: Main code is [dllmain.cpp](dllmain.cpp), helpers in [help/](help/)

## 📧 Support

- **Questions:** Create an issue on GitHub
- **Bugs:** Report with reproduction steps
- **Ideas:** Start a discussion
- **Contributions:** See [CONTRIBUTING.md](CONTRIBUTING.md)

---

**Last Updated:** 2024  
**Documentation Version:** 1.0  
**Project:** GLHook OpenGL Hooking Library
