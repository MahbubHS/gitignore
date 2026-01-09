# Contributing to gitignore

Thank you for your interest in contributing to gitignore! This document provides guidelines and instructions for contributing.

## 🎯 Ways to Contribute

- 🐛 Report bugs
- 💡 Suggest new features
- 📝 Improve documentation
- 🔧 Submit pull requests
- 🌐 Add new language templates
- 🧪 Write tests
- 🎨 Improve user experience

## 🚀 Getting Started

### Prerequisites

- GCC or Clang compiler
- Make
- libcurl development files
- Git

### Setting Up Development Environment

1. **Fork the repository**
   ```bash
   # Click "Fork" on GitHub, then clone your fork
   git clone https://github.com/mahbubhs/gitignore.git
   cd gitignore
   ```

2. **Create a development branch**
   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b fix/your-bug-fix
   ```

3. **Build the project**
   ```bash
   make templates
   make
   ```

4. **Test your changes**
   ```bash
   ./gitignore --version
   ./gitignore list
   # Test your specific changes
   ```

## 📋 Development Guidelines

### Code Style

- Use **4 spaces** for indentation (no tabs)
- Follow existing code style
- Keep functions small and focused
- Add comments for complex logic
- Use meaningful variable names

**Example:**
```c
// Good
int create_gitignore_file(const char *template_name, int options) {
    // Implementation
}

// Avoid
int crt(const char *t, int o) {
    // Implementation
}
```

### Commit Messages

Follow the **Conventional Commits** specification:

```
type(scope): subject

body

footer
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, etc.)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

**Examples:**
```bash
git commit -m "feat(sync): add progress bar for downloads"
git commit -m "fix(cache): resolve memory leak in cache_template"
git commit -m "docs(readme): update installation instructions"
```

### Adding New Features

1. **Discuss first** - Open an issue to discuss your idea
2. **Write tests** - Add tests for new functionality
3. **Update documentation** - Update README.md and help text
4. **Follow patterns** - Use existing code patterns

### Adding New Templates

To add a built-in template:

1. Edit `Makefile` in the `templates` target
2. Add your template following the pattern:
   ```makefile
   @echo "# YourLanguage" > $(TEMPLATE_DIR)/yourlang.gitignore
   @echo "pattern1" >> $(TEMPLATE_DIR)/yourlang.gitignore
   @echo "pattern2" >> $(TEMPLATE_DIR)/yourlang.gitignore
   ```

3. Add to the detection logic in `src/features.c`:
   ```c
   // In detect_project_type function
   {"your-config-file", "yourlang"},
   ```

## 🧪 Testing

### Manual Testing

```bash
# Build
make clean && make templates && make

# Test basic commands
./gitignore --help
./gitignore --version
./gitignore list

# Test init
cd /tmp
./path/to/gitignore init python
cat .gitignore

# Test auto-detect
cd /tmp/test-project
echo '{}' > package.json
./path/to/gitignore auto

# Test sync
./path/to/gitignore sync rust

# Test dry-run
./path/to/gitignore --dry-run init python node
```

### Automated Testing

We use GitHub Actions for CI/CD. Your pull request will automatically:

- Build on Ubuntu and macOS
- Run basic functionality tests
- Check for memory leaks (if valgrind configured)
- Run security scans

## 📝 Pull Request Process

1. **Update your fork**
   ```bash
   git checkout main
   git pull upstream main
   git checkout your-feature-branch
   git rebase main
   ```

2. **Ensure quality**
   - Code builds without warnings
   - All tests pass
   - Documentation is updated
   - No memory leaks

3. **Create pull request**
   - Write clear description
   - Link related issues
   - Add screenshots/examples if applicable

4. **Address review feedback**
   - Respond to comments
   - Make requested changes
   - Push updates

### Pull Request Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Performance improvement

## Testing
How did you test this?

## Checklist
- [ ] Code builds successfully
- [ ] Tests pass
- [ ] Documentation updated
- [ ] Commit messages follow convention
```

## 🐛 Reporting Bugs

### Before Reporting

1. Check existing issues
2. Test with latest version
3. Gather information:
   - OS and version
   - gitignore version
   - Steps to reproduce
   - Expected vs actual behavior

### Bug Report Template

```markdown
**Description**
Clear description of the bug

**To Reproduce**
1. Run command: `gitignore ...`
2. See error

**Expected Behavior**
What should happen

**Environment**
- OS: Ubuntu 22.04
- Version: gitignore 2.0.0
- Built with: GCC 11.3.0

**Additional Context**
Any other relevant information
```

## 💡 Suggesting Features

### Feature Request Template

```markdown
**Problem**
What problem does this solve?

**Proposed Solution**
How should it work?

**Alternatives Considered**
Other approaches you've thought about

**Example Usage**
```bash
gitignore your-new-feature
```

**Additional Context**
Mockups, references, etc.
```

## 🏗️ Project Structure

```
gitignore/
├── src/
│   ├── main.c              # Entry point
│   ├── help.c              # Help system
│   ├── init.c              # Init functionality
│   ├── sync.c              # GitHub sync
│   ├── utils.c             # Utilities
│   ├── features.c          # New features
│   ├── global_backup.c     # Global & backup
│   └── cache_config.c      # Cache & config
├── gitignore.h             # Main header
├── Makefile                # Build system
├── templates/              # Built-in templates
├── man/                    # Manual page
├── .github/
│   └── workflows/          # CI/CD
└── README.md               # Documentation
```

## 📚 Resources

- [C Programming Style Guide](https://www.kernel.org/doc/html/latest/process/coding-style.html)
- [Conventional Commits](https://www.conventionalcommits.org/)
- [GitHub's gitignore repository](https://github.com/github/gitignore)

## 🤝 Code of Conduct

### Our Pledge

We pledge to make participation in our project a harassment-free experience for everyone.

### Our Standards

**Positive behavior:**
- Being respectful
- Accepting constructive criticism
- Focusing on what's best for the community
- Showing empathy

**Unacceptable behavior:**
- Harassment or discrimination
- Trolling or insulting comments
- Publishing private information
- Unprofessional conduct

## 📬 Contact

- **Issues**: [GitHub Issues](https://github.com/mahbubhs/gitignore/issues)
- **Discussions**: [GitHub Discussions](https://github.com/mahbubhs/gitignore/discussions)
- **Email**: maintainer@example.com

## 🎉 Recognition

Contributors will be recognized in:
- README.md
- Release notes
- GitHub contributors page

Thank you for contributing! 🚀

---

**Questions?** Feel free to ask in [GitHub Discussions](https://github.com/mahbubhs/gitignore/discussions)