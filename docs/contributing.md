# Contributing to gitignore

Guidelines for contributing to the gitignore project.

## 🎯 Ways to Contribute

### Code Contributions

- 🐛 Fix bugs and issues
- ✨ Add new features
- 🔧 Improve performance
- 🛡️ Enhance security
- 🧪 Add tests

### Content Contributions

- 📝 Improve documentation
- 🌐 Add new templates
- 🎨 Update existing templates
- 📚 Write tutorials

### Community Contributions

- 🐛 Report bugs
- 💡 Suggest features
- 📖 Help others
- 🌟 Star the repository

## 🚀 Getting Started

### Prerequisites

- Git
- GCC or Clang compiler
- Make
- libcurl development libraries
- Basic C programming knowledge

### Development Setup

1. **Fork the repository:**

   ```bash
   # Click "Fork" on GitHub
   ```

2. **Clone your fork:**

   ```bash
   git clone https://github.com/yourusername/gitignore.git
   cd gitignore
   ```

3. **Set up upstream remote:**

   ```bash
   git remote add upstream https://github.com/mahbubhs/gitignore.git
   ```

4. **Create development branch:**

   ```bash
   git checkout -b feature/your-feature-name
   ```

5. **Build the project:**

   ```bash
   make templates
   make
   ```

6. **Verify installation:**
   ```bash
   ./gitignore --version
   ```

## 🔧 Development Workflow

### 1. Choose an Issue

