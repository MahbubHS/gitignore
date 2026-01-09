// init.c - Enhanced with append mode and merge strategies
#include "gitignore.h"

int init_gitignore(char **langs, int count, int dry_run) {
    if (dry_run) {
        print_info("[DRY RUN] Would create .gitignore");
        if (langs && count > 0) {
            printf("  Templates: ");
            for (int i = 0; i < count; i++) {
                printf("%s%s", langs[i], i < count - 1 ? ", " : "\n");
            }
        }
        return 0;
    }
    
    // Auto backup if enabled
    if (g_config && g_config->auto_backup && file_exists(".gitignore")) {
        if (g_config->verbose) {
            print_info("Auto-backup enabled, creating backup...");
        }
        backup_gitignore();
    }
    
    // If no languages specified, check for auto.gitignore
    if (langs == NULL || count == 0) {
        char *auto_path = get_template_path(AUTO_TEMPLATE);
        if (auto_path && file_exists(auto_path)) {
            // Copy auto.gitignore to .gitignore
            FILE *src = fopen(auto_path, "r");
            FILE *dst = fopen(".gitignore", "w");
            
            if (!src || !dst) {
                print_error("Could not create .gitignore", ERR_PERMISSION_DENIED);
                if (src) fclose(src);
                if (dst) fclose(dst);
                free(auto_path);
                return 1;
            }
            
            char buffer[4096];
            size_t bytes;
            while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
                fwrite(buffer, 1, bytes, dst);
            }
            
            fclose(src);
            fclose(dst);
            free(auto_path);
            
            print_success(".gitignore created from auto.gitignore");
            return 0;
        }
        free(auto_path);
        
        return create_empty_gitignore();
    }
    
    // Remove duplicates and filter comments
    langs = remove_duplicates(langs, &count);
    
    if (count == 0) {
        print_warning("No valid templates after filtering");
        return create_empty_gitignore();
    }
    
    // Merge templates
    int result = merge_templates(langs, count, ".gitignore", MERGE_REPLACE);
    
    if (result == 0) {
        print_success(".gitignore created successfully");
        printf("  Templates: ");
        for (int i = 0; i < count; i++) {
            printf("%s%s%s%s", COLOR_GREEN, langs[i], COLOR_RESET,
                   i < count - 1 ? ", " : "\n");
        }
    }
    
    return result;
}

int append_gitignore(char **langs, int count, merge_strategy_t strategy, int dry_run) {
    if (!file_exists(".gitignore")) {
        print_error(".gitignore does not exist. Use 'init' to create one", ERR_FILE_NOT_FOUND);
        return 1;
    }
    
    if (dry_run) {
        print_info("[DRY RUN] Would append to .gitignore");
        printf("  Strategy: %s\n", 
               strategy == MERGE_APPEND ? "append" : 
               strategy == MERGE_REPLACE ? "replace" : "smart");
        return 0;
    }
    
    // Auto backup if enabled
    if (g_config && g_config->auto_backup) {
        if (g_config->verbose) {
            print_info("Auto-backup enabled, creating backup...");
        }
        backup_gitignore();
    }
    
    // Remove duplicates
    langs = remove_duplicates(langs, &count);
    
    if (strategy == MERGE_APPEND) {
        // Simple append mode
        FILE *f = fopen(".gitignore", "a");
        if (!f) {
            print_error("Could not open .gitignore", ERR_PERMISSION_DENIED);
            return 1;
        }
        
        fprintf(f, "\n# Appended by gitignore tool\n");
        
        for (int i = 0; i < count; i++) {
            char *template_path = get_template_path(langs[i]);
            FILE *tmpl = NULL;
            
            if (template_path && file_exists(template_path)) {
                tmpl = fopen(template_path, "r");
            }
            
            if (!tmpl) {
                char builtin_path[MAX_PATH_LEN];
                snprintf(builtin_path, sizeof(builtin_path), "templates/%s.gitignore", langs[i]);
                tmpl = fopen(builtin_path, "r");
            }
            
            if (tmpl) {
                fprintf(f, "\n# === %s ===\n", langs[i]);
                char line[MAX_LINE_LEN];
                while (fgets(line, sizeof(line), tmpl)) {
                    fputs(line, f);
                }
                fclose(tmpl);
            }
            
            if (template_path) free(template_path);
        }
        
        fclose(f);
        print_success("Templates appended to .gitignore");
        
    } else {
        // Smart merge - remove duplicates across entire file
        return merge_templates(langs, count, ".gitignore", MERGE_SMART);
    }
    
    return 0;
}

