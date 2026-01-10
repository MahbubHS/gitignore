# Usage Guide

Complete reference for all gitignore commands and options.

## 📖 Command Syntax

```bash
gitignore [command] [options] [arguments]
```

## 🎯 Core Commands

### Adding Patterns

Add individual patterns to `.gitignore`:

```bash
# Single pattern
gitignore node_modules/

# Multiple patterns (space-separated)
gitignore node_modules/ *.log .env dist/

# Patterns with wildcards
gitignore *.tmp *.swp *~ *.bak
```

**Smart Features:**
- ✅ Automatically creates `.gitignore` if it doesn't exist
- ✅ Checks for duplicates before adding
- ✅ Preserves existing patterns
- ✅ Works with relative and absolute paths

### Initializing with Templates

Create `.gitignore` from built-in templates:

```bash
# Single template
gitignore init python

# Multiple templates
gitignore init python node vscode linux

# All templates for a project type
gitignore init rust cpp go
```

**Features:**
- ✅ Merges multiple templates intelligently
- ✅ Removes duplicate patterns automatically
- ✅ Preserves existing `.gitignore` content
- ✅ No external template files needed

### Auto-Detection

Automatically detect project type and apply appropriate templates:

```bash
gitignore auto
```

**Detection Logic:**
- `package.json` → Node.js
- `requirements.txt` / `setup.py` → Python
- `Cargo.toml` → Rust
- `go.mod` → Go
- `pom.xml` / `build.gradle` → Java
- And more...

## 🎨 Advanced Commands

### Interactive Mode

User-friendly template selection:

```bash
# Short form
gitignore -t

# Long form
gitignore interactive
```

Shows numbered list of templates with descriptions.

### Template Management

```bash
# List all available templates
gitignore list

# Filter templates
gitignore list python

# Show only built-in templates
gitignore list --builtin

# Show only custom templates
gitignore list --local

# Preview template contents
gitignore show python
gitignore cat rust
```

### GitHub Sync

Download official templates from GitHub's repository:

```bash
# Single language
gitignore sync python

# Multiple languages
gitignore sync rust go java
```

**Features:**
- ✅ Downloads from github.com/github/gitignore
- ✅ Merges with existing `.gitignore`
- ✅ 24-hour caching for performance
- ✅ Automatic duplicate removal

### Global Gitignore

Manage global `.gitignore` file:

```bash
# Create global gitignore
gitignore global init

# Add templates to global gitignore
gitignore global add macos vscode linux
```

**Setup Git to use global gitignore:**
```bash
git config --global core.excludesfile ~/.gitignore_global
```

### Backup & Restore

```bash
# Create backup before changes
gitignore backup

# List available backups
gitignore backups
gitignore history

# Restore from backup
gitignore restore
gitignore restore backup_2024-01-10_14-30-00
```

### Append vs Update

```bash
# Append (simple concatenation)
gitignore append python

# Update (smart merging with deduplication)
gitignore update node
```

### Cache Management

```bash
# Clear downloaded template cache
gitignore cache clear
```

## ⚙️ Command-Line Options

### Global Flags

```bash
# Show help
gitignore --help
gitignore -h

# Show version
gitignore --version
gitignore -v

# Dry run (preview changes)
gitignore --dry-run init python

# Verbose output
gitignore --verbose init python

# Quiet mode (minimal output)
gitignore --quiet init python
```

### Special Flags

```bash
# Force add command names as patterns
gitignore --add init sync list

# Alternative short forms
gitignore -i python    # Same as init python
gitignore -s python    # Same as sync python
```

## 🔍 Pattern vs Command Detection

Gitignore intelligently distinguishes between commands and patterns:

### Automatically Treated as Patterns:
```bash
gitignore node_modules/     # Contains /
gitignore *.log            # Contains *
gitignore .env             # Contains .
gitignore myfile.txt       # File exists
```

### Require `--add` Flag (Command Conflicts):
```bash
# These are commands, not patterns
gitignore init             # Command
gitignore sync             # Command
gitignore list             # Command

# To add as patterns, use --add
gitignore --add init      # Adds 'init' as pattern
gitignore --add sync list # Adds both as patterns
```

**Error for ambiguous arguments:**
```bash
$ gitignore sync
✗ Error: Ambiguous argument - did you mean a command or pattern?

If you want to use the 'sync' command, check: gitignore --help
If you want to add 'sync' as a pattern, use: gitignore --add sync
```

## 📋 Examples

### Basic Workflow

```bash
# Start new project
mkdir my-project && cd my-project
git init

# Initialize with templates
gitignore init python vscode

# Add custom patterns
gitignore .env secrets/ *.log

# Backup current state
gitignore backup

# Update with more templates
gitignore init linux

# Check what changed
git diff .gitignore
```

### Advanced Usage

```bash
# Preview changes without applying
gitignore --dry-run init rust go

# Use interactive mode
gitignore -t

# Sync from GitHub
gitignore sync python node

# Manage global gitignore
gitignore global init
gitignore global add macos windows

# Work with backups
gitignore backups
gitignore restore backup_2024-01-10_12-00-00
```

### Multi-Language Projects

```bash
# Full-stack application
gitignore init python node java

# Add all relevant tools
gitignore init vscode intellij docker

# Add OS-specific patterns
gitignore init linux macos windows

# Custom patterns
gitignore dist/ build/ target/ *.tmp
```

## 🎯 Best Practices

### 1. Use Auto-Detection First
```bash
gitignore auto  # Let gitignore detect your project type
```

### 2. Layer Templates
```bash
# Start with language
gitignore init python

# Add editor
gitignore init vscode

# Add OS
gitignore init linux

# Add custom patterns
gitignore .env secrets/
```

### 3. Regular Backups
```bash
# Before major changes
gitignore backup

# List backups
gitignore backups
```

### 4. Use Dry Run for Safety
```bash
# Always preview first
gitignore --dry-run init rust
```

### 5. Combine Commands Efficiently
```bash
# Instead of multiple commands
gitignore init python
gitignore init vscode
gitignore init linux

# Use one command
gitignore init python vscode linux
```

## 🆘 Troubleshooting

### Common Issues

**"Pattern already exists"**
- Gitignore automatically skips duplicates
- Use `--verbose` to see what's happening

**"Template not found"**
- Check available templates: `gitignore list`
- Verify spelling

**"Permission denied"**
- Check file permissions on `.gitignore`
- Ensure write access to current directory

**"Network error" on sync**
- Check internet connection
- GitHub might be rate-limiting requests

### Getting Help

```bash
# Show all commands
gitignore --help

# List available templates
gitignore list

# Show template contents
gitignore show python

# Verbose output
gitignore --verbose init python
```

## 📚 Related Topics

- [Template Reference](templates.md) - All available templates
- [Configuration Guide](configuration.md) - Customize behavior
- [Troubleshooting](troubleshooting.md) - Common issues and solutions