- Check [GitHub Issues](https://github.com/mahbubhs/gitignore/issues)
- Look for "good first issue" or "help wanted" labels
- Comment on the issue to indicate you're working on it

### 2. Create a Branch

```bash
# For features
git checkout -b feature/add-kotlin-template

# For bug fixes
git checkout -b fix/pattern-detection-bug

# For documentation
git checkout -b docs/update-installation-guide
```

### 3. Make Changes

- Follow the existing code style
- Add comments for complex logic
- Update documentation as needed
- Write tests for new features

### 4. Test Your Changes

```bash
# Build and test
make clean && make
./gitignore --version

# Run existing tests
make test

# Test your specific changes
./gitignore init python
./gitignore list
```

### 5. Commit Changes

```bash
# Stage your changes
git add .

# Commit with descriptive message
git commit -m "feat: add Kotlin language template

- Add templates/kotlin.gitignore with common Kotlin patterns
- Update template generation script
- Add tests for Kotlin template"

# For multiple commits, use rebase to clean up
git rebase -i HEAD~3
```

### 6. Push and Create Pull Request

```bash
# Push your branch
git push origin feature/your-feature-name

# Create Pull Request on GitHub
# Fill out the PR template
# Link to related issues
```

## 📝 Adding Templates

### Built-in Templates

1. **Create template file:**

   ```bash
   cat > templates/kotlin.gitignore << 'EOF'
   # Kotlin
   *.class
   *.jar
   .gradle/
   build/
   *.log
   EOF
   ```

2. **Regenerate embedded templates:**

   ```bash
   make regen-templates
   ```

3. **Rebuild:**

   ```bash
   make
   ```

4. **Test:**
   ```bash
   ./gitignore init kotlin
   cat .gitignore
   ```

### Template Guidelines

- **Focus:** Include only relevant patterns for the language/tool
- **Comments:** Use `#` for section headers
- **Patterns:** One per line, no duplicates
- **Testing:** Verify patterns work with real projects
- **Sources:** Base on official documentation or community standards

### Template Categories

- **Languages:** Programming languages (Python, Java, etc.)
- **Frameworks:** Web frameworks (Django, React, etc.)
- **Tools:** Development tools (Docker, Terraform, etc.)
- **Editors:** IDEs and editors (VS Code, IntelliJ, etc.)
- **OS:** Operating systems (Linux, macOS, Windows)

## 🐛 Reporting Bugs

### Bug Report Template

```markdown
**Describe the bug**
A clear description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:

1. Go to '...'
2. Run '....'
3. See error

**Expected behavior**
A clear description of what you expected to happen.

**Screenshots**
If applicable, add screenshots to help explain your problem.

**Environment:**

- OS: [e.g. Ubuntu 20.04]
- Version: [e.g. gitignore 2.0.0]
- Installation method: [source/build/package]

**Additional context**
Add any other context about the problem here.
```

### Debugging Tips

```bash
# Enable verbose output
gitignore --verbose init python

# Check configuration
cat ~/.config/gitignore/config.conf

# Test with dry-run
gitignore --dry-run init python

# Check file permissions
ls -la .gitignore
```

## 💡 Feature Requests

### Feature Request Template

```markdown
**Is your feature request related to a problem? Please describe.**
A clear description of what the problem is.

**Describe the solution you'd like**
A clear description of what you want to happen.

**Describe alternatives you've considered**
A clear description of any alternative solutions.

**Additional context**
Add any other context or screenshots about the feature request here.
```

### Feature Development

1. **Discuss first:** Open an issue to discuss the feature
2. **Design:** Agree on the implementation approach
3. **Implement:** Follow the development workflow
4. **Document:** Update documentation and help text

## 📚 Documentation

### Updating Documentation

- Keep docs in `docs/` directory
- Use Markdown format
- Include code examples
- Test documentation locally

### Documentation Structure

```
docs/
├── index.md              # Main documentation page
├── installation.md       # Installation guide
├── usage.md             # Usage reference
├── templates.md         # Template reference
├── configuration.md     # Configuration guide
├── building.md          # Development setup
├── api-reference.md     # API documentation
├── troubleshooting.md   # Common issues
└── contributing.md      # This file
```

## 🧪 Testing

### Running Tests

```bash
# Run all tests
make test

# Manual testing
./gitignore --version
./gitignore list
./gitignore show python
```

### Writing Tests

- Add test cases to appropriate functions
- Test edge cases and error conditions
- Verify both success and failure paths
- Include integration tests

### Test Coverage

- Core functionality (init, sync, list)
- Template operations
- Configuration loading
- Error handling
- Cross-platform compatibility

## 🎨 Code Style

### C Code Style

```c
// Function declarations
int function_name(int param1, char *param2) {
    // Variable declarations
    int result = 0;
    char *buffer = NULL;

    // Error checking
    if (param1 < 0) {
        return ERR_INVALID_ARGUMENT;
    }

    // Main logic with comments
    buffer = malloc(MAX_LINE_LEN);
    if (!buffer) {
        return ERR_OUT_OF_MEMORY;
    }

    // Cleanup
    free(buffer);
    return result;
}
```

### Guidelines

- **Indentation:** 4 spaces
- **Line length:** Max 100 characters
- **Functions:** lowercase_with_underscores
- **Variables:** descriptive_names
- **Comments:** Explain why, not what
- **Error handling:** Check all return values
- **Memory:** Free allocated memory

### Naming Conventions

- **Functions:** `verb_noun()` (e.g., `load_config()`, `merge_templates()`)
- **Variables:** `noun` or `noun_adjective` (e.g., `config`, `template_count`)
- **Constants:** `UPPER_CASE` (e.g., `MAX_PATH_LEN`)
- **Types:** `type_name_t` (e.g., `config_t`, `error_code_t`)

## 🔄 Pull Request Process

### Before Submitting

- [ ] Code compiles without warnings
- [ ] All tests pass
- [ ] Documentation updated
- [ ] Commit messages are clear
- [ ] Branch is up to date with main

### PR Template

```markdown
## Description

Brief description of the changes.

## Type of Change

- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Code refactoring
- [ ] Performance improvement

## Testing

Describe the testing performed.

## Screenshots (if applicable)

Add screenshots of UI changes.

## Checklist

- [ ] My code follows the project's style guidelines
- [ ] I have performed a self-review of my own code
- [ ] I have commented my code, particularly in hard-to-understand areas
- [ ] My changes generate no new warnings
- [ ] I have added tests that prove my fix is effective
- [ ] New and existing unit tests pass
- [ ] I have updated the documentation
```

### Review Process

1. **Automated checks:** CI/CD pipeline runs tests
2. **Code review:** Maintainers review code quality
3. **Testing:** Additional manual testing if needed
4. **Merge:** Approved PRs are merged

## 📋 Commit Guidelines

### Commit Message Format

```
type(scope): description

[optional body]

[optional footer]
```

### Types

- **feat:** New feature
- **fix:** Bug fix
- **docs:** Documentation changes
- **style:** Code style changes
- **refactor:** Code refactoring
- **test:** Adding tests
- **chore:** Maintenance tasks

### Examples

```
feat: add Kotlin language template

- Add templates/kotlin.gitignore
- Update template generation
- Add Kotlin to language list

Closes #123
```

```
fix: correct pattern detection for command names

Fix issue where 'sync' was treated as pattern instead of command.
Add better error messages for ambiguous arguments.

Fixes #456
```

## 🤝 Community Guidelines

### Code of Conduct

- Be respectful and inclusive
- Focus on constructive feedback
- Help newcomers learn
- Maintain professional communication

### Getting Help

- **Documentation:** Check `docs/` directory first
- **Issues:** Search existing issues before creating new ones
- **Discussions:** Use GitHub Discussions for questions
- **Discord/Slack:** Join community chat if available

### Recognition

Contributors are recognized in:

- CHANGELOG.md for releases
- GitHub contributor statistics
- Special mentions in release notes

## 📚 Resources

- [Architecture Overview](architecture.md)
- [API Reference](api-reference.md)
- [Building Guide](building.md)
- [Main README](../README.md)
- [GitHub Issues](https://github.com/mahbubhs/gitignore/issues)

---

Thank you for contributing to gitignore! 🎉
