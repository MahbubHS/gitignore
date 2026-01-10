# Configuration Guide

Customize gitignore behavior with configuration files and environment variables.

## 📁 Configuration Files

### Main Configuration File

**Location:** `~/.config/gitignore/config.conf`

**Format:** Simple key=value pairs

```conf
# gitignore configuration file

# Output settings
verbose=false
quiet=false
use_color=true

# Backup settings
auto_backup=true

# Cache settings
cache_enabled=true
cache_duration=86400

# Default templates (one per line)
default_templates=python
default_templates=vscode
```

### Creating Configuration File

```bash
# Create config directory
mkdir -p ~/.config/gitignore

# Create config file
cat > ~/.config/gitignore/config.conf << 'EOF'
# My gitignore configuration

# Enable colors and verbosity
use_color=true
verbose=false
quiet=false

# Always backup before changes
auto_backup=true

# Cache GitHub templates for 24 hours
cache_enabled=true
cache_duration=86400

# Default templates for new projects
default_templates=python
default_templates=vscode
default_templates=linux
EOF
```

## ⚙️ Configuration Options

### Output Settings

#### `verbose`

Control detailed output.

```conf
verbose=false  # Default: minimal output
verbose=true   # Show detailed operation info
```

**Command-line override:** `--verbose` / `-V`

**Example output with verbose:**

```bash
$ gitignore --verbose init python
✓ Loading configuration from /home/user/.config/gitignore/config.conf
✓ Found built-in template: python
✓ Creating .gitignore
✓ Added 15 patterns from python template
✓ Backup created: backup_2024-01-10_14-30-00
✓ Operation completed successfully
```

#### `quiet`

Suppress non-essential output.

```conf
quiet=false  # Default: show progress
quiet=true   # Only show errors
```

**Command-line override:** `--quiet` / `-q`

**Example with quiet:**

```bash
$ gitignore --quiet init python
# No output unless error occurs
```

#### `use_color`

Enable colored terminal output.

```conf
use_color=true   # Default: use colors
use_color=false  # Plain text output
```

**Supported colors:**

- 🔴 Red: Errors
- 🟢 Green: Success
- 🟡 Yellow: Warnings
- 🔵 Blue: Info
- 🟣 Magenta: Progress

### Backup Settings

#### `auto_backup`

Automatically backup `.gitignore` before modifications.

```conf
auto_backup=true   # Default: create backups
auto_backup=false  # Don't create backups
```

**Backup location:** `~/.config/gitignore/backups/`

**Backup naming:** `backup_YYYY-MM-DD_HH-MM-SS`

### Cache Settings

#### `cache_enabled`

Enable caching of downloaded GitHub templates.

```conf
cache_enabled=true   # Default: use cache
cache_enabled=false  # Always download fresh
```

#### `cache_duration`

How long to cache GitHub templates (in seconds).

```conf
cache_duration=86400  # Default: 24 hours
cache_duration=3600   # 1 hour
cache_duration=604800 # 1 week
```

**Cache location:** `~/.config/gitignore/cache/`

### Default Templates

#### `default_templates`

Templates to apply by default (can specify multiple).

```conf
# Single template
default_templates=python

# Multiple templates
default_templates=python
default_templates=vscode
default_templates=linux
```

**Usage:** These templates are applied when no specific templates are requested.

## 🌐 Environment Variables

### PREFIX

Installation prefix for custom installations.

```bash
# Install to custom location
export PREFIX=~/.local
make install

# Or specify during install
make PREFIX=/opt/gitignore install
```

### PATH

Ensure gitignore is in executable path.

```bash
# Add to PATH
export PATH="$HOME/.local/bin:$PATH"

# Verify
which gitignore
```

## 📂 Directory Structure

### Configuration Directories

```
~/.config/gitignore/
├── config.conf           # Main configuration
├── templates/           # Custom templates
│   └── mytemplate.gitignore
├── cache/               # Downloaded templates
│   ├── python.gitignore
│   └── node.gitignore
└── backups/             # .gitignore backups
    ├── backup_2024-01-10_14-30-00
    └── backup_2024-01-10_15-45-12
```

