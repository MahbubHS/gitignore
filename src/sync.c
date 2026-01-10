// sync.c - FIXED: Download from GitHub and merge smartly
#include "gitignore.h"

// Callback for curl to write data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct {
        char *data;
        size_t size;
    } *mem = (void *)userp;
    
    char *ptr = realloc(mem->data, mem->size + realsize + 1);
    if (!ptr) {
        print_error("Out of memory", ERR_OUT_OF_MEMORY);
        return 0;
    }
    
    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;
    
    return realsize;
}

int download_template(const char *lang, char *buffer, size_t *size) {
    // Check cache first
    char *cached_content = NULL;
    if (get_cached_template(lang, &cached_content) == 0) {
        strncpy(buffer, cached_content, *size - 1);
        buffer[*size - 1] = '\0';
        *size = strlen(cached_content);
        free(cached_content);
        return 0;
    }
    
    CURL *curl;
    CURLcode res;
    char url[512];
    
    // Construct GitHub raw URL
    snprintf(url, sizeof(url), "%s%s.gitignore", GITHUB_RAW_URL, lang);
    
    struct {
        char *data;
        size_t size;
    } chunk = {0};
    
    curl = curl_easy_init();
    if (!curl) {
        print_error("Could not initialize curl", ERR_CURL_INIT_FAILED);
        return 1;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "gitignore-tool/2.0");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    
    res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        if (g_config && !g_config->quiet) {
            fprintf(stderr, "%sError downloading %s: %s%s\n", 
                    COLOR_RED, lang, curl_easy_strerror(res), COLOR_RESET);
        }
        curl_easy_cleanup(curl);
        if (chunk.data) free(chunk.data);
        return 1;
    }
    
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    curl_easy_cleanup(curl);
    
    if (response_code != 200) {
        if (g_config && !g_config->quiet) {
            fprintf(stderr, "%sTemplate '%s' not found on GitHub (HTTP %ld)%s\n", 
                    COLOR_RED, lang, response_code, COLOR_RESET);
        }
        if (chunk.data) free(chunk.data);
        return 1;
    }
    
    if (chunk.data && chunk.size > 0) {
        strncpy(buffer, chunk.data, *size - 1);
        buffer[*size - 1] = '\0';
        *size = chunk.size;
        
        // Cache the downloaded template
        cache_template(lang, chunk.data);
        
        free(chunk.data);
        return 0;
    }
    
    if (chunk.data) free(chunk.data);
    return 1;
}

int sync_gitignore(char **langs, int count, int dry_run) {
    if (dry_run) {
        print_info("[DRY RUN] Would sync templates from GitHub");
        printf("  Templates: ");
        for (int i = 0; i < count; i++) {
            printf("%s%s", langs[i], i < count - 1 ? ", " : "\n");
        }
        return 0;
    }
    
    // Check if .gitignore exists
    int gitignore_exists = file_exists(".gitignore");
    
    // Auto backup if enabled and file exists
    if (g_config && g_config->auto_backup && gitignore_exists) {
        if (g_config->verbose) {
            print_info("Auto-backup enabled, creating backup...");
        }
        backup_gitignore();
    }
    
    // Remove duplicates and filter comments
    langs = remove_duplicates(langs, &count);
    
    if (count == 0) {
        print_warning("No valid templates after filtering");
        return 1;
    }
    
    // FIXED: Read existing patterns for deduplication
    char **existing_patterns = NULL;
    int existing_count = 0;
    
    if (gitignore_exists) {
        FILE *existing = fopen(".gitignore", "r");
        if (existing) {
            existing_patterns = malloc(sizeof(char*) * 10000);
            char line[MAX_LINE_LEN];
            
            while (fgets(line, sizeof(line), existing)) {
                if (!is_comment(line) && strlen(line) > 1) {
                    line[strcspn(line, "\n\r")] = 0;
                    char *trimmed = line;
                    while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
                    
                    if (strlen(trimmed) > 0) {
                        existing_patterns[existing_count++] = strdup(trimmed);
                    }
                }
            }
            fclose(existing);
        }
    }
    
    // Open for append if exists, create if not
    FILE *out = fopen(".gitignore", gitignore_exists ? "a" : "w");
    if (!out) {
        print_error("Could not create .gitignore", ERR_PERMISSION_DENIED);
        
        if (existing_patterns) {
            for (int i = 0; i < existing_count; i++) {
                free(existing_patterns[i]);
            }
            free(existing_patterns);
        }
        return 1;
    }
    
    // Add header
    if (!gitignore_exists) {
        fprintf(out, "# Generated by gitignore tool v%s\n", VERSION);
        fprintf(out, "# Synced from https://github.com/github/gitignore\n\n");
    } else {
        fprintf(out, "\n# Synced from GitHub by gitignore tool\n");
    }
    
    if (!g_config || !g_config->quiet) {
        printf("%sSyncing templates from GitHub...%s\n", COLOR_BOLD, COLOR_RESET);
    }
    
    int success_count = 0;
    
    for (int i = 0; i < count; i++) {
        char buffer[65536];
        size_t size = sizeof(buffer);
        
        if (!g_config || !g_config->quiet) {
            print_progress(langs[i], i + 1, count);
        }
        
        if (download_template(langs[i], buffer, &size) == 0) {
            fprintf(out, "\n# ===== %s =====\n", langs[i]);
            
            // Write line by line, checking for duplicates
            const char *line_start = buffer;
            const char *line_end;
            
            while (*line_start) {
                line_end = strchr(line_start, '\n');
                if (!line_end) line_end = line_start + strlen(line_start);
                
                int line_len = line_end - line_start;
                char line[MAX_LINE_LEN];
                strncpy(line, line_start, line_len);
                line[line_len] = '\0';
                
                // Check for duplicates
                if (gitignore_exists && line_len > 0 && line[0] != '#') {
                    char *trimmed = line;
                    while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
                    
                    int is_duplicate = 0;
                    for (int j = 0; j < existing_count; j++) {
                        if (strcmp(trimmed, existing_patterns[j]) == 0) {
                            is_duplicate = 1;
                            break;
                        }
                    }
                    
                    if (!is_duplicate) {
                        fprintf(out, "%s\n", line);
                    }
                } else {
                    fprintf(out, "%s\n", line);
                }
                
                line_start = *line_end ? line_end + 1 : line_end;
            }
            
            success_count++;
            
            if (g_config && !g_config->quiet) {
                printf("  %s✓%s %s\n", COLOR_GREEN, COLOR_RESET, langs[i]);
            }
        } else {
            if (g_config && !g_config->quiet) {
                printf("  %s✗%s %s (not found or network error)\n", 
                       COLOR_RED, COLOR_RESET, langs[i]);
            }
        }
    }
    
    fclose(out);
    
    // Free existing patterns
    if (existing_patterns) {
        for (int i = 0; i < existing_count; i++) {
            free(existing_patterns[i]);
        }
        free(existing_patterns);
    }
    
    if (success_count > 0) {
        if (gitignore_exists) {
            print_success(".gitignore updated successfully");
        } else {
            print_success(".gitignore synced successfully");
        }
        printf("  %s%d/%d%s templates downloaded\n", 
               COLOR_BOLD, success_count, count, COLOR_RESET);
    } else {
        print_error("No templates could be downloaded", ERR_NETWORK_ERROR);
        return 1;
    }
    
    return 0;
}