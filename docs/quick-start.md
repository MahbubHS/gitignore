# Quick Start Guide

Get up and running with gitignore in under 5 minutes.

## 🚀 Basic Usage

### 1. Auto-Detect Project Type

The easiest way to get started:

```bash
cd your-project
gitignore auto
```

This automatically detects your project type and creates an appropriate `.gitignore`.

### 2. Initialize with Templates

For specific languages/frameworks:

```bash
# Single language
gitignore init python

# Multiple languages
gitignore init python node vscode
```

### 3. Add Individual Patterns

```bash
# Add common patterns
gitignore node_modules/ *.log .env

# Add custom patterns
gitignore dist/ build/ *.tmp
```

## 📋 Step-by-Step Examples

### Example 1: New Python Project

```bash
# Create new project
mkdir my-python-app
cd my-python-app
git init

# Initialize with Python template
gitignore init python

# Add editor-specific patterns
gitignore init vscode

# Add custom patterns
gitignore .env secrets/
```

**Result:** Your `.gitignore` now contains Python, VS Code, and custom patterns.

### Example 2: Node.js Project

```bash
cd existing-node-project

# Auto-detect (will find package.json)
gitignore auto

# Or manually specify
gitignore init node

# Add more patterns
gitignore node_modules/ dist/ .env
```

### Example 3: Multi-Language Project

```bash
cd my-fullstack-app

# Initialize with multiple templates
gitignore init python node java

# Add OS-specific patterns
gitignore init linux

# Add editor patterns
gitignore init vscode intellij
```

## 🎯 Interactive Mode

For beginners or when unsure:

```bash
# Start interactive template selection
gitignore -t

# Or use long form
gitignore interactive
```

This shows a numbered menu of available templates.

## 🔍 Preview Before Applying

Use dry-run to see what would happen:

```bash
# Preview auto-detection
gitignore --dry-run auto

# Preview template initialization
gitignore --dry-run init python node
```

## 📁 What Gets Created

After running gitignore, you'll have:

```
your-project/
├── .gitignore          # Main gitignore file
├── .gitignore_backup/  # Automatic backups (if enabled)
└── ...other files
```

## ✅ Verification

Check your `.gitignore` was created correctly:

```bash
# View contents
cat .gitignore

# Check git status
git status

# See what files are ignored
git ls-files --others --ignored --exclude-standard
```

## 🆘 Common Issues

### "No templates found"
- Run `gitignore list` to see available templates
- Check if you spelled the template name correctly

### "Permission denied"
- Try running with `sudo` if installing system-wide
- Or install to user directory: `make PREFIX=~/.local install`

### "Command not found"
- Ensure gitignore is in your PATH
- Try full path: `/usr/local/bin/gitignore --version`

## 🎉 You're Done!

Your project now has a proper `.gitignore` file. Here are some next steps:

- [Learn all commands](usage.md)
- [Explore available templates](templates.md)
- [Customize behavior](configuration.md)
- [Contribute new templates](contributing.md)

## 📚 Further Reading

- [Complete Usage Guide](usage.md) - All commands and options
- [Template Reference](templates.md) - Available built-in templates
- [Configuration Guide](configuration.md) - Customize gitignore