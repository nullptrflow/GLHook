# Contributing to GLHook

Thank you for your interest in contributing to GLHook! This document provides guidelines and instructions for contributing.

## Code of Conduct

- Be respectful and constructive
- Provide clear descriptions of changes
- Test your code before submitting
- Follow the existing code style

## Getting Started

1. **Fork the repository**
2. **Clone your fork:**
   ```bash
   git clone https://github.com/yourusername/GLHook.git
   cd GLHook
   ```
3. **Create a feature branch:**
   ```bash
   git checkout -b feature/your-feature-name
   ```

## Making Changes

### Code Style

- Use **consistent indentation** (4 spaces)
- Follow **C++ naming conventions:**
  - Functions: `camelCase` or `snake_case`
  - Variables: `snake_case`
  - Constants: `UPPER_CASE`
  - Types: `PascalCase`

### Example:

```cpp
// Function naming
bool install_hook(void* target, void* hook_func, void** original, HookInfo* info)

// Variable naming
HookInfo hook_info = {};
LPVOID patch_target = nullptr;

// Constants
const size_t HOOK_SIZE = 14;
```

### Commit Messages

Use clear, descriptive commit messages:

```
feat: Add new glGetError hook for debugging
fix: Resolve memory leak in trampoline cleanup
docs: Update hooking mechanism documentation
refactor: Simplify instruction handling logic
```

## Testing

- Test changes on both **x86 and x64** architectures
- Verify no regressions in existing hooks
- Test memory allocation and cleanup
- Validate with different OpenGL applications

## Submitting Changes

1. **Push to your fork:**
   ```bash
   git push origin feature/your-feature-name
   ```

2. **Create a Pull Request** with:
   - Clear title describing the change
   - Detailed description of what changed and why
   - Reference to related issues (if any)
   - Test results

3. **Pull Request Template:**
   ```markdown
   ## Description
   Brief description of changes
   
   ## Type of Change
   - [ ] Bug fix
   - [ ] New feature
   - [ ] Enhancement
   - [ ] Documentation
   
   ## Testing
   - [ ] Tested on x86
   - [ ] Tested on x64
   - [ ] No regressions
   
   ## Checklist
   - [ ] Code follows style guidelines
   - [ ] Comments added for complex logic
   - [ ] Documentation updated
   ```

## Reporting Issues

When reporting bugs, include:

- **Environment:** Windows version, architecture (x86/x64)
- **OpenGL Application:** Which app you were targeting
- **Steps to Reproduce:** Clear steps to reproduce the issue
- **Expected vs Actual:** What should happen vs what actually happens
- **Error Messages:** Any error output or crashes
- **Attachments:** Crash dumps or logs if applicable

## Enhancement Suggestions

Have ideas for improvements?

1. Check existing issues to avoid duplicates
2. Create an issue with detailed proposal
3. Include use cases and benefits
4. Link any related resources or references

## Questions?

- Open a discussion in the Issues section
- Check existing documentation
- Review the README for common questions

## License

By contributing, you agree your code will be licensed under the MIT License.

---

Thank you for contributing to GLHook!
