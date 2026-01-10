// features.c - FIXED with pattern deduplication
#include "gitignore.h"

// List available templates
int list_templates(const char *filter, int show_local, int show_builtin) {
    print_info("Available templates:\n");
    
    int count = 0;
    
    // List custom templates
    if (show_local) {
        char *config_path = get_config_path();
        if (config_path) {
            DIR *dir = opendir(config_path);
            if (dir) {
                printf("\n%s%sCustom Templates:%s\n", COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
                
                struct dirent *entry;
                while ((entry = readdir(dir)) != NULL) {
                    if (strstr(entry->d_name, ".gitignore")) {
                        char *name = strdup(entry->d_name);
                        char *dot = strstr(name, ".gitignore");
                        if (dot) *dot = '\0';
                        
                        if (!filter || strstr(name, filter)) {
                            printf("  %s•%s %s%s%s (custom)\n", COLOR_GREEN, COLOR_RESET, 
                                   COLOR_BOLD, name, COLOR_RESET);
                            count++;
                        }
                        free(name);
                    }
                }
                closedir(dir);
            }
            free(config_path);
        }
    }
    
    // List built-in templates
    if (show_builtin) {
        printf("\n%s%sBuilt-in Templates:%s\n", COLOR_BOLD, COLOR_YELLOW, COLOR_RESET);
        
        const char **builtins = get_builtin_template_names();
        for (int i = 0; builtins[i] != NULL; i++) {
            if (!filter || strstr(builtins[i], filter)) {
                printf("  %s•%s %s\n", COLOR_GREEN, COLOR_RESET, builtins[i]);
                count++;
            }
        }
    }
    
    printf("\n%sTotal: %d template(s)%s\n", COLOR_BOLD, count, COLOR_RESET);
    return 0;
}

// Show template content
int show_template(const char *lang) {
    char *template_path = get_template_path(lang);
    FILE *f = NULL;
    const char *builtin_content = NULL;
    
    // Check custom first
    if (template_path && file_exists(template_path)) {
        f = fopen(template_path, "r");
    }
    
    // Check built-in
    if (!f) {
        builtin_content = get_builtin_template(lang);
    }
    
    if (!f && !builtin_content) {
        print_error("Template not found", ERR_FILE_NOT_FOUND);
        if (template_path) free(template_path);
        return 1;
    }
    
    printf("%s%s=== %s ===%s\n", COLOR_BOLD, COLOR_CYAN, lang, COLOR_RESET);
    
    if (f) {
        char line[MAX_LINE_LEN];
        while (fgets(line, sizeof(line), f)) {
            printf("%s", line);
        }
        fclose(f);
    } else {
        printf("%s", builtin_content);
    }
    
    if (template_path) free(template_path);
    return 0;
}

// Auto-detect project type
int auto_detect(int dry_run) {
    print_info("Auto-detecting project type...");
    
    char **detected_langs = malloc(sizeof(char*) * MAX_LANGS);
    int count = 0;
    
    if (detect_project_type(&detected_langs, &count) != 0) {
        print_error("Could not detect project type", ERR_INVALID_ARGUMENT);
        free(detected_langs);
        return 1;
    }
    
    if (count == 0) {
        print_warning("No project files detected. Creating empty .gitignore");
        free(detected_langs);
        return create_empty_gitignore();
    }
    
    printf("\n%sDetected:%s ", COLOR_BOLD, COLOR_RESET);
    for (int i = 0; i < count; i++) {
        printf("%s%s%s%s", COLOR_GREEN, detected_langs[i], COLOR_RESET, 
               i < count - 1 ? ", " : "\n");
    }
    
    if (dry_run) {
        print_info("[DRY RUN] Would create .gitignore with detected templates");
        free(detected_langs);
        return 0;
    }
    
    int result = init_gitignore(detected_langs, count, 0);
    free(detected_langs);
    return result;
}

int detect_project_type(char ***langs, int *count) {
    *count = 0;
    
    struct {
        const char *file;
        const char *lang;
    } indicators[] = {
        {"package.json", "node"},
        {"requirements.txt", "python"},
        {"setup.py", "python"},
        {"Pipfile", "python"},
        {"Cargo.toml", "rust"},
        {"go.mod", "go"},
        {"pom.xml", "java"},
        {"build.gradle", "java"},
        {"Gemfile", "ruby"},
        {"composer.json", "php"},
        {"*.csproj", "visualstudio"},
        {".vscode", "vscode"},
        {".idea", "intellij"},
        {"CMakeLists.txt", "c"},
        {"Makefile", "c"},
        {NULL, NULL}
    };
    
    for (int i = 0; indicators[i].file != NULL; i++) {
        if (file_exists(indicators[i].file)) {
            // Check for duplicates
            int duplicate = 0;
            for (int j = 0; j < *count; j++) {
                if (strcmp((*langs)[j], indicators[i].lang) == 0) {
                    duplicate = 1;
                    break;
                }
            }
            
            if (!duplicate) {
                (*langs)[(*count)++] = (char*)indicators[i].lang;
            }
        }
    }
    
    // Always add OS-specific
    const char *os = NULL;
    #ifdef __APPLE__
        os = "macos";
    #elif __linux__
        os = "linux";
    #elif _WIN32
        os = "windows";
    #endif
    
    if (os) {
        (*langs)[(*count)++] = (char*)os;
    }
    
    return 0;
}

// FIXED: Add multiple patterns with deduplication
int add_patterns(char **patterns, int count, int dry_run) {
    if (!patterns || count == 0) {
        print_error("No patterns provided", ERR_INVALID_ARGUMENT);
        return 1;
    }
    
    if (dry_run) {
        printf("%s[DRY RUN]%s Would add %d pattern(s):\n", 
               COLOR_YELLOW, COLOR_RESET, count);
        for (int i = 0; i < count; i++) {
            printf("  %s+%s %s%s%s\n", COLOR_GREEN, COLOR_RESET,
                   COLOR_BOLD, patterns[i], COLOR_RESET);
        }
        return 0;
    }
    
    // Read existing patterns
    char **existing_patterns = NULL;
    int existing_count = 0;
    
    if (file_exists(".gitignore")) {
        FILE *f = fopen(".gitignore", "r");
        if (f) {
            existing_patterns = malloc(sizeof(char*) * 10000);
            char line[MAX_LINE_LEN];
            
            while (fgets(line, sizeof(line), f)) {
                if (!is_comment(line) && strlen(line) > 1) {
                    line[strcspn(line, "\n\r")] = 0;
                    char *trimmed = line;
                    while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
                    
                    if (strlen(trimmed) > 0) {
                        existing_patterns[existing_count++] = strdup(trimmed);
                    }
                }
            }
            fclose(f);
        }
    }
    
    // Open for appending
    FILE *f = fopen(".gitignore", "a");
    if (!f) {
        print_error("Could not open .gitignore for writing", ERR_PERMISSION_DENIED);
        
        if (existing_patterns) {
            for (int i = 0; i < existing_count; i++) {
                free(existing_patterns[i]);
            }
            free(existing_patterns);
        }
        return 1;
    }
    
    // Check if we need newline before adding
    if (existing_count > 0) {
        fseek(f, -1, SEEK_END);
        char last_char = fgetc(f);
        if (last_char != '\n') {
            fprintf(f, "\n");
        }
    }
    
    // Add comment header for multiple patterns
    if (count > 1) {
        fprintf(f, "\n# Added by gitignore tool\n");
    } else if (count == 1 && existing_count > 0) {
        fprintf(f, "\n");
    }
    
    // Add patterns (skipping duplicates)
    int added_count = 0;
    for (int i = 0; i < count; i++) {
        // Check if pattern already exists
        int is_duplicate = 0;
        for (int j = 0; j < existing_count; j++) {
            if (strcmp(patterns[i], existing_patterns[j]) == 0) {
                is_duplicate = 1;
                if (g_config && g_config->verbose) {
                    printf("  %sSkipping duplicate:%s %s\n", 
                           COLOR_YELLOW, COLOR_RESET, patterns[i]);
                }
                break;
            }
        }
        
        if (!is_duplicate) {
            fprintf(f, "%s\n", patterns[i]);
            added_count++;
        }
    }
    
    fclose(f);
    
    // Free existing patterns
    if (existing_patterns) {
        for (int i = 0; i < existing_count; i++) {
            free(existing_patterns[i]);
        }
        free(existing_patterns);
    }
    
    // Print success message
    if (added_count == 0) {
        print_warning("All patterns already exist in .gitignore");
    } else if (added_count == 1) {
        print_success("Pattern added to .gitignore");
        for (int i = 0; i < count; i++) {
            int is_dup = 0;
            for (int j = 0; j < existing_count; j++) {
                if (strcmp(patterns[i], existing_patterns[j]) == 0) {
                    is_dup = 1;
                    break;
                }
            }
            if (!is_dup) {
                printf("  %s+%s %s\n", COLOR_GREEN, COLOR_RESET, patterns[i]);
            }
        }
    } else {
        print_success("Patterns added to .gitignore");
        printf("  %sAdded: %d pattern(s)%s\n", COLOR_BOLD, added_count, COLOR_RESET);
        for (int i = 0; i < count; i++) {
            int is_dup = 0;
            for (int j = 0; j < existing_count; j++) {
                if (strcmp(patterns[i], existing_patterns[j]) == 0) {
                    is_dup = 1;
                    break;
                }
            }
            if (!is_dup) {
                printf("  %s+%s %s\n", COLOR_GREEN, COLOR_RESET, patterns[i]);
            }
        }
    }
    
    return 0;
}

// Interactive mode
int interactive_mode(void) {
    printf("%s%s=== Interactive Mode ===%s\n\n", COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
    printf("Available templates:\n\n");
    
    const char **templates = get_builtin_template_names();
    
    char *selected[MAX_LANGS];
    int count = 0;
    
    for (int i = 0; templates[i] != NULL; i++) {
        printf("  %d) %s\n", i + 1, templates[i]);
    }
    
    printf("\nEnter template numbers separated by spaces (0 to finish): ");
    
    char input[1024];
    if (fgets(input, sizeof(input), stdin)) {
        char *token = strtok(input, " \n");
        while (token != NULL) {
            int idx = atoi(token);
            if (idx == 0) break;
            if (idx > 0 && idx <= 50 && templates[idx - 1] != NULL) {
                selected[count++] = (char*)templates[idx - 1];
            }
            token = strtok(NULL, " \n");
        }
    }
    
    if (count == 0) {
        print_warning("No templates selected");
        return 0;
    }
    
    printf("\n%sSelected:%s ", COLOR_BOLD, COLOR_RESET);
    for (int i = 0; i < count; i++) {
        printf("%s%s%s%s", COLOR_GREEN, selected[i], COLOR_RESET,
               i < count - 1 ? ", " : "\n");
    }
    
    printf("\nCreate/update .gitignore? (y/n): ");
    char confirm[10];
    if (fgets(confirm, sizeof(confirm), stdin)) {
        if (confirm[0] == 'y' || confirm[0] == 'Y') {
            return init_gitignore(selected, count, 0);
        }
    }
    
    print_info("Cancelled");
    return 0;
}