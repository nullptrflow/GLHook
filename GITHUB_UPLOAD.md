# How to Push GLHook to GitHub

Follow these steps to upload your project to GitHub.

## Step 1: Create Repository on GitHub

1. Go to https://github.com/new
2. Enter repository name: `GLHook`
3. Select visibility: `Public` (recommended for open source)
4. Check "Add a README file" - **No** (we already have one)
5. Select license: **MIT License**
6. Click "Create repository"

## Step 2: Connect Local Repository to GitHub

After creating the repository, GitHub will show commands. Run these in your local repository:

```bash
git remote add origin https://github.com/YOUR_USERNAME/GLHook.git
git branch -M main
git push -u origin main
```

**Replace `YOUR_USERNAME` with your actual GitHub username.**

### Alternative (if using SSH):

```bash
git remote add origin git@github.com:YOUR_USERNAME/GLHook.git
git branch -M main
git push -u origin main
```

## Step 3: Verify Upload

1. Visit https://github.com/YOUR_USERNAME/GLHook
2. Check that all files are visible:
   - `README.md` - Main documentation
   - `LICENSE` - MIT License
   - `dllmain.cpp` - Main source code
   - `API.md` - API documentation
   - `EXAMPLES.md` - Code examples
   - `QUICKSTART.md` - Getting started guide
   - `CONTRIBUTING.md` - Contribution guidelines
   - `help/` - Helper libraries

## Step 4: Update Repository Description

1. Click repository **Settings**
2. Under "Repository details", add:
   - **Description:** `A lightweight OpenGL hooking library for intercepting and modifying OpenGL function calls`
   - **Homepage URL:** (optional - leave blank or add your website)
   - **Topics:** Add tags like: `opengl`, `hooking`, `windows`, `c++`, `dll`

## Step 5: Add Topics/Tags (Optional but Recommended)

These help people find your project:

```
- opengl
- hooking
- windows
- dll
- graphics
- c++
- reverse-engineering
```

## Troubleshooting

### "fatal: remote origin already exists"

```bash
git remote remove origin
git remote add origin https://github.com/YOUR_USERNAME/GLHook.git
```

### "Authentication failed"

**For HTTPS:**
- Use Personal Access Token instead of password
- Create token at: https://github.com/settings/tokens
- Grant `repo` scope permissions

**For SSH:**
- Add SSH key to GitHub: https://github.com/settings/keys
- Generate key: `ssh-keygen -t ed25519 -C "your_email@example.com"`

### "Updates were rejected"

If you get an error about rejected updates:

```bash
git pull origin main --allow-unrelated-histories
git push -u origin main
```

## After Pushing

1. **GitHub Pages** (Optional):
   - Go to Settings → Pages
   - Select branch: `main`
   - Folder: `/ (root)`
   - This creates automatic documentation site

2. **Add CI/CD** (Optional):
   - Create `.github/workflows/build.yml`
   - Add automated builds and tests

3. **Track Issues**:
   - Enable GitHub Issues (enabled by default)
   - Create issue templates for bug reports

## File Structure on GitHub

After pushing, your repository will look like:

```
GLHook/
├── README.md
├── LICENSE
├── API.md
├── EXAMPLES.md
├── QUICKSTART.md
├── CONTRIBUTING.md
├── .gitignore
├── .gitattributes
├── dllmain.cpp
└── help/
    ├── buffer.c
    ├── buffer.h
    ├── hde64.c
    ├── hde64.h
    ├── pstdint.h
    └── table64.h
```

## Next Steps

1. Promote your project on:
   - Stack Overflow
   - Reddit (r/cpp, r/golang, r/Windows, r/learnprogramming)
   - GitHub Trending (automatic if it gets stars)
   - HackerNews (if significant)

2. Get feedback:
   - Watch for Issues and Pull Requests
   - Respond to community feedback
   - Keep documentation updated

3. Consider adding:
   - GitHub Actions for CI/CD
   - Release tags and changelog
   - More examples and tutorials
   - YouTube tutorial or blog post

## Commands Reference

### View remote configuration
```bash
git remote -v
```

### Change remote URL
```bash
git remote set-url origin https://github.com/YOUR_USERNAME/GLHook.git
```

### Check commit history
```bash
git log --oneline
```

### Push specific branch
```bash
git push -u origin main
```

### Pull latest changes
```bash
git pull origin main
```

---

Your GLHook project is now ready to share with the world! 🚀
