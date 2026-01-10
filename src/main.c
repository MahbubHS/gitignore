// main.c - Enhanced main with FIXED pattern handling
#include "gitignore.h"

config_t *g_config = NULL;

int main(int argc, char *argv[]) {
    // Load configuration
    g_config = load_config();
    apply_config(g_config);
    
    if (argc == 1) {
        show_help();
        free_config(g_config);
        return 0;
    }

    int result = parse_flags(argc, argv);
    free_config(g_config);
    return result;
}

int parse_flags(int argc, char *argv[]) {
    const char *flag = argv[1];
    int dry_run = 0;
    merge_strategy_t strategy = MERGE_SMART;
    
    // Check for dry-run flag anywhere in args
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--dry-run") == 0) {
            dry_run = 1;
            // Remove this arg by shifting
            for (int j = i; j < argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            argc--;
            i--;
        } else if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-V") == 0) {
            g_config->verbose = 1;
            for (int j = i; j < argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            argc--;
            i--;
        } else if (strcmp(argv[i], "--quiet") == 0 || strcmp(argv[i], "-q") == 0) {
            g_config->quiet = 1;
            for (int j = i; j < argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            argc--;
            i--;
        }
    }
    
    // Re-get flag after potential shifts
    if (argc < 2) {
        show_help();
        return 0;
    }
    flag = argv[1];
    
    // Help flag
    if (strcmp(flag, "-h") == 0 || strcmp(flag, "--help") == 0) {
        show_help();
        return 0;
    }
    
    // Version flag
    if (strcmp(flag, "-v") == 0 || strcmp(flag, "--version") == 0) {
        show_version();
        return 0;
    }
    
    // List templates
    if (strcmp(flag, "list") == 0 || strcmp(flag, "--list") == 0 || strcmp(flag, "-l") == 0) {
        int show_local = 0, show_builtin = 0;
        const char *filter = NULL;
        
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "--local") == 0) show_local = 1;
            else if (strcmp(argv[i], "--builtin") == 0) show_builtin = 1;
            else filter = argv[i];
        }
        
        if (!show_local && !show_builtin) {
            show_local = show_builtin = 1;
        }
        
        return list_templates(filter, show_local, show_builtin);
    }
    
    // Show template
    if (strcmp(flag, "show") == 0 || strcmp(flag, "cat") == 0) {
        if (argc < 3) {
            print_error("show requires a template name", ERR_INVALID_ARGUMENT);
            return 1;
        }
        return show_template(argv[2]);
    }
    
    // Auto-detect
    if (strcmp(flag, "auto") == 0 || strcmp(flag, "--auto") == 0) {
        return auto_detect(dry_run);
    }
    
    // Interactive mode - FIXED: Added -t flag
    if (strcmp(flag, "interactive") == 0 || strcmp(flag, "--interactive") == 0 || 
        strcmp(flag, "-I") == 0 || strcmp(flag, "-t") == 0) {
        return interactive_mode();
    }
    
    // Global gitignore
    if (strcmp(flag, "global") == 0) {
        if (argc < 3) {
            print_error("global requires a subcommand (init/add)", ERR_INVALID_ARGUMENT);
            return 1;
        }
        
        if (strcmp(argv[2], "init") == 0) {
            return global_init();
        } else if (strcmp(argv[2], "add") == 0) {
            if (argc < 4) {
                print_error("global add requires template names", ERR_INVALID_ARGUMENT);
                return 1;
            }
            return global_add(&argv[3], argc - 3);
        }
    }
    
    // Backup commands
    if (strcmp(flag, "backup") == 0) {
        return backup_gitignore();
    }
    
    if (strcmp(flag, "restore") == 0) {
        const char *backup_name = argc > 2 ? argv[2] : NULL;
        return restore_gitignore(backup_name);
    }
    
    if (strcmp(flag, "backups") == 0 || strcmp(flag, "history") == 0) {
        return list_backups();
    }
    
    // Cache commands
    if (strcmp(flag, "cache") == 0) {
        if (argc < 3) {
            print_error("cache requires a subcommand (clear)", ERR_INVALID_ARGUMENT);
            return 1;
        }
        
        if (strcmp(argv[2], "clear") == 0) {
            return clear_cache();
        }
    }
    
    // FIXED: --add flag now only for conflicting names
    if (strcmp(flag, "-a") == 0 || strcmp(flag, "--add") == 0) {
        if (argc < 3) {
            print_error("--add requires at least one pattern", ERR_INVALID_ARGUMENT);
            return 1;
        }
        // Add all remaining arguments as patterns (multiple patterns support)
        return add_patterns(&argv[2], argc - 2, dry_run);
    }
    
    // Init flag
    if (strcmp(flag, "init") == 0 || strcmp(flag, "--init") == 0 || strcmp(flag, "-i") == 0) {
        if (argc == 2) {
            return init_gitignore(NULL, 0, dry_run);
        } else {
            return init_gitignore(&argv[2], argc - 2, dry_run);
        }
    }
    
    // Append/Update mode
    if (strcmp(flag, "append") == 0 || strcmp(flag, "update") == 0) {
        if (argc < 3) {
            print_error("append/update requires template names", ERR_INVALID_ARGUMENT);
            return 1;
        }
        
        strategy = strcmp(flag, "append") == 0 ? MERGE_APPEND : MERGE_SMART;
        return append_gitignore(&argv[2], argc - 2, strategy, dry_run);
    }
    
    // Sync flag
    if (strcmp(flag, "sync") == 0 || strcmp(flag, "--sync") == 0 || strcmp(flag, "-s") == 0) {
        if (argc == 2) {
            print_error("sync requires at least one language argument", ERR_INVALID_ARGUMENT);
            return 1;
        }
        return sync_gitignore(&argv[2], argc - 2, dry_run);
    }
    
    // FIXED: Default behavior - check if it's a command name
    // If first arg is a command name, user must use --add to add it as pattern
    if (is_command_name(flag)) {
        print_error("Ambiguous argument - did you mean a command or pattern?", ERR_INVALID_ARGUMENT);
        printf("\nIf you want to use the '%s' command, check: gitignore --help\n", flag);
        printf("If you want to add '%s' as a pattern, use: gitignore --add %s\n", flag, flag);
        return 1;
    }
    
    // FIXED: Treat all remaining args as patterns (supports multiple patterns)
    // gitignore node_modules/ *.log .env
    return add_patterns(&argv[1], argc - 1, dry_run);
}

