# Usage Guide

This comprehensive guide covers all gitignore commands, options, and usage patterns for effective .gitignore file management.

## 📋 Table of Contents

- [Quick Start](#-quick-start)
- [Command Reference](#-command-reference)
- [Global Options](#-global-options)
- [Advanced Usage](#-advanced-usage)
- [Best Practices](#-best-practices)
- [Troubleshooting](#-troubleshooting)

## 🚀 Quick Start

### Basic Usage Patterns

```bash
# Initialize project with templates
gitignore init python vscode linux

# Add custom patterns
gitignore .env secrets/ *.log

# Auto-detect project type
gitignore auto

# Preview changes first
gitignore --dry-run init rust
```

### Common Workflows

| Scenario                | Command                               | Description                 |
| ----------------------- | ------------------------------------- | --------------------------- |
| **New Python Project**  | `gitignore init python vscode`        | Language + editor templates |
| **Node.js Application** | `gitignore init node linux docker`    | Runtime + OS + container    |
| **Full-Stack App**      | `gitignore init python node postgres` | Multiple technologies       |
| **Custom Patterns**     | `gitignore .env dist/ build/`         | Project-specific ignores    |

## 📖 Command Reference

### Core Commands

#### `gitignore init [templates...]`

Initialize or update `.gitignore` with built-in templates.

**Syntax:**

```bash
gitignore init <template> [template...] [options]
```

**Examples:**

```bash
# Single template
gitignore init python

# Multiple templates
gitignore init python vscode linux

# With options
gitignore init rust --verbose --dry-run
```

**Available Templates:**

| Category       | Templates                                          | Description            |
| -------------- | -------------------------------------------------- | ---------------------- |
| **Languages**  | `python`, `node`, `rust`, `go`, `java`, `cpp`, `c` | Programming languages  |
| **Frameworks** | `django`, `flask`, `react`, `vue`, `angular`       | Web frameworks         |
| **Tools**      | `docker`, `kubernetes`, `terraform`                | DevOps tools           |
| **Editors**    | `vscode`, `intellij`, `vim`, `emacs`               | IDE configurations     |
| **OS**         | `linux`, `macos`, `windows`                        | Operating system files |

**Features:**

- ✅ **Smart Merging:** Automatically removes duplicate patterns
- ✅ **Backup Creation:** Saves previous `.gitignore` before changes
- ✅ **Template Validation:** Verifies template existence before processing
- ✅ **Progress Feedback:** Shows which templates are being applied

#### `gitignore sync [templates...]`

Download and apply official templates from GitHub's gitignore repository.

**Syntax:**

```bash
gitignore sync <template> [template...] [options]
```

**Examples:**

```bash
# Sync single template
gitignore sync python

# Sync multiple templates
gitignore sync rust go java

# Force refresh cache
gitignore sync python --no-cache
```

**Network Features:**

- 🔄 **Intelligent Caching:** 24-hour cache for downloaded templates
- 📡 **Offline Fallback:** Uses cached templates when offline
- ⚡ **Parallel Downloads:** Fetches multiple templates concurrently
- 🔒 **Rate Limiting:** Respects GitHub API limits

#### `gitignore auto`

Automatically detect project type and apply appropriate templates.

**Detection Rules:**

| File Pattern                   | Detected Template | Priority |
| ------------------------------ | ----------------- | -------- |
| `package.json`                 | `node`            | High     |
| `requirements.txt`, `setup.py` | `python`          | High     |
| `Cargo.toml`                   | `rust`            | High     |
| `go.mod`                       | `go`              | High     |
| `pom.xml`, `build.gradle`      | `java`            | High     |
| `Dockerfile`                   | `docker`          | Medium   |
| `.vscode/`                     | `vscode`          | Low      |

**Examples:**

```bash
# Auto-detect and apply
gitignore auto

# Preview what would be applied
gitignore auto --dry-run

# Force re-detection
gitignore auto --force
```

#### `gitignore [patterns...]`

Add custom patterns directly to `.gitignore`.

**Syntax:**

```bash
gitignore <pattern> [pattern...] [options]
```

**Examples:**

```bash
# Single pattern
gitignore node_modules/

# Multiple patterns
gitignore *.log .env dist/

# With wildcards
gitignore *.tmp *.swp *~

# Force add command names
gitignore --add init sync list
```

**Pattern Intelligence:**

- 🧠 **Duplicate Detection:** Skips existing patterns
- 📁 **Directory Recognition:** Handles paths with `/`
- 🌟 **Wildcard Support:** Processes `*`, `?`, `[]` patterns
- 🔍 **Command vs Pattern:** Automatically distinguishes between commands and patterns

### Template Management

#### `gitignore list [filter]`

List available templates with descriptions.

**Syntax:**

```bash
gitignore list [filter] [options]
```

**Examples:**

```bash
# List all templates
gitignore list

# Filter by language
gitignore list python

# Show detailed information
gitignore list --verbose

# List only built-in templates
gitignore list --builtin
```

**Output Format:**

```
Available Templates:
├── Languages
│   ├── python     - Python projects
│   ├── node       - Node.js applications
│   └── rust       - Rust projects
├── Tools
│   ├── docker     - Docker containers
│   └── kubernetes - Kubernetes manifests
└── Editors
    ├── vscode     - Visual Studio Code
    └── intellij   - IntelliJ IDEA
```

#### `gitignore show <template>`

Display template contents without applying.

**Syntax:**

```bash
gitignore show <template> [options]
```

**Examples:**

```bash
# Show template contents
gitignore show python

# Show with line numbers
gitignore show python --numbered

# Show multiple templates
gitignore show python node
```

### Interactive Commands

#### `gitignore interactive`

Launch interactive template selection interface.

**Features:**

- 📋 **Numbered Selection:** Choose templates by number
- 🔍 **Search/Filter:** Find templates by typing
- 📄 **Preview:** See template contents before applying
- ↩️ **Multi-Select:** Choose multiple templates at once

**Usage:**

```bash
gitignore interactive
# or
gitignore -t
```

### Global Gitignore Management

#### `gitignore global init`

Create and initialize global `.gitignore` file.

**Setup Process:**

```bash
# Initialize global gitignore
gitignore global init

# Configure Git to use it
git config --global core.excludesfile ~/.gitignore_global
```

#### `gitignore global add [templates...]`

Add templates to global `.gitignore`.

**Examples:**

```bash
# Add OS-specific patterns
gitignore global add macos linux

# Add editor configurations
gitignore global add vscode intellij
```

### Backup & Recovery

#### `gitignore backup`

Create timestamped backup of current `.gitignore`.

**Backup Location:** `.gitignore.backup.YYYY-MM-DD_HH-MM-SS`

#### `gitignore backups`

List all available backups.

**Output:**

```
Available Backups:
├── .gitignore.backup.2024-01-15_14-30-22  (2.1KB)
├── .gitignore.backup.2024-01-10_09-15-33  (1.8KB)
└── .gitignore.backup.2024-01-05_16-45-12  (1.5KB)
```

#### `gitignore restore [backup]`

Restore `.gitignore` from backup.

**Examples:**

```bash
# Restore latest backup
gitignore restore

# Restore specific backup
gitignore restore .gitignore.backup.2024-01-10_09-15-33

# List backups first
gitignore backups
```

### Cache Management

#### `gitignore cache clear`

Clear downloaded template cache.

**Effects:**

- 🗑️ Removes all cached GitHub templates
- 🔄 Forces fresh downloads on next sync
- 💾 Frees disk space

## ⚙️ Global Options

### Output Control

| Flag        | Short | Description     | Example                    |
| ----------- | ----- | --------------- | -------------------------- |
| `--verbose` | `-v`  | Detailed output | `gitignore -v init python` |
| `--quiet`   | `-q`  | Minimal output  | `gitignore -q init python` |
| `--dry-run` | `-n`  | Preview changes | `gitignore -n init python` |

### Behavior Modification

| Flag         | Description              | Example                            |
| ------------ | ------------------------ | ---------------------------------- |
| `--force`    | Overwrite without backup | `gitignore --force init python`    |
| `--no-cache` | Skip cache for sync      | `gitignore sync python --no-cache` |
| `--add`      | Force pattern addition   | `gitignore --add init sync`        |

### Information

| Flag        | Short | Description              |
| ----------- | ----- | ------------------------ |
| `--help`    | `-h`  | Show help information    |
| `--version` | `-V`  | Show version information |

## 🎨 Advanced Usage

### Command Chaining

Combine multiple operations efficiently:

```bash
# Initialize, add patterns, backup
gitignore init python vscode && gitignore .env dist/ && gitignore backup
```

### Template Combinations

**Web Development Stack:**

```bash
gitignore init node react typescript vscode docker
```

**Data Science Project:**

```bash
gitignore init python jupyter vscode linux
```

**System Administration:**

```bash
gitignore init shell docker ansible linux
```

### Custom Workflow Scripts

Create reusable setup scripts:

```bash
#!/bin/bash
# setup-gitignore.sh

# Initialize project
gitignore init python django vscode

# Add custom patterns
gitignore .env media/ staticfiles/

# Create backup
gitignore backup

echo "Gitignore setup complete!"
```

### Integration with Git Hooks

Automate gitignore management:

```bash
# .git/hooks/post-commit
#!/bin/bash
# Auto-backup after commits
gitignore backup > /dev/null
```

## 🧠 Pattern Intelligence

### Automatic Detection Logic

Gitignore uses sophisticated heuristics to distinguish commands from patterns:

**Always Treated as Patterns:**

- Contains path separators: `node_modules/`, `src/main/`
- Contains wildcards: `*.log`, `temp*`, `file?.txt`
- Contains dots: `.env`, `.DS_Store`
- Existing files: `myfile.txt` (if file exists)

**Command Conflicts (Require `--add`):**

- `init`, `sync`, `list`, `show`, `auto`
- `backup`, `restore`, `global`
- Any built-in template name

**Error Handling:**

```bash
$ gitignore sync
✗ Error: Ambiguous argument 'sync'
  → Did you mean the 'sync' command? Use: gitignore sync <template>
  → Or add as pattern? Use: gitignore --add sync
```

## 📊 Performance Optimization

### Caching Strategies

| Operation              | Cache Type | Duration | Purpose                 |
| ---------------------- | ---------- | -------- | ----------------------- |
| **Template Downloads** | File cache | 24 hours | Reduce network requests |
| **Template Metadata**  | Memory     | Session  | Fast lookups            |
| **Configuration**      | Memory     | Session  | Avoid file I/O          |

### Memory Usage

| Component              | Typical Usage | Notes                   |
| ---------------------- | ------------- | ----------------------- |
| **Binary + Templates** | ~2MB          | Embedded templates      |
| **Runtime Memory**     | < 1MB         | Configuration + buffers |
| **Cache Storage**      | Variable      | Downloaded templates    |

## 🛡️ Safety Features

### Automatic Backups

- 📦 **Pre-Modification:** Backup created before any changes
- 🏷️ **Timestamped:** Format: `YYYY-MM-DD_HH-MM-SS`
- 🔄 **Easy Restore:** `gitignore restore` to recover
- 📂 **Retention:** Manual cleanup required

### Dry Run Mode

Preview all changes before applying:

```bash
$ gitignore --dry-run init python
[DRY RUN] Would create .gitignore with:
  - Python.gitignore (148 patterns)
  - Would backup existing .gitignore to .gitignore.backup.2024-01-15_14-30-22
  - No actual changes made
```

### Conflict Resolution

- 🔍 **Duplicate Detection:** Automatic pattern deduplication
- ⚖️ **Merge Intelligence:** Smart conflict resolution
- 📝 **Preserve Comments:** Maintains existing comments
- 🔄 **Non-Destructive:** Never removes user-added patterns

## 🎯 Best Practices

### Project Setup Workflow

```bash
# 1. Initialize repository
git init

# 2. Auto-detect project type
gitignore auto

# 3. Add additional templates
gitignore init vscode linux

# 4. Add custom patterns
gitignore .env secrets/ logs/

# 5. Create backup
gitignore backup
```

### Template Selection Guidelines

| Project Type     | Recommended Templates              | Rationale                         |
| ---------------- | ---------------------------------- | --------------------------------- |
| **Web App**      | `node`, `react`, `vscode`, `linux` | Runtime + framework + editor + OS |
| **API Service**  | `python`, `docker`, `linux`        | Language + container + OS         |
| **CLI Tool**     | `rust`, `linux`, `macos`           | Language + cross-platform         |
| **Data Science** | `python`, `jupyter`, `vscode`      | Language + notebook + editor      |

### Maintenance Practices

- 🔄 **Regular Backups:** Before major changes
- 📋 **Version Control:** Commit `.gitignore` changes
- 🔍 **Review Patterns:** Audit periodically for relevance
- 📚 **Stay Updated:** Use `sync` for latest official templates

### Performance Tips

- ⚡ **Use Cache:** Let caching reduce network requests
- 📦 **Batch Operations:** Combine multiple templates in one command
- 🔍 **Preview First:** Use `--dry-run` for complex operations
- 🧹 **Clean Cache:** Periodically clear old cached templates

## 🆘 Troubleshooting

### Common Issues & Solutions

#### "Template not found" Error

**Symptoms:**

```bash
✗ Error: Template 'xyz' not found
```

**Solutions:**

```bash
# Check available templates
gitignore list

# Search for similar templates
gitignore list | grep -i xyz

# Use sync for official templates
gitignore sync xyz
```

#### Permission Denied

**Symptoms:**

```bash
✗ Error: Permission denied writing .gitignore
```

**Solutions:**

```bash
# Check file permissions
ls -la .gitignore

# Fix permissions
chmod 644 .gitignore

# Check directory permissions
ls -ld .
```

#### Network Connection Issues

**Symptoms:**

```bash
✗ Error: Network request failed
```

**Solutions:**

```bash
# Check internet connection
curl -I https://github.com

# Use cached templates
gitignore sync python  # Will use cache if available

# Force offline mode
gitignore init python  # Uses built-in templates
```

#### Command vs Pattern Confusion

**Symptoms:**

```bash
✗ Error: Ambiguous argument 'init'
```

**Solutions:**

```bash
# For command usage
gitignore init python

# For pattern addition
gitignore --add init

# Get help
gitignore --help
```

### Diagnostic Commands

```bash
# Show version and build info
gitignore --version

# Verbose output for debugging
gitignore --verbose init python

# Dry run to preview
gitignore --dry-run init python

# Check cache status
gitignore cache info  # (if implemented)
```

### Getting Help

```bash
# General help
gitignore --help

# Command-specific help
gitignore init --help

# List all templates
gitignore list

# Show template contents
gitignore show python
```

## 📚 Related Documentation

- **[Installation Guide](installation.md)** - Setup and system requirements
- **[Template Reference](templates.md)** - Complete template catalog
- **[Configuration Guide](configuration.md)** - Customization options
- **[API Reference](api-reference.md)** - Developer documentation
- **[Contributing Guide](contributing.md)** - Development guidelines
