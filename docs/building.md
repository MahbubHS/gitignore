# Building and Development

Complete guide for building, testing, and developing gitignore.

## 🏗️ Build Requirements

### System Requirements

- **Operating System:** Linux, macOS, or Windows (WSL)
- **Compiler:** GCC 4.8+ or Clang 3.5+
- **Build System:** GNU Make
- **Memory:** 256MB RAM minimum
- **Disk Space:** 50MB for source + build artifacts

### Dependencies

#### Required Libraries

**libcurl** - For GitHub template synchronization

```bash
# Ubuntu/Debian
sudo apt-get install libcurl4-openssl-dev

# macOS (Homebrew)
brew install curl

# Fedora/RHEL
sudo dnf install libcurl-devel

# Arch Linux
sudo pacman -S curl
```

#### Build Tools

**GCC/Clang Compiler**

```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# macOS
xcode-select --install

# Fedora/RHEL
sudo dnf install gcc make
```

## 🚀 Building from Source

### 1. Clone Repository

```bash
git clone https://github.com/mahbubhs/gitignore.git
cd gitignore
```

### 2. Generate Templates

Compile templates into the binary:

```bash
make templates
```

This runs `scripts/generate_templates.sh` to create `src/templates.c`.

### 3. Build Binary

```bash
make
```

This creates the `gitignore` executable.

### 4. Verify Build

```bash
# Check binary exists
ls -la gitignore

# Test basic functionality
./gitignore --version
./gitignore --help
```

### 5. Install (Optional)

```bash
# Install system-wide
sudo make install

# Or install to custom location
make PREFIX=~/.local install
```

## 🛠️ Development Workflow

### Setting Up Development Environment

1. **Clone and build:**

   ```bash
   git clone https://github.com/mahbubhs/gitignore.git
   cd gitignore
   make templates
   make
   ```

2. **Create feature branch:**

   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make changes and test:**

   ```bash
   # Edit source files
   vim src/main.c

   # Rebuild
   make clean && make

   # Test changes
   ./gitignore --help
   ```

### Development Build

Build with debug symbols and no optimizations:

```bash
make dev
```

This uses:

- `-g` for debugging
- `-O0` for no optimization
- `-Wall -Wextra` for warnings

## 🧪 Testing

### Running Tests

```bash
# Run test suite
make test

# Or run manually
./gitignore --version
./gitignore list
./gitignore show python
```

### Manual Testing Scenarios

**Basic functionality:**

```bash
# Test version
./gitignore --version

# Test help
./gitignore --help

# Test template listing
./gitignore list

# Test template preview
./gitignore show python
```

**Core features:**

```bash
# Create test directory
mkdir /tmp/gitignore-test
cd /tmp/gitignore-test

# Test pattern adding
../path/to/gitignore *.log

# Test template initialization
../path/to/gitignore init python

# Test auto-detection (create package.json first)
echo '{}' > package.json
../path/to/gitignore auto
```

**Advanced features:**

```bash
# Test dry-run
./gitignore --dry-run init rust

# Test backup/restore
./gitignore backup
./gitignore backups
./gitignore restore
```

### Cross-Platform Testing

**Linux:**

```bash
# Native Linux testing
make clean && make
./gitignore --version
```

**macOS:**

```bash
# On macOS system
make clean && make
./gitignore --version
```

**Windows (WSL):**

```bash
# In WSL environment
make clean && make
./gitignore --version
```

## 🔧 Makefile Targets

### Main Targets

```bash
make              # Build everything (templates + binary)
make all          # Same as make

make templates    # Generate src/templates.c from templates/
make regen-templates  # Force regenerate templates (clears cache)

make clean        # Remove build artifacts (*.o, binary)
make distclean    # Clean + remove generated files

make install      # Install to PREFIX (default: /usr/local)
make uninstall    # Remove installed files

make test         # Run basic tests
make dev          # Build with debug flags

make package      # Create distribution tarball
make show-prefix  # Display installation paths

make help         # Show all available targets
```

### Custom Variables