// FIXED: Check if name conflicts with a command
int is_command_name(const char *name) {
    const char *commands[] = {
        "init", "sync", "list", "show", "cat", "auto", "interactive",
        "append", "update", "global", "backup", "restore", "backups",
        "history", "cache", NULL
    };
    
    for (int i = 0; commands[i] != NULL; i++) {
        if (strcmp(name, commands[i]) == 0) {
            return 1;
        }
    }
    
    return 0;
}

// Helper to determine if string is a path/pattern vs language name
int is_path_or_pattern(const char *name) {
    // Check for path indicators
    if (strchr(name, '/') || strchr(name, '.') || strchr(name, '*')) {
        return 1;
    }
    
    // Check for known language/editor names
    if (is_language_name(name)) {
        return 0;
    }
    
    // If file/directory exists, it's likely a path
    struct stat st;
    if (stat(name, &st) == 0) {
        return 1;
    }
    
    // Default to pattern if ambiguous
    return 1;
}

int is_language_name(const char *name) {
    // Common language/editor names
    const char *known_names[] = {
        "python", "node", "rust", "c", "cpp", "java", "go", "ruby",
        "php", "swift", "kotlin", "scala", "haskell", "lua", "perl",
        "vscode", "vim", "emacs", "intellij", "eclipse", "visualstudio",
        "macos", "linux", "windows", "auto", NULL
    };
    
    for (int i = 0; known_names[i] != NULL; i++) {
        if (strcasecmp(name, known_names[i]) == 0) {
            return 1;
        }
    }
    
    return 0;
}