int create_empty_gitignore(void) {
    FILE *f = fopen(".gitignore", "w");
    if (!f) {
        print_error("Could not create .gitignore", ERR_PERMISSION_DENIED);
        return 1;
    }
    
    fprintf(f, "# .gitignore\n");
    fprintf(f, "# Add your ignore patterns here\n\n");
    
    fclose(f);
    print_success(".gitignore created (empty)");
    return 0;
}

int merge_templates(char **langs, int count, const char *output, merge_strategy_t strategy) {
    FILE *out = NULL;
    char **existing_patterns = NULL;
    int existing_count = 0;
    
    // If smart merge, read existing patterns first
    if (strategy == MERGE_SMART && file_exists(output)) {
        FILE *existing = fopen(output, "r");
        if (existing) {
            existing_patterns = malloc(sizeof(char*) * 1000);
            char line[MAX_LINE_LEN];
            
            while (fgets(line, sizeof(line), existing)) {
                // Skip comments and empty lines
                if (!is_comment(line) && strlen(line) > 1) {
                    // Trim newline
                    line[strcspn(line, "\n")] = 0;
                    existing_patterns[existing_count++] = strdup(line);
                }
            }
            fclose(existing);
        }
    }
    
    out = fopen(output, "w");
    if (!out) {
        print_error("Could not create output file", ERR_PERMISSION_DENIED);
        return 1;
    }
    
    fprintf(out, "# Generated by gitignore tool v%s\n", VERSION);
    fprintf(out, "# https://github.com/mahbubhs/gitignore\n\n");
    
    for (int i = 0; i < count; i++) {
        char *template_path = get_template_path(langs[i]);
        FILE *tmpl = NULL;
        
        if (template_path && file_exists(template_path)) {
            tmpl = fopen(template_path, "r");
        }
        
        if (!tmpl) {
            char builtin_path[MAX_PATH_LEN];
            snprintf(builtin_path, sizeof(builtin_path), "templates/%s.gitignore", langs[i]);
            tmpl = fopen(builtin_path, "r");
        }
        
        if (tmpl) {
            fprintf(out, "# ===== %s =====\n", langs[i]);
            
            char line[MAX_LINE_LEN];
            while (fgets(line, sizeof(line), tmpl)) {
                // Smart merge: skip duplicates
                if (strategy == MERGE_SMART && !is_comment(line)) {
                    char clean_line[MAX_LINE_LEN];
                    strncpy(clean_line, line, sizeof(clean_line));
                    clean_line[strcspn(clean_line, "\n")] = 0;
                    
                    int is_duplicate = 0;
                    for (int j = 0; j < existing_count; j++) {
                        if (strcmp(clean_line, existing_patterns[j]) == 0) {
                            is_duplicate = 1;
                            break;
                        }
                    }
                    
                    if (is_duplicate) continue;
                }
                
                fputs(line, out);
            }
            
            fprintf(out, "\n");
            fclose(tmpl);
            
            if (g_config && g_config->verbose) {
                printf("  %s+%s %s\n", COLOR_GREEN, COLOR_RESET, langs[i]);
            }
        } else {
            print_warning("Template not found, skipping");
            printf("  %s\n", langs[i]);
        }
        
        if (template_path) free(template_path);
    }
    
    fclose(out);
    
    // Free existing patterns
    if (existing_patterns) {
        for (int i = 0; i < existing_count; i++) {
            free(existing_patterns[i]);
        }
        free(existing_patterns);
    }
    
    return 0;
}