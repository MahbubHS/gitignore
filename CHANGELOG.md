# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.0.0] - 2026-01-10

### Added

#### Core Features
- **17+ Built-in Templates** - Python, Node.js, Rust, C, C++, Java, Go, Swift, Ruby, PHP, TypeScript, VS Code, Vim, IntelliJ, macOS, Linux, Windows all compiled into binary
- **Smart Append Mode** - Automatically appends to existing `.gitignore` files instead of overwriting
- **Automatic Duplicate Removal** - Intelligently removes duplicate patterns across all operations (init, sync, add)
- **Multiple Pattern Support** - Add multiple file/path patterns at once: `gitignore node_modules/ *.log .env`
- **Swift Template** - Added built-in Swift/iOS template
- **Windows Template** - Added built-in Windows-specific patterns template

#### Template Management
- **Auto Template Generator** - `scripts/generate_templates.sh` automatically generates `src/templates.c` from `templates/` directory
- **Easy Template Addition** - Simply drop `.gitignore` files into `templates/` directory and run `make regen-templates`
- **Template Source Files** - All templates now maintained as separate `.gitignore` files in `templates/` directory
- **No Manual C Editing** - Template generator handles all C code generation and escaping automatically

#### Installation & Build
- **Smart PREFIX Detection** - Install to custom locations easily with `PREFIX=/path make install`
- **Environment Variable Support** - Use `export PREFIX=~/.local` for user-local installations
- **Install Script PREFIX** - Installation script accepts custom prefix: `./install.sh ~/.local`
- **Show PREFIX Target** - New `make show-prefix` to display installation configuration
- **Setup Templates Target** - New `make setup-templates` to initialize template directory
- **Regenerate Templates Target** - New `make regen-templates` to force template regeneration
- **Distribution Clean** - New `make distclean` to remove all generated files including `templates.c`

#### User Experience
- **Better Conflict Detection** - Clear error messages when pattern names conflict with command names
- **Helpful Error Messages** - Suggests correct usage when ambiguous arguments are detected
- **Interactive Mode Shortcuts** - Added `-t` flag as quick alternative to `-I` for interactive mode
- **Colored Progress Output** - Visual feedback during template generation and builds
- **Comprehensive Help** - Enhanced `make help` with detailed target descriptions

### Changed

#### Template System
- **Templates Now Embedded** - All built-in templates compiled directly into binary (no external files needed at runtime)
- **Template Priority System** - Custom templates (`~/.config/gitignore/templates/`) override built-in templates
- **Removed External Template Files** - No longer need `templates/` directory after build (templates embedded in binary)

#### Build System
- **Makefile Improvements** - Smart PREFIX detection, better output formatting, comprehensive targets
- **Template Generation Process** - Automated via `make templates` before compilation
- **Source Structure** - Added `templates.c` to source files (auto-generated, not manually edited)

#### Installation
- **Flexible Installation Paths** - No longer hardcoded to `/usr/local`
- **No-Sudo Installation** - Can install to user directories like `~/.local` without root privileges
- **PATH Helper** - Installation script offers to add custom install paths to shell configuration

#### Operations
- **Init Behavior** - Now appends to existing `.gitignore` instead of replacing
- **Sync Behavior** - Downloads from GitHub and merges with existing patterns, removing duplicates
- **Pattern Addition** - Checks for duplicates before adding, skips existing patterns
- **All Operations Non-Destructive** - Preserves existing `.gitignore` content across all operations

### Fixed

- **Built-in Templates Not Working** - Fixed issue where built-in templates weren't accessible (now properly embedded)
- **Init Overwriting Files** - Fixed `init` command overwriting existing `.gitignore` files
- **Sync Overwriting Files** - Fixed `sync` command overwriting existing `.gitignore` files
- **Duplicate Patterns** - Fixed issue where duplicate patterns could be added
- **Template Not Found** - Fixed Swift and other built-in templates not being found
- **Windows Support** - Added Windows-specific template (was missing)
- **Manual Template Editing** - Removed need to manually edit C code for new templates

### Documentation

- **README.md** - Complete rewrite with badges, better examples, comprehensive feature documentation
- **ADDING_TEMPLATES.md** - New comprehensive guide for adding custom templates
- **Makefile Help** - Enhanced with detailed descriptions of all targets
- **Install Script Help** - Added `--help` flag with usage examples
- **Man Page** - Updated with all new features and examples
- **CHANGELOG.md** - Added this changelog following Keep a Changelog format

### Developer Experience

- **Template Addition Workflow** - Simplified from manual C editing to drop-in `.gitignore` files
- **Auto-generated Code** - No more manual escaping or C string formatting
- **Clear Build Process** - Templates auto-generated before compilation
- **Better Makefile Targets** - Intuitive targets for common development tasks
- **Comprehensive Documentation** - Added `docs/` directory with guides

### Performance

- **Faster Template Access** - Templates compiled into binary (no file I/O)
- **Efficient Deduplication** - Optimized duplicate detection algorithm
- **Template Caching** - GitHub-synced templates cached for 24 hours
- **Reduced Dependencies** - No external template files needed after build

### Security

- **No External File Access** - Built-in templates don't require file system access
- **Validation** - Better input validation for pattern names
- **Safe Escaping** - Auto-generator handles special character escaping correctly

## [1.0.0] - 2025-01-09

### Added
- Initial release
- Basic init and sync functionality
- Custom template support from `~/.config/gitignore/templates/`
- GitHub template synchronization
- Cache system for downloaded templates
- Backup and restore functionality
- Auto-detection of project types
- Interactive mode
- Global gitignore support
- Colored terminal output
- Man page documentation

---

## Migration Guide

### From v1.0.0 to v2.0.0

#### Breaking Changes
None - v2.0.0 is fully backward compatible with v1.0.0

#### New Features to Try

1. **Install to Custom Location:**
   ```bash
   # Old way: hardcoded /usr/local
   sudo make install
   
   # New way: flexible
   make PREFIX=~/.local install  # No sudo needed!
   ```

2. **Add Custom Built-in Templates:**
   ```bash
   # Old way: edit templates.c manually (error-prone)
   vim src/templates.c
   
   # New way: drop in a file
   echo "*.ext" > templates/mytemplate.gitignore
   make regen-templates
   make
   ```

3. **Multiple Patterns:**
   ```bash
   # Old way: one at a time
   gitignore node_modules/
   gitignore .env
   gitignore *.log
   
   # New way: all at once
   gitignore node_modules/ .env *.log
   ```

4. **Non-Destructive Operations:**
   ```bash
   # v1.0: Would overwrite
   gitignore init python  # Lost existing patterns!
   
   # v2.0: Preserves and merges
   gitignore init python  # Keeps existing + adds Python
   ```

#### Recommended Actions

1. **Check Installation Path:**
   ```bash
   make show-prefix
   ```

2. **Regenerate Templates** (if you had custom modifications):
   ```bash
   make distclean
   make
   ```

3. **Test Append Behavior:**
   ```bash
   # Create test .gitignore
   echo "test" > .gitignore
   
   # Add template
   gitignore init python
   
   # Verify "test" is still there
   grep "test" .gitignore
   ```

---

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/mahbubhs/gitignore/tags).

## Links

- [Unreleased Changes](https://github.com/mahbubhs/gitignore/compare/v2.0.0...HEAD)
- [v2.0.0 Release](https://github.com/mahbubhs/gitignore/releases/tag/v2.0.0)
- [v1.0.0 Release](https://github.com/mahbubhs/gitignore/releases/tag/v1.0.0)