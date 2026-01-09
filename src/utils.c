// utils.c - Enhanced utility functions
#include "gitignore.h"

char* get_config_path(void) {
    const char *home = getenv("HOME");
    if (!home) {
        print_error("HOME environment variable not set", ERR_INVALID_ARGUMENT);
        return NULL;
    }
    
    char *path = malloc(MAX_PATH_LEN);
    if (!path) return NULL;
    
    snprintf(path, MAX_PATH_LEN, "%s/%s/%s", home, CONFIG_DIR, TEMPLATES_DIR);
    
    // Create directory if it doesn't exist
    char dir_path[MAX_PATH_LEN];
    snprintf(dir_path, sizeof(dir_path), "%s/%s", home, CONFIG_DIR);
    
    struct stat st = {0};
    if (stat(dir_path, &st) == -1) {
        mkdir(dir_path, 0755);
    }
    
    if (stat(path, &st) == -1) {
        mkdir(path, 0755);
    }
    
    return path;
}

char* get_template_path(const char *lang) {
    char *config_path = get_config_path();
    if (!config_path) return NULL;
    
    char *full_path = malloc(MAX_PATH_LEN);
    if (!full_path) {
        free(config_path);
        return NULL;
    }
    
    // Check if it already has .gitignore extension
    if (strstr(lang, ".gitignore")) {
        snprintf(full_path, MAX_PATH_LEN, "%s/%s", config_path, lang);
    } else {
        snprintf(full_path, MAX_PATH_LEN, "%s/%s.gitignore", config_path, lang);
    }
    
    free(config_path);
    return full_path;
}

int file_exists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

int is_comment(const char *line) {
    if (!line) return 1;
    
    // Skip whitespace
    while (*line == ' ' || *line == '\t') {
        line++;
    }
    
    // Check if line starts with #
    return (*line == '#' || *line == '\0' || *line == '\n');
}

char** remove_duplicates(char **langs, int *count) {
    if (!langs || *count == 0) return langs;
    
    int new_count = 0;
    char **filtered = malloc(sizeof(char*) * (*count));
    
    for (int i = 0; i < *count; i++) {
        // Skip if starts with # (comment)
        if (langs[i][0] == '#') {
            if (g_config && g_config->verbose) {
                printf("  %sSkipping comment:%s %s\n", 
                       COLOR_YELLOW, COLOR_RESET, langs[i]);
            }
            continue;
        }
        
        // Check for duplicates
        int is_duplicate = 0;
        for (int j = 0; j < new_count; j++) {
            if (strcasecmp(langs[i], filtered[j]) == 0) {
                is_duplicate = 1;
                if (g_config && g_config->verbose) {
                    printf("  %sSkipping duplicate:%s %s\n", 
                           COLOR_YELLOW, COLOR_RESET, langs[i]);
                }
                break;
            }
        }
        
        if (!is_duplicate) {
            filtered[new_count++] = langs[i];
        }
    }
    
    // Update the original array
    for (int i = 0; i < new_count; i++) {
        langs[i] = filtered[i];
    }
    
    *count = new_count;
    free(filtered);
    
    return langs;
}

// Colored output functions
void print_error(const char *msg, error_code_t code) {
    if (g_config && g_config->quiet) return;
    
    const char *color = (g_config && g_config->use_color) ? COLOR_RED : "";
    const char *reset = (g_config && g_config->use_color) ? COLOR_RESET : "";
    const char *bold = (g_config && g_config->use_color) ? COLOR_BOLD : "";
    
    fprintf(stderr, "%s%s✗ Error:%s %s", bold, color, reset, msg);
    
    if (g_config && g_config->verbose) {
        fprintf(stderr, " (code: %d)", code);
    }
    
    fprintf(stderr, "\n");
}

void print_success(const char *msg) {
    if (g_config && g_config->quiet) return;
    
    const char *color = (g_config && g_config->use_color) ? COLOR_GREEN : "";
    const char *reset = (g_config && g_config->use_color) ? COLOR_RESET : "";
    const char *bold = (g_config && g_config->use_color) ? COLOR_BOLD : "";
    
    printf("%s%s✓%s %s\n", bold, color, reset, msg);
}

void print_warning(const char *msg) {
    if (g_config && g_config->quiet) return;
    
    const char *color = (g_config && g_config->use_color) ? COLOR_YELLOW : "";
    const char *reset = (g_config && g_config->use_color) ? COLOR_RESET : "";
    const char *bold = (g_config && g_config->use_color) ? COLOR_BOLD : "";
    
    printf("%s%s⚠%s  %s\n", bold, color, reset, msg);
}

void print_info(const char *msg) {
    if (g_config && g_config->quiet) return;
    
    const char *color = (g_config && g_config->use_color) ? COLOR_CYAN : "";
    const char *reset = (g_config && g_config->use_color) ? COLOR_RESET : "";
    
    printf("%sℹ%s  %s\n", color, reset, msg);
}

void print_progress(const char *task, int current, int total) {
    if (g_config && g_config->quiet) return;
    
    const char *color = (g_config && g_config->use_color) ? COLOR_BLUE : "";
    const char *reset = (g_config && g_config->use_color) ? COLOR_RESET : "";
    const char *bold = (g_config && g_config->use_color) ? COLOR_BOLD : "";
    
    int percent = (current * 100) / total;
    int bar_width = 30;
    int filled = (bar_width * current) / total;
    
    printf("\r  %s[%s", color, reset);
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) {
            printf("%s█%s", color, reset);
        } else {
            printf(" ");
        }
    }
    printf("%s]%s %s%3d%%%s %s(%d/%d)%s %s", 
           color, reset, bold, percent, reset,
           color, current, total, reset, task);
    
    fflush(stdout);
    
    if (current == total) {
        printf("\n");
    }
}