// templates.c - Built-in templates embedded in binary
#include "gitignore.h"

// Built-in template structure
typedef struct {
    const char *name;
    const char *content;
} builtin_template_t;

// Python template
static const char *python_template = 
"# Byte-compiled / optimized / DLL files\n"
"__pycache__/\n"
"*.py[cod]\n"
"*$py.class\n"
"\n"
"# C extensions\n"
"*.so\n"
"\n"
"# Distribution / packaging\n"
".Python\n"
"build/\n"
"develop-eggs/\n"
"dist/\n"
"downloads/\n"
"eggs/\n"
".eggs/\n"
"lib/\n"
"lib64/\n"
"parts/\n"
"sdist/\n"
"var/\n"
"wheels/\n"
"*.egg-info/\n"
".installed.cfg\n"
"*.egg\n"
"\n"
"# Virtual environments\n"
"venv/\n"
"env/\n"
"ENV/\n"
".venv\n"
"\n"
"# IDEs\n"
".idea/\n"
".vscode/\n"
"*.swp\n"
"*.swo\n"
"\n"
"# Environment\n"
".env\n"
".env.local\n";

// Node.js template
static const char *node_template =
"# Dependencies\n"
"node_modules/\n"
"npm-debug.log*\n"
"yarn-debug.log*\n"
"yarn-error.log*\n"
"\n"
"# Build outputs\n"
"dist/\n"
"build/\n"
".next/\n"
".nuxt/\n"
"out/\n"
"\n"
"# Cache\n"
".npm\n"
".eslintcache\n"
".node_repl_history\n"
"\n"
"# Environment\n"
".env\n"
".env.local\n"
".env.*.local\n"
"\n"
"# Logs\n"
"logs\n"
"*.log\n"
"\n"
"# OS\n"
".DS_Store\n"
"Thumbs.db\n";

// Rust template
static const char *rust_template =
"# Build output\n"
"target/\n"
"Cargo.lock\n"
"\n"
"# Backup files\n"
"**/*.rs.bk\n"
"*.pdb\n";

// C template
static const char *c_template =
"# Object files\n"
"*.o\n"
"*.ko\n"
"*.obj\n"
"*.elf\n"
"\n"
"# Libraries\n"
"*.a\n"
"*.la\n"
"*.lo\n"
"*.so\n"
"*.so.*\n"
"*.dylib\n"
"\n"
"# Executables\n"
"*.exe\n"
"*.out\n"
"*.app\n"
"\n"
"# Debug files\n"
"*.dSYM/\n"
"*.su\n";

// C++ template
static const char *cpp_template =
"# Compiled Object files\n"
"*.slo\n"
"*.lo\n"
"*.o\n"
"*.obj\n"
"\n"
"# Compiled Dynamic libraries\n"
"*.so\n"
"*.dylib\n"
"*.dll\n"
"\n"
"# Compiled Static libraries\n"
"*.lai\n"
"*.la\n"
"*.a\n"
"*.lib\n"
"\n"
"# Executables\n"
"*.exe\n"
"*.out\n"
"*.app\n";

// Java template
static const char *java_template =
"# Compiled class files\n"
"*.class\n"
"\n"
"# Package Files\n"
"*.jar\n"
"*.war\n"
"*.nar\n"
"*.ear\n"
"*.zip\n"
"*.tar.gz\n"
"*.rar\n"
"\n"
"# Build tools\n"
"target/\n"
".gradle/\n"
"build/\n"
"\n"
"# IDE\n"
".idea/\n"
"*.iml\n"
".classpath\n"
".project\n"
".settings/\n";

// Go template
static const char *go_template =
"# Binaries\n"
"*.exe\n"
"*.exe~\n"
"*.dll\n"
"*.so\n"
"*.dylib\n"
"\n"
"# Test binary\n"
"*.test\n"
"\n"
"# Output\n"
"*.out\n"
"\n"
"# Vendor\n"
"vendor/\n";

// Swift template
static const char *swift_template =
"# Xcode\n"
"build/\n"
"*.pbxuser\n"
"!default.pbxuser\n"
"*.mode1v3\n"
"!default.mode1v3\n"
"*.mode2v3\n"
"!default.mode2v3\n"
"*.perspectivev3\n"
"!default.perspectivev3\n"
"xcuserdata/\n"
"*.xccheckout\n"
"*.moved-aside\n"
"DerivedData\n"
"*.hmap\n"
"*.ipa\n"
"*.xcuserstate\n"
"\n"
"# Swift Package Manager\n"
".build/\n"
"Packages/\n"
"Package.pins\n"
"Package.resolved\n";

