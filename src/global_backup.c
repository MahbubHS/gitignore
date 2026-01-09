// global_backup.c - Global gitignore and backup features
#include "gitignore.h"

// Global gitignore functions
int global_init(void) {
    const char *home = getenv("HOME");
    if (!home) {
        print_error("HOME environment variable not set", ERR_INVALID_ARGUMENT);
        return 1;
    }
    
    char global_path[MAX_PATH_LEN];
    snprintf(global_path, sizeof(global_path), "%s/%s", home, GLOBAL_GITIGNORE);
    
    if (file_exists(global_path)) {
        print_warning("Global .gitignore already exists");
        printf("  Location: %s\n", global_path);
        return 0;
    }
    
    FILE *f = fopen(global_path, "w");
    if (!f) {
        print_error("Could not create global .gitignore", ERR_PERMISSION_DENIED);
        return 1;
    }
    
    fprintf(f, "# Global .gitignore\n");
    fprintf(f, "# This file affects all git repositories on this system\n");
    fprintf(f, "# Configure with: git config --global core.excludesfile ~/.gitignore_global\n\n");
    
    // Add common OS-specific patterns
    #ifdef __APPLE__
        fprintf(f, "# macOS\n");
        fprintf(f, ".DS_Store\n");
        fprintf(f, ".AppleDouble\n");
        fprintf(f, ".LSOverride\n\n");
    #elif __linux__
        fprintf(f, "# Linux\n");
        fprintf(f, "*~\n");
        fprintf(f, ".directory\n\n");
    #elif _WIN32
        fprintf(f, "# Windows\n");
        fprintf(f, "Thumbs.db\n");
        fprintf(f, "ehthumbs.db\n");
        fprintf(f, "Desktop.ini\n\n");
    #endif
    
    fclose(f);
    
    print_success("Global .gitignore created");
    printf("  Location: %s\n", global_path);
    printf("\n  To enable globally, run:\n");
    printf("  %sgit config --global core.excludesfile ~/.gitignore_global%s\n",
           COLOR_BOLD, COLOR_RESET);
    
    return 0;
}

int global_add(char **langs, int count) {
    const char *home = getenv("HOME");
    if (!home) {
        print_error("HOME environment variable not set", ERR_INVALID_ARGUMENT);
        return 1;
    }
    
    char global_path[MAX_PATH_LEN];
    snprintf(global_path, sizeof(global_path), "%s/%s", home, GLOBAL_GITIGNORE);
    
    if (!file_exists(global_path)) {
        print_error("Global .gitignore does not exist. Run 'gitignore global init' first",
                   ERR_FILE_NOT_FOUND);
        return 1;
    }
    
    FILE *f = fopen(global_path, "a");
    if (!f) {
        print_error("Could not open global .gitignore", ERR_PERMISSION_DENIED);
        return 1;
    }
    
    fprintf(f, "\n# Added by gitignore tool\n");
    
    for (int i = 0; i < count; i++) {
        char *template_path = get_template_path(langs[i]);
        FILE *tmpl = NULL;
        
        if (template_path && file_exists(template_path)) {
            tmpl = fopen(template_path, "r");
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
    
    print_success("Templates added to global .gitignore");
    printf("  Location: %s\n", global_path);
    
    return 0;
}

// Backup functions
int backup_gitignore(void) {
    if (!file_exists(".gitignore")) {
        print_error(".gitignore does not exist", ERR_FILE_NOT_FOUND);
        return 1;
    }
    
    char *backup_path = get_backup_path();
    if (!backup_path) {
        print_error("Could not determine backup path", ERR_INVALID_ARGUMENT);
        return 1;
    }
    
    // Create backup directory if it doesn't exist
    struct stat st = {0};
    if (stat(backup_path, &st) == -1) {
        mkdir(backup_path, 0755);
    }
    
    // Generate backup filename with timestamp
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char backup_file[MAX_PATH_LEN];
    snprintf(backup_file, sizeof(backup_file), 
             "%s/gitignore_%04d%02d%02d_%02d%02d%02d.bak",
             backup_path, 
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);
    
    // Copy file
    FILE *src = fopen(".gitignore", "r");
    FILE *dst = fopen(backup_file, "w");
    
    if (!src || !dst) {
        print_error("Could not create backup", ERR_PERMISSION_DENIED);
        if (src) fclose(src);
        if (dst) fclose(dst);
        free(backup_path);
        return 1;
    }
    
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dst);
    }
    
    fclose(src);
    fclose(dst);
    free(backup_path);
    
    print_success("Backup created");
    printf("  %s\n", backup_file);
    
    return 0;
}

int restore_gitignore(const char *backup_name) {
    char *backup_path = get_backup_path();
    if (!backup_path) {
        print_error("Could not determine backup path", ERR_INVALID_ARGUMENT);
        return 1;
    }
    
    // If no specific backup name, list available backups
    if (!backup_name) {
        DIR *dir = opendir(backup_path);
        if (!dir) {
            print_error("No backups found", ERR_FILE_NOT_FOUND);
            free(backup_path);
            return 1;
        }
        
        printf("%sAvailable backups:%s\n", COLOR_BOLD, COLOR_RESET);
        struct dirent *entry;
        int count = 0;
        while ((entry = readdir(dir)) != NULL) {
            if (strstr(entry->d_name, ".bak")) {
                printf("  %d) %s\n", ++count, entry->d_name);
            }
        }
        closedir(dir);
        
        if (count == 0) {
            print_error("No backups found", ERR_FILE_NOT_FOUND);
            free(backup_path);
            return 1;
        }
        
        printf("\nUse: gitignore restore <backup_name>\n");
        free(backup_path);
        return 0;
    }
    
    // Restore specific backup
    char backup_file[MAX_PATH_LEN];
    snprintf(backup_file, sizeof(backup_file), "%s/%s", backup_path, backup_name);
    
    if (!file_exists(backup_file)) {
        print_error("Backup not found", ERR_FILE_NOT_FOUND);
        free(backup_path);
        return 1;
    }
    
    FILE *src = fopen(backup_file, "r");
    FILE *dst = fopen(".gitignore", "w");
    
    if (!src || !dst) {
        print_error("Could not restore backup", ERR_PERMISSION_DENIED);
        if (src) fclose(src);
        if (dst) fclose(dst);
        free(backup_path);
        return 1;
    }
    
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dst);
    }
    
    fclose(src);
    fclose(dst);
    free(backup_path);
    
    print_success("Backup restored");
    return 0;
}

int list_backups(void) {
    char *backup_path = get_backup_path();
    if (!backup_path) {
        print_error("Could not determine backup path", ERR_INVALID_ARGUMENT);
        return 1;
    }
    
    DIR *dir = opendir(backup_path);
    if (!dir) {
        print_error("No backups found", ERR_FILE_NOT_FOUND);
        free(backup_path);
        return 1;
    }
    
    printf("%s%sBackup History:%s\n\n", COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
    
    struct dirent *entry;
    int count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".bak")) {
            printf("  %s•%s %s\n", COLOR_GREEN, COLOR_RESET, entry->d_name);
            count++;
        }
    }
    closedir(dir);
    free(backup_path);
    
    if (count == 0) {
        print_info("No backups found");
    } else {
        printf("\n%sTotal: %d backup(s)%s\n", COLOR_BOLD, count, COLOR_RESET);
    }
    
    return 0;
}