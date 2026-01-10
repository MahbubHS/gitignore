# API Reference

This comprehensive API reference documents all public functions, data structures, constants, and internal interfaces of the gitignore system.

## 📋 Table of Contents

- [Architecture Overview](#-architecture-overview)
- [Data Structures](#-data-structures)
- [Core Functions](#-core-functions)
- [Template Management](#-template-management)
- [Configuration System](#-configuration-system)
- [Cache Management](#-cache-management)
- [Backup & Recovery](#-backup--recovery)
- [Utility Functions](#-utility-functions)
- [Constants & Macros](#-constants--macros)
- [Error Handling](#-error-handling)

## 🏗️ Architecture Overview

### Code Organization

The gitignore codebase is organized into focused modules:

```
src/
├── main.c           # CLI entry point and command dispatch
├── init.c           # Template initialization logic
├── sync.c           # GitHub synchronization
├── utils.c          # Core utilities and helpers
├── features.c       # Advanced features (auto-detect, interactive)
├── global_backup.c  # Global .gitignore management
├── cache_config.c   # Configuration and caching system
└── templates.c      # Generated template database
```

### Design Principles

- **Modular Architecture:** Each file has a single responsibility
- **Error Propagation:** Consistent error codes across all functions
- **Memory Safety:** Careful allocation/deallocation patterns
- **Cross-Platform:** POSIX-compliant C code
- **Performance First:** Embedded templates, efficient algorithms

## 🏛️ Data Structures

### Configuration Structure

#### `config_t`

Global configuration container for user preferences and system settings.

```c
typedef struct {
    char **default_templates;    // Default templates to apply
    int default_count;          // Number of default templates
    int auto_backup;            // Auto-backup before changes (bool)
    int cache_enabled;          // Enable template caching (bool)
    int cache_duration;         // Cache TTL in seconds (default: 86400)
    int verbose;                // Verbose output mode (bool)
    int quiet;                  // Quiet mode - minimal output (bool)
    int use_color;              // Use ANSI color codes (bool)
    char *config_dir;           // Configuration directory path
    char *cache_dir;            // Cache directory path
} config_t;
```

**Field Details:**

| Field               | Type     | Default               | Description                                |
| ------------------- | -------- | --------------------- | ------------------------------------------ |
| `default_templates` | `char**` | `NULL`                | Array of template names applied by default |
| `default_count`     | `int`    | `0`                   | Length of `default_templates` array        |
| `auto_backup`       | `int`    | `1`                   | Create backups before file modifications   |
| `cache_enabled`     | `int`    | `1`                   | Enable GitHub template caching             |
| `cache_duration`    | `int`    | `86400`               | Cache lifetime in seconds (24 hours)       |
| `verbose`           | `int`    | `0`                   | Show detailed operation information        |
| `quiet`             | `int`    | `0`                   | Suppress non-error output                  |
| `use_color`         | `int`    | `1`                   | Use ANSI color codes in output             |
| `config_dir`        | `char*`  | `~/.config/gitignore` | User configuration directory               |
| `cache_dir`         | `char*`  | `~/.cache/gitignore`  | Template cache directory                   |

**Usage Example:**

```c
config_t *config = load_config();
if (config->auto_backup) {
    backup_gitignore();
}
```

### Merge Strategy Enumeration

#### `merge_strategy_t`

Defines how templates are combined when multiple templates are applied.

```c
typedef enum {
    MERGE_APPEND,     // Simple concatenation without deduplication
    MERGE_REPLACE,    // Replace entire file content
    MERGE_SMART       // Intelligent merge with duplicate removal
} merge_strategy_t;
```

**Strategy Details:**

| Strategy        | Description                       | Use Case                   | Performance |
| --------------- | --------------------------------- | -------------------------- | ----------- |
| `MERGE_APPEND`  | Concatenates templates end-to-end | Simple additions           | O(n)        |
| `MERGE_REPLACE` | Overwrites entire file            | Clean slate initialization | O(m)        |
| `MERGE_SMART`   | Merges with deduplication         | Multi-template projects    | O(n log n)  |

### Error Code System

#### `error_code_t`

Comprehensive error classification system used throughout the application.

```c
typedef enum {
    ERR_SUCCESS = 0,           // Operation completed successfully
    ERR_FILE_NOT_FOUND,        // File or directory not found
    ERR_NETWORK_ERROR,         // Network or HTTP error
    ERR_PERMISSION_DENIED,     // Insufficient file permissions
    ERR_INVALID_TEMPLATE,      // Template name not recognized
    ERR_CURL_INIT_FAILED,      // libcurl initialization failure
    ERR_OUT_OF_MEMORY,         // Memory allocation failure
    ERR_INVALID_ARGUMENT,      // Invalid function parameter
    ERR_CACHE_ERROR,           // Cache operation failure
    ERR_PARSE_ERROR,           // Configuration parsing error
    ERR_BACKUP_FAILED,         // Backup creation failure
    ERR_TEMPLATE_NOT_FOUND,    // Template retrieval failure
    ERR_MERGE_CONFLICT         // Template merge conflict
} error_code_t;
```

**Error Categories:**

| Range     | Category      | Handling                  |
| --------- | ------------- | ------------------------- |
| `0`       | Success       | No action required        |
| `1-99`    | File System   | Check permissions, paths  |
| `100-199` | Network       | Retry, check connectivity |
| `200-299` | Configuration | Validate settings         |
| `300-399` | Template      | Check template names      |

## 🔧 Core Functions

### Command Line Interface

#### `parse_flags()`

**Signature:**

```c
int parse_flags(int argc, char *argv[]);
```

**Parameters:**

- `argc` (int): Number of command-line arguments
- `argv` (char\*[]): Array of argument strings

**Returns:** `int` - Exit code (0 = success, non-zero = error)

**Description:**
Main command-line argument parser and dispatcher. Processes global flags and routes commands to appropriate handlers.

**Global Flags Processed:**

- `--help`, `-h`: Show help information
- `--version`, `-V`: Show version information
- `--verbose`, `-v`: Enable verbose output
- `--quiet`, `-q`: Enable quiet mode
- `--dry-run`, `-n`: Preview changes without applying
- `--color`: Force color output
- `--no-color`: Disable color output

**Command Dispatch:**

```c
// Example dispatch logic
if (strcmp(argv[1], "init") == 0) {
    return handle_init(argc - 1, argv + 1);
} else if (strcmp(argv[1], "sync") == 0) {
    return handle_sync(argc - 1, argv + 1);
}
// ... other commands
```

**Error Handling:**
Returns appropriate error codes for invalid arguments or command failures.

### Template Initialization

#### `init_gitignore()`

**Signature:**

```c
int init_gitignore(char **templates, int count, int dry_run);
```

**Parameters:**

- `templates` (char\*[]): Array of template names
- `count` (int): Number of templates in array
- `dry_run` (int): Preview mode flag (non-zero = preview only)

**Returns:** `int` - Error code (0 = success)

**Description:**
Initializes or updates `.gitignore` file with specified built-in templates. Creates backup automatically unless disabled.

**Process Flow:**

1. Validate template names
2. Create backup (if enabled)
3. Load template contents
4. Merge templates using smart strategy
5. Write to `.gitignore`
6. Report success/failure

**Example Usage:**

```c
char *templates[] = {"python", "vscode", "linux"};
int result = init_gitignore(templates, 3, 0);
if (result != ERR_SUCCESS) {
    print_error("Failed to initialize gitignore", result);
}
```

#### `add_patterns()`

**Signature:**

```c
int add_patterns(char **patterns, int count, int dry_run);
```

**Parameters:**

- `patterns` (char\*[]): Array of patterns to add
- `count` (int): Number of patterns
- `dry_run` (int): Preview mode flag

**Returns:** `int` - Error code

**Description:**
Adds individual patterns to existing `.gitignore` file. Performs duplicate detection and preserves existing content.

**Pattern Processing:**

- Checks for existing patterns
- Handles wildcards and paths
- Maintains file formatting
- Creates backup before changes

## 📋 Template Management

### Template Retrieval

#### `get_builtin_template()`

**Signature:**

```c
const char* get_builtin_template(const char *name);
```

**Parameters:**

- `name` (const char\*): Template name (e.g., "python", "node")

**Returns:** `const char*` - Template content or NULL if not found

**Description:**
Retrieves compiled-in template content by name. Templates are embedded in the binary for performance.

**Performance Characteristics:**

- **Time Complexity:** O(1) - Hash-based lookup
- **Memory:** No additional allocation (returns pointer to static data)
- **Thread Safety:** Read-only data, safe for concurrent access

**Template Database:**

```c
// Generated by scripts/generate_templates.sh
static const char *builtin_templates[] = {
    "python", python_template_content,
    "node", node_template_content,
    // ... more templates
    NULL, NULL
};
```

#### `merge_templates()`

**Signature:**

```c
int merge_templates(char **templates, int count, const char *output_path,
                   merge_strategy_t strategy);
```

**Parameters:**

- `templates` (char\*[]): Array of template names
- `count` (int): Number of templates
- `output_path` (const char\*): Output file path (usually ".gitignore")
- `strategy` (merge_strategy_t): Merge strategy to use

**Returns:** `int` - Error code

**Description:**
Combines multiple templates into a single output file using the specified merge strategy.

**Merge Algorithm:**

```c
// Smart merge pseudocode
for each template:
    load template content
    split into lines
    for each line:
        if not duplicate and not comment:
            add to merged set
sort merged lines
write to output file
```

### Template Discovery

#### `list_templates()`

**Signature:**

```c
int list_templates(const char *filter, int show_local, int show_builtin);
```

**Parameters:**

- `filter` (const char\*): Optional name filter (NULL for all)
- `show_local` (int): Include custom templates
- `show_builtin` (int): Include built-in templates

**Returns:** `int` - Error code

**Description:**
Displays available templates with filtering and categorization.

**Output Format:**

```
Built-in Templates:
├── Languages
│   ├── python     - Python projects
│   ├── node       - Node.js applications
│   └── rust       - Rust projects
└── Tools
    ├── docker     - Docker containers
    └── vscode     - Visual Studio Code

Custom Templates:
├── django        - Django web framework
└── react-native  - React Native mobile apps
```

#### `show_template()`

**Signature:**

```c
int show_template(const char *name);
```

**Parameters:**

- `name` (const char\*): Template name to display

**Returns:** `int` - Error code

**Description:**
Displays the contents of a template with syntax highlighting and line numbers.

**Display Features:**

- Line numbering
- Syntax highlighting for .gitignore patterns
- Section headers for organization
- Pattern explanations

## 🌐 GitHub Synchronization

### Sync Operations

#### `sync_gitignore()`

**Signature:**

```c
int sync_gitignore(char **templates, int count, int dry_run);
```

**Parameters:**

- `templates` (char\*[]): Array of template names to sync
- `count` (int): Number of templates
- `dry_run` (int): Preview mode flag

**Returns:** `int` - Error code

**Description:**
Downloads official templates from GitHub's gitignore repository and applies them.

**Sync Process:**

1. Check cache for each template
2. Download missing templates from GitHub
3. Cache downloaded content
4. Merge with existing `.gitignore`
5. Update cache timestamps

**GitHub API Details:**

- **Base URL:** `https://raw.githubusercontent.com/github/gitignore/main/`
- **Template Path:** `{TemplateName}.gitignore`
- **Rate Limiting:** Respects GitHub's API limits
- **Caching:** 24-hour local cache

#### `download_template()`

**Signature:**

```c
int download_template(const char *name, char **buffer, size_t *size);
```

**Parameters:**

- `name` (const char\*): Template name to download
- `buffer` (char\*\*): Output buffer for content (allocated by function)
- `size` (size_t\*): Output content size

**Returns:** `int` - Error code

**Description:**
Downloads a single template from GitHub with proper error handling and memory management.

**Implementation Details:**

```c
// HTTP request setup
CURL *curl = curl_easy_init();
curl_easy_setopt(curl, CURLOPT_URL, url);
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

// Execute request
CURLcode res = curl_easy_perform(curl);
if (res != CURLE_OK) {
    return ERR_NETWORK_ERROR;
}
```

## ⚙️ Configuration System

### Configuration Management

#### `load_config()`

**Signature:**

```c
config_t* load_config(void);
```

**Returns:** `config_t*` - Configuration structure (never NULL)

**Description:**
Loads user configuration from `~/.config/gitignore/config.conf`. Falls back to sensible defaults if file doesn't exist.

**Configuration File Format:**

```ini
# gitignore configuration
[general]
auto_backup = true
verbose = false
use_color = true

[cache]
enabled = true
duration = 86400

[templates]
default = python,vscode,linux
```

#### `save_config()`

**Signature:**

```c
int save_config(const config_t *config);
```

**Parameters:**

- `config` (const config_t\*): Configuration to save

**Returns:** `int` - Error code

**Description:**
Persists configuration to disk with proper error handling and atomic writes.

#### `apply_config()`

**Signature:**

```c
void apply_config(const config_t *config);
```

**Parameters:**

- `config` (const config_t\*): Configuration to apply

**Description:**
Applies configuration settings to global state and runtime behavior.

## 💾 Cache Management

### Cache Operations

#### `init_cache()`

**Signature:**

```c
int init_cache(void);
```

**Returns:** `int` - Error code

**Description:**
Creates cache directory structure and validates permissions.

**Cache Structure:**

```
~/.cache/gitignore/
├── templates/     # Downloaded GitHub templates
├── metadata/      # Cache metadata and timestamps
└── config/        # Cache configuration
```

#### `get_cached_template()`

**Signature:**

```c
int get_cached_template(const char *name, char **content);
```

**Parameters:**

- `name` (const char\*): Template name
- `content` (char\*\*): Output content buffer (allocated)

**Returns:** `int` - Error code

**Description:**
Retrieves template from local cache if available and not expired.

**Cache Validation:**

```c
// Check if cache file exists and is fresh
struct stat st;
if (stat(cache_path, &st) == 0) {
    time_t age = time(NULL) - st.st_mtime;
    if (age < g_config->cache_duration) {
        return load_cached_content(cache_path, content);
    }
}
return ERR_CACHE_MISS;
```

#### `cache_template()`

**Signature:**

```c
int cache_template(const char *name, const char *content);
```

**Parameters:**

- `name` (const char\*): Template name
- `content` (const char\*): Content to cache

**Returns:** `int` - Error code

**Description:**
Stores template content in local cache with timestamp.

#### `clear_cache()`

**Signature:**

```c
int clear_cache(void);
```

**Returns:** `int` - Error code

**Description:**
Removes all cached templates and metadata. Useful for troubleshooting or freeing disk space.

## 🔄 Backup & Recovery

### Backup Operations

#### `backup_gitignore()`

**Signature:**

```c
int backup_gitignore(void);
```

**Returns:** `int` - Error code

**Description:**
Creates timestamped backup of current `.gitignore` file.

**Backup Naming:**

```
.gitignore.backup.YYYY-MM-DD_HH-MM-SS
```

**Backup Process:**

1. Check if `.gitignore` exists
2. Generate timestamped filename
3. Copy file with error handling
4. Return backup path

#### `restore_gitignore()`

**Signature:**

```c
int restore_gitignore(const char *backup_name);
```

**Parameters:**

- `backup_name` (const char\*): Specific backup to restore (NULL for interactive selection)

**Returns:** `int` - Error code

**Description:**
Restores `.gitignore` from specified backup file.

#### `list_backups()`

**Signature:**

```c
int list_backups(void);
```

**Returns:** `int` - Error code

**Description:**
Displays all available backup files with timestamps and sizes.

**Output Format:**

```
Available Backups:
├── .gitignore.backup.2024-01-15_14-30-22  (2.1KB) [latest]
├── .gitignore.backup.2024-01-10_09-15-33  (1.8KB)
└── .gitignore.backup.2024-01-05_16-45-12  (1.5KB)
```

## 🛠️ Utility Functions

### File System Utilities

#### `file_exists()`

**Signature:**

```c
int file_exists(const char *path);
```

**Parameters:**

- `path` (const char\*): File path to check

**Returns:** `int` - 1 if exists, 0 if not

**Description:**
Cross-platform file existence check.

#### `create_empty_gitignore()`

**Signature:**

```c
int create_empty_gitignore(void);
```

**Returns:** `int` - Error code

**Description:**
Creates empty `.gitignore` file if it doesn't exist.

### String Processing

#### `remove_duplicates()`

**Signature:**

```c
char** remove_duplicates(char **strings, int *count);
```

**Parameters:**

- `strings` (char\*\*): Input string array
- `count` (int\*): Input/Output array length

**Returns:** `char**` - New array without duplicates

**Description:**
Removes duplicate strings from array, preserving order.

**Algorithm:**

```c
// Hash-based deduplication
for each string:
    if not in hashset:
        add to result
        insert into hashset
```

#### `is_comment()`

**Signature:**

```c
int is_comment(const char *line);
```

**Parameters:**

- `line` (const char\*): Line to check

**Returns:** `int` - 1 if comment, 0 if not

**Description:**
Checks if a line is a .gitignore comment (starts with #).

### Project Detection

#### `auto_detect()`

**Signature:**

```c
int auto_detect(int dry_run);
```

**Parameters:**

- `dry_run` (int): Preview mode flag

**Returns:** `int` - Error code

**Description:**
Automatically detects project type and applies appropriate templates.

**Detection Rules:**

```c
// File-based detection
if (file_exists("package.json")) return "node";
if (file_exists("Cargo.toml")) return "rust";
if (file_exists("requirements.txt")) return "python";
// ... more rules
```

#### `detect_project_type()`

**Signature:**

```c
int detect_project_type(char ***templates, int *count);
```

**Parameters:**

- `templates` (char\*\*\*): Output template array (allocated)
- `count` (int\*): Output template count

**Returns:** `int` - Error code

**Description:**
Analyzes project files to determine appropriate templates.

### Command Analysis

#### `is_path_or_pattern()`

**Signature:**

```c
int is_path_or_pattern(const char *string);
```

**Parameters:**

- `string` (const char\*): String to analyze

**Returns:** `int` - 1 if path/pattern, 0 if command

**Description:**
Intelligently distinguishes between file patterns and command names.

**Detection Logic:**

```c
// Pattern indicators
if (contains(string, "/")) return 1;  // Path separator
if (contains(string, "*")) return 1;  // Wildcard
if (contains(string, ".")) return 1;  // Extension
if (file_exists(string)) return 1;    // Existing file
return 0;  // Likely a command
```

#### `is_command_name()`

**Signature:**

```c
int is_command_name(const char *string);
```

**Parameters:**

- `string` (const char\*): String to check

**Returns:** `int` - 1 if command name, 0 if not

**Description:**
Checks if string conflicts with built-in command names.

## 🎨 User Interface

### Output Functions

#### `print_error()`

**Signature:**

```c
void print_error(const char *message, error_code_t code);
```

**Parameters:**

- `message` (const char\*): Error message
- `code` (error_code_t): Error classification

**Description:**
Displays error messages with appropriate formatting and colors.

#### `print_success()`

**Signature:**

```c
void print_success(const char *message);
```

**Parameters:**

- `message` (const char\*): Success message

**Description:**
Displays success confirmation with green coloring.

#### `print_warning()`

**Signature:**

```c
void print_warning(const char *message);
```

**Parameters:**

- `message` (const char\*): Warning message

**Description:**
Displays warnings with yellow coloring.

#### `print_info()`

**Signature:**

```c
void print_info(const char *message);
```

**Parameters:**

- `message` (const char\*): Information message

**Description:**
Displays informational messages.

#### `print_progress()`

**Signature:**

```c
void print_progress(const char *task, int current, int total);
```

**Parameters:**

- `task` (const char\*): Task description
- `current` (int): Current progress
- `total` (int): Total items

**Description:**
Shows progress indicators for long-running operations.

### Interactive Mode

#### `interactive_mode()`

**Signature:**

```c
int interactive_mode(void);
```

**Returns:** `int` - Error code

**Description:**
Launches interactive template selection interface with numbered choices.

## 📊 Constants & Macros

### Version Information

```c
#define VERSION_MAJOR 2
#define VERSION_MINOR 0
#define VERSION_PATCH 0
#define VERSION_STRING "2.0.0"
```

### Path Definitions

```c
#define CONFIG_DIR_NAME ".config"
#define CONFIG_APP_DIR "gitignore"
#define CACHE_DIR_NAME ".cache"
#define TEMPLATES_DIR_NAME "templates"
#define BACKUP_PREFIX ".gitignore.backup."
#define GLOBAL_GITIGNORE ".gitignore_global"
```

### Limits & Constraints

```c
#define MAX_TEMPLATES 100        // Maximum templates per operation
#define MAX_PATH_LENGTH 4096     // Maximum file path length
#define MAX_LINE_LENGTH 8192     // Maximum line length
#define MAX_PATTERN_LENGTH 1024  // Maximum pattern length
#define CACHE_DURATION_DEFAULT 86400  // 24 hours in seconds
```

### GitHub Integration

```c
#define GITHUB_BASE_URL "https://raw.githubusercontent.com"
#define GITHUB_REPO "github/gitignore"
#define GITHUB_BRANCH "main"
#define GITHUB_TEMPLATE_EXT ".gitignore"
```

### ANSI Color Codes

```c
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_BOLD    "\x1b[1m"
```

## 🚨 Error Handling

### Error Code Reference

| Code | Name                     | Category      | Description                   | Recovery                   |
| ---- | ------------------------ | ------------- | ----------------------------- | -------------------------- |
| `0`  | `ERR_SUCCESS`            | Success       | Operation completed           | N/A                        |
| `1`  | `ERR_FILE_NOT_FOUND`     | File System   | File/directory missing        | Check paths                |
| `2`  | `ERR_NETWORK_ERROR`      | Network       | HTTP connection failed        | Retry, check internet      |
| `3`  | `ERR_PERMISSION_DENIED`  | File System   | Access denied                 | Check permissions          |
| `4`  | `ERR_INVALID_TEMPLATE`   | Template      | Unknown template name         | Use `list` command         |
| `5`  | `ERR_CURL_INIT_FAILED`   | Network       | libcurl initialization failed | Check libcurl installation |
| `6`  | `ERR_OUT_OF_MEMORY`      | System        | Memory allocation failed      | Free memory, restart       |
| `7`  | `ERR_INVALID_ARGUMENT`   | Input         | Invalid parameter             | Check function usage       |
| `8`  | `ERR_CACHE_ERROR`        | Cache         | Cache operation failed        | Clear cache, retry         |
| `9`  | `ERR_PARSE_ERROR`        | Configuration | Config file malformed         | Check syntax               |
| `10` | `ERR_BACKUP_FAILED`      | Backup        | Backup creation failed        | Check disk space           |
| `11` | `ERR_TEMPLATE_NOT_FOUND` | Template      | Template unavailable          | Check template name        |
| `12` | `ERR_MERGE_CONFLICT`     | Merge         | Template conflict             | Use different strategy     |

### Error Propagation

All functions follow consistent error handling patterns:

```c
// Error checking pattern
int result = some_operation();
if (result != ERR_SUCCESS) {
    print_error("Operation failed", result);
    return result;  // Propagate error
}
```

### Recovery Strategies

- **File System Errors:** Check permissions, disk space, file locks
- **Network Errors:** Implement retry logic with exponential backoff
- **Memory Errors:** Free resources, suggest process restart
- **Configuration Errors:** Fall back to defaults, validate input

## 📚 Related Documentation

- **[Architecture Overview](architecture.md)** - System design and data flow
- **[Building Guide](building.md)** - Compilation and development setup
- **[Contributing Guide](contributing.md)** - Development guidelines and standards
- **[Testing Guide](testing.md)** - Quality assurance procedures
