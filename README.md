# gitignore v2.0

A professional, feature-rich command-line tool for creating and managing `.gitignore` files with support for multiple programming languages, editors, and smart automation.

## ✨ Features

- 🚀 **Quick initialization** - Create `.gitignore` files instantly
- 🌐 **GitHub sync** - Download official templates from GitHub's repository
- 🎨 **Customizable** - Use your own custom templates
- 🔄 **Smart merging** - Automatically removes duplicates and handles comments
- 📦 **Built-in templates** - 10+ common languages and editors pre-configured
- ⚡ **Fast and lightweight** - Written in C for maximum performance
- 🎯 **Auto-detection** - Automatically detect project type and suggest templates
- 🎨 **Interactive mode** - User-friendly template selection
- 💾 **Backup & restore** - Automatic backups with easy restoration
- 🌍 **Global gitignore** - Manage system-wide git ignore patterns
- 📊 **Progress indicators** - Visual feedback for long operations
- 🎨 **Colored output** - Beautiful, informative terminal output
- 💨 **Template caching** - Fast repeated operations with smart caching
- 🔍 **Template preview** - View template contents before applying

## 🎯 New in v2.0

1. **Add patterns directly**: `gitignore node_modules/` - Add file/path patterns easily
2. **Interactive mode**: Choose templates from a visual menu
3. **Auto-detection**: Scan project and suggest appropriate templates
4. **Append/Update modes**: Add to existing `.gitignore` without overwriting
5. **Global gitignore**: Manage system-wide ignore patterns
6. **Backup system**: Automatic backups with easy restore
7. **Template caching**: 24-hour cache for faster repeated downloads
8. **Configuration file**: Customize behavior with `config.conf`
9. **Progress bars**: Visual feedback for sync operations
10. **Colored output**: Beautiful, informative terminal display

## 📥 Installation

### Build from source

```bash
# Clone the repository
git clone https://github.com/mahbubhs/gitignore.git
cd gitignore

# Build and install
make templates  # Create built-in templates
make            # Compile
sudo make install
```

### Requirements

- **GCC** or **Clang** compiler
- **libcurl** (for sync feature)
- **Make**

**Install dependencies:**

```bash
# Ubuntu/Debian
sudo apt-get install build-essential libcurl4-openssl-dev

# macOS
brew install curl

# Fedora
sudo dnf install gcc make libcurl-devel
```

## 🚀 Quick Start

```bash
# Create .gitignore for a Python project
gitignore init python

# Auto-detect project type and create .gitignore
gitignore auto

# Interactive mode
gitignore interactive

# Add a pattern to existing .gitignore
gitignore node_modules/
gitignore *.log
```

## 📚 Usage

### Basic Commands

```bash
# Show help
gitignore
gitignore -h
gitignore --help

# Show version
gitignore -v
gitignore --version
```

### Initialize .gitignore

```bash
# Create empty or use auto.gitignore (if exists)
gitignore init

# Create with specific languages
gitignore init python node
gitignore -i rust c cpp
gitignore init python node vscode  # Mix languages and editors
```

### Sync from GitHub

