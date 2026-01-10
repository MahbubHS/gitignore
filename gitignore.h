// gitignore.h - Complete header with built-in templates
#ifndef GITIGNORE_H
#define GITIGNORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <curl/curl.h>
#include <dirent.h>
#include <time.h>

#define VERSION "2.0.0"
#define MAX_LANGS 100
#define MAX_PATH_LEN 512
#define MAX_LINE_LEN 1024
#define CACHE_DURATION 86400
#define GITHUB_RAW_URL "https://raw.githubusercontent.com/github/gitignore/main/"

// ANSI Color codes
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BOLD    "\x1b[1m"

// Config paths
#define CONFIG_DIR ".config/gitignore"
#define TEMPLATES_DIR "templates"
#define CACHE_DIR "cache"
#define BACKUP_DIR "backups"
#define AUTO_TEMPLATE "auto.gitignore"
#define CONFIG_FILE "config.conf"
#define GLOBAL_GITIGNORE ".gitignore_global"

// Error codes
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

// Merge strategies
typedef enum {
    MERGE_APPEND,
    MERGE_REPLACE,
    MERGE_SMART
} merge_strategy_t;

// Config structure
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

// Function declarations - Core
void show_help(void);
void show_version(void);
int parse_flags(int argc, char *argv[]);

// Function declarations - Main features
int init_gitignore(char **langs, int count, int dry_run);
int sync_gitignore(char **langs, int count, int dry_run);
int list_templates(const char *filter, int show_local, int show_builtin);
int show_template(const char *lang);
int append_gitignore(char **langs, int count, merge_strategy_t strategy, int dry_run);
int add_patterns(char **patterns, int count, int dry_run);
int auto_detect(int dry_run);
int interactive_mode(void);

// Function declarations - Global gitignore
int global_init(void);
int global_add(char **langs, int count);

// Function declarations - Backup & Restore
int backup_gitignore(void);
int restore_gitignore(const char *backup_name);
int list_backups(void);

// Function declarations - Cache
int init_cache(void);
int get_cached_template(const char *lang, char **content);
int cache_template(const char *lang, const char *content);
int clear_cache(void);

// Function declarations - Config
config_t* load_config(void);
void free_config(config_t *config);
int save_config(config_t *config);
void apply_config(config_t *config);

// Function declarations - Built-in templates
const char* get_builtin_template(const char *name);
int is_builtin_template(const char *name);
const char** get_builtin_template_names(void);

// Helper functions
char* get_config_path(void);
char* get_cache_path(void);
char* get_backup_path(void);
char* get_template_path(const char *lang);
int file_exists(const char *path);
int create_empty_gitignore(void);
int merge_templates(char **langs, int count, const char *output, merge_strategy_t strategy);
int download_template(const char *lang, char *buffer, size_t *size);
char** remove_duplicates(char **langs, int *count);
int is_comment(const char *line);
void print_error(const char *msg, error_code_t code);
void print_success(const char *msg);
void print_warning(const char *msg);
void print_info(const char *msg);
void print_progress(const char *task, int current, int total);
int detect_project_type(char ***langs, int *count);
int is_language_name(const char *name);
int is_path_or_pattern(const char *name);
int is_command_name(const char *name);

// NEW: Duplicate removal and merging
char** remove_duplicate_patterns(char **patterns, int *count);
int pattern_exists_in_file(const char *pattern);

extern config_t *g_config;

#endif