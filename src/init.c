// init.c - FIXED: Always append and remove duplicates
#include "gitignore.h"

int init_gitignore(char **langs, int count, int dry_run) {
    if (dry_run) {
        print_info("[DRY RUN] Would create/update .gitignore");
        if (langs && count > 0) {
            printf("  Templates: ");
            for (int i = 0; i < count; i++) {
                printf("%s%s", langs[i], i < count - 1 ? ", " : "\n");
            }
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
    
    // If no languages specified, check for auto.gitignore
    if (langs == NULL || count == 0) {
        char *auto_path = get_template_path(AUTO_TEMPLATE);
        if (auto_path && file_exists(auto_path)) {
            // Use auto.gitignore - append mode
            char *auto_langs[] = {"auto"};
            free(auto_path);
            return merge_templates(auto_langs, 1, ".gitignore", 
                                 gitignore_exists ? MERGE_SMART : MERGE_REPLACE);
        }
        free(auto_path);
        
        // Create empty if file doesn't exist
        if (!gitignore_exists) {
            return create_empty_gitignore();
        }
        
        print_info(".gitignore already exists (no changes)");
        return 0;
    }
    
    // Remove duplicates and filter comments
    langs = remove_duplicates(langs, &count);
    
    if (count == 0) {
        print_warning("No valid templates after filtering");
        if (!gitignore_exists) {
            return create_empty_gitignore();
        }
        return 0;
    }
    
    // FIXED: Always use SMART merge (append + dedup)
    int result = merge_templates(langs, count, ".gitignore", 
                                gitignore_exists ? MERGE_SMART : MERGE_REPLACE);
    
    if (result == 0) {
        if (gitignore_exists) {
            print_success(".gitignore updated successfully");
        } else {
            print_success(".gitignore created successfully");
        }
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
    
    // FIXED: Default to SMART merge
    int result = merge_templates(langs, count, ".gitignore", MERGE_SMART);
    
    if (result == 0) {
        print_success("Templates added to .gitignore");
    }
    
    return result;
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
    // FIXED: Read existing patterns for deduplication
    char **existing_patterns = NULL;
    int existing_count = 0;
    
    if (strategy == MERGE_SMART && file_exists(output)) {
        FILE *existing = fopen(output, "r");
        if (existing) {
            existing_patterns = malloc(sizeof(char*) * 10000);
            char line[MAX_LINE_LEN];
            
            while (fgets(line, sizeof(line), existing)) {
                // Skip comments and empty lines
                if (!is_comment(line) && strlen(line) > 1) {
                    // Trim newline and whitespace
                    line[strcspn(line, "\n\r")] = 0;
                    
                    // Trim leading/trailing whitespace
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
    
    // Open output file
    FILE *out = fopen(output, strategy == MERGE_REPLACE ? "w" : "a");
    if (!out) {
        print_error("Could not open output file", ERR_PERMISSION_DENIED);
        
        // Free existing patterns
        if (existing_patterns) {
            for (int i = 0; i < existing_count; i++) {
                free(existing_patterns[i]);
            }
            free(existing_patterns);
        }
        return 1;
    }
    
    // Add header only for new files
    if (strategy == MERGE_REPLACE) {
        fprintf(out, "# Generated by gitignore tool v%s\n", VERSION);
    } else {
        // Add separator for appended content
        fprintf(out, "\n# Added by gitignore tool\n");
    }
    
    for (int i = 0; i < count; i++) {
        const char *template_content = NULL;
        char *custom_path = NULL;
        FILE *tmpl = NULL;
        
        // Priority 1: Check custom template
        custom_path = get_template_path(langs[i]);
        if (custom_path && file_exists(custom_path)) {
            tmpl = fopen(custom_path, "r");
            if (g_config && g_config->verbose) {
                printf("  Using custom template: %s\n", langs[i]);
            }
        }
        
        // Priority 2: Check built-in template
        if (!tmpl) {
            template_content = get_builtin_template(langs[i]);
            if (template_content && g_config && g_config->verbose) {
                printf("  Using built-in template: %s\n", langs[i]);
            }
        }
        
        if (tmpl || template_content) {
            fprintf(out, "\n# ===== %s =====\n", langs[i]);
            
            if (tmpl) {
                // Read from custom file
                char line[MAX_LINE_LEN];
                while (fgets(line, sizeof(line), tmpl)) {
                    // Check for duplicates in SMART mode
                    if (strategy == MERGE_SMART && !is_comment(line)) {
                        char clean_line[MAX_LINE_LEN];
                        strncpy(clean_line, line, sizeof(clean_line));
                        clean_line[strcspn(clean_line, "\n\r")] = 0;
                        
                        // Trim whitespace
                        char *trimmed = clean_line;
                        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
                        
                        // Check if already exists
                        int is_duplicate = 0;
                        for (int j = 0; j < existing_count; j++) {
                            if (strcmp(trimmed, existing_patterns[j]) == 0) {
                                is_duplicate = 1;
                                break;
                            }
                        }
                        
                        if (is_duplicate) continue;
                    }
                    
                    fputs(line, out);
                }
                fclose(tmpl);
            } else {
                // Write built-in content
                const char *line_start = template_content;
                const char *line_end;
                
                while (*line_start) {
                    line_end = strchr(line_start, '\n');
                    if (!line_end) line_end = line_start + strlen(line_start);
                    
                    int line_len = line_end - line_start;
                    char line[MAX_LINE_LEN];
                    strncpy(line, line_start, line_len);
                    line[line_len] = '\0';
                    
                    // Check for duplicates in SMART mode
                    if (strategy == MERGE_SMART && line_len > 0 && line[0] != '#') {
                        // Trim whitespace
                        char *trimmed = line;
                        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
                        
                        // Check if already exists
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
            }
            
            if (g_config && g_config->verbose) {
                printf("  %s+%s %s\n", COLOR_GREEN, COLOR_RESET, langs[i]);
            }
        } else {
            print_warning("Template not found, skipping");
            printf("  %s\n", langs[i]);
        }
        
        if (custom_path) free(custom_path);
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