### Custom Templates Directory

**Location:** `~/.config/gitignore/templates/`

Create custom templates that override built-ins:

```bash
mkdir -p ~/.config/gitignore/templates

# Create custom Python template
cat > ~/.config/gitignore/templates/python.gitignore << 'EOF'
# My Custom Python Template
*.pyc
__pycache__/
*.pyo
.env
venv/
dist/
build/
EOF

# Use it
gitignore init python  # Uses custom template
```

**Priority order:**

1. Custom templates (`~/.config/gitignore/templates/`)
2. Built-in templates (compiled in binary)

## 🔧 Advanced Configuration

### Global Gitignore Setup

Configure Git to use global gitignore:

```bash
# Create global gitignore
gitignore global init

# Configure Git
git config --global core.excludesfile ~/.gitignore_global
```

### Project-Specific Configuration

Create `.gitignore.local` for project-specific patterns:

```bash
# Create local config
echo "local-config.txt" >> .gitignore.local

# Include in main .gitignore
echo ".gitignore.local" >> .gitignore
```

### CI/CD Configuration

For automated environments:

```bash
# Disable colors in CI
export NO_COLOR=1

# Or set in config
echo "use_color=false" >> ~/.config/gitignore/config.conf
```

## 🔍 Configuration Validation

### Check Current Configuration

```bash
# View loaded configuration
gitignore --verbose --help

# Check config file syntax
cat ~/.config/gitignore/config.conf
```

### Reset to Defaults

```bash
# Remove config file
rm ~/.config/gitignore/config.conf

# gitignore will use built-in defaults
```

### Debug Configuration Loading

```bash
# Verbose mode shows config loading
gitignore --verbose list

# Check file permissions
ls -la ~/.config/gitignore/config.conf
```

## 📋 Configuration Examples

### Developer Setup

```conf
# Developer configuration
verbose=true
use_color=true
auto_backup=true
cache_enabled=true
cache_duration=86400

default_templates=python
default_templates=vscode
default_templates=linux
```

### CI/CD Setup

```conf
# CI/CD configuration
verbose=false
quiet=true
use_color=false
auto_backup=false
cache_enabled=true
cache_duration=3600
```

### Minimal Setup

```conf
# Minimal configuration
use_color=true
auto_backup=true
cache_enabled=true
```

### Power User Setup

```conf
# Power user configuration
verbose=true
use_color=true
auto_backup=true
cache_enabled=true
cache_duration=604800

default_templates=python
default_templates=node
default_templates=rust
default_templates=go
default_templates=vscode
default_templates=intellij
default_templates=linux
default_templates=macos
default_templates=windows
```

## 🆘 Troubleshooting

### Configuration Not Loading

**Symptoms:**

- Settings not applied
- Verbose mode shows "Loading default configuration"

**Solutions:**

```bash
# Check file exists
ls -la ~/.config/gitignore/config.conf

# Check permissions
chmod 644 ~/.config/gitignore/config.conf

# Check syntax (no spaces around =)
grep -n "=" ~/.config/gitignore/config.conf
```

### Invalid Configuration Values

**Symptoms:**

- Warnings about invalid values
- Settings revert to defaults

**Solutions:**

```bash
# Check boolean values (true/false)
grep -E "(true|false)" ~/.config/gitignore/config.conf

# Check numeric values
grep -E "[0-9]+" ~/.config/gitignore/config.conf
```

### Cache Issues

**Symptoms:**

- Old templates still used
- Cache not updating

**Solutions:**

```bash
# Clear cache
gitignore cache clear

# Check cache directory
ls -la ~/.config/gitignore/cache/

# Disable cache temporarily
echo "cache_enabled=false" >> ~/.config/gitignore/config.conf
```

## 📚 Related Topics

- [Usage Guide](usage.md) - Command-line options
- [Installation](installation.md) - Setting up gitignore
- [Troubleshooting](troubleshooting.md) - Common issues
