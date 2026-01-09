// main.c - Enhanced main with new features
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
    
    // Interactive mode
    if (strcmp(flag, "interactive") == 0 || strcmp(flag, "-I") == 0) {
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
    
    // Add pattern to gitignore (new feature #26)
    if (strcmp(flag, "-a") == 0 || strcmp(flag, "--add") == 0) {
        if (argc < 3) {
            print_error("--add requires a file path or pattern", ERR_INVALID_ARGUMENT);
            return 1;
        }
        return add_pattern(argv[2], dry_run);
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
    
    // Default: treat as pattern to add (feature #26)
    // Check if it looks like a path/pattern vs a language name
    if (is_path_or_pattern(flag)) {
        return add_pattern(flag, dry_run);
    }
    
    print_error("Unknown command. Use -h for help", ERR_INVALID_ARGUMENT);
    return 1;
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