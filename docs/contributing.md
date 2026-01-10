# Contributing to gitignore

Comprehensive guide to contributing to the gitignore project. This document covers development setup, coding standards, testing guidelines, and community expectations for contributors.

## 📋 Table of Contents

- [Code of Conduct](#-code-of-conduct)
- [Getting Started](#-getting-started)
- [Development Environment](#-development-environment)
- [Development Workflow](#-development-workflow)
- [Contributing Areas](#-contributing-areas)
- [Code Standards](#-code-standards)
- [Testing Guidelines](#-testing-guidelines)
- [Documentation Standards](#-documentation-standards)
- [Pull Request Process](#-pull-request-process)
- [Release Process](#-release-process)
- [Community Resources](#-community-resources)

## 🤝 Code of Conduct

### Our Standards

**Inclusive Environment:**

- Be respectful and welcoming to all contributors
- Use inclusive language and avoid discriminatory content
- Focus on constructive feedback and collaborative problem-solving
- Help newcomers learn and grow within the community

**Professional Communication:**

- Keep discussions technical and on-topic
- Provide clear, actionable feedback
- Respect differing viewpoints and experiences
- Maintain professional tone in all interactions

### Enforcement

Violations of this code of conduct may result in:

- Private warning from maintainers
- Temporary or permanent ban from project communication channels
- Removal of contributions or privileges

## 🚀 Getting Started

### Prerequisites

**System Requirements:**

- **Operating System:** Linux, macOS, or Windows (WSL)
- **Compiler:** GCC 4.8+ or Clang 3.5+
- **Build System:** GNU Make 3.81+
- **Dependencies:** libcurl 7.50+
- **Version Control:** Git 2.0+

**Skill Requirements:**

- **Programming:** Intermediate C programming knowledge
- **Tools:** Familiarity with command-line development
- **Git:** Understanding of branching, rebasing, and pull requests
- **Documentation:** Markdown writing skills

### Quick Setup

```bash
# 1. Fork and clone the repository
git clone https://github.com/yourusername/gitignore.git
cd gitignore

# 2. Set up upstream remote
git remote add upstream https://github.com/mahbubhs/gitignore.git

# 3. Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install build-essential libcurl4-openssl-dev

# 4. Build the project
make templates
make

# 5. Verify installation
./gitignore --version
```

### Development Environment Setup

**Recommended IDEs:**

- **VS Code:** With C/C++ extension and GitLens
- **CLion:** Full-featured C/C++ IDE
- **Vim/Emacs:** With C development plugins

**VS Code Configuration:**

```json
{
  "C_Cpp.default.compilerPath": "gcc",
  "C_Cpp.default.cStandard": "c99",
  "C_Cpp.default.cppStandard": "c++11",
  "C_Cpp.default.includePath": ["/usr/include", "${workspaceFolder}/src"],
  "C_Cpp.default.defines": ["DEBUG"],
  "editor.formatOnSave": true,
  "editor.codeActionsOnSave": {
    "source.fixAll.eslint": true
  }
}
```

## 🛠️ Development Environment

### Project Structure

```
gitignore/
├── src/                    # Source code
│   ├── main.c             # CLI entry point
│   ├── init.c             # Template initialization
│   ├── sync.c             # GitHub synchronization
│   ├── templates.c        # Embedded templates (generated)
│   ├── utils.c            # Utility functions
│   ├── cache_config.c     # Configuration and caching
│   ├── features.c         # Feature flags and detection
│   ├── help.c             # Help system
│   └── global_backup.c    # Backup functionality
├── templates/             # Source .gitignore templates
├── docs/                  # Documentation
├── scripts/               # Build and utility scripts
├── man/                   # Manual pages
├── tests/                 # Test suite
├── Makefile              # Build configuration
└── README.md             # Project overview
```

### Build System

**Available Targets:**

```bash
make help          # Show all available targets
make               # Build the main binary
make dev           # Build with debug symbols
make test          # Run test suite
make clean         # Clean build artifacts
make templates     # Generate embedded templates
make install       # Install system-wide
make uninstall     # Remove installation
```

**Custom Build Options:**

```bash
# Debug build with sanitizers
make CFLAGS="-fsanitize=address -fsanitize=undefined" LDFLAGS="-fsanitize=address -fsanitize=undefined"

# Optimized release build
make CFLAGS="-O3 -march=native" LDFLAGS="-s"

# Cross-compilation
make CC=aarch64-linux-gnu-gcc
```

### Development Tools

**Code Quality:**

```bash
# Static analysis
cppcheck --enable=all --std=c99 src/

# Code formatting
clang-format -i src/*.c src/*.h

# Memory leak detection
valgrind --leak-check=full ./gitignore list
```

**Testing Tools:**

```bash
# Unit testing framework (if available)
# Integration testing
make test

# Performance benchmarking
time ./gitignore list
```

## 🔄 Development Workflow

### 1. Choose a Task

**Finding Issues:**

- Review [GitHub Issues](https://github.com/mahbubhs/gitignore/issues)
- Look for labels: `good first issue`, `help wanted`, `bug`, `enhancement`
- Check [Discussions](https://github.com/mahbubhs/gitignore/discussions) for ideas
- Review the [roadmap](../ROADMAP.md) for planned features

**Issue Triage:**

- Comment on the issue to indicate interest
- Ask clarifying questions if needed
- Wait for maintainer approval before starting work

### 2. Create a Branch

**Branch Naming Convention:**

```bash
# Feature branches
git checkout -b feature/add-kotlin-template
git checkout -b feature/improve-error-messages

# Bug fix branches
git checkout -b fix/segmentation-fault-in-sync
git checkout -b fix/memory-leak-in-template-merge

# Documentation branches
git checkout -b docs/update-api-reference
git checkout -b docs/add-contributing-guide

# Maintenance branches
git checkout -b chore/update-dependencies
git checkout -b refactor/cleanup-code-style
```

**Branch Management:**

```bash
# Keep branch updated with upstream
git fetch upstream
git rebase upstream/main

# Push branch to your fork
git push origin feature/your-feature-name
```

### 3. Implement Changes

**Code Development Process:**

1. **Understand the codebase:** Read relevant source files and documentation
2. **Plan your changes:** Consider impact on existing functionality
3. **Write tests first:** For new features, implement tests before code
4. **Implement solution:** Follow established patterns and conventions
5. **Test thoroughly:** Run all tests and verify functionality
6. **Update documentation:** Modify docs for any user-facing changes

**Incremental Development:**

```bash
# Make small, focused commits
git add -p  # Interactive staging
git commit -m "feat: implement basic template validation

- Add validate_template() function
- Check for common syntax errors
- Return descriptive error messages"

# Build and test after each commit
make && ./gitignore --version
```

### 4. Testing and Validation

**Testing Checklist:**

- [ ] Code compiles without warnings (`make clean && make`)
- [ ] All existing tests pass (`make test`)
- [ ] New functionality works as expected
- [ ] Edge cases are handled properly
- [ ] Error conditions return appropriate codes
- [ ] Memory leaks are absent (`valgrind ./gitignore ...`)
- [ ] Performance is acceptable
- [ ] Cross-platform compatibility verified

**Manual Testing Scenarios:**

```bash
# Basic functionality
./gitignore --version
./gitignore list | head -10

# Template operations
./gitignore init python
./gitignore show python | head -20

# Error conditions
./gitignore init nonexistent-template
./gitignore --invalid-option

# Configuration
./gitignore --verbose init python
```

### 5. Commit and Push

**Commit Message Standards:**

```bash
# Format: type(scope): description

# Types:
# feat: new feature
# fix: bug fix
# docs: documentation
# style: formatting
# refactor: code restructuring
# test: testing
# chore: maintenance

# Examples:
git commit -m "feat(templates): add Rust language template

- Add templates/rust.gitignore with Cargo and target patterns
- Update template generation script
- Add Rust to built-in template list

Closes #123"

git commit -m "fix(sync): resolve segmentation fault in network timeout

Fix null pointer dereference when network request times out.
Add proper error handling for curl operations.

Fixes #456"
```

**Commit Best Practices:**

- Keep commits focused and atomic
- Write clear, descriptive messages
- Reference issue numbers when applicable
- Use `git commit --amend` for fixes to recent commits
- Use interactive rebase to clean up commit history

### 6. Create Pull Request

**PR Preparation:**

```bash
# Ensure branch is up to date
git fetch upstream
git rebase upstream/main

# Push final version
git push origin feature/your-feature-name --force-with-lease
```

**PR Template:**

```markdown
## Description

[Brief description of the changes and their purpose]

## Type of Change

- [ ] 🐛 Bug fix (non-breaking change fixing an issue)
- [ ] ✨ New feature (non-breaking change adding functionality)
- [ ] 💥 Breaking change (fix or feature causing existing functionality to change)
- [ ] 📚 Documentation update
- [ ] 🔧 Code refactoring
- [ ] 🧪 Tests addition or modification

## Testing

### Test Environment

- OS: [e.g., Ubuntu 22.04, macOS 13.0]
- Compiler: [e.g., GCC 11.3, Clang 15.0]
- Dependencies: [e.g., libcurl 7.85]

### Test Cases Performed

- [ ] Unit tests pass (`make test`)
- [ ] Manual testing of new functionality
- [ ] Regression testing of existing features
- [ ] Cross-platform testing (if applicable)
- [ ] Memory leak testing with Valgrind

### Test Results

[Describe test outcomes, include any relevant output or screenshots]

## Checklist

- [ ] My code follows the project's style guidelines
- [ ] I have performed a self-review of my own code
- [ ] I have commented my code, particularly in hard-to-understand areas
- [ ] My changes generate no new warnings or errors
- [ ] I have added tests that prove my fix/feature works
- [ ] New and existing unit tests pass locally
- [ ] I have updated the documentation accordingly
- [ ] I have tested on multiple platforms (Linux, macOS, WSL)
- [ ] Commit messages follow conventional commit format

## Additional Notes

[Any additional context, considerations, or follow-up work needed]

Closes #[issue_number]
```

## 🎯 Contributing Areas

### Code Contributions

**Core Functionality:**

- Command-line argument parsing improvements
- Template merging algorithm enhancements
- Network request optimization
- Memory management improvements
- Error handling standardization

**New Features:**

- Additional template sources (GitLab, custom repositories)
- Template validation and linting
- Interactive mode for template selection
- Configuration file hot-reloading
- Plugin system for custom functionality

**Performance Optimizations:**

- Reduce memory usage for large template sets
- Improve startup time
- Optimize network request caching
- Parallel template processing

### Template Contributions

**Adding New Templates:**

```bash
# 1. Create template file
cat > templates/kotlin.gitignore << 'EOF'
# Kotlin
*.class
*.jar
.gradle/
build/
*.log
.kotlin/
EOF

# 2. Test template
make regen-templates
make
./gitignore init kotlin
cat .gitignore

# 3. Verify patterns work
# Test with real Kotlin project
```

**Template Guidelines:**

- **Relevance:** Only include patterns specific to the language/tool
- **Organization:** Group related patterns with comments
- **Standards:** Base on official documentation or community consensus
- **Testing:** Verify patterns work with real projects
- **Updates:** Keep templates current with language/tool evolution

**Template Categories:**
| Category | Examples | Focus |
|----------|----------|-------|
| Languages | Python, Java, Go | Language-specific patterns |
| Frameworks | Django, React, Spring | Framework conventions |
| Tools | Docker, Terraform, Kubernetes | Tool-specific ignores |
| Editors | VS Code, IntelliJ, Vim | IDE-generated files |
| OS | Linux, macOS, Windows | OS-specific files |

### Documentation Contributions

**Documentation Types:**

- **User Guides:** Installation, usage, configuration
- **API Reference:** Function documentation, data structures
- **Developer Docs:** Architecture, contributing guidelines
- **Tutorials:** Step-by-step guides for common tasks

**Documentation Standards:**

- Use clear, concise language
- Include practical examples
- Keep content up-to-date
- Follow consistent formatting
- Test all code examples

### Testing Contributions

**Test Coverage Areas:**

- Unit tests for individual functions
- Integration tests for command workflows
- Regression tests for bug fixes
- Performance tests for optimizations
- Compatibility tests for cross-platform

**Test Implementation:**

```c
// Example test function
void test_template_merge(void) {
    // Setup
    const char *base = "*.log\nbuild/";
    const char *overlay = "target/\n*.tmp";

    // Execute
    char *result = merge_templates(base, overlay);

    // Assert
    assert(result != NULL);
    assert(strstr(result, "*.log") != NULL);
    assert(strstr(result, "build/") != NULL);
    assert(strstr(result, "target/") != NULL);
    assert(strstr(result, "*.tmp") != NULL);

    // Cleanup
    free(result);
}
```

## 📏 Code Standards

### C Coding Standards

**Language Version:**

- Use C99 standard features
- Avoid C11/C18 features for broader compatibility
- Use `//` comments for new code (C99 standard)

**Code Formatting:**

```c
// Function declarations (K&R style)
int function_name(int param1, const char *param2) {
    // Variable declarations at function start
    int result = 0;
    char *buffer = NULL;

    // Error checking first
    if (param1 < 0 || param2 == NULL) {
        return ERR_INVALID_ARGUMENT;
    }

    // Main logic with proper indentation
    buffer = malloc(MAX_LINE_LEN);
    if (buffer == NULL) {
        return ERR_OUT_OF_MEMORY;
    }

    // Use braces for all control structures
    if (some_condition) {
        do_something();
    } else {
        do_something_else();
    }

    // Cleanup and return
    free(buffer);
    return result;
}
```

**Naming Conventions:**

```c
// Functions: snake_case with descriptive names
int load_configuration(const char *config_path);
void free_template_list(template_list_t *list);

// Variables: snake_case, descriptive
char *config_file_path = NULL;
int template_count = 0;
bool use_color_output = true;

// Constants: UPPER_SNAKE_CASE
#define MAX_PATH_LENGTH 4096
#define DEFAULT_CACHE_DURATION 86400

// Types: snake_case with _t suffix
typedef struct {
    char *name;
    char *content;
    size_t size;
} template_t;

// Enums: PascalCase for type, UPPER_SNAKE for values
typedef enum {
    ERR_SUCCESS = 0,
    ERR_INVALID_ARGUMENT,
    ERR_OUT_OF_MEMORY,
    ERR_FILE_NOT_FOUND
} ErrorCode;
```

### Error Handling

**Error Code Pattern:**

```c
// Define error codes
typedef enum {
    ERR_SUCCESS = 0,
    ERR_INVALID_ARGUMENT = -1,
    ERR_OUT_OF_MEMORY = -2,
    ERR_FILE_NOT_FOUND = -3,
    ERR_NETWORK_ERROR = -4
} error_code_t;

// Function returns error codes
error_code_t load_config(const char *path, config_t *config) {
    if (path == NULL || config == NULL) {
        return ERR_INVALID_ARGUMENT;
    }

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return ERR_FILE_NOT_FOUND;
    }

    // ... implementation ...

    fclose(file);
    return ERR_SUCCESS;
}

// Caller handles errors
error_code_t result = load_config(config_path, &config);
if (result != ERR_SUCCESS) {
    fprintf(stderr, "Failed to load config: %s\n", error_to_string(result));
    return EXIT_FAILURE;
}
```

**Resource Management:**

```c
// RAII-like pattern for C
char *safe_strdup(const char *str) {
    if (str == NULL) return NULL;
    char *dup = strdup(str);
    if (dup == NULL) {
        perror("strdup failed");
        exit(EXIT_FAILURE);
    }
    return dup;
}

// Use goto for cleanup
error_code_t process_file(const char *path) {
    error_code_t result = ERR_SUCCESS;
    FILE *file = NULL;
    char *buffer = NULL;

    file = fopen(path, "r");
    if (file == NULL) {
        result = ERR_FILE_NOT_FOUND;
        goto cleanup;
    }

    buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        result = ERR_OUT_OF_MEMORY;
        goto cleanup;
    }

    // ... process file ...

cleanup:
    free(buffer);
    if (file) fclose(file);
    return result;
}
```

### Memory Management

**Allocation Patterns:**

```c
// Always check allocation results
char *buffer = malloc(size);
if (buffer == NULL) {
    perror("malloc failed");
    return ERR_OUT_OF_MEMORY;
}

// Use calloc for zero-initialized memory
config_t *config = calloc(1, sizeof(config_t));
if (config == NULL) {
    return ERR_OUT_OF_MEMORY;
}

// Free in reverse order of allocation
free(config->templates);
free(config->cache_dir);
free(config);
```

**String Handling:**

```c
// Safe string operations
#define SAFE_STRCPY(dest, src, size) \
    do { \
        strncpy(dest, src, size - 1); \
        dest[size - 1] = '\0'; \
    } while (0)

// Use asprintf for formatted strings (POSIX)
char *message = NULL;
if (asprintf(&message, "Error: %s (line %d)", error_msg, line_num) == -1) {
    return ERR_OUT_OF_MEMORY;
}
// Use message...
free(message);
```

## 🧪 Testing Guidelines

### Test Organization

**Test Structure:**

```
tests/
├── unit/              # Unit tests for individual functions
├── integration/       # Integration tests for workflows
├── fixtures/          # Test data and templates
├── mocks/            # Mock implementations
└── test_runner.c     # Main test runner
```

**Test Categories:**

- **Unit Tests:** Test individual functions in isolation
- **Integration Tests:** Test complete workflows and interactions
- **Regression Tests:** Ensure bug fixes remain fixed
- **Performance Tests:** Validate performance requirements
- **Compatibility Tests:** Cross-platform and cross-version testing

### Writing Tests

**Test Function Pattern:**

```c
// Test function naming: test_<function>_<scenario>
void test_merge_templates_basic(void) {
    // Arrange
    const char *base = "*.log\nbuild/";
    const char *overlay = "target/\n*.tmp";
    char *result = NULL;

    // Act
    result = merge_templates(base, overlay);

    // Assert
    assert(result != NULL);
    assert(strstr(result, "*.log") != NULL);
    assert(strstr(result, "*.tmp") != NULL);

    // Cleanup
    free(result);
}

void test_merge_templates_null_input(void) {
    // Test error conditions
    char *result = merge_templates(NULL, "content");
    assert(result == NULL);
}
```

**Test Runner Implementation:**

```c
typedef void (*test_function_t)(void);

typedef struct {
    const char *name;
    test_function_t function;
} test_case_t;

static const test_case_t test_cases[] = {
    {"merge_templates_basic", test_merge_templates_basic},
    {"merge_templates_null_input", test_merge_templates_null_input},
    // ... more tests
};

int main(int argc, char *argv[]) {
    size_t passed = 0;
    size_t total = sizeof(test_cases) / sizeof(test_cases[0]);

    printf("Running %zu tests...\n", total);

    for (size_t i = 0; i < total; i++) {
        printf("Running %s... ", test_cases[i].name);
        fflush(stdout);

        // Capture signals for crash detection
        test_cases[i].function();

        printf("PASSED\n");
        passed++;
    }

    printf("\nResults: %zu/%zu tests passed\n", passed, total);
    return (passed == total) ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

### Test Coverage Goals

**Coverage Metrics:**

- **Function Coverage:** 90%+ of all functions tested
- **Branch Coverage:** 80%+ of all branches tested
- **Line Coverage:** 85%+ of executable lines tested

**Coverage Areas:**

- Normal execution paths
- Error conditions and edge cases
- Memory allocation failures
- Network failures and timeouts
- File system permission issues
- Cross-platform differences

### Continuous Integration

**CI Pipeline Requirements:**

```yaml
# .github/workflows/ci.yml
name: CI
on: [push, pull_request]

jobs:
  test:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
        compiler: [gcc, clang]

    steps:
      - uses: actions/checkout@v3
      - name: Install dependencies
        run: |
          # Install system dependencies
      - name: Build
        run: make
      - name: Test
        run: make test
      - name: Memory check
        run: make memcheck
```

## 📚 Documentation Standards

### Documentation Structure

**README Standards:**

- Clear project description and purpose
- Installation instructions for multiple platforms
- Basic usage examples
- Build and development setup
- Contributing guidelines link
- License and contact information

**API Documentation:**

```c
/**
 * @brief Merges two .gitignore template contents
 *
 * Combines base and overlay templates, removing duplicates and
 * maintaining proper ordering. Base patterns take precedence.
 *
 * @param base Base template content (can be NULL)
 * @param overlay Overlay template content (can be NULL)
 * @return Merged template content or NULL on error
 *
 * @note Caller is responsible for freeing returned string
 * @note Empty lines and comments are preserved
 *
 * @example
 * const char *base = "*.log\nbuild/";
 * const char *overlay = "target/\n*.tmp";
 * char *result = merge_templates(base, overlay);
 * // result contains all patterns, no duplicates
 * free(result);
 */
char *merge_templates(const char *base, const char *overlay);
```

### Documentation Tools

**Generation:**

```bash
# Generate API documentation
doxygen Doxyfile

# Build manual pages
make man

# Validate documentation
make docs-check
```

**Style Guidelines:**

- Use Markdown for all documentation
- Include table of contents for long documents
- Provide code examples for all features
- Keep screenshots updated
- Test all command examples

## 🔄 Pull Request Process

### PR Review Process

**Automated Checks:**

1. **CI/CD Pipeline:** Tests run automatically on all platforms
2. **Code Quality:** Static analysis and formatting checks
3. **Security:** Vulnerability scanning for dependencies
4. **License:** License compatibility checking

**Manual Review:**

1. **Code Review:** At least one maintainer reviews code
2. **Testing:** Reviewer may perform additional testing
3. **Documentation:** Ensure docs are updated and accurate
4. **Design:** Verify implementation follows project architecture

**Review Criteria:**

- Code follows established patterns and standards
- Tests are comprehensive and pass
- Documentation is complete and accurate
- No breaking changes without justification
- Performance impact is acceptable
- Security implications are addressed

### PR States

**Draft PRs:**

- Work in progress, not ready for review
- Use for early feedback or collaborative development
- Convert to ready when complete

**Ready for Review:**

- All changes implemented and tested
- Documentation updated
- Commit history clean
- Follows PR template

**Under Review:**

- Reviewers assigned and actively reviewing
- Address review comments promptly
- May require additional commits

**Approved:**

- All review criteria met
- Ready for merge
- May be merged by author or maintainer

### Merging

**Merge Strategies:**

- **Squash and Merge:** For single-commit features
- **Rebase and Merge:** For multi-commit features with clean history
- **Merge Commit:** Rarely used, only for major version bumps

**Merge Requirements:**

- All CI checks pass
- At least one approval from maintainer
- No outstanding review comments
- Branch up to date with main

## 📦 Release Process

### Version Numbering

**Semantic Versioning:** `MAJOR.MINOR.PATCH`

- **MAJOR:** Breaking changes
- **MINOR:** New features, backward compatible
- **PATCH:** Bug fixes, backward compatible

**Pre-release Labels:**

- `alpha`: Early testing, may have bugs
- `beta`: Feature complete, stability testing
- `rc`: Release candidate, final testing

### Release Checklist

**Pre-Release:**

- [ ] All planned features implemented
- [ ] All tests pass on all platforms
- [ ] Documentation updated and accurate
- [ ] Changelog written
- [ ] Version numbers updated in code
- [ ] Release notes prepared

**Release Process:**

1. Create release branch from main
2. Update version numbers
3. Run final test suite
4. Create GitHub release
5. Build and publish packages
6. Update documentation site
7. Announce release

**Post-Release:**

- Monitor for issues
- Address bug reports promptly
- Plan next release cycle
- Update roadmap

## 🌐 Community Resources

### Communication Channels

**GitHub:**

- **Issues:** Bug reports and feature requests
- **Discussions:** General questions and community topics
- **Pull Requests:** Code contributions
- **Wiki:** Community documentation

**External:**

- **Stack Overflow:** Technical questions (tag: `gitignore-tool`)
- **Reddit:** Community discussions (r/gitignore)
- **Discord/Slack:** Real-time chat (if available)

### Getting Help

**Self-Service:**

1. Check existing documentation
2. Search GitHub issues and discussions
3. Review troubleshooting guide
4. Run diagnostic commands

**Community Support:**

- Ask clear, specific questions
- Provide context and examples
- Include error messages and environment details
- Be patient and respectful

### Recognition

**Contributor Recognition:**

- GitHub contributor statistics
- CHANGELOG.md mentions
- Release notes acknowledgments
- Special "Contributor of the Month" features

**Milestone Celebrations:**

- First-time contributors welcomed
- Significant contribution milestones
- Community events and AMAs

## 📚 Additional Resources

- **[Architecture Overview](architecture.md)** - System design and patterns
- **[API Reference](api-reference.md)** - Complete function documentation
- **[Building Guide](building.md)** - Development environment setup
- **[Troubleshooting Guide](troubleshooting.md)** - Common issues and solutions
- **[Code of Conduct](code-of-conduct.md)** - Community guidelines

---

**Thank you for contributing to gitignore!** 🎉

Your contributions help make .gitignore management better for developers worldwide. Whether you're fixing bugs, adding features, or improving documentation, every contribution is valued and appreciated.

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