```bash
# Set installation prefix
make PREFIX=/usr/local install

# Set compiler
make CC=clang

# Set compiler flags
make CFLAGS="-O2 -Wall"
```

## 📁 Project Structure

```
gitignore/
├── src/                    # Source code
│   ├── main.c             # CLI interface
│   ├── init.c             # Template initialization
│   ├── sync.c             # GitHub sync
│   ├── utils.c            # Utilities
│   ├── features.c         # Advanced features
│   ├── global_backup.c    # Global gitignore
│   ├── cache_config.c     # Config & caching
│   └── templates.c        # Generated templates
├── templates/             # Template sources
│   ├── python.gitignore
│   ├── node.gitignore
│   └── ...
├── scripts/               # Build scripts
│   └── generate_templates.sh
├── man/                   # Manual pages
│   └── gitignore.1
├── docs/                  # Documentation
├── Makefile              # Build system
└── README.md             # User documentation
```

## 🔄 Template Development

### Adding New Templates

1. **Create template file:**

   ```bash
   cat > templates/kotlin.gitignore << 'EOF'
   # Kotlin
   *.class
   *.jar
   .gradle/
   build/
   EOF
   ```

2. **Regenerate templates:**

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
   ```

### Template Format Guidelines

- One pattern per line
- Use `#` for comments
- Include common patterns for the language/tool
- Keep focused on relevant files
- Follow existing naming conventions

### Template Generation Script

The `scripts/generate_templates.sh` script:

- Reads all `.gitignore` files from `templates/`
- Converts them to C string literals
- Creates lookup function `get_builtin_template()`
- Outputs to `src/templates.c`

## 🐛 Debugging

### Debug Build

```bash
# Build with debug symbols
make dev

# Run with debugger
gdb ./gitignore
(gdb) run --version
```

### Verbose Logging

```bash
# Enable verbose output
./gitignore --verbose init python

# Or set in config
echo "verbose=true" >> ~/.config/gitignore/config.conf
```

### Common Issues

**"undefined reference to curl"**

- Install libcurl development headers
- Check linker flags in Makefile

**"templates.c not found"**

- Run `make templates` first
- Check templates/ directory exists

**"permission denied"**

- Use `sudo make install` for system install
- Or install to user directory

## 📊 Performance Tuning

### Compiler Optimizations

```makefile
# In Makefile, adjust CFLAGS
CFLAGS = -O2 -march=native -flto
```

### Memory Usage

- Templates compiled into binary (no runtime file I/O)
- Minimal heap allocation
- Configurable cache limits

### Profiling

```bash
# Build with profiling
make CFLAGS="-pg" LDFLAGS="-pg"

# Run and analyze
./gitignore list
gprof ./gitignore gmon.out
```

## 🚀 Release Process

### Creating Releases

1. **Update version:**

   ```c
   // In gitignore.h
   #define VERSION "2.1.0"
   ```

2. **Update changelog:**

   ```bash
   vim CHANGELOG.md
   ```

3. **Build and test:**

   ```bash
   make clean && make
   make test
   ```

4. **Create release:**
   ```bash
   git tag v2.1.0
   git push origin v2.1.0
   ```

### Distribution Packages

**Tarball creation:**

```bash
make package
```

**Docker image:**

```dockerfile
FROM alpine:latest
COPY gitignore /usr/local/bin/
ENTRYPOINT ["gitignore"]
```

## 🤝 Contributing

### Development Guidelines

- Follow existing code style
- Add comments for complex logic
- Update documentation
- Write tests for new features
- Use meaningful commit messages

### Pull Request Process

1. Fork repository
2. Create feature branch
3. Make changes
4. Test thoroughly
5. Update documentation
6. Submit PR with description

### Code Style

- Use 4-space indentation
- Max line length: 100 characters
- Functions: lowercase with underscores
- Variables: descriptive names
- Error handling: check return values

## 📚 Related Topics

- [Architecture](architecture.md) - System design
- [API Reference](api-reference.md) - Function documentation
- [Contributing](../CONTRIBUTING.md) - Contribution guidelines