Download official templates directly from [GitHub's gitignore repository](https://github.com/github/gitignore):

```bash
# Sync single language
gitignore sync python

# Sync multiple languages
gitignore sync python node rust go

# With verbose output
gitignore --verbose sync python
```

### Auto-Detection

Let `gitignore` scan your project and suggest appropriate templates:

```bash
# Auto-detect and create .gitignore
gitignore auto

# Preview what would be detected (dry run)
gitignore --dry-run auto
```

**Detects:**
- `package.json` → Node.js
- `requirements.txt`, `setup.py` → Python
- `Cargo.toml` → Rust
- `go.mod` → Go
- `pom.xml`, `build.gradle` → Java
- And more...

### Interactive Mode

Choose templates from a visual menu:

```bash
gitignore interactive
```

### Add Patterns

Add files or path patterns directly to `.gitignore`:

```bash
# Add patterns
gitignore node_modules/
gitignore *.log
gitignore .env
gitignore dist/

# Force pattern mode when ambiguous
gitignore --add rust    # Adds 'rust' as pattern, not template
```

### Append/Update Mode

```bash
# Append templates to existing .gitignore
gitignore append python

# Smart update (removes duplicates)
gitignore update node vscode
```

### List Templates

```bash
# List all available templates
gitignore list

# List only custom templates
gitignore list --local

# List only built-in templates
gitignore list --builtin

# Filter by keyword
gitignore list web
gitignore list python
```

### Show Template Content

```bash
# Preview template before using
gitignore show python
gitignore cat rust
```

### Global Gitignore

Manage system-wide ignore patterns:

```bash
# Create global .gitignore
gitignore global init

# Add templates to global .gitignore
gitignore global add macos vscode

# Configure git to use it
git config --global core.excludesfile ~/.gitignore_global
```

### Backup & Restore

```bash
# Create backup of current .gitignore
gitignore backup

# List available backups
gitignore backups
gitignore history

# Restore from backup
gitignore restore
gitignore restore gitignore_20260109_143022.bak
```

### Cache Management

```bash
# Clear template cache
gitignore cache clear
```

### Dry Run

Preview what would happen without making changes:

```bash
gitignore --dry-run init python node
gitignore --dry-run auto
gitignore --dry-run sync rust
```

## 🎨 Customization

### Custom Templates

Create your own templates in `~/.config/gitignore/templates/`:

```bash
# Create config directory
mkdir -p ~/.config/gitignore/templates

# Add custom template
cat > ~/.config/gitignore/templates/myproject.gitignore << EOF
# My custom project template
.myconfig
temp/
*.tmp
secrets/
EOF

# Use it
gitignore init myproject
```

### Auto Template

Create an `auto.gitignore` template that will be used when running `gitignore init` without arguments:

```bash
cat > ~/.config/gitignore/templates/auto.gitignore << EOF
# Auto-generated template
.DS_Store
*.log
*.tmp
.env
node_modules/
__pycache__/
EOF

# Now 'gitignore init' will use this template
gitignore init
```

### Configuration File

Create `~/.config/gitignore/config.conf`:

```conf
# gitignore configuration file

auto_backup=true
cache_enabled=true
cache_duration=86400
verbose=false
use_color=true
```

**Options:**
- `auto_backup` - Automatically backup .gitignore before changes
- `cache_enabled` - Enable template caching
- `cache_duration` - Cache duration in seconds (default: 86400 = 24h)
- `verbose` - Show detailed output
- `use_color` - Enable colored terminal output

## 🗂️ Directory Structure

```
$HOME/.config/gitignore/
├── config.conf                # Configuration file
├── templates/                 # Custom templates
│   ├── auto.gitignore        # Auto template
│   ├── python.gitignore      # Custom Python template
│   ├── myproject.gitignore   # Your custom templates
│   └── ...
├── cache/                     # Downloaded template cache
│   ├── python.cache
│   └── ...
└── backups/                   # .gitignore backups
    ├── gitignore_20260109_143022.bak
    └── ...
```

## 📖 Examples

### Example 1: New Python Project

```bash
cd my-python-project
gitignore init python
# Creates .gitignore with Python patterns
```

### Example 2: Web Development Project

```bash
cd my-web-app
gitignore init node vscode
# Creates .gitignore with Node.js and VS Code patterns
```

### Example 3: Auto-detect Multi-language Project

```bash
cd my-project
gitignore auto
# Output: Detected: python, node, vscode, macos
# Creates .gitignore with all detected templates
```

### Example 4: Update Existing .gitignore

```bash
# Already have .gitignore, want to add Rust patterns
gitignore append rust

# Smart merge (removes duplicates)
gitignore update rust vscode
```

### Example 5: Interactive Selection

```bash
gitignore interactive
# Shows menu:
#   1) python
#   2) node
#   3) rust
#   ...
# Enter: 1 3 5
# Creates .gitignore with selected templates
```

### Example 6: Backup Before Major Changes

```bash
gitignore backup
gitignore sync python node rust
# If something goes wrong:
gitignore restore
```

## 🎨 Built-in Templates

The tool comes with built-in templates for:

- **Languages**: Python, Node.js, Rust, C, C++, Java, Go
- **Editors**: VS Code, Vim, IntelliJ
- **Operating Systems**: macOS, Linux, Windows

More templates available via GitHub sync!

## ⚙️ Advanced Usage

### Combine Multiple Features

```bash
# Auto-detect, verbose, dry-run
gitignore --verbose --dry-run auto

# Sync with backup
gitignore backup && gitignore sync python node

# Interactive with custom templates
gitignore interactive
# (Your custom templates will appear in the list)
```

### Pattern Recognition

The tool intelligently recognizes patterns vs template names:

```bash
gitignore node           # Template: Node.js
gitignore node_modules/  # Pattern: node_modules/
gitignore *.log          # Pattern: *.log
gitignore python         # Template: Python
gitignore --add python   # Pattern: python (forced)
```

## 🤝 Contributing

Contributions are welcome! Feel free to:

- Report bugs
- Suggest new features
- Submit pull requests
- Improve documentation

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details

## 👨‍💻 Author

Created with ❤️ for developers who love clean repositories

## 🔗 Links

- [GitHub Repository](https://github.com/mahbubhs/gitignore)
- [Issue Tracker](https://github.com/mahbubhs/gitignore/issues)
- [Official gitignore Templates](https://github.com/github/gitignore)

## 📝 Changelog

### v2.0.0 (2026-01-09)
- ✨ Add patterns directly to .gitignore
- ✨ Interactive template selection mode
- ✨ Auto-detection of project type
- ✨ Append/update modes for existing .gitignore
- ✨ Global gitignore support
- ✨ Backup and restore system
- ✨ Template caching with 24-hour TTL
- ✨ Configuration file support
- ✨ Progress bars for long operations
- ✨ Colored terminal output
- ✨ Dry-run mode
- ✨ Verbose and quiet modes
- ✨ Template listing and preview
- 🐛 Improved error handling
- 🐛 Better duplicate detection
- 📝 Comprehensive documentation

### v1.0.0
- Initial release
- Basic init, sync functionality
- Custom template support