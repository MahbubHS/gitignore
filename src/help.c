// help.c - FIXED help with corrected usage
#include "gitignore.h"

void show_help(void) {
    printf("%s%sgitignore%s - Professional .gitignore file generator v%s\n\n", 
           COLOR_BOLD, COLOR_CYAN, COLOR_RESET, VERSION);
    
    printf("%sUSAGE:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  gitignore [FLAGS] [OPTIONS] [ARGS]\n");
    printf("  gitignore <pattern...>             # Add one or more patterns to .gitignore\n\n");
    
    printf("%sFLAGS:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  %s-h, --help%s          Show this help message\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s-v, --version%s       Show version information\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s-i, --init, init%s    Initialize .gitignore file\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s-s, --sync, sync%s    Sync .gitignore from GitHub repository\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s-l, --list, list%s    List available templates\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s-a, --add%s           Force add as pattern (when name conflicts with command)\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s-t, -I, interactive%s Interactive template selection\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s-V, --verbose%s       Verbose output\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s-q, --quiet%s         Quiet mode (errors only)\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s--dry-run%s           Show what would happen without doing it\n\n", COLOR_GREEN, COLOR_RESET);
    
    printf("%sCOMMANDS:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  %sinit [langs...]%s              Create .gitignore with specified templates\n", 
           COLOR_YELLOW, COLOR_RESET);
    printf("  %ssync [langs...]%s              Download templates from GitHub\n", 
           COLOR_YELLOW, COLOR_RESET);
    printf("  %sappend [langs...]%s            Append templates to existing .gitignore\n", 
           COLOR_YELLOW, COLOR_RESET);
    printf("  %supdate [langs...]%s            Smart merge templates (removes duplicates)\n", 
           COLOR_YELLOW, COLOR_RESET);
    printf("  %slist [filter]%s                List available templates\n", 
           COLOR_YELLOW, COLOR_RESET);
    printf("  %sshow <lang>%s                  Show template content\n", 
           COLOR_YELLOW, COLOR_RESET);
    printf("  %sauto%s                         Auto-detect project and create .gitignore\n", 
           COLOR_YELLOW, COLOR_RESET);
    printf("  %sinteractive, -t, -I%s          Interactive template selection mode\n\n", 
           COLOR_YELLOW, COLOR_RESET);
    
    printf("%sGLOBAL COMMANDS:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  %sglobal init%s                  Create global .gitignore\n", 
           COLOR_YELLOW, COLOR_RESET);
    printf("  %sglobal add [langs...]%s        Add templates to global .gitignore\n\n", 
           COLOR_YELLOW, COLOR_RESET);
    
    printf("%sBACKUP COMMANDS:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  %sbackup%s                       Backup current .gitignore\n", 
           COLOR_YELLOW, COLOR_RESET);
    printf("  %srestore [backup_name]%s        Restore from backup\n", 
           COLOR_YELLOW, COLOR_RESET);
    printf("  %sbackups, history%s             List available backups\n\n", 
           COLOR_YELLOW, COLOR_RESET);
    
    printf("%sCACHE COMMANDS:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  %scache clear%s                  Clear template cache\n\n", 
           COLOR_YELLOW, COLOR_RESET);
    
    printf("%sEXAMPLES:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  %s# Create with auto template or empty%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore init\n\n");
    
    printf("  %s# Create with specific templates%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore init python node vscode\n\n");
    
    printf("  %s# Download and merge from GitHub%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore sync rust c cpp\n\n");
    
    printf("  %s# Auto-detect project type%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore auto\n\n");
    
    printf("  %s# Interactive mode%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore interactive\n");
    printf("  gitignore -t              # Short form\n");
    printf("  gitignore -I              # Alternative short form\n\n");
    
    printf("  %s# Add single file/path pattern%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore node_modules/\n");
    printf("  gitignore *.log\n");
    printf("  gitignore .env\n\n");
    
    printf("  %s# Add MULTIPLE patterns at once%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore node_modules/ *.log .env dist/\n");
    printf("  gitignore *.tmp *.swp *~\n\n");
    
    printf("  %s# Force add when name conflicts with command%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore --add init      # Adds 'init' as pattern, not command\n");
    printf("  gitignore --add sync list # Adds both as patterns\n\n");
    
    printf("  %s# Append to existing .gitignore%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore append python\n\n");
    
    printf("  %s# List templates%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore list\n");
    printf("  gitignore list --local\n");
    printf("  gitignore list web          # Filter by keyword\n\n");
    
    printf("  %s# Show template content%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore show python\n\n");
    
    printf("  %s# Global gitignore%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore global init\n");
    printf("  gitignore global add macos vscode\n\n");
    
    printf("  %s# Dry run%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  gitignore --dry-run init python node\n\n");
    
    printf("%sADDING PATTERNS:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  By default, any argument that looks like a file/path is treated as a pattern:\n");
    printf("    • Contains %s/%s (slash) → pattern\n", COLOR_YELLOW, COLOR_RESET);
    printf("    • Contains %s.%s (dot)   → pattern\n", COLOR_YELLOW, COLOR_RESET);
    printf("    • Contains %s*%s (star)  → pattern\n", COLOR_YELLOW, COLOR_RESET);
    printf("    • File/dir exists     → pattern\n\n");
    
    printf("  Use %s--add%s when pattern name conflicts with a command:\n", COLOR_YELLOW, COLOR_RESET);
    printf("    gitignore --add init    # 'init' as pattern\n");
    printf("    gitignore --add sync    # 'sync' as pattern\n");
    printf("    gitignore --add list    # 'list' as pattern\n\n");
    
    printf("%sCONFIGURATION:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  Custom templates:  %s$HOME/.config/gitignore/templates/%s\n", 
           COLOR_MAGENTA, COLOR_RESET);
    printf("  Auto template:     %s$HOME/.config/gitignore/templates/auto.gitignore%s\n", 
           COLOR_MAGENTA, COLOR_RESET);
    printf("  Config file:       %s$HOME/.config/gitignore/config.conf%s\n", 
           COLOR_MAGENTA, COLOR_RESET);
    printf("  Cache directory:   %s$HOME/.config/gitignore/cache/%s\n", 
           COLOR_MAGENTA, COLOR_RESET);
    printf("  Backup directory:  %s$HOME/.config/gitignore/backups/%s\n\n", 
           COLOR_MAGENTA, COLOR_RESET);
    
    printf("%sFEATURES:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("  • Smart duplicate removal\n");
    printf("  • Multiple patterns support\n");
    printf("  • Template caching (24h)\n");
    printf("  • Automatic backups\n");
    printf("  • Project auto-detection\n");
    printf("  • Interactive mode\n");
    printf("  • Global gitignore support\n");
    printf("  • Custom templates\n");
    printf("  • Colored output\n");
    printf("  • Progress indicators\n\n");
    
    printf("For more information: %shttps://github.com/yourusername/gitignore%s\n", 
           COLOR_BLUE, COLOR_RESET);
}

void show_version(void) {
    printf("%s%sgitignore%s version %s%s%s\n", 
           COLOR_BOLD, COLOR_CYAN, COLOR_RESET,
           COLOR_BOLD, VERSION, COLOR_RESET);
    printf("Professional .gitignore file generator\n\n");
    
    printf("%sFeatures enabled:%s\n", COLOR_BOLD, COLOR_RESET);
    
    if (g_config) {
        printf("  Cache:        %s%s%s\n", 
               g_config->cache_enabled ? COLOR_GREEN : COLOR_RED,
               g_config->cache_enabled ? "enabled" : "disabled",
               COLOR_RESET);
        printf("  Auto backup:  %s%s%s\n", 
               g_config->auto_backup ? COLOR_GREEN : COLOR_RED,
               g_config->auto_backup ? "enabled" : "disabled",
               COLOR_RESET);
        printf("  Color output: %s%s%s\n", 
               g_config->use_color ? COLOR_GREEN : COLOR_RED,
               g_config->use_color ? "enabled" : "disabled",
               COLOR_RESET);
        printf("  Verbose:      %s%s%s\n", 
               g_config->verbose ? COLOR_GREEN : COLOR_RED,
               g_config->verbose ? "enabled" : "disabled",
               COLOR_RESET);
    }
    
    printf("\n%sCopyright © 2026 gitignore contributors%s\n", 
           COLOR_CYAN, COLOR_RESET);
    printf("License: MIT\n");
}