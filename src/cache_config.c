// cache_config.c - Cache and configuration management
#include "gitignore.h"

// Cache functions
int init_cache(void) {
    char *cache_path = get_cache_path();
    if (!cache_path) return 1;
    
    struct stat st = {0};
    if (stat(cache_path, &st) == -1) {
        mkdir(cache_path, 0755);
    }
    
    free(cache_path);
    return 0;
}

int get_cached_template(const char *lang, char **content) {
    if (!g_config || !g_config->cache_enabled) {
        return 1;
    }
    
    char *cache_path = get_cache_path();
    if (!cache_path) return 1;
    
    char cache_file[MAX_PATH_LEN];
    snprintf(cache_file, sizeof(cache_file), "%s/%s.cache", cache_path, lang);
    free(cache_path);
    
    if (!file_exists(cache_file)) {
        return 1;
    }
    
    // Check if cache is still valid
    struct stat st;
    if (stat(cache_file, &st) != 0) {
        return 1;
    }
    
    time_t now = time(NULL);
    if (difftime(now, st.st_mtime) > g_config->cache_duration) {
        // Cache expired
        unlink(cache_file);
        return 1;
    }
    
    // Read cached content
    FILE *f = fopen(cache_file, "r");
    if (!f) return 1;
    
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    if (file_size < 0) {
        fclose(f);
        return 1;
    }
    
    size_t size = (size_t)file_size;
    *content = malloc(size + 1);
    if (!*content) {
        fclose(f);
        return 1;
    }
    
    size_t bytes_read = fread(*content, 1, size, f);
    if (bytes_read != size) {
        free(*content);
        fclose(f);
        return 1;
    }
    (*content)[size] = '\0';
    fclose(f);
    
    if (g_config->verbose) {
        print_info("Using cached template");
    }
    
    return 0;
}

int cache_template(const char *lang, const char *content) {
    if (!g_config || !g_config->cache_enabled) {
        return 0;
    }
    
    char *cache_path = get_cache_path();
    if (!cache_path) return 1;
    
    char cache_file[MAX_PATH_LEN];
    snprintf(cache_file, sizeof(cache_file), "%s/%s.cache", cache_path, lang);
    free(cache_path);
    
    FILE *f = fopen(cache_file, "w");
    if (!f) return 1;
    
    fputs(content, f);
    fclose(f);
    
    return 0;
}

int clear_cache(void) {
    char *cache_path = get_cache_path();
    if (!cache_path) return 1;
    
    DIR *dir = opendir(cache_path);
    if (!dir) {
        free(cache_path);
        print_info("Cache already empty");
        return 0;
    }
    
    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".cache")) {
            char file_path[MAX_PATH_LEN];
            snprintf(file_path, sizeof(file_path), "%s/%s", cache_path, entry->d_name);
            unlink(file_path);
            count++;
        }
    }
    
    closedir(dir);
    free(cache_path);
    
    print_success("Cache cleared");
    printf("  Removed %d cached template(s)\n", count);
    
    return 0;
}

// Config functions
config_t* load_config(void) {
    config_t *config = malloc(sizeof(config_t));
    if (!config) return NULL;
    
    // Default values
    config->default_templates = NULL;
    config->default_count = 0;
    config->auto_backup = 0;
    config->cache_enabled = 1;
    config->cache_duration = CACHE_DURATION;
    config->verbose = 0;
    config->quiet = 0;
    config->use_color = isatty(STDOUT_FILENO);
    
    // Try to load config file
    char *config_path = get_config_path();
    if (!config_path) return config;
    
    char config_file[MAX_PATH_LEN];
    snprintf(config_file, sizeof(config_file), "%s/../%s", config_path, CONFIG_FILE);
    free(config_path);
    
    FILE *f = fopen(config_file, "r");
    if (!f) return config;
    
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), f)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') continue;
        
        char key[256], value[256];
        if (sscanf(line, "%[^=]=%s", key, value) == 2) {
            // Trim whitespace
            char *k = key;
            while (*k == ' ' || *k == '\t') k++;
            char *v = value;
            while (*v == ' ' || *v == '\t') v++;
            
            if (strcmp(k, "auto_backup") == 0) {
                config->auto_backup = (strcmp(v, "true") == 0);
            } else if (strcmp(k, "cache_enabled") == 0) {
                config->cache_enabled = (strcmp(v, "true") == 0);
            } else if (strcmp(k, "cache_duration") == 0) {
                config->cache_duration = atoi(v);
            } else if (strcmp(k, "verbose") == 0) {
                config->verbose = (strcmp(v, "true") == 0);
            } else if (strcmp(k, "use_color") == 0) {
                config->use_color = (strcmp(v, "true") == 0);
            }
        }
    }
    
    fclose(f);
    return config;
}

void free_config(config_t *config) {
    if (!config) return;
    
    if (config->default_templates) {
        for (int i = 0; i < config->default_count; i++) {
            free(config->default_templates[i]);
        }
        free(config->default_templates);
    }
    
    free(config);
}

int save_config(config_t *config) {
    if (!config) return 1;
    
    char *config_path = get_config_path();
    if (!config_path) return 1;
    
    char config_file[MAX_PATH_LEN];
    snprintf(config_file, sizeof(config_file), "%s/../%s", config_path, CONFIG_FILE);
    free(config_path);
    
    FILE *f = fopen(config_file, "w");
    if (!f) return 1;
    
    fprintf(f, "# gitignore configuration file\n\n");
    fprintf(f, "auto_backup=%s\n", config->auto_backup ? "true" : "false");
    fprintf(f, "cache_enabled=%s\n", config->cache_enabled ? "true" : "false");
    fprintf(f, "cache_duration=%d\n", config->cache_duration);
    fprintf(f, "verbose=%s\n", config->verbose ? "true" : "false");
    fprintf(f, "use_color=%s\n", config->use_color ? "true" : "false");
    
    fclose(f);
    return 0;
}

void apply_config(config_t *config) {
    if (!config) return;
    
    // Initialize cache if enabled
    if (config->cache_enabled) {
        init_cache();
    }
    
    // Apply color settings
    if (!config->use_color) {
        // Disable colors by clearing the macros (would need refactoring)
        // For now, just respect the setting when printing
    }
}

char* get_cache_path(void) {
    const char *home = getenv("HOME");
    if (!home) return NULL;
    
    char *path = malloc(MAX_PATH_LEN);
    if (!path) return NULL;
    
    snprintf(path, MAX_PATH_LEN, "%s/%s/%s", home, CONFIG_DIR, CACHE_DIR);
    return path;
}

char* get_backup_path(void) {
    const char *home = getenv("HOME");
    if (!home) return NULL;
    
    char *path = malloc(MAX_PATH_LEN);
    if (!path) return NULL;
    
    snprintf(path, MAX_PATH_LEN, "%s/%s/%s", home, CONFIG_DIR, BACKUP_DIR);
    return path;
}