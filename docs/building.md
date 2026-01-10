# Building and Development Guide

This comprehensive guide covers building, testing, and contributing to the gitignore project with professional development practices.

## 📋 Table of Contents

- [System Requirements](#-system-requirements)
- [Quick Start](#-quick-start)
- [Dependencies](#-dependencies)
- [Building from Source](#-building-from-source)
- [Development Environment](#-development-environment)
- [Testing Strategy](#-testing-strategy)
- [CI/CD Pipeline](#-ci-cd-pipeline)
- [Release Process](#-release-process)
- [Contributing](#-contributing)

## 🖥️ System Requirements

### Minimum Requirements

| Component      | Minimum         | Recommended       | Notes                             |
| -------------- | --------------- | ----------------- | --------------------------------- |
| **CPU**        | x86_64 or ARM64 | Modern multi-core | Any POSIX-compliant architecture  |
| **Memory**     | 256MB RAM       | 512MB+ RAM        | Build process + test execution    |
| **Disk Space** | 100MB           | 500MB+            | Source + build artifacts + cache  |
| **Network**    | Optional        | Broadband         | Required for GitHub sync features |

### Supported Platforms

#### Operating Systems

| OS                | Status             | Architecture          | Notes                        |
| ----------------- | ------------------ | --------------------- | ---------------------------- |
| **Linux**         | ✅ Fully Supported | x86_64, ARM64, ARM32  | Primary development platform |
| **macOS**         | ✅ Fully Supported | x86_64, Apple Silicon | Full feature parity          |
| **Windows (WSL)** | ✅ Fully Supported | x86_64                | Windows Subsystem for Linux  |
| **FreeBSD**       | ✅ Supported       | x86_64, ARM64         | Community tested             |
| **OpenBSD**       | ✅ Supported       | x86_64                | Limited testing              |

#### Compiler Support

| Compiler      | Minimum Version | Recommended | Status           |
| ------------- | --------------- | ----------- | ---------------- |
| **GCC**       | 4.8.0           | 11.0+       | ✅ Production    |
| **Clang**     | 3.5.0           | 14.0+       | ✅ Production    |
| **Intel ICC** | 18.0            | 2021+       | ✅ Community     |
| **MSVC**      | N/A             | N/A         | ❌ Not supported |

### Build Tools

| Tool           | Version  | Purpose             | Installation           |
| -------------- | -------- | ------------------- | ---------------------- |
| **GNU Make**   | 3.81+    | Build orchestration | System package manager |
| **Git**        | 2.0+     | Version control     | System package manager |
| **pkg-config** | Optional | Library detection   | System package manager |

## 🚀 Quick Start

### One-Line Build (Linux/macOS)

```bash
git clone https://github.com/mahbubhs/gitignore.git && \
cd gitignore && \
make templates && make && \
sudo make install
```

### Docker Build

```bash
# Build in isolated environment
docker run --rm -v $(pwd):/src -w /src \
  gcc:11 make templates && make

# Test the build
docker run --rm -v $(pwd):/src -w /src \
  ubuntu:20.04 ./gitignore --version
```

### Verification

```bash
# Check installation
gitignore --version
gitignore --help

# Basic functionality test
gitignore list | head -10
gitignore show python | head -5
```

## 📦 Dependencies

### Required Dependencies

#### libcurl - HTTP Client Library

**Purpose:** GitHub template synchronization and network operations

**Installation by Platform:**

**Ubuntu/Debian:**

```bash
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev
```

**Fedora/RHEL/CentOS:**

```bash
sudo dnf install libcurl-devel
# or
sudo yum install libcurl-devel
```

**Arch Linux:**

```bash
sudo pacman -S curl
```

**macOS (Homebrew):**

```bash
brew install curl
```

**macOS (MacPorts):**

```bash
sudo port install curl
```

**FreeBSD:**

```bash
sudo pkg install curl
```

**OpenBSD:**

```bash
doas pkg_add curl
```

**Windows (MSYS2):**

```bash
pacman -S mingw-w64-x86_64-curl
```

### Optional Dependencies

#### Development Tools

**Documentation Generation:**

```bash
# For man page generation
sudo apt-get install groff  # Linux
brew install groff          # macOS
```

**Code Analysis:**

```bash
# Static analysis
sudo apt-get install cppcheck
pip install flake8           # Python linting

# Code coverage
sudo apt-get install lcov gcovr
```

**Development IDEs:**

```bash
# VS Code extensions
code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cmake-tools
```

## 🏗️ Building from Source

### Standard Build Process

#### 1. Clone Repository

```bash
# HTTPS clone
git clone https://github.com/mahbubhs/gitignore.git
cd gitignore

# SSH clone (if you have SSH keys set up)
git clone git@github.com:mahbubhs/gitignore.git
cd gitignore
```

#### 2. Generate Template Database

```bash
# Generate embedded templates
make templates

# This runs: scripts/generate_templates.sh
# Creates: src/templates.c (auto-generated)
```

**What happens here:**

- Reads all `.gitignore` files from `templates/` directory
- Converts them to C string literals
- Creates lookup table for `get_builtin_template()`
- Embeds ~50KB of template data into binary

#### 3. Build Binary

```bash
# Standard optimized build
make

# Development build with debug symbols
make dev

# Verbose build output
make V=1
```

**Build Output:**

```
gcc -O2 -Wall -Wextra -Werror -std=c99 -pedantic \
    -I. -DCURL_STATICLIB \
    -c src/main.c -o src/main.o
gcc -O2 -Wall -Wextra -Werror -std=c99 -pedantic \
    -I. -DCURL_STATICLIB \
    -c src/templates.c -o src/templates.o
# ... more compilation steps
gcc src/*.o -lcurl -o gitignore
```

#### 4. Verify Build

```bash
# Check binary properties
ls -la gitignore
file gitignore
ldd gitignore  # Linux: check dynamic dependencies

# Functional test
./gitignore --version
./gitignore --help
./gitignore list | wc -l
```

#### 5. Installation

```bash
# Install to system directories (/usr/local)
sudo make install

# Install to custom prefix
make PREFIX=~/.local install

# Install to home directory
make PREFIX=~ install
```

**Installation Layout:**

```
/usr/local/
├── bin/gitignore              # Executable
├── share/man/man1/gitignore.1 # Manual page
└── share/doc/gitignore/       # Documentation
```

### Advanced Build Options

#### Custom Compiler

```bash
# Use Clang instead of GCC
make CC=clang

# Use Intel compiler
make CC=icc

# Cross-compilation
make CC=aarch64-linux-gnu-gcc
```

#### Build Flags

```bash
# Maximum optimization
make CFLAGS="-O3 -march=native -flto"

# Debug build
make CFLAGS="-O0 -g -fsanitize=address"

# Static linking
make LDFLAGS="-static -lcurl -lssl -lcrypto -lz"
```

#### Parallel Builds

```bash
# Use all available cores
make -j$(nproc)

# Use specific number of jobs
make -j4
```

### Build Artifacts

| File              | Purpose               | Size        | Generated By     |
| ----------------- | --------------------- | ----------- | ---------------- |
| `gitignore`       | Main executable       | ~2MB        | `make`           |
| `src/*.o`         | Object files          | ~100KB each | `make`           |
| `src/templates.c` | Generated template DB | ~50KB       | `make templates` |
| `gitignore.1.gz`  | Compressed man page   | ~5KB        | `make install`   |

## 🛠️ Development Environment

### Development Setup

#### 1. Clone with Submodules (if any)

```bash
git clone --recursive https://github.com/mahbubhs/gitignore.git
cd gitignore
```

#### 2. Install Development Dependencies

```bash
# Ubuntu/Debian
sudo apt-get install \
    build-essential \
    libcurl4-openssl-dev \
    gdb \
    valgrind \
    cppcheck \
    clang-format \
    git \
    pkg-config

# macOS
brew install \
    gcc \
    curl \
    gdb \
    valgrind \
    cppcheck \
    clang-format \
    git
```

#### 3. Configure Git

```bash
# Set up Git hooks for code quality
ln -s ../../scripts/pre-commit .git/hooks/pre-commit
chmod +x .git/hooks/pre-commit
```

### Development Workflow

#### Feature Development

```bash
# Create feature branch
git checkout -b feature/add-template-validation
git pull origin main

# Make changes
vim src/main.c
make dev  # Debug build

# Test changes
./gitignore --help
./gitignore list

# Commit with conventional format
git add .
git commit -m "feat: add template validation with error messages

- Add validate_template() function
- Return descriptive errors for invalid templates
- Update error handling in init command"

# Push and create PR
git push origin feature/add-template-validation
```

#### Code Quality Checks

```bash
# Format code
clang-format -i src/*.c src/*.h

# Static analysis
cppcheck --enable=all --std=c99 src/

# Memory leak detection
valgrind --leak-check=full ./gitignore list

# Address sanitizer
make CFLAGS="-fsanitize=address -g" LDFLAGS="-fsanitize=address"
```

### Debugging

#### GDB Debugging

```bash
# Build with debug symbols
make dev

# Start debugging
gdb ./gitignore
(gdb) break main
(gdb) run --help
(gdb) print argc
(gdb) backtrace
```

#### Memory Debugging

```bash
# Valgrind for memory leaks
valgrind --leak-check=full --track-origins=yes ./gitignore list

# Address sanitizer
make CFLAGS="-fsanitize=address -g" LDFLAGS="-fsanitize=address"
./gitignore list
```

#### Performance Profiling

```bash
# GNU gprof
make CFLAGS="-pg" LDFLAGS="-pg"
./gitignore list
gprof ./gitignore gmon.out > profile.txt

# Callgrind (Valgrind)
valgrind --tool=callgrind ./gitignore sync python
kcachegrind callgrind.out.*
```

## 🧪 Testing Strategy

### Test Categories

#### Unit Tests

**Framework:** Custom test framework in `tests/`

```bash
# Run unit tests
make test

# Run specific test suite
make test-unit

# Verbose test output
make test V=1
```

**Test Structure:**

```
tests/
├── test_main.c          # CLI interface tests
├── test_templates.c     # Template system tests
├── test_config.c        # Configuration tests
├── test_utils.c         # Utility function tests
└── test_runner.c        # Test orchestration
```

#### Integration Tests

**End-to-End Testing:**

```bash
# Create test environment
mkdir /tmp/gitignore-test && cd /tmp/gitignore-test

# Test full workflow
../../../gitignore init python vscode
../../../gitignore backup
../../../gitignore list
../../../gitignore show python

# Test error conditions
../../../gitignore init nonexistent-template  # Should fail gracefully
```

#### Cross-Platform Testing

**Docker-based Testing:**

```dockerfile
# Dockerfile.test
FROM ubuntu:20.04
RUN apt-get update && apt-get install -y \
    build-essential libcurl4-openssl-dev make git
COPY . /src
WORKDIR /src
RUN make templates && make
RUN ./gitignore --version
```

**Multi-Architecture Testing:**

```bash
# x86_64 build
make clean && make

# ARM64 cross-compilation
make CC=aarch64-linux-gnu-gcc clean all

# Test on actual ARM64 hardware
scp gitignore arm64-host:/tmp/
ssh arm64-host /tmp/gitignore --version
```

### Test Automation

#### GitHub Actions CI

**Workflow Configuration:** `.github/workflows/ci.yml`

```yaml
name: CI
on: [push, pull_request]
jobs:
  test:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest]
        compiler: [gcc, clang]
    steps:
      - uses: actions/checkout@v3
      - name: Install dependencies
        run: |
          if [ "$RUNNER_OS" == "Linux" ]; then
            sudo apt-get install libcurl4-openssl-dev
          elif [ "$RUNNER_OS" == "macOS" ]; then
            brew install curl
          fi
      - name: Build
        run: make CC=${{ matrix.compiler }}
      - name: Test
        run: make test
```

#### Local Test Execution

```bash
# Run full test suite
make test-all

# Run tests with coverage
make test-coverage

# Generate test report
make test-report
```

### Performance Testing

#### Benchmark Suite

```bash
# Template lookup performance
time ./gitignore show python > /dev/null

# Large template merge
time ./gitignore init python node rust go java

# Cache performance
time ./gitignore sync python  # First run (download)
time ./gitignore sync python  # Second run (cache)
```

#### Memory Usage Analysis

```bash
# Valgrind massif
valgrind --tool=massif ./gitignore list
ms_print massif.out.*

# Peak memory usage
/usr/bin/time -v ./gitignore init python
```

## 🔄 CI/CD Pipeline

### GitHub Actions

#### Build Matrix

**Supported Configurations:**

- **OS:** Ubuntu 20.04/22.04, macOS 11/12, Windows (WSL)
- **Compiler:** GCC 9/11/12, Clang 11/14/15
- **Architecture:** x86_64, ARM64

#### Pipeline Stages

1. **Lint:** Code formatting and static analysis
2. **Build:** Multi-compiler, multi-OS compilation
3. **Test:** Unit tests and integration tests
4. **Package:** Create release artifacts
5. **Deploy:** Upload to package registries

#### Quality Gates

- **Code Coverage:** > 80% (target)
- **Static Analysis:** Zero critical issues
- **Memory Leaks:** Zero leaks in test suite
- **Performance:** No regressions > 5%

### Release Automation

#### Automated Releases

```yaml
# .github/workflows/release.yml
name: Release
on:
  push:
    tags: ["v*"]
jobs:
  release:
    runs-on: ubuntu-latest
    steps:
      - name: Build binaries
        run: |
          make clean && make
          make package
      - name: Create GitHub release
        uses: actions/create-release@v1
        with:
          tag_name: ${{ github.ref }}
          release_name: Release ${{ github.ref }}
          body: |
            ## Changes
            See CHANGELOG.md for details
      - name: Upload assets
        uses: actions/upload-release-asset@v1
        with:
          upload_url: ${{ steps.create_release.outputs.upload_url }}
          asset_path: ./gitignore-${{ github.ref }}.tar.gz
          asset_name: gitignore-${{ github.ref }}.tar.gz
          asset_content_type: application/gzip
```

## 🚀 Release Process

### Version Management

#### Semantic Versioning

**Format:** `MAJOR.MINOR.PATCH`

| Version   | Description      | Example |
| --------- | ---------------- | ------- |
| **MAJOR** | Breaking changes | `2.0.0` |
| **MINOR** | New features     | `1.5.0` |
| **PATCH** | Bug fixes        | `1.4.3` |

#### Version Locations

```c
// src/gitignore.h
#define VERSION_MAJOR 2
#define VERSION_MINOR 0
#define VERSION_PATCH 0
#define VERSION_STRING "2.0.0"
```

```makefile
# Makefile
VERSION = 2.0.0
```

### Release Checklist

#### Pre-Release

- [ ] Update version numbers in all locations
- [ ] Update CHANGELOG.md with release notes
- [ ] Run full test suite: `make test-all`
- [ ] Cross-platform testing completed
- [ ] Documentation updated
- [ ] Code review completed

#### Release

- [ ] Create release branch: `git checkout -b release/v2.0.0`
- [ ] Tag release: `git tag -a v2.0.0 -m "Release v2.0.0"`
- [ ] Push tag: `git push origin v2.0.0`
- [ ] CI/CD builds successfully
- [ ] Create GitHub release with binaries
- [ ] Update package repositories

#### Post-Release

- [ ] Merge release branch to main
- [ ] Update website/documentation
- [ ] Announce release on social media
- [ ] Monitor for critical issues

### Distribution Channels

#### Package Managers

**Homebrew (macOS):**

```ruby
# Formula/gitignore.rb
class Gitignore < Formula
  desc "Gitignore file management tool"
  homepage "https://github.com/mahbubhs/gitignore"
  url "https://github.com/mahbubhs/gitignore/archive/v2.0.0.tar.gz"
  sha256 "..."

  depends_on "curl"

  def install
    system "make", "PREFIX=#{prefix}", "install"
  end

  test do
    system "#{bin}/gitignore", "--version"
  end
end
```

**Linux Packages:**

- **Ubuntu PPA:** `sudo add-apt-repository ppa:mahbubhs/gitignore`
- **Arch AUR:** `yay -S gitignore-git`
- **Fedora COPR:** `sudo dnf copr enable mahbubhs/gitignore`

#### Binary Releases

**GitHub Releases:** Pre-compiled binaries for:

- Linux x86_64, ARM64
- macOS x86_64, Apple Silicon
- Windows x86_64 (via WSL)

## 🤝 Contributing

### Development Guidelines

#### Code Style

**C Code Standards:**

```c
// Function naming: snake_case
int parse_flags(int argc, char *argv[]) {
    // Variable naming: descriptive
    int verbose_flag = 0;

    // Constants: UPPER_SNAKE_CASE
    #define MAX_PATH_LENGTH 4096

    // Error handling: check everything
    if (argc < 1) {
        return ERR_INVALID_ARGUMENT;
    }

    // Memory management: free resources
    char *buffer = malloc(size);
    if (!buffer) {
        return ERR_OUT_OF_MEMORY;
    }
    // ... use buffer ...
    free(buffer);

    return ERR_SUCCESS;
}
```

#### Commit Conventions

**Format:** `type(scope): description`

| Type         | Description      | Example                               |
| ------------ | ---------------- | ------------------------------------- |
| **feat**     | New feature      | `feat: add template validation`       |
| **fix**      | Bug fix          | `fix: handle empty template files`    |
| **docs**     | Documentation    | `docs: update installation guide`     |
| **style**    | Code style       | `style: format with clang-format`     |
| **refactor** | Code restructure | `refactor: simplify template merging` |
| **test**     | Testing          | `test: add integration tests`         |
| **chore**    | Maintenance      | `chore: update dependencies`          |

#### Pull Request Process

1. **Fork and Clone:**

   ```bash
   git clone https://github.com/yourusername/gitignore.git
   cd gitignore
   git remote add upstream https://github.com/mahbubhs/gitignore.git
   ```

2. **Create Feature Branch:**

   ```bash
   git checkout -b feature/your-feature-name
   git pull upstream main
   ```

3. **Development:**

   ```bash
   make dev  # Debug build
   # Make changes
   make test  # Run tests
   ```

4. **Pre-Commit Checks:**

   ```bash
   clang-format -i src/*.c src/*.h  # Format code
   cppcheck src/                    # Static analysis
   make test-all                    # Full test suite
   ```

5. **Commit and Push:**

   ```bash
   git add .
   git commit -m "feat: add your feature description"
   git push origin feature/your-feature-name
   ```

6. **Create Pull Request:**
   - Use PR template
   - Reference related issues
   - Provide test cases
   - Update documentation

### Code Review Checklist

- [ ] **Functionality:** Code works as intended
- [ ] **Testing:** Adequate test coverage
- [ ] **Documentation:** Code is well-documented
- [ ] **Style:** Follows project conventions
- [ ] **Performance:** No obvious inefficiencies
- [ ] **Security:** No security vulnerabilities
- [ ] **Compatibility:** Works on all supported platforms

### Getting Help

- **Issues:** [GitHub Issues](https://github.com/mahbubhs/gitignore/issues)
- **Discussions:** [GitHub Discussions](https://github.com/mahbubhs/gitignore/discussions)
- **Documentation:** [docs/](docs/) directory
- **Community:** [Discord/Slack] (if available)

## 📚 Related Documentation

- **[Installation Guide](installation.md)** - User installation instructions
- **[Usage Guide](usage.md)** - Command reference and examples
- **[API Reference](api-reference.md)** - Developer function documentation
- **[Contributing Guide](contributing.md)** - Detailed contribution guidelines
- **[Architecture](architecture.md)** - System design and data flow
