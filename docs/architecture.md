# Architecture Overview

Detailed technical architecture of the gitignore tool.

## 🏗️ System Architecture

Gitignore is a compiled C application with embedded templates and smart merging capabilities.

### Core Components

```
gitignore/
├── src/                    # Source code
│   ├── main.c             # Command-line interface & argument parsing
│   ├── init.c             # Template initialization
│   ├── sync.c             # GitHub template synchronization
│   ├── utils.c            # Utility functions
│   ├── features.c         # Advanced features (backup, global)
│   ├── global_backup.c    # Global gitignore management
│   ├── cache_config.c     # Caching and configuration
│   └── templates.c        # Embedded template data (generated)
├── templates/             # Template source files
├── scripts/               # Build scripts
├── man/                   # Manual pages
└── Makefile              # Build system
```

## 📊 Data Flow

### 1. Command Processing

```
User Input → main.c::parse_flags() → Command Dispatch → Feature Implementation
```

### 2. Template Merging

```
Template Selection → templates.c::get_builtin_template() → merge_templates() → .gitignore
```

### 3. GitHub Sync

```
Language Request → download_template() → Cache Check → HTTP Download → Cache Store → Merge
```

## 🔧 Key Components

### Main Entry Point (`main.c`)

**Responsibilities:**

- Command-line argument parsing
- Global flag handling (`--dry-run`, `--verbose`, `--quiet`)
- Command dispatch to appropriate modules
- Configuration loading

**Key Functions:**

- `parse_flags()` - Main argument processing
- `is_command_name()` - Command vs pattern detection
- `is_path_or_pattern()` - Smart pattern recognition

### Template System (`templates.c`)

**Generated from:** `scripts/generate_templates.sh`

**Features:**

- All templates compiled into binary
- No external file dependencies
- Fast access (no disk I/O)
- Template lookup by name

**Structure:**

```c
const char* get_builtin_template(const char *name) {
    if (strcmp(name, "python") == 0) return python_template;
    if (strcmp(name, "node") == 0) return node_template;
    // ... etc
}
```

### Configuration System (`cache_config.c`)

**Configuration File:** `~/.config/gitignore/config.conf`

**Features:**

- User preferences (colors, verbosity)
- Cache settings
- Auto-backup configuration
- Default templates

**Structure:**

```c
typedef struct {
    char **default_templates;
    int default_count;
    int auto_backup;
    int cache_enabled;
    int cache_duration;
    int verbose;
    int quiet;
    int use_color;
} config_t;
```

### Caching System

**Cache Location:** `~/.config/gitignore/cache/`

**Features:**

- GitHub template caching (24h TTL)
- Automatic cache invalidation
- Performance optimization

**Cache Structure:**

```
~/.config/gitignore/cache/
├── python.gitignore
├── node.gitignore
└── ... (other synced templates)
```

### Backup System

**Backup Location:** `~/.config/gitignore/backups/`

**Features:**

- Automatic pre-modification backups
- Timestamped backup files
- Easy restoration

**Backup Naming:** `backup_YYYY-MM-DD_HH-MM-SS`

## 🔄 Core Algorithms

### Smart Merging Algorithm

**Purpose:** Intelligently combine multiple templates while removing duplicates.

**Process:**

1. Load all requested templates
2. Split into individual patterns
3. Normalize patterns (trim whitespace, remove comments)
4. Deduplication using hash-based comparison
5. Sort patterns for consistency
6. Write to `.gitignore`

**Key Features:**

- Preserves existing `.gitignore` content
- Handles different merge strategies (`MERGE_APPEND`, `MERGE_SMART`)
- Comment preservation
- Empty line handling

### Pattern Detection Algorithm

**Purpose:** Distinguish between commands and file patterns.

**Rules:**

1. Contains `/` → Path pattern
2. Contains `*` → Wildcard pattern
3. Contains `.` → Hidden file pattern
4. File exists in filesystem → Path pattern
5. Matches known command names → Requires `--add` flag
6. Default → Pattern

### Auto-Detection Algorithm

**Purpose:** Automatically determine project type from files.

**Detection Map:**

```c
// File → Template mapping
"package.json" → "node"
"requirements.txt" → "python"
"setup.py" → "python"
"Cargo.toml" → "rust"
"go.mod" → "go"
"pom.xml" → "java"
"build.gradle" → "java"
```

## 📁 Directory Structure

### Source Layout

```
src/
├── main.c          # CLI entry point (263 lines)
├── init.c          # Template initialization
├── sync.c          # GitHub synchronization
├── utils.c         # String/file utilities
├── features.c      # Interactive mode, auto-detect
├── global_backup.c # Global gitignore features
├── cache_config.c  # Configuration and caching
└── templates.c     # Generated template data
```

### Configuration Directories

```
~/.config/gitignore/
├── config.conf           # User configuration
├── templates/           # Custom user templates
├── cache/               # Downloaded templates
└── backups/             # .gitignore backups
```

## 🔗 Dependencies

### Required Libraries

- **libcurl** - HTTP downloads for GitHub sync
- **Standard C Library** - File I/O, string manipulation

### Build Dependencies

- **GCC/Clang** - C compiler
- **Make** - Build system
- **libcurl-dev** - Development headers

## 🚀 Performance Characteristics

### Memory Usage

- **Base:** ~2MB (binary + embedded templates)
- **Per Operation:** Minimal heap allocation
- **Cache:** Configurable cache size limits

### Speed Optimizations

- Templates compiled into binary (no file reads)
- Hash-based deduplication
- Efficient string operations
- Minimal system calls

### Scalability

- Handles unlimited template combinations
- Linear performance with template count
- Cached GitHub templates for repeated use

## 🛡️ Error Handling

### Error Categories

- **File System Errors** - Permission denied, disk full
- **Network Errors** - Connection failed, timeout
- **Template Errors** - Template not found, invalid format
- **Configuration Errors** - Invalid config file

### Error Propagation

```
Function Call → Error Check → Error Code Return → User-Friendly Message
```

### Recovery Mechanisms

- Automatic backup creation
- Graceful degradation (continue with available templates)
- Clear error messages with suggestions

## 🔧 Build System

### Makefile Targets

```makefile
all: templates $(TARGET)          # Build everything
templates: templates.c            # Generate embedded templates
clean:                            # Remove build artifacts
install:                          # Install system-wide
test:                             # Run test suite
regen-templates:                  # Force template regeneration
```

### Template Generation Process

```
templates/*.gitignore → scripts/generate_templates.sh → src/templates.c
```

**Generated Code Structure:**

```c
// Auto-generated file - DO NOT EDIT
const char *python_template = "# Python\n*.pyc\n__pycache__/\n...";
const char *node_template = "# Node.js\nnode_modules/\n...";
// ... etc

const char* get_builtin_template(const char *name) {
    // Lookup logic
}
```

## 🧪 Testing Strategy

### Unit Tests

- Individual function testing
- Mock file system operations
- Template parsing validation

### Integration Tests

- End-to-end command testing
- Template merging verification
- GitHub sync testing

### Manual Testing

- Cross-platform compatibility
- Real-world usage scenarios

## 🔮 Future Architecture

### Planned Improvements

- **Plugin System** - Dynamic template loading
- **Template Validation** - Syntax checking
- **Performance Monitoring** - Operation timing
- **Configuration UI** - Interactive configuration

### Scalability Considerations

- Template database expansion
- Multi-threaded operations
- Memory-mapped template storage
- Distributed caching

## 📚 Related Documentation

- [API Reference](api-reference.md) - Function documentation
- [Building Guide](building.md) - Development setup
- [Contributing](contributing.md) - Development guidelines
