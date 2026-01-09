// features.c - New features implementation
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
                            printf("  %s•%s %s%s%s\n", COLOR_GREEN, COLOR_RESET, 
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
        
        const char *builtins[] = {
            "python", "node", "rust", "c", "cpp", "java", "go", 
            "ruby", "php", "swift", "kotlin", "typescript",
            "vscode", "vim", "intellij", "macos", "linux", "windows",
            NULL
        };
        
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
    
    if (template_path && file_exists(template_path)) {
        f = fopen(template_path, "r");
    }
    
    if (!f) {
        // Try built-in
        char builtin_path[MAX_PATH_LEN];
        snprintf(builtin_path, sizeof(builtin_path), "templates/%s.gitignore", lang);
        f = fopen(builtin_path, "r");
    }
    
    if (!f) {
        print_error("Template not found", ERR_FILE_NOT_FOUND);
        if (template_path) free(template_path);
        return 1;
    }
    
    printf("%s%s=== %s ===%s\n", COLOR_BOLD, COLOR_CYAN, lang, COLOR_RESET);
    
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }
    
    fclose(f);
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

// Add pattern to .gitignore (Feature #26)
int add_pattern(const char *pattern, int dry_run) {
    if (dry_run) {
        printf("%s[DRY RUN]%s Would add pattern: %s%s%s\n", 
               COLOR_YELLOW, COLOR_RESET, COLOR_BOLD, pattern, COLOR_RESET);
        return 0;
    }
    
    // Check if .gitignore exists
    FILE *f = fopen(".gitignore", "r");
    int exists = (f != NULL);
    if (f) fclose(f);
    
    // Open for appending
    f = fopen(".gitignore", "a");
    if (!f) {
        print_error("Could not open .gitignore for writing", ERR_PERMISSION_DENIED);
        return 1;
    }
    
    // Add newline if file exists and doesn't end with one
    if (exists) {
        fseek(f, -1, SEEK_END);
        char last_char = fgetc(f);
        if (last_char != '\n') {
            fprintf(f, "\n");
        }
    }
    
    // Add the pattern
    fprintf(f, "%s\n", pattern);
    fclose(f);
    
    print_success("Pattern added to .gitignore");
    printf("  %s+%s %s\n", COLOR_GREEN, COLOR_RESET, pattern);
    
    return 0;
}

// Interactive mode
int interactive_mode(void) {
    printf("%s%s=== Interactive Mode ===%s\n\n", COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
    printf("Available templates:\n\n");
    
    const char *templates[] = {
        "python", "node", "rust", "c", "cpp", "java", "go",
        "vscode", "vim", "macos", "linux", "windows", NULL
    };
    
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
            if (idx > 0 && idx <= 20 && templates[idx - 1] != NULL) {
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
    
    printf("\nCreate .gitignore? (y/n): ");
    char confirm[10];
    if (fgets(confirm, sizeof(confirm), stdin)) {
        if (confirm[0] == 'y' || confirm[0] == 'Y') {
            return init_gitignore(selected, count, 0);
        }
    }
    
    print_info("Cancelled");
    return 0;
}