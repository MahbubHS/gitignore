# gitignore

[![Version](https://img.shields.io/badge/version-2.0.0-blue.svg)](https://github.com/mahbubhs/gitignore/releases)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/mahbubhs/gitignore)
[![Platform](https://img.shields.io/badge/platform-linux%20%7C%20macos%20%7C%20windows-lightgrey.svg)](https://github.com/mahbubhs/gitignore)
[![Language](https://img.shields.io/badge/language-C-orange.svg)](https://github.com/mahbubhs/gitignore)

> Professional .gitignore file generator with 17+ built-in templates, smart merging, and automatic duplicate removal.

## ✨ Features

- 🚀 **17+ Built-in Templates** - Python, Node, Rust, C, C++, Java, Go, Swift, Ruby, PHP, TypeScript, VS Code, Vim, IntelliJ, macOS, Linux, Windows
- 🔄 **Smart Merging** - Automatically appends to existing `.gitignore` and removes duplicates
- 📝 **Multiple Patterns** - Add multiple files/paths at once: `gitignore node_modules/ *.log .env`
- 🎯 **Auto-detection** - Scans your project and suggests appropriate templates
- 🎨 **Interactive Mode** - User-friendly template selection with `-t` or `-I` flags
- 🌐 **GitHub Sync** - Download official templates from GitHub's repository
- 💾 **Backup & Restore** - Automatic backups with easy restoration
- 🔧 **Template Generator** - Easy-to-add custom templates (just drop `.gitignore` files)
- 🎯 **Smart PREFIX** - Install anywhere: `PREFIX=~/.local make install`
- ⚡ **Fast** - Templates compiled into binary, no external files needed
- 🎨 **Colored Output** - Beautiful terminal display
- 🪟 **Cross-platform** - Linux, macOS, and Windows support

## 📥 Installation

### Quick Install (Recommended)

```bash
curl -sSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash
```

### Install to Custom Location

```bash
# Install to ~/.local (no sudo required)
curl -sSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash -s ~/.local

# Or use environment variable
export PREFIX=~/.local
curl -sSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash
```

### Build from Source

```bash
git clone https://github.com/mahbubhs/gitignore.git
cd gitignore
make
sudo make install
```

### Custom Installation Path

```bash
# Install to custom location
make PREFIX=~/.local install

# Or set environment variable
export PREFIX=/opt/gitignore
make install

# Check installation path
make show-prefix
```

### Requirements

- GCC or Clang compiler
- libcurl (for GitHub sync)
- Make

**Install dependencies:**

```bash
# Ubuntu/Debian
sudo apt-get install build-essential libcurl4-openssl-dev

# macOS
brew install curl

# Fedora/RHEL
sudo dnf install gcc make libcurl-devel
```

## 🚀 Quick Start

```bash
# Auto-detect project type
gitignore auto

# Create with specific templates
gitignore init python node

# Add multiple patterns at once
gitignore node_modules/ *.log .env dist/

# Interactive mode (quick)
gitignore -t
```

## 📚 Usage

### Adding Patterns (Most Common)

**Single pattern:**
```bash
gitignore node_modules/
gitignore *.log
gitignore .env
```

**Multiple patterns at once:**
```bash
# Add several patterns in one command
gitignore node_modules/ *.log .env dist/

# Add temporary files
gitignore *.tmp *.swp *~ *.bak
```

**✨ Smart Features:**
- ✅ Checks for duplicates before adding
- ✅ Appends to existing `.gitignore`
- ✅ Skips patterns that already exist
- ✅ Works even if `.gitignore` doesn't exist

### Initialize with Templates

**Using built-in templates:**
```bash
# Single language
gitignore init python

# Multiple languages
gitignore init python node rust

# Language + Editor + OS
gitignore init python vscode macos
```

**✨ Smart Features:**
- ✅ Works even if `.gitignore` exists (appends, doesn't overwrite)
- ✅ Removes duplicate patterns automatically
- ✅ Preserves existing patterns
- ✅ No external template files needed (compiled into binary)

### Auto-Detection

```bash
gitignore auto
```

**Automatically detects:**
- `package.json` → Node.js
- `requirements.txt` / `setup.py` → Python  
- `Cargo.toml` → Rust
- `go.mod` → Go
- `pom.xml` / `build.gradle` → Java
- And more...

### Interactive Mode

```bash
# Three ways to start
gitignore interactive
gitignore -t      # Quick shortcut
gitignore -I      # Alternative shortcut
```

Shows a numbered menu of available templates for easy selection.

### GitHub Sync

Download official templates directly from GitHub:

```bash
# Single language
gitignore sync python

# Multiple languages
gitignore sync rust c cpp go
```

**Features:**
- ✅ Downloads from GitHub's official repository
- ✅ Merges with existing `.gitignore`
- ✅ Removes duplicates automatically
- ✅ Caches for 24 hours (faster repeated operations)

### List & Preview Templates

```bash
# Show all available templates
gitignore list

# Filter templates
gitignore list python

# Show built-in only
gitignore list --builtin

# Show custom only
gitignore list --local

# Preview template contents
gitignore show python
gitignore cat rust
```

## 🎯 Pattern vs Command Detection

The tool intelligently distinguishes between patterns and commands:

### Automatically Detected as Pattern:
- Contains `/` (slash): `node_modules/`, `dist/`
- Contains `*` (asterisk): `*.log`, `*.tmp`
- Contains `.` (dot): `.env`, `.DS_Store`
- File/directory exists: `myfile.txt`

### Requires `--add` Flag (Conflicts with Commands):

**Command names:** `init`, `sync`, `list`, `show`, `cat`, `auto`, `interactive`, `append`, `update`, `global`, `backup`, `restore`, `backups`, `history`, `cache`

```bash
# These run commands
gitignore init        # Runs init command
gitignore sync        # Runs sync command

# To add as patterns, use --add
gitignore --add init      # Adds 'init' as pattern
gitignore --add sync list # Adds 'sync' and 'list' as patterns
```

**Clear error messages:**
```bash
$ gitignore sync
✗ Error: Ambiguous argument - did you mean a command or pattern?

If you want to use the 'sync' command, check: gitignore --help
If you want to add 'sync' as a pattern, use: gitignore --add sync
```

## 🎨 Built-in Templates (17+)

### Languages (11)
- **python** - Python projects
- **node** - Node.js / JavaScript
- **rust** - Rust projects
- **c** - C projects
- **cpp** - C++ projects
- **java** - Java projects
- **go** - Go projects
- **swift** - Swift / iOS projects
- **ruby** - Ruby projects
- **php** - PHP projects
- **typescript** - TypeScript projects

### Editors (3)
- **vscode** - Visual Studio Code
- **vim** - Vim editor
- **intellij** - IntelliJ IDEA / JetBrains IDEs

### Operating Systems (3)
- **macos** - macOS specific patterns
- **linux** - Linux specific patterns
- **windows** - Windows specific patterns

**✨ All templates are compiled into the binary - no external files needed!**

## 🔧 Advanced Features

### Global gitignore

```bash
# Create global .gitignore
gitignore global init

# Add templates to global .gitignore
gitignore global add macos vscode

# Enable in git
git config --global core.excludesfile ~/.gitignore_global
```

### Backup & Restore

```bash
# Create backup
gitignore backup

# List available backups
gitignore backups
gitignore history

# Restore from backup
gitignore restore <backup_name>
```

### Append/Update Modes

```bash
# Append templates (simple append)
gitignore append python

# Smart update (removes duplicates)
gitignore update node vscode
```

### Dry Run

Preview what would happen without making changes:

```bash
gitignore --dry-run init python node
gitignore --dry-run auto
gitignore --dry-run sync rust
```

### Cache Management

```bash
# Clear template cache (GitHub synced templates)
gitignore cache clear
```

## 🎨 Custom Templates

### Using Custom Templates

Create your own templates in `~/.config/gitignore/templates/`:

```bash
# Create custom template
mkdir -p ~/.config/gitignore/templates
cat > ~/.config/gitignore/templates/myproject.gitignore << 'EOF'
# My custom project patterns
.myconfig
secrets/
*.secret
temp/
EOF

# Use it
gitignore init myproject
```

**Template Priority:** Custom templates override built-in templates with the same name.

### Adding Built-in Templates (For Developers)

Want to add a new built-in template? It's easy!

```bash
# 1. Create template file
cat > templates/django.gitignore << 'EOF'
# Django
*.log
*.pot
*.pyc
__pycache__/
db.sqlite3
media/
staticfiles/
EOF

# 2. Regenerate templates.c
make regen-templates

# 3. Build
make

# Done! Your template is now built-in
gitignore init django
```

**See `docs/ADDING_TEMPLATES.md` for complete guide.**

## ⚙️ Configuration

Configuration file: `~/.config/gitignore/config.conf`

```conf
# gitignore configuration file

auto_backup=true         # Auto-backup before changes
cache_enabled=true       # Enable template caching
cache_duration=86400     # Cache duration (24 hours)
verbose=false            # Verbose output
use_color=true          # Colored terminal output
```

## 📖 Examples

### Example 1: New Python Project
```bash
cd my-python-project
gitignore init python vscode
```

### Example 2: Existing Project
```bash
cd existing-project
# .gitignore already exists with some patterns
gitignore init rust
# ✅ Rust patterns added
# ✅ Existing patterns preserved
# ✅ Duplicates removed
```

### Example 3: Quick Pattern Addition
```bash
gitignore node_modules/ .env *.log dist/
# ✅ All 4 patterns added at once
# ✅ Duplicates automatically skipped
```

### Example 4: Multi-language Project
```bash
gitignore init python node rust go
# ✅ All 4 languages merged
# ✅ Duplicates removed
# ✅ Works even if .gitignore exists
```

### Example 5: Full Workflow
```bash
# Start a new project
mkdir my-app && cd my-app
git init

# Auto-detect and create .gitignore
gitignore auto

# Add custom patterns
gitignore secrets/ *.key

# Backup before major changes
gitignore backup

# Update with more templates
gitignore init docker nginx
```

## 🚀 How It Works

### Smart Merging

```bash
# Existing .gitignore:
node_modules/
.env

# Run: gitignore init python

# Result:
node_modules/     # ✅ Kept (from existing)
.env              # ✅ Kept (from existing)
__pycache__/      # ✅ Added (from Python template)
*.py[cod]         # ✅ Added (from Python template)
# No duplicates!
```

### Template Priority

```
1. Custom templates (~/.config/gitignore/templates/)  ← Highest priority
2. Built-in templates (compiled in binary)            ← Fallback
```

### Duplicate Detection

```bash
# Add pattern
gitignore node_modules/

# Try to add again
gitignore node_modules/
# Output: ⚠ All patterns already exist in .gitignore
```

## 🛠️ Development

### Build with Debug

```bash
make dev
```

### Run Tests

```bash
make test
```

### Add New Template

```bash
# 1. Create template file in templates/
echo "*.myext" > templates/mytemplate.gitignore

# 2. Regenerate
make regen-templates

# 3. Build
make

# 4. Test
./gitignore init mytemplate
```

### Makefile Targets

```bash
make                    # Build project
make templates          # Generate templates.c
make regen-templates    # Force regenerate templates
make clean              # Clean build artifacts
make distclean          # Clean including generated files
make install            # Install to PREFIX
make uninstall          # Remove installation
make test               # Run tests
make package            # Create distribution tarball
make show-prefix        # Show installation prefix
make help               # Show all targets
```

## 📊 Performance

- ✅ Templates compiled into binary (no file I/O)
- ✅ Fast pattern matching and deduplication
- ✅ Efficient duplicate detection algorithm
- ✅ Template caching for GitHub sync (24h TTL)

## 🤝 Contributing

Contributions are welcome! See [CONTRIBUTING.md](CONTRIBUTING.md)

### Quick Contribution Guide

1. Fork the repository
2. Create your feature branch: `git checkout -b feature/amazing-feature`
3. Add your template to `templates/` directory
4. Run `make regen-templates`
5. Test: `make && make test`
6. Commit: `git commit -m 'feat: add amazing-feature template'`
7. Push: `git push origin feature/amazing-feature`
8. Open a Pull Request

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details

## 🙏 Acknowledgments

- [GitHub's gitignore repository](https://github.com/github/gitignore) for inspiration
- All contributors who help improve this tool

## 📬 Links

- **GitHub Repository:** https://github.com/mahbubhs/gitignore
- **Issue Tracker:** https://github.com/mahbubhs/gitignore/issues
- **Changelog:** [CHANGELOG.md](CHANGELOG.md)
- **Documentation:** [docs/](docs/)

## 📈 Project Stats

![GitHub stars](https://img.shields.io/github/stars/mahbubhs/gitignore?style=social)
![GitHub forks](https://img.shields.io/github/forks/mahbubhs/gitignore?style=social)
![GitHub issues](https://img.shields.io/github/issues/mahbubhs/gitignore)
![GitHub pull requests](https://img.shields.io/github/issues-pr/mahbubhs/gitignore)

---

**Made with ❤️ for developers who value clean repositories**

**Star ⭐ this repository if you find it helpful!**