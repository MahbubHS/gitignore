# API Reference

Complete reference for all functions, data structures, and constants in gitignore.

## 📋 Table of Contents

- [Data Structures](#data-structures)
- [Functions](#functions)
- [Constants](#constants)
- [Error Codes](#error-codes)

## 🏗️ Data Structures

### config_t

User configuration structure.

```c
typedef struct {
    char **default_templates;    // Default templates to apply
    int default_count;          // Number of default templates
    int auto_backup;            // Auto-backup before changes
    int cache_enabled;          // Enable template caching
    int cache_duration;         // Cache duration in seconds
    int verbose;                // Verbose output mode
    int quiet;                  // Quiet mode (minimal output)
    int use_color;              // Use colored terminal output
} config_t;
```

**Fields:**

- `default_templates`: Array of template names to apply by default
- `default_count`: Length of default_templates array
- `auto_backup`: Boolean - create backups automatically
- `cache_enabled`: Boolean - enable GitHub template caching
- `cache_duration`: Cache TTL in seconds (default: 86400 = 24h)
- `verbose`: Boolean - show detailed output
- `quiet`: Boolean - suppress non-error output
- `use_color`: Boolean - use ANSI color codes

### Merge Strategies

```c
typedef enum {
    MERGE_APPEND,     // Simple append without deduplication
    MERGE_REPLACE,    // Replace entire file
    MERGE_SMART       // Smart merge with deduplication
} merge_strategy_t;
```

### Error Codes

```c
typedef enum {
    ERR_SUCCESS = 0,
    ERR_FILE_NOT_FOUND,
    ERR_NETWORK_ERROR,
    ERR_PERMISSION_DENIED,
    ERR_INVALID_TEMPLATE,
    ERR_CURL_INIT_FAILED,
    ERR_OUT_OF_MEMORY,
    ERR_INVALID_ARGUMENT,
    ERR_CACHE_ERROR
} error_code_t;
```

## 🔧 Functions

### Core Functions

#### parse_flags

Parse command-line arguments and dispatch to appropriate handlers.

```c
int parse_flags(int argc, char *argv[]);
```

**Parameters:**

- `argc`: Number of command-line arguments
- `argv`: Array of command-line argument strings

**Returns:** Exit code (0 on success, non-zero on error)

**Description:** Main entry point for command processing. Handles global flags like `--dry-run`, `--verbose`, etc.

#### init_gitignore

Initialize .gitignore with specified templates.

```c
int init_gitignore(char **langs, int count, int dry_run);
```

**Parameters:**

- `langs`: Array of template names
- `count`: Number of templates
- `dry_run`: If true, preview changes without applying

**Returns:** 0 on success, error code on failure

#### add_patterns

Add individual patterns to .gitignore.

```c
int add_patterns(char **patterns, int count, int dry_run);
```

**Parameters:**

- `patterns`: Array of patterns to add
- `count`: Number of patterns
- `dry_run`: Preview mode flag

**Returns:** 0 on success, error code on failure

### Template Functions

#### get_builtin_template

Retrieve embedded template content by name.

```c
const char* get_builtin_template(const char *name);
```

**Parameters:**

- `name`: Template name (e.g., "python", "node")

**Returns:** Template content as string, or NULL if not found

**Description:** Returns compiled-in template data. No file I/O required.

#### merge_templates

Merge multiple templates into a single .gitignore file.

```c
int merge_templates(char **langs, int count, const char *output, merge_strategy_t strategy);
```

**Parameters:**

- `langs`: Array of template names
- `count`: Number of templates
- `output`: Output filename (usually ".gitignore")
- `strategy`: Merge strategy (APPEND, REPLACE, or SMART)

**Returns:** 0 on success, error code on failure

#### list_templates

Display available templates.

```c
int list_templates(const char *filter, int show_local, int show_builtin);
```

**Parameters:**

- `filter`: Optional name filter
- `show_local`: Show custom templates
- `show_builtin`: Show built-in templates

**Returns:** 0 on success, error code on failure

#### show_template

Display template contents.

```c
int show_template(const char *lang);
```

**Parameters:**

- `lang`: Template name to display

**Returns:** 0 on success, error code on failure

### GitHub Sync Functions

#### sync_gitignore

Download and apply templates from GitHub.

```c
int sync_gitignore(char **langs, int count, int dry_run);
```

**Parameters:**

- `langs`: Array of template names to sync
- `count`: Number of templates
- `dry_run`: Preview mode flag

**Returns:** 0 on success, error code on failure

#### download_template

Download a single template from GitHub.

```c
int download_template(const char *lang, char *buffer, size_t *size);
```

**Parameters:**

- `lang`: Template name to download
- `buffer`: Output buffer for template content
- `size`: Buffer size (updated with actual content size)

**Returns:** 0 on success, error code on failure

### Configuration Functions

#### load_config

Load user configuration from file.

```c
config_t* load_config(void);
```

**Returns:** Pointer to config structure, or default config if file not found

#### save_config

Save configuration to file.

```c
int save_config(config_t *config);
```

**Parameters:**

- `config`: Configuration to save

**Returns:** 0 on success, error code on failure

#### apply_config

Apply configuration settings globally.

```c
void apply_config(config_t *config);
```

**Parameters:**

- `config`: Configuration to apply

**Description:** Sets global variables based on config values.

### Cache Functions

#### init_cache

Initialize cache directory structure.

```c
int init_cache(void);
```

**Returns:** 0 on success, error code on failure

#### get_cached_template

Retrieve template from cache.

```c
int get_cached_template(const char *lang, char **content);
```

**Parameters:**

- `lang`: Template name
- `content`: Output pointer for cached content

**Returns:** 0 on success, error code on failure

#### cache_template

Store template in cache.

```c
int cache_template(const char *lang, const char *content);
```

**Parameters:**

- `lang`: Template name
- `content`: Template content to cache

**Returns:** 0 on success, error code on failure

#### clear_cache

Remove all cached templates.

```c
int clear_cache(void);
```

**Returns:** 0 on success, error code on failure

### Backup Functions

#### backup_gitignore

Create backup of current .gitignore.

```c
int backup_gitignore(void);
```

**Returns:** 0 on success, error code on failure

#### restore_gitignore

Restore .gitignore from backup.

```c
int restore_gitignore(const char *backup_name);
```

**Parameters:**

- `backup_name`: Backup filename (optional - prompts if NULL)

**Returns:** 0 on success, error code on failure

#### list_backups

Display available backups.

```c
int list_backups(void);
```

**Returns:** 0 on success, error code on failure

### Utility Functions

#### file_exists

Check if file exists.

```c
int file_exists(const char *path);
```

**Parameters:**

- `path`: File path to check

**Returns:** 1 if exists, 0 if not

#### create_empty_gitignore

Create empty .gitignore if it doesn't exist.

```c
int create_empty_gitignore(void);
```

**Returns:** 0 on success, error code on failure

#### remove_duplicates

Remove duplicate strings from array.

```c
char** remove_duplicates(char **langs, int *count);
```

**Parameters:**

- `langs`: Array of strings
- `count`: Pointer to array length (modified)

**Returns:** New array without duplicates

#### is_comment

Check if line is a comment.

```c
int is_comment(const char *line);
```

**Parameters:**

- `line`: Line to check

**Returns:** 1 if comment, 0 if not

### Detection Functions

#### auto_detect

Automatically detect project type and apply templates.

```c
int auto_detect(int dry_run);
```

**Parameters:**

- `dry_run`: Preview mode flag

**Returns:** 0 on success, error code on failure

#### detect_project_type

Analyze project files to determine type.

```c
int detect_project_type(char ***langs, int *count);
```

**Parameters:**

- `langs`: Output array of detected template names
- `count`: Output count of detected templates

**Returns:** 0 on success, error code on failure

#### is_language_name

Check if string is a known language name.

```c
int is_language_name(const char *name);
```

**Parameters:**

- `name`: String to check

**Returns:** 1 if known language, 0 if not

#### is_path_or_pattern

Determine if string is a file path/pattern.

```c
int is_path_or_pattern(const char *name);
```

**Parameters:**

- `name`: String to analyze

**Returns:** 1 if path/pattern, 0 if command

#### is_command_name

Check if string conflicts with a command name.

```c
int is_command_name(const char *name);
```

**Parameters:**

- `name`: String to check

**Returns:** 1 if command name, 0 if not

### UI Functions

#### show_help

Display help information.

```c
void show_help(void);
```

#### show_version

Display version information.

```c
void show_version(void);
```

#### interactive_mode

Start interactive template selection.

```c
int interactive_mode(void);
```

**Returns:** 0 on success, error code on failure

### Output Functions

#### print_error

Print error message.

```c
void print_error(const char *msg, error_code_t code);
```

**Parameters:**

- `msg`: Error message
- `code`: Error code for categorization

#### print_success

Print success message.

```c
void print_success(const char *msg);
```

**Parameters:**

- `msg`: Success message

#### print_warning

Print warning message.

```c
void print_warning(const char *msg);
```

**Parameters:**

- `msg`: Warning message

#### print_info

Print info message.

```c
void print_info(const char *msg);
```

**Parameters:**

- `msg`: Info message

#### print_progress

Print progress indicator.

```c
void print_progress(const char *task, int current, int total);
```

**Parameters:**

- `task`: Task description
- `current`: Current progress
- `total`: Total items

## 📊 Constants

### Version Information

```c
#define VERSION "2.0.0"
```

### Paths and Directories

```c
#define CONFIG_DIR ".config/gitignore"
#define TEMPLATES_DIR "templates"
#define CACHE_DIR "cache"
#define BACKUP_DIR "backups"
#define AUTO_TEMPLATE "auto.gitignore"
#define CONFIG_FILE "config.conf"
#define GLOBAL_GITIGNORE ".gitignore_global"
```

### Limits

```c
#define MAX_LANGS 100
#define MAX_PATH_LEN 512
#define MAX_LINE_LEN 1024
#define CACHE_DURATION 86400  // 24 hours
```

### GitHub Integration

```c
#define GITHUB_RAW_URL "https://raw.githubusercontent.com/github/gitignore/main/"
```

### ANSI Color Codes

```c
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BOLD    "\x1b[1m"
```

## 🚨 Error Codes

| Code | Name                    | Description                      |
| ---- | ----------------------- | -------------------------------- |
| 0    | `ERR_SUCCESS`           | Operation completed successfully |
| 1    | `ERR_FILE_NOT_FOUND`    | File or directory not found      |
| 2    | `ERR_NETWORK_ERROR`     | Network connection or HTTP error |
| 3    | `ERR_PERMISSION_DENIED` | Insufficient permissions         |
| 4    | `ERR_INVALID_TEMPLATE`  | Template name not recognized     |
| 5    | `ERR_CURL_INIT_FAILED`  | Failed to initialize libcurl     |
| 6    | `ERR_OUT_OF_MEMORY`     | Memory allocation failed         |
| 7    | `ERR_INVALID_ARGUMENT`  | Invalid function argument        |
| 8    | `ERR_CACHE_ERROR`       | Cache operation failed           |

## 🔧 Global Variables

### Configuration

```c
extern config_t *g_config;
```

Global configuration instance, loaded at startup.

## 📚 Related Topics

- [Architecture](architecture.md) - System design overview
- [Building](building.md) - Compilation and development
- [Contributing](contributing.md) - Development guidelines
