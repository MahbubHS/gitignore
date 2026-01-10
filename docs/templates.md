# Template Reference

Complete guide to all available templates in gitignore.

## 📋 Template Categories

Gitignore includes templates for languages, editors, and operating systems.

## 🖥️ Programming Languages

### Python

**Template:** `python`  
**Files:** `templates/python.gitignore`

Ignores:

- Bytecode files (`*.pyc`, `__pycache__/`)
- Distribution files (`build/`, `dist/`)
- Package files (`*.egg-info/`)
- Virtual environments (`venv/`, `env/`)
- IDE files (`.vscode/`, `.idea/`)
- OS files (`.DS_Store`, `Thumbs.db`)

### Node.js

**Template:** `node`  
**Files:** `templates/node.gitignore`

Ignores:

- Dependencies (`node_modules/`)
- Logs (`logs/`, `*.log`)
- Runtime data (`pids/`, `*.pid`)
- Coverage reports (`coverage/`)
- Build outputs (`dist/`, `build/`)
- Environment files (`.env`)
- IDE files (`.vscode/`, `.idea/`)

### Rust

**Template:** `rust`  
**Files:** `templates/rust.gitignore`

Ignores:

- Cargo build artifacts (`target/`)
- Backup files (`*.bk`)
- IDE files (`.vscode/`, `.idea/`)
- OS files (`.DS_Store`, `Thumbs.db`)

### C

**Template:** `c`  
**Files:** `templates/c.gitignore`

Ignores:

- Object files (`*.o`, `*.ko`)
- Executables and libraries (`*.exe`, `*.dll`, `*.so`)
- Build directories (`build/`, `bin/`)
- IDE files (`.vscode/`, `.idea/`)
- OS files (`.DS_Store`, `Thumbs.db`)

### C++

**Template:** `cpp`  
**Files:** `templates/cpp.gitignore`

Ignores:

- Object files (`*.o`, `*.obj`)
- Executables (`*.exe`, `*.out`)
- Build artifacts (`build/`, `cmake-build/`)
- IDE files (`.vscode/`, `.idea/`, `*.sln`)
- OS files (`.DS_Store`, `Thumbs.db`)

### Java

**Template:** `java`  
**Files:** `templates/java.gitignore`

Ignores:

- Class files (`*.class`)
- Build outputs (`target/`, `out/`)
- IDE files (`.idea/`, `*.iml`, `.classpath`)
- Maven/Gradle files (`.gradle/`)
- OS files (`.DS_Store`, `Thumbs.db`)

### Go

**Template:** `go`  
**Files:** `templates/go.gitignore`

Ignores:

- Binaries (`*.exe`, `*.exe~`)
- Build cache (`*.tmp`)
- Test binaries (`*.test`)
- Go coverage files (`*.out`)
- Vendor directory (`vendor/`)
- IDE files (`.vscode/`, `.idea/`)

## 🛠️ Development Tools

### Visual Studio Code

**Template:** `vscode`  
**Files:** `templates/vscode.gitignore`

Ignores:

- VS Code settings (`.vscode/`)
- OS generated files (`.DS_Store`, `Thumbs.db`)

### Vim

**Template:** `vim`  
**Files:** `templates/vim.gitignore`

Ignores:

- Swap files (`*.swp`, `*.swo`)
- Backup files (`*.bak`, `*~`)
- Vim undo files (`*.un~`)

### IntelliJ IDEA

**Template:** `intellij`  
**Files:** `templates/intellij.gitignore`

Ignores:

- IntelliJ project files (`.idea/`)
- Out directories (`out/`)
- Class files (`*.class`)
- Eclipse project files (`.project`, `.classpath`)

## 🖥️ Operating Systems

### macOS

**Template:** `macos`  
**Files:** `templates/macos.gitignore`

Ignores:

- `.DS_Store` files
- Spotlight metadata (`.Spotlight-V100/`)
- Trash files (`.Trashes/`)
- Temporary files (`.TemporaryItems/`)

### Linux

**Template:** `linux`  
**Files:** `templates/linux.gitignore`

Ignores:

- `*~` backup files
- Core dump files (`core`)
- Temporary files in `/tmp/`

### Windows

**Template:** `windows`  
**Files:** `templates/windows.gitignore`

Ignores:

- `Thumbs.db`
- `ehthumbs.db`
- `Desktop.ini`
- Recycle Bin files (`$RECYCLE.BIN/`)
- Windows system files (`*.stackdump`)

## 🔧 Custom Templates

### Creating Custom Templates

Create your own templates in `~/.config/gitignore/templates/`:

```bash
# Create directory
mkdir -p ~/.config/gitignore/templates

# Create custom template
cat > ~/.config/gitignore/templates/django.gitignore << 'EOF'
# Django
*.log
*.pot
*.pyc
__pycache__/
db.sqlite3
media/
staticfiles/
EOF
```

### Template Priority

1. **Custom templates** (`~/.config/gitignore/templates/`) - Highest priority
2. **Built-in templates** (compiled in binary) - Fallback

### Template Format

Templates are simple text files with one pattern per line:

```gitignore
# Comments start with #
*.log
__pycache__/
.env
dist/
build/
```

**Best practices:**

- One pattern per line
- Use comments to explain sections
- Include common patterns for the language/tool
- Keep it focused and relevant

## 📊 Template Statistics

| Category           | Count  | Examples                                |
| ------------------ | ------ | --------------------------------------- |
| Languages          | 8      | Python, Node.js, Rust, C, C++, Java, Go |
| Editors            | 3      | VS Code, Vim, IntelliJ                  |
| Operating Systems  | 3      | macOS, Linux, Windows                   |
| **Total Built-in** | **14** | All categories combined                 |

## 🔍 Viewing Templates

### List All Templates

```bash
# Show all templates
gitignore list

# Show built-in only
gitignore list --builtin

# Show custom only
gitignore list --local

# Filter by name
gitignore list python
```

### Preview Template Contents

```bash
# Show template contents
gitignore show python
gitignore cat node

# Works with custom templates too
gitignore show mytemplate
```

## 🌐 GitHub Sync Templates

In addition to built-in templates, you can sync official templates from GitHub:

```bash
# Sync Python template from GitHub
gitignore sync python

# Sync multiple templates
gitignore sync rust go java
```

**Synced templates are cached for 24 hours for performance.**

## 📋 Template Usage Examples

### Web Development

```bash
gitignore init node python vscode linux
```

### System Programming

```bash
gitignore init c cpp go linux
```

### Data Science

```bash
gitignore init python jupyter vscode linux
```

### Mobile Development

```bash
gitignore init swift java vscode macos
```

### Full-Stack Project

```bash
gitignore init python node java vscode intellij linux macos windows
```

## 🆘 Template Troubleshooting

### "Template not found"

```bash
# Check available templates
gitignore list

# Check spelling
gitignore list py  # Shows python-related templates
```

### Custom templates not showing

```bash
# Ensure correct directory
ls ~/.config/gitignore/templates/

# Check file permissions
ls -la ~/.config/gitignore/templates/*.gitignore
```

### Template not applying correctly

```bash
# Preview what the template contains
gitignore show python

# Use dry-run to see what would be added
gitignore --dry-run init python
```

## 🤝 Contributing Templates

Want to add a new built-in template?

1. Create the template file in `templates/`
2. Run `make regen-templates`
3. Test: `make && ./gitignore init yourtemplate`
4. Submit a pull request

See [Contributing Guide](contributing.md) for details.

## 📚 Related Topics

- [Usage Guide](usage.md) - How to use templates
- [Contributing](contributing.md) - Add new templates
- [Building](building.md) - Template compilation process
