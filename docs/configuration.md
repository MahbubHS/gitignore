# Configuration Guide

Comprehensive guide to customizing gitignore behavior through configuration files, environment variables, and advanced settings.

## 📋 Table of Contents

- [Configuration Overview](#-configuration-overview)
- [Configuration Files](#-configuration-files)
- [Core Settings](#-core-settings)
- [Advanced Configuration](#-advanced-configuration)
- [Environment Variables](#-environment-variables)
- [Directory Structure](#-directory-structure)
- [Configuration Examples](#-configuration-examples)
- [Troubleshooting](#-troubleshooting)

## 🎯 Configuration Overview

### Configuration Architecture

Gitignore uses a hierarchical configuration system with multiple override levels:

```
┌─────────────────────────────────────┐
│      Command-line Flags             │  Highest Priority
│   --verbose, --quiet, --dry-run     │
├─────────────────────────────────────┤
│      Environment Variables          │
│   GITIGNORE_VERBOSE, NO_COLOR       │
├─────────────────────────────────────┤
│      User Configuration File        │
│   ~/.config/gitignore/config.conf   │
├─────────────────────────────────────┤
│      Built-in Defaults              │  Lowest Priority
│   Compiled default values           │
└─────────────────────────────────────┘
```

### Configuration Methods

| Method            | Scope          | Persistence   | Override           |
| ----------------- | -------------- | ------------- | ------------------ |
| **Command Flags** | Single command | None          | All other settings |
| **Environment**   | Session        | Current shell | File config        |
| **Config File**   | User           | Permanent     | Defaults           |
| **Built-in**      | System         | N/A           | N/A                |

## 📁 Configuration Files

### Primary Configuration File

**Location:** `~/.config/gitignore/config.conf`

**Format:** INI-style key=value pairs with optional sections

**File Structure:**

```ini
# gitignore Configuration File
# Lines starting with # are comments
# Format: key=value (no spaces around =)

[general]
verbose = false
quiet = false
use_color = true

[backup]
auto_backup = true
backup_dir = ~/.config/gitignore/backups

[cache]
enabled = true
duration = 86400
cache_dir = ~/.config/gitignore/cache

[templates]
default = python,vscode,linux
custom_dir = ~/.config/gitignore/templates
```

### Configuration File Creation

#### Automated Setup

```bash
# Create configuration directory
mkdir -p ~/.config/gitignore

# Generate default configuration
cat > ~/.config/gitignore/config.conf << 'EOF'
# gitignore Configuration
# Generated on $(date)

[general]
verbose = false
quiet = false
use_color = true

[backup]
auto_backup = true

[cache]
enabled = true
duration = 86400

[templates]
default = python,vscode
EOF
```

#### Interactive Configuration

```bash
# Use gitignore to help create config
gitignore config init  # (if implemented)

# Or manually create with validation
vim ~/.config/gitignore/config.conf
```

### Configuration Validation

**Automatic Validation:**

- Unknown keys generate warnings
- Invalid values fall back to defaults
- Malformed files use defaults entirely

**Manual Validation:**

```bash
# Test configuration loading
gitignore --verbose list 2>&1 | grep -i config

# Validate syntax
python3 -c "
import configparser
config = configparser.ConfigParser()
config.read('~/.config/gitignore/config.conf')
print('Configuration syntax: OK')
"
```

## ⚙️ Core Settings

### Output Control

#### `verbose` (boolean)

**Description:** Enable detailed operation logging

**Values:** `true`, `false` (default: `false`)

**Configuration:**

```ini
verbose = true
```

**Command Override:** `--verbose`, `-v`

**Effects:**

- Shows configuration loading details
- Displays template processing steps
- Reports cache hits/misses
- Shows file operation progress

**Example Output:**

```bash
$ gitignore --verbose init python
[CONFIG] Loading configuration from /home/user/.config/gitignore/config.conf
[TEMPLATE] Found built-in template: python (148 patterns)
[BACKUP] Created backup: .gitignore.backup.2024-01-15_14-30-22
[MERGE] Successfully merged 148 patterns
[SUCCESS] Operation completed
```

#### `quiet` (boolean)

**Description:** Suppress non-error output

**Values:** `true`, `false` (default: `false`)

**Configuration:**

```ini
quiet = true
```

**Command Override:** `--quiet`, `-q`

**Use Cases:**

- Scripting and automation
- CI/CD pipelines
- Non-interactive environments

#### `use_color` (boolean)

**Description:** Enable ANSI color codes in output

**Values:** `true` (default), `false`

**Configuration:**

```ini
use_color = false
```

**Color Scheme:**

- 🔴 **Red:** Errors and failures
- 🟢 **Green:** Success confirmations
- 🟡 **Yellow:** Warnings
- 🔵 **Blue:** Information messages
- 🟣 **Magenta:** Progress indicators
- ⚪ **White:** Normal text

**Environment Override:** `NO_COLOR=1` (disables colors)

### Backup Management

#### `auto_backup` (boolean)

**Description:** Automatically create backups before modifying `.gitignore`

**Values:** `true` (default), `false`

**Configuration:**

```ini
auto_backup = true
```

**Backup Details:**

- **Location:** `~/.config/gitignore/backups/` (configurable)
- **Naming:** `backup_YYYY-MM-DD_HH-MM-SS`
- **Format:** Exact copy of original file
- **Retention:** Manual cleanup required

#### `backup_dir` (string)

**Description:** Custom backup directory path

**Default:** `~/.config/gitignore/backups`

**Configuration:**

```ini
backup_dir = /mnt/backup/gitignore
```

**Requirements:**

- Directory must exist and be writable
- Relative paths resolved from config file location

### Cache Configuration

#### `cache.enabled` (boolean)

**Description:** Enable caching of downloaded GitHub templates

**Values:** `true` (default), `false`

**Configuration:**

```ini
[cache]
enabled = true
```

**Benefits:**

- Faster subsequent sync operations
- Reduced network usage
- Offline capability for cached templates

#### `cache.duration` (integer)

**Description:** Cache lifetime in seconds

**Default:** `86400` (24 hours)

**Configuration:**

```ini
[cache]
duration = 604800  # 1 week
```

**Common Values:**

- `3600` - 1 hour
- `86400` - 24 hours (default)
- `604800` - 1 week
- `2592000` - 30 days

#### `cache.dir` (string)

**Description:** Cache storage directory

**Default:** `~/.config/gitignore/cache`

**Configuration:**

```ini
[cache]
dir = /tmp/gitignore-cache
```

### Template Settings

#### `templates.default` (string list)

**Description:** Default templates to apply when none specified

**Format:** Comma-separated list or multiple lines

**Configuration:**

```ini
# Single line
templates.default = python,vscode,linux

# Or multiple entries
templates.default = python
templates.default = vscode
templates.default = linux
```

**Use Cases:**

- Personal project preferences
- Team standards
- Common technology stacks

#### `templates.custom_dir` (string)

**Description:** Directory for custom template storage

**Default:** `~/.config/gitignore/templates`

**Configuration:**

```ini
templates.custom_dir = ~/my-templates
```

**Template Priority:**

1. Custom templates (highest priority)
2. Built-in templates
3. GitHub sync templates (if available)

## 🔧 Advanced Configuration

### Network Settings

#### `network.timeout` (integer)

**Description:** Network request timeout in seconds

**Default:** `30`

**Configuration:**

```ini
[network]
timeout = 60
```

#### `network.retries` (integer)

**Description:** Number of retry attempts for failed requests

**Default:** `3`

**Configuration:**

```ini
[network]
retries = 5
```

#### `network.user_agent` (string)

**Description:** HTTP User-Agent header for requests

**Default:** `gitignore/2.0.0`

**Configuration:**

```ini
[network]
user_agent = MyApp/1.0 (gitignore fork)
```

### Performance Tuning

#### `performance.max_concurrent` (integer)

**Description:** Maximum concurrent network requests

**Default:** `5`

**Configuration:**

```ini
[performance]
max_concurrent = 10
```

#### `performance.buffer_size` (integer)

**Description:** Memory buffer size for file operations (KB)

**Default:** `64`

**Configuration:**

```ini
[performance]
buffer_size = 128
```

### Development Settings

#### `development.debug` (boolean)

**Description:** Enable debug logging and assertions

**Default:** `false`

**Configuration:**

```ini
[development]
debug = true
```

**Effects:**

- Additional debug output
- Runtime assertions enabled
- Memory leak detection (if compiled with debug)

#### `development.profile` (boolean)

**Description:** Enable performance profiling

**Default:** `false`

**Configuration:**

```ini
[development]
profile = true
```

### Security Settings

#### `security.verify_ssl` (boolean)

**Description:** Verify SSL certificates for HTTPS requests

**Default:** `true`

**Configuration:**

```ini
[security]
verify_ssl = false  # Not recommended for production
```

#### `security.allowed_hosts` (string list)

**Description:** Restrict network requests to specific hosts

**Default:** Unrestricted

**Configuration:**

```ini
[security]
allowed_hosts = github.com,raw.githubusercontent.com
```

## 🌐 Environment Variables

### Core Environment Variables

#### `GITIGNORE_CONFIG`

**Description:** Override default configuration file path

**Example:**

```bash
export GITIGNORE_CONFIG=/etc/gitignore.conf
gitignore list
```

#### `GITIGNORE_VERBOSE`

**Description:** Force verbose output (equivalent to `--verbose`)

**Example:**

```bash
export GITIGNORE_VERBOSE=1
gitignore init python
```

#### `NO_COLOR`

**Description:** Disable colored output (respects no-color.org standard)

**Example:**

```bash
export NO_COLOR=1
gitignore list
```

### Path and Directory Variables

#### `XDG_CONFIG_HOME`

**Description:** Override XDG base directory for configuration

**Default:** `~/.config`

**Example:**

```bash
export XDG_CONFIG_HOME=/mnt/config
# Configuration loaded from /mnt/config/gitignore/config.conf
```

#### `XDG_CACHE_HOME`

**Description:** Override XDG base directory for cache

**Default:** `~/.cache`

**Example:**

```bash
export XDG_CACHE_HOME=/tmp/cache
# Cache stored in /tmp/cache/gitignore/
```

#### `TMPDIR`

**Description:** Temporary directory for operations

**Default:** System temporary directory

**Example:**

```bash
export TMPDIR=/dev/shm
gitignore sync python  # Uses shared memory for temp files
```

### Build and Installation Variables

#### `PREFIX`

**Description:** Installation prefix for custom builds

**Example:**

```bash
export PREFIX=~/.local
make install
# Installs to ~/.local/bin/gitignore
```

#### `CC` / `CXX`

**Description:** Override default compiler

**Example:**

```bash
export CC=clang
export CXX=clang++
make
```

## 📂 Directory Structure

### Complete Directory Layout

```
~/.config/gitignore/
├── config.conf              # Main configuration file
├── templates/               # Custom templates directory
│   ├── myproject.gitignore
│   └── corporate.gitignore
├── cache/                   # Downloaded template cache
│   ├── python.gitignore
│   ├── node.gitignore
│   └── timestamps.json
├── backups/                 # Automatic backups
│   ├── backup_2024-01-15_14-30-22
│   └── backup_2024-01-10_09-15-33
└── logs/                    # Debug logs (if enabled)
    └── gitignore.log
```

### Directory Permissions

**Recommended Permissions:**

```bash
# Configuration directory
chmod 755 ~/.config/gitignore

# Configuration file
chmod 644 ~/.config/gitignore/config.conf

# Templates directory
chmod 755 ~/.config/gitignore/templates

# Template files
chmod 644 ~/.config/gitignore/templates/*.gitignore

# Cache directory
chmod 755 ~/.config/gitignore/cache

# Backup directory
chmod 755 ~/.config/gitignore/backups
```

### Custom Directory Configuration

**All directories can be customized:**

```ini
[directories]
config = ~/.config/gitignore
cache = ~/.cache/gitignore
backups = ~/.backups/gitignore
templates = ~/my-templates
logs = /var/log/gitignore
```

## 📋 Configuration Examples

### Development Environment

```ini
# Developer Configuration
[general]
verbose = true
use_color = true
quiet = false

[backup]
auto_backup = true

[cache]
enabled = true
duration = 3600  # 1 hour for faster development

[development]
debug = true

[templates]
default = python,vscode,linux
```

### Production Server

```ini
# Production Configuration
[general]
verbose = false
use_color = false
quiet = true

[backup]
auto_backup = true

[cache]
enabled = true
duration = 604800  # 1 week

[network]
timeout = 60
retries = 5

[security]
verify_ssl = true
```

### CI/CD Pipeline

```ini
# CI/CD Configuration
[general]
verbose = false
use_color = false
quiet = true

[backup]
auto_backup = false

[cache]
enabled = true
duration = 3600  # 1 hour

[performance]
max_concurrent = 10

[development]
debug = false
profile = false
```

### Corporate Environment

```ini
# Corporate Configuration
[general]
verbose = false
use_color = true

[backup]
auto_backup = true

[cache]
enabled = true
duration = 86400

[templates]
default = python,vscode,linux,corporate
custom_dir = /etc/gitignore/templates

[security]
verify_ssl = true
allowed_hosts = github.com,raw.githubusercontent.com,corporate.gitlab.com
```

### Minimal Configuration

```ini
# Minimal Configuration
use_color = true
auto_backup = true
cache_enabled = true
```

### Power User Configuration

```ini
# Power User Configuration
[general]
verbose = true
use_color = true

[backup]
auto_backup = true

[cache]
enabled = true
duration = 2592000  # 30 days

[network]
timeout = 120
retries = 10

[performance]
max_concurrent = 20
buffer_size = 256

[templates]
default = python,node,rust,go,java,vscode,intellij,linux,macos,windows,docker,kubernetes,terraform

[development]
debug = true
profile = true
```

## 🆘 Troubleshooting

### Configuration Loading Issues

#### Configuration Not Found

**Symptoms:**

```
[WARNING] Configuration file not found, using defaults
```

**Solutions:**

```bash
# Check file existence
ls -la ~/.config/gitignore/config.conf

# Create directory structure
mkdir -p ~/.config/gitignore

# Check permissions
touch ~/.config/gitignore/config.conf
chmod 644 ~/.config/gitignore/config.conf
```

#### Invalid Configuration Syntax

**Symptoms:**

```
[ERROR] Invalid configuration value for 'verbose': 'yes'
[WARNING] Using default value: false
```

**Solutions:**

```bash
# Validate boolean values
grep -E "verbose.*=" ~/.config/gitignore/config.conf
# Should be: verbose = true  (not verbose=yes)

# Check for syntax errors
cat ~/.config/gitignore/config.conf | grep -n "="
```

#### Permission Denied

**Symptoms:**

```
[ERROR] Cannot read configuration file: Permission denied
```

**Solutions:**

```bash
# Fix permissions
chmod 644 ~/.config/gitignore/config.conf

# Check parent directory permissions
ls -ld ~/.config/gitignore/
chmod 755 ~/.config/gitignore/
```

### Cache Issues

#### Cache Not Working

**Symptoms:**

- Templates always download fresh
- Cache directory empty

**Solutions:**

```bash
# Check cache directory
ls -la ~/.config/gitignore/cache/

# Create cache directory
mkdir -p ~/.config/gitignore/cache

# Check configuration
grep -A5 "\[cache\]" ~/.config/gitignore/config.conf
```

#### Stale Cache

**Symptoms:**

- Old templates still used after updates

**Solutions:**

```bash
# Clear cache manually
rm -rf ~/.config/gitignore/cache/*

# Or use command
gitignore cache clear  # (if implemented)

# Check cache duration
grep "cache_duration" ~/.config/gitignore/config.conf
```

### Template Issues

#### Custom Templates Not Loading

**Symptoms:**

- Custom templates not appearing in `list`
- Built-in templates used instead

**Solutions:**

```bash
# Check custom template directory
ls -la ~/.config/gitignore/templates/

# Verify template files
file ~/.config/gitignore/templates/*.gitignore

# Check configuration
grep "custom_dir" ~/.config/gitignore/config.conf
```

### Environment Variable Issues

#### Variables Not Taking Effect

**Symptoms:**

- Environment variables ignored

**Solutions:**

```bash
# Export before running
export GITIGNORE_VERBOSE=1
gitignore list

# Check variable is set
echo $GITIGNORE_VERBOSE

# Use inline
GITIGNORE_VERBOSE=1 gitignore list
```

### Performance Issues

#### Slow Configuration Loading

**Symptoms:**

- Long startup times

**Solutions:**

```bash
# Check configuration file size
wc -l ~/.config/gitignore/config.conf

# Simplify configuration
# Remove unnecessary settings

# Check file permissions
ls -la ~/.config/gitignore/config.conf
```

## 📚 Related Documentation

- **[Installation Guide](installation.md)** - Initial setup and system requirements
- **[Usage Guide](usage.md)** - Command-line usage and examples
- **[API Reference](api-reference.md)** - Developer interface documentation
- **[Troubleshooting](troubleshooting.md)** - Common issues and solutions
