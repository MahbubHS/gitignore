# Template Reference

This comprehensive guide covers all available templates in gitignore, including built-in templates, custom templates, and GitHub synchronization.

## 📋 Table of Contents

- [Template Overview](#-template-overview)
- [Programming Languages](#-programming-languages)
- [Development Tools](#-development-tools)
- [Operating Systems](#-operating-systems)
- [Infrastructure & DevOps](#-infrastructure--devops)
- [Custom Templates](#-custom-templates)
- [Template Management](#-template-management)
- [Statistics & Analytics](#-statistics--analytics)

## 🎯 Template Overview

### Template Architecture

Gitignore uses a multi-tier template system for maximum flexibility:

```
┌─────────────────────────────────────┐
│        Custom Templates             │  Highest Priority
│   ~/.config/gitignore/templates/    │
├─────────────────────────────────────┤
│        Built-in Templates           │  Compiled in binary
│   templates/*.gitignore             │
├─────────────────────────────────────┤
│        GitHub Sync Templates        │  Downloaded on-demand
│   github.com/github/gitignore       │
└─────────────────────────────────────┘
```

### Template Categories

| Category              | Count  | Description                   | Examples               |
| --------------------- | ------ | ----------------------------- | ---------------------- |
| **Languages**         | 8      | Programming language specific | Python, Node.js, Rust  |
| **Editors**           | 3      | IDE and editor configurations | VS Code, IntelliJ, Vim |
| **Operating Systems** | 3      | OS-specific files             | macOS, Linux, Windows  |
| **Infrastructure**    | 4      | DevOps and container tools    | Docker, Kubernetes     |
| **Total Built-in**    | **18** | All categories combined       | -                      |

### Template Features

- 📦 **Embedded Design:** All built-in templates compiled into binary (no external files)
- 🔄 **Smart Merging:** Automatic deduplication when combining templates
- 📥 **GitHub Sync:** Download official templates from github.com/github/gitignore
- 🎨 **Custom Support:** User-defined templates with highest priority
- ⚡ **Performance:** O(1) lookup time for built-in templates
- 💾 **Caching:** 24-hour cache for downloaded templates

## 🖥️ Programming Languages

### Python

**Template:** `python`  
**Use Case:** Python applications, libraries, and data science projects  
**Pattern Count:** 148 lines

**Ignored Categories:**

- **Bytecode:** `*.pyc`, `*.pyo`, `__pycache__/`
- **Distribution:** `build/`, `dist/`, `*.egg-info/`
- **Virtual Environments:** `venv/`, `env/`, `.venv/`
- **Testing:** `.coverage`, `htmlcov/`, `.tox/`
- **Documentation:** `docs/_build/`
- **IDE Files:** `.vscode/`, `.idea/`, `*.swp`
- **OS Files:** `.DS_Store`, `Thumbs.db`

**Usage Examples:**

```bash
# Basic Python project
gitignore init python

# Django application
gitignore init python django vscode

# Data science project
gitignore init python jupyter vscode linux
```

### Node.js

**Template:** `node`  
**Use Case:** Node.js applications, npm packages, and JavaScript projects  
**Pattern Count:** 156 lines

**Ignored Categories:**

- **Dependencies:** `node_modules/`, `npm-debug.log*`
- **Logs:** `logs/`, `*.log`, `pids/`
- **Runtime:** `lib-cov/`, `*.pid`, `*.seed`
- **Coverage:** `coverage/`, `.nyc_output/`
- **Build:** `dist/`, `build/`, `.next/`
- **Environment:** `.env`, `.env.local`
- **IDE:** `.vscode/`, `.idea/`

**Framework-Specific Patterns:**

- **React/Next.js:** `.next/`, `out/`
- **Angular:** `dist/`, `tmp/`
- **Vue.js:** `dist/`, `.cache/`

### Rust

**Template:** `rust`  
**Use Case:** Rust applications, libraries, and Cargo projects  
**Pattern Count:** 89 lines

**Ignored Categories:**

- **Build Artifacts:** `target/`, `debug/`, `release/`
- **Cargo:** `Cargo.lock` (optional), `.cargo/`
- **Backup Files:** `*.bk`, `*.orig`
- **IDE:** `.vscode/`, `.idea/`, `*.swp`
- **OS:** `.DS_Store`, `Thumbs.db`

**Cargo-Specific:**

- **Registry:** `~/.cargo/registry/`
- **Git:** `~/.cargo/git/`
- **Target Directories:** `target/`

### C/C++

**Templates:** `c`, `cpp`  
**Use Case:** System programming, embedded development, and compiled applications

#### C Template

**Pattern Count:** 67 lines

- **Object Files:** `*.o`, `*.ko`, `*.obj`
- **Executables:** `*.exe`, `*.out`, `*.app`
- **Libraries:** `*.a`, `*.so`, `*.dylib`
- **Build:** `build/`, `bin/`, `cmake-build/`

#### C++ Template

**Pattern Count:** 94 lines

- **Object Files:** `*.o`, `*.obj`, `*.lib`
- **Executables:** `*.exe`, `*.out`
- **Build Systems:** `cmake-build/`, `build/`
- **IDE:** `*.sln`, `*.vcxproj`, `.vs/`

### Java

**Template:** `java`  
**Use Case:** Java applications, Maven/Gradle projects, and JVM development  
**Pattern Count:** 112 lines

**Ignored Categories:**

- **Compiled Classes:** `*.class`, `*.jar`, `*.war`
- **Build Outputs:** `target/`, `out/`, `build/`
- **IDE Files:** `.idea/`, `*.iml`, `.classpath`
- **Maven:** `.mvn/`, `pom.xml.tag`
- **Gradle:** `.gradle/`, `gradle-app.setting`
- **Eclipse:** `.project`, `.settings/`

### Go

**Template:** `go`  
**Use Case:** Go applications, modules, and microservices  
**Pattern Count:** 78 lines

**Ignored Categories:**

- **Binaries:** `*.exe`, `*.exe~`, `*.dll`
- **Build Cache:** `*.tmp`, `*.test`
- **Coverage:** `*.out`, `coverage.txt`
- **Vendor:** `vendor/`, `Gopkg.lock`
- **IDE:** `.vscode/`, `.idea/`

**Go-Specific:**

- **Modules:** `go.sum` (optional)
- **Test Cache:** Cached test results
- **Build:** Temporary build files

## 🛠️ Development Tools

### Editors & IDEs

#### Visual Studio Code

**Template:** `vscode`  
**Pattern Count:** 23 lines

**Ignored:**

- `.vscode/` (settings, extensions, launch configs)
- OS-specific files in workspace

#### IntelliJ IDEA

**Template:** `intellij`  
**Pattern Count:** 45 lines

**Ignored:**

- `.idea/` (project files, workspace settings)
- `out/` (compilation output)
- `*.iml` (module files)
- Eclipse compatibility files

#### Vim

**Template:** `vim`  
**Pattern Count:** 18 lines

**Ignored:**

- `*.swp`, `*.swo` (swap files)
- `*.bak`, `*~` (backup files)
- `*.un~` (undo files)
- `.netrwhist` (netrw history)

### Version Control

#### Git

**Template:** `git`  
**Pattern Count:** 12 lines

**Ignored:**

- `.git/` (Git repository data)
- `.gitignore` (self-referential)
- Git-specific temporary files

## 🖥️ Operating Systems

### macOS

**Template:** `macos`  
**Pattern Count:** 156 lines

**Ignored Categories:**

- **System Files:** `.DS_Store`, `.AppleDouble/`
- **Spotlight:** `.Spotlight-V100/`, `.TemporaryItems/`
- **Trash:** `.Trashes/`
- **Metadata:** `._*`, `.DocumentRevisions-V100/`
- **Applications:** Temporary and cache files

### Linux

**Template:** `linux`  
**Pattern Count:** 34 lines

**Ignored:**

- `*~` (backup files)
- `core` (core dumps)
- `/tmp/` contents
- System-specific temporary files

### Windows

**Template:** `windows`  
**Pattern Count:** 67 lines

**Ignored:**

- `Thumbs.db`, `ehthumbs.db`
- `Desktop.ini`
- `$RECYCLE.BIN/`
- `*.stackdump`
- Windows system cache files

## 🏗️ Infrastructure & DevOps

### Docker

**Template:** `docker`  
**Pattern Count:** 45 lines

**Ignored:**

- Container images and layers
- Build contexts
- Docker-specific cache files
- Compose override files

### Kubernetes

**Template:** `kubernetes`  
**Pattern Count:** 23 lines

**Ignored:**

- `kubectl` configuration dumps
- Secret files
- Temporary manifests

### Terraform

**Template:** `terraform`  
**Pattern Count:** 34 lines

**Ignored:**

- `.terraform/`
- `*.tfstate*`
- Plan files
- Provider cache

### Ansible

**Template:** `ansible`  
**Pattern Count:** 28 lines

**Ignored:**

- `*.retry` files
- Vault-encrypted files
- Temporary inventory files

## 🎨 Custom Templates

### Creating Custom Templates

Create personalized templates in your user configuration directory:

```bash
# Create templates directory
mkdir -p ~/.config/gitignore/templates

# Create a custom Django template
cat > ~/.config/gitignore/templates/django.gitignore << 'EOF'
# Django Project Template
# Database
*.sqlite3
*.db

# Static files
staticfiles/
static/

# Media files
media/

# Environment variables
.env
.env.local

# Python bytecode
__pycache__/
*.pyc

# IDE
.vscode/
.idea/
EOF
```

### Template Discovery

Custom templates are automatically discovered and have the highest priority:

```bash
# List all templates (custom + built-in)
gitignore list

# Use custom template
gitignore init myproject

# Custom templates override built-in ones
gitignore init python  # Uses your custom python template if it exists
```

### Template Format Standards

**Structure your templates professionally:**

```gitignore
# ===========================================
# Custom Template for [Project Type]
# ===========================================
# Description: Brief explanation of what this template ignores
# Author: Your Name
# Version: 1.0.0
# Last Updated: YYYY-MM-DD

# 1. Build Artifacts
# ------------------
dist/
build/
target/

# 2. Dependencies
# ---------------
node_modules/
vendor/

# 3. Environment & Secrets
# ------------------------
.env*
secrets/

# 4. IDE & Editor Files
# ---------------------
.vscode/
.idea/
*.swp

# 5. OS Generated Files
# ---------------------
.DS_Store
Thumbs.db

# 6. Logs & Temporary Files
# -------------------------
*.log
*.tmp
.cache/
```

### Template Validation

Ensure your templates follow best practices:

- ✅ **One pattern per line**
- ✅ **Use comments for organization**
- ✅ **Include comprehensive coverage**
- ✅ **Test with dry-run mode**
- ✅ **Follow naming conventions**

## 🔧 Template Management

### Listing Templates

```bash
# Show all available templates
gitignore list

# Filter by keyword
gitignore list python

# Show detailed information
gitignore list --verbose

# Show only built-in templates
gitignore list --builtin

# Show only custom templates
gitignore list --local
```

### Previewing Templates

```bash
# Display template contents
gitignore show python

# Show multiple templates
gitignore show python node

# Preview custom template
gitignore show mytemplate
```

### Template Information

Each template includes metadata:

```bash
$ gitignore show python --info
Template: python
Source: Built-in
Lines: 148
Categories: Python, IDE, OS
Last Updated: 2024-01-15
Description: Comprehensive Python .gitignore
```

## 🌐 GitHub Synchronization

### Syncing Official Templates

Download templates directly from GitHub's official repository:

```bash
# Sync single template
gitignore sync python

# Sync multiple templates
gitignore sync rust go java

# Force refresh (ignore cache)
gitignore sync python --no-cache
```

### Sync Features

- **Source:** `github.com/github/gitignore`
- **Caching:** 24-hour local cache for performance
- **Merging:** Smart integration with existing `.gitignore`
- **Fallback:** Uses cache when offline
- **Updates:** Always gets latest official templates

### Available Sync Templates

All official GitHub templates are available for sync, including:

| Language                 | Framework             | Tool               |
| ------------------------ | --------------------- | ------------------ |
| Python, JavaScript, Java | Django, React, Spring | Docker, Kubernetes |
| Go, Rust, C/C++          | Rails, Laravel        | Terraform, Ansible |
| PHP, Ruby, Swift         | Vue, Angular          | AWS, Azure         |

## 📊 Statistics & Analytics

### Template Metrics

| Category              | Templates | Avg Patterns | Total Patterns | Coverage |
| --------------------- | --------- | ------------ | -------------- | -------- |
| **Languages**         | 8         | 98           | 784            | 43.6%    |
| **Editors**           | 3         | 29           | 87             | 4.8%     |
| **Operating Systems** | 3         | 86           | 258            | 14.3%    |
| **Infrastructure**    | 4         | 33           | 132            | 7.3%     |
| **Total Built-in**    | **18**    | **69**       | **1,261**      | **100%** |

### Usage Statistics

**Most Popular Templates:**

1. `python` - 148 patterns (Data science, web dev)
2. `node` - 156 patterns (Full-stack JavaScript)
3. `vscode` - 23 patterns (Universal editor)
4. `linux` - 34 patterns (Cross-platform dev)
5. `macos` - 156 patterns (macOS development)

### Performance Characteristics

- **Binary Size:** ~2MB (including all templates)
- **Memory Usage:** < 1MB runtime
- **Lookup Time:** O(1) for built-in templates
- **Cache TTL:** 24 hours for GitHub sync
- **Load Time:** Instant (embedded templates)

## 🛠️ Template Development

### Contributing New Templates

1. **Create Template:**

   ```bash
   # Add to templates/ directory
   vim templates/swift.gitignore
   ```

2. **Regenerate Database:**

   ```bash
   make regen-templates
   ```

3. **Test Template:**

   ```bash
   make && ./gitignore init swift
   ```

4. **Validate:**
   ```bash
   ./gitignore show swift
   ./gitignore --dry-run init swift
   ```

### Template Guidelines

- **Comprehensive:** Cover all common ignore patterns
- **Organized:** Use comments and sections
- **Cross-Platform:** Include OS-specific patterns
- **Future-Proof:** Consider emerging tools and practices
- **Documented:** Add clear comments explaining sections

## 🆘 Troubleshooting

### Template Not Found

```bash
# Check available templates
gitignore list

# Check spelling and variations
gitignore list | grep -i template

# For custom templates
ls ~/.config/gitignore/templates/
```

### Template Not Applying

```bash
# Preview template contents
gitignore show python

# Dry run to see changes
gitignore --dry-run init python

# Check file permissions
ls -la .gitignore
```

### Custom Template Issues

```bash
# Verify template location
ls -la ~/.config/gitignore/templates/

# Check template syntax
cat ~/.config/gitignore/templates/mytemplate.gitignore

# Test template loading
gitignore list --local
```

### Sync Problems

```bash
# Check network connectivity
curl -I https://github.com

# Clear cache and retry
gitignore cache clear
gitignore sync python

# Use built-in templates as fallback
gitignore init python
```

## 📚 Related Documentation

- **[Usage Guide](usage.md)** - How to use templates effectively
- **[Configuration Guide](configuration.md)** - Customizing template behavior
- **[Contributing Guide](contributing.md)** - Adding new templates
- **[Building Guide](building.md)** - Template compilation process
- **[API Reference](api-reference.md)** - Template system internals