// VS Code template
static const char *vscode_template =
".vscode/\n"
"*.code-workspace\n"
".history/\n";

// Vim template
static const char *vim_template =
"# Swap files\n"
"[._]*.s[a-v][a-z]\n"
"[._]*.sw[a-p]\n"
"[._]s[a-rt-v][a-z]\n"
"[._]ss[a-gi-z]\n"
"[._]sw[a-p]\n"
"\n"
"# Session\n"
"Session.vim\n"
"Sessionx.vim\n"
"\n"
"# Temporary\n"
".netrwhist\n"
"*~\n"
"tags\n"
"[._]*.un~\n";

// IntelliJ template
static const char *intellij_template =
"# IntelliJ IDEA\n"
".idea/\n"
"*.iml\n"
"*.iws\n"
"out/\n"
"\n"
"# CMake\n"
"cmake-build-*/\n";

// macOS template
static const char *macos_template =
"# General\n"
".DS_Store\n"
".AppleDouble\n"
".LSOverride\n"
"\n"
"# Thumbnails\n"
"._*\n"
"\n"
"# Files that might appear in the root\n"
".DocumentRevisions-V100\n"
".fseventsd\n"
".Spotlight-V100\n"
".TemporaryItems\n"
".Trashes\n"
".VolumeIcon.icns\n"
".com.apple.timemachine.donotpresent\n";

// Linux template
static const char *linux_template =
"*~\n"
"\n"
"# temporary files\n"
".directory\n"
".Trash-*\n"
"\n"
"# .nfs files\n"
".nfs*\n";

// Windows template
static const char *windows_template =
"# Windows thumbnail cache\n"
"Thumbs.db\n"
"Thumbs.db:encryptable\n"
"ehthumbs.db\n"
"ehthumbs_vista.db\n"
"\n"
"# Folder config file\n"
"[Dd]esktop.ini\n"
"\n"
"# Recycle Bin\n"
"$RECYCLE.BIN/\n"
"\n"
"# Windows shortcuts\n"
"*.lnk\n";

// Ruby template
static const char *ruby_template =
"*.gem\n"
"*.rbc\n"
"/.config\n"
"/coverage/\n"
"/InstalledFiles\n"
"/pkg/\n"
"/spec/reports/\n"
"/spec/examples.txt\n"
"/test/tmp/\n"
"/test/version_tmp/\n"
"/tmp/\n"
"\n"
"# Bundler\n"
".bundle\n"
"vendor/bundle\n";

// PHP template
static const char *php_template =
"# Composer\n"
"vendor/\n"
"composer.lock\n"
"\n"
"# Laravel\n"
".env\n"
"/storage/*.key\n"
"/storage/logs/*\n"
"\n"
"# Cache\n"
"*.cache\n";

// TypeScript template
static const char *typescript_template =
"# Build outputs\n"
"dist/\n"
"build/\n"
"*.tsbuildinfo\n"
"\n"
"# Dependencies\n"
"node_modules/\n";

// All built-in templates
static const builtin_template_t builtin_templates[] = {
    {"python", python_template},
    {"node", node_template},
    {"rust", rust_template},
    {"c", c_template},
    {"cpp", cpp_template},
    {"java", java_template},
    {"go", go_template},
    {"swift", swift_template},
    {"ruby", ruby_template},
    {"php", php_template},
    {"typescript", typescript_template},
    {"vscode", vscode_template},
    {"vim", vim_template},
    {"intellij", intellij_template},
    {"macos", macos_template},
    {"linux", linux_template},
    {"windows", windows_template},
    {NULL, NULL}
};

// Get built-in template by name
const char* get_builtin_template(const char *name) {
    for (int i = 0; builtin_templates[i].name != NULL; i++) {
        if (strcasecmp(builtin_templates[i].name, name) == 0) {
            return builtin_templates[i].content;
        }
    }
    return NULL;
}

// Check if template is built-in
int is_builtin_template(const char *name) {
    return get_builtin_template(name) != NULL;
}

// Get all built-in template names
const char** get_builtin_template_names(void) {
    static const char *names[32];
    int count = 0;
    
    for (int i = 0; builtin_templates[i].name != NULL && count < 31; i++) {
        names[count++] = builtin_templates[i].name;
    }
    names[count] = NULL;
    
    return names;
}