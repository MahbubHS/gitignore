# Troubleshooting Guide

Comprehensive guide to diagnosing and resolving issues with gitignore. This guide covers common problems, diagnostic procedures, and recovery strategies.

## 📋 Table of Contents

- [Quick Diagnosis](#-quick-diagnosis)
- [Installation Issues](#-installation-issues)
- [Build and Compilation](#-build-and-compilation)
- [Runtime Errors](#-runtime-errors)
- [Configuration Problems](#-configuration-problems)
- [Template Issues](#-template-issues)
- [Network and Sync](#-network-and-sync)
- [File System Problems](#-file-system-problems)
- [Platform-Specific Issues](#-platform-specific-issues)
- [Performance Issues](#-performance-issues)
- [Advanced Diagnostics](#-advanced-diagnostics)
- [Recovery Procedures](#-recovery-procedures)

## 🚨 Quick Diagnosis

### System Health Check

Run this diagnostic script to identify common issues:

```bash
#!/bin/bash
# gitignore-diagnostic.sh

echo "=== Gitignore Diagnostic Report ==="
echo "Date: $(date)"
echo "User: $(whoami)"
echo "Working Directory: $(pwd)"
echo

echo "1. Installation Check:"
which gitignore >/dev/null 2>&1 && echo "✓ gitignore found in PATH" || echo "✗ gitignore not found in PATH"
gitignore --version 2>/dev/null && echo "✓ gitignore executable works" || echo "✗ gitignore execution failed"
echo

echo "2. Permissions Check:"
ls -la $(which gitignore 2>/dev/null) 2>/dev/null || echo "Cannot check binary permissions"
ls -ld ~/.config/gitignore 2>/dev/null || echo "Config directory not found"
ls -la ~/.config/gitignore/config.conf 2>/dev/null || echo "Config file not found"
echo

echo "3. Dependencies Check:"
ldd $(which gitignore 2>/dev/null) | grep -q libcurl && echo "✓ libcurl linked" || echo "✗ libcurl not linked"
pkg-config --exists libcurl && echo "✓ libcurl development headers found" || echo "✗ libcurl development headers missing"
echo

echo "4. Basic Functionality Test:"
timeout 10s gitignore list >/dev/null 2>&1 && echo "✓ Template listing works" || echo "✗ Template listing failed"
echo

echo "=== End Report ==="
```

**Run the diagnostic:**

```bash
chmod +x gitignore-diagnostic.sh
./gitignore-diagnostic.sh
```

### Quick Health Commands

```bash
# Installation and version
which gitignore && gitignore --version

# Basic functionality
gitignore list | head -5

# Configuration status
ls -la ~/.config/gitignore/ 2>/dev/null || echo "No config directory"

# Cache status
ls -la ~/.config/gitignore/cache/ 2>/dev/null || echo "No cache directory"
```

## 🛠️ Installation Issues

### Command Not Found

**Error Message:**

```bash
$ gitignore --version
bash: gitignore: command not found
```

**Root Causes:**

1. Binary not installed
2. PATH not configured
3. Installation failed

**Diagnostic Steps:**

```bash
# Check if binary exists anywhere
find /usr -name gitignore 2>/dev/null
find ~ -name gitignore 2>/dev/null

# Check PATH contents
echo $PATH | tr ':' '\n'

# Check which directories are in PATH
ls -la /usr/local/bin/gitignore 2>/dev/null || echo "Not in /usr/local/bin"
ls -la ~/.local/bin/gitignore 2>/dev/null || echo "Not in ~/.local/bin"
```

**Solutions:**

**Option 1: Add to PATH (Temporary)**

```bash
export PATH="$HOME/.local/bin:$PATH"
gitignore --version
```

**Option 2: Add to PATH (Permanent)**

```bash
# Add to ~/.bashrc or ~/.zshrc
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

**Option 3: Create Symlink**

```bash
sudo ln -s /path/to/gitignore /usr/local/bin/gitignore
```

**Option 4: Reinstall**

```bash
# Remove old installation
sudo make uninstall 2>/dev/null || true
rm -rf ~/.config/gitignore

# Fresh install
make clean && make && sudo make install
```

### Permission Denied During Installation

**Error Message:**

```bash
$ sudo make install
install: cannot create directory '/usr/local/bin': Permission denied
```

**Solutions:**

```bash
# Check current user permissions
whoami
groups

# Use correct prefix for user installation
make PREFIX=~/.local install

# Or create installation directory
sudo mkdir -p /usr/local/bin
sudo chown $(whoami) /usr/local/bin
make install
```

### Library Not Found

**Error Message:**

```bash
/usr/bin/ld: cannot find -lcurl
```

**Diagnostic:**

```bash
# Check if libcurl is installed
pkg-config --exists libcurl && echo "libcurl found" || echo "libcurl missing"

# Check library paths
ldconfig -p | grep curl

# Check development headers
find /usr -name "curl.h" 2>/dev/null
```

**Solutions by Platform:**

**Ubuntu/Debian:**

```bash
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev
```

**Fedora/RHEL:**

```bash
sudo dnf install libcurl-devel
```

**Arch Linux:**

```bash
sudo pacman -S curl
```

**macOS:**

```bash
# With Homebrew
brew install curl

# With MacPorts
sudo port install curl
```

## 🔨 Build and Compilation

### Templates Not Generated

**Error Message:**

```bash
make
src/templates.c: No such file or directory
```

**Root Cause:** Template generation step skipped or failed

**Diagnostic:**

```bash
# Check if templates directory exists
ls -la templates/

# Check template files
ls -la templates/*.gitignore | wc -l

# Manual template generation
make templates
ls -la src/templates.c
```

**Solutions:**

```bash
# Force regeneration
make regen-templates

# Check for script issues
bash scripts/generate_templates.sh

# Verify template files
find templates -name "*.gitignore" -exec wc -l {} \;
```

### Compiler Errors

**Common Error Types:**

**Header Not Found:**

```c
fatal error: curl/curl.h: No such file or directory
```

**Solutions:**

```bash
# Check include paths
pkg-config --cflags libcurl

# Add include path manually
make CFLAGS="-I/usr/include/curl"

# Check development package
dpkg -l | grep libcurl
```

**Undefined Reference:**

```c
undefined reference to `curl_easy_init'
```

**Solutions:**

```bash
# Check library linking
pkg-config --libs libcurl

# Add library path
make LDFLAGS="-L/usr/lib/x86_64-linux-gnu -lcurl"

# Static linking
make LDFLAGS="-static -lcurl -lssl -lcrypto -lz"
```

### Makefile Issues

**Error:** `make: *** No rule to make target 'something'`

**Diagnostic:**

```bash
# Show all available targets
make help

# Dry run to see what would execute
make -n

# Check Makefile syntax
make -f /dev/null -p | head -20
```

**Common Solutions:**

```bash
# Clean and rebuild
make clean
make

# Use different make
gmake  # BSD make
bmake  # Alternative make

# Override variables
make CC=clang CFLAGS="-O2 -Wall"
```

### Cross-Compilation Issues

**Target:** ARM64 on x86_64 host

**Setup:**

```bash
# Install cross-compiler
sudo apt-get install gcc-aarch64-linux-gnu

# Cross-compile
make CC=aarch64-linux-gnu-gcc

# Test on target
scp gitignore user@arm64-host:/tmp/
ssh user@arm64-host /tmp/gitignore --version
```

## 🚨 Runtime Errors

### Segmentation Fault

**Error:** `Segmentation fault (core dumped)`

**Diagnostic:**

```bash
# Enable core dumps
ulimit -c unlimited

# Run with debugging
gdb ./gitignore core
(gdb) bt  # Backtrace

# Run with valgrind
valgrind ./gitignore list

# Address sanitizer (if compiled with)
make CFLAGS="-fsanitize=address" LDFLAGS="-fsanitize=address"
./gitignore list
```

**Common Causes:**

- NULL pointer dereference
- Buffer overflow
- Memory corruption
- Stack overflow

### Memory Issues

**Error:** `malloc(): corrupted top size`

**Diagnostic:**

```bash
# Memory debugging
valgrind --leak-check=full ./gitignore list

# Track allocations
valgrind --tool=massif ./gitignore list
ms_print massif.out.*
```

**Causes:**

- Buffer overflows
- Memory leaks
- Double frees
- Invalid memory access

### Library Loading Errors

**Error:** `libcurl.so.4: cannot open shared object file`

**Diagnostic:**

```bash
# Check library dependencies
ldd $(which gitignore)

# Check library paths
echo $LD_LIBRARY_PATH

# Find library location
find /usr -name "libcurl.so.*" 2>/dev/null
```

**Solutions:**

```bash
# Add library path
export LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"

# Rebuild with rpath
make LDFLAGS="-Wl,-rpath,/usr/local/lib"

# Static linking
make LDFLAGS="-static -lcurl"
```

## ⚙️ Configuration Problems

### Configuration Not Loading

**Symptoms:**

- Settings ignored
- Default behavior used
- Verbose mode shows "using defaults"

**Diagnostic:**

```bash
# Check configuration file location
ls -la ~/.config/gitignore/config.conf

# Check file permissions
stat ~/.config/gitignore/config.conf

# Validate syntax
cat ~/.config/gitignore/config.conf | grep -v '^#' | grep '='

# Test configuration loading
strace -e open gitignore list 2>&1 | grep config
```

**Common Issues:**

**File Not Found:**

```bash
# Create configuration directory
mkdir -p ~/.config/gitignore

# Create default config
cat > ~/.config/gitignore/config.conf << 'EOF'
verbose = false
use_color = true
auto_backup = true
cache_enabled = true
EOF
```

**Permission Issues:**

```bash
# Fix permissions
chmod 644 ~/.config/gitignore/config.conf
chmod 755 ~/.config/gitignore
```

**Syntax Errors:**

```bash
# Check for common mistakes
grep -n " =" ~/.config/gitignore/config.conf  # Spaces around =
grep -n "true\|false" ~/.config/gitignore/config.conf  # Boolean values
```

### Invalid Configuration Values

**Error:** `Invalid configuration value for 'cache_duration': '24h'`

**Diagnostic:**

```bash
# Check configuration parsing
gitignore --verbose list 2>&1 | grep -i config

# Validate values
grep "cache_duration" ~/.config/gitignore/config.conf
```

**Valid Values by Type:**

| Setting             | Type    | Valid Examples            |
| ------------------- | ------- | ------------------------- |
| `verbose`           | boolean | `true`, `false`           |
| `cache_duration`    | integer | `3600`, `86400`, `604800` |
| `use_color`         | boolean | `true`, `false`           |
| `default_templates` | string  | `python`, `python,vscode` |

## 📋 Template Issues

### Template Not Found

**Error:** `Template 'xyz' not found`

**Diagnostic:**

```bash
# List available templates
gitignore list

# Search for similar templates
gitignore list | grep -i xyz

# Check custom templates
ls -la ~/.config/gitignore/templates/

# Check built-in templates
./gitignore list --builtin-only
```

**Solutions:**

**Typo in Template Name:**

```bash
# Common corrections
gitignore init python    # not 'pyhton'
gitignore init javascript # not 'js'
gitignore init go        # not 'golang'
```

**Missing Custom Template:**

```bash
# Create custom template
mkdir -p ~/.config/gitignore/templates
cat > ~/.config/gitignore/templates/mytemplate.gitignore << 'EOF'
# My Custom Template
*.log
temp/
build/
EOF

# Use custom template
gitignore init mytemplate
```

**Template File Corruption:**

```bash
# Regenerate built-in templates
make regen-templates

# Clear cache
rm -rf ~/.config/gitignore/cache/
```

### Template Merge Conflicts

**Error:** `Template merge conflict detected`

**Diagnostic:**

```bash
# Preview merge
gitignore --dry-run init python vscode

# Check existing .gitignore
cat .gitignore | head -20

# Use different merge strategy
gitignore init python --merge=replace
```

**Resolution Strategies:**

```bash
# Option 1: Clean merge
gitignore init python --merge=smart

# Option 2: Replace entirely
gitignore init python --merge=replace

# Option 3: Manual resolution
cp .gitignore .gitignore.backup
gitignore init python --merge=append
# Edit .gitignore manually
```

## 🌐 Network and Sync

### Network Connection Failed

**Error:** `Network error: Failed to connect to github.com`

**Diagnostic:**

```bash
# Test basic connectivity
ping -c 3 github.com

# Test HTTPS
curl -I https://github.com

# Test specific endpoint
curl -I https://raw.githubusercontent.com/github/gitignore/main/Python.gitignore

# Check DNS resolution
nslookup github.com

# Check proxy settings
echo $http_proxy $https_proxy
```

**Solutions:**

**Corporate Proxy:**

```bash
# Set proxy environment variables
export http_proxy="http://proxy.company.com:8080"
export https_proxy="http://proxy.company.com:8080"

# Or configure in git
git config --global http.proxy http://proxy.company.com:8080
```

**SSL/TLS Issues:**

```bash
# Disable SSL verification (not recommended for production)
export GIT_SSL_NO_VERIFY=true

# Update CA certificates
sudo apt-get install ca-certificates
sudo update-ca-certificates
```

**Firewall Blocking:**

```bash
# Check firewall rules
sudo ufw status
sudo iptables -L

# Test with different DNS
curl --resolve github.com:443:140.82.121.4 https://github.com
```

### Sync Template Not Found

**Error:** `GitHub template 'xyz' not found`

**Diagnostic:**

```bash
# Check if template exists on GitHub
curl -s https://api.github.com/repos/github/gitignore/contents | jq -r '.[].name' | grep -i xyz

# List available GitHub templates
curl -s https://api.github.com/repos/github/gitignore/contents | jq -r '.[].name' | grep gitignore

# Check cache
ls -la ~/.config/gitignore/cache/
```

**Solutions:**

```bash
# Use built-in template instead
gitignore init python  # Uses built-in templates

# Clear cache and retry
rm -rf ~/.config/gitignore/cache/
gitignore sync python

# Manual download
curl -o python.gitignore https://raw.githubusercontent.com/github/gitignore/main/Python.gitignore
```

### Rate Limiting

**Error:** `HTTP 403: API rate limit exceeded`

**Diagnostic:**

```bash
# Check rate limit status
curl -H "Authorization: token $GITHUB_TOKEN" https://api.github.com/rate_limit

# Check if using token
echo $GITHUB_TOKEN
```

**Solutions:**

```bash
# Use GitHub token
export GITHUB_TOKEN=your_token_here
gitignore sync python

# Wait for rate limit reset
# Rate limits reset hourly for authenticated requests

# Use cache instead
gitignore init python  # Uses built-in templates
```

## 📁 File System Problems

### Permission Denied

**Error:** `Permission denied writing .gitignore`

**Diagnostic:**

```bash
# Check directory permissions
ls -ld .

# Check file permissions if exists
ls -la .gitignore

# Check disk space
df -h .

# Check SELinux/AppArmor
ls -laZ . 2>/dev/null || echo "No SELinux"
```

**Solutions:**

**Directory Permissions:**

```bash
# Fix permissions
chmod 755 .

# Check parent directories
ls -ld ..
chmod 755 ..
```

**SELinux Issues:**

```bash
# Check SELinux status
sestatus

# Temporarily disable for testing
setenforce 0

# Set proper context
chcon -t user_home_t .gitignore
```

**File Locking:**

```bash
# Check for file locks
lsof .gitignore

# Check if file is open in editor
fuser .gitignore
```

### Disk Space Issues

**Error:** `No space left on device`

**Diagnostic:**

```bash
# Check disk usage
df -h .

# Check inode usage
df -i .

# Find large files
du -sh * | sort -hr | head -10

# Clean up
rm -rf ~/.config/gitignore/cache/*
```

**Solutions:**

```bash
# Free up space
rm -rf ~/.cache/*
rm -rf ~/.config/gitignore/cache/*

# Use external storage for cache
export XDG_CACHE_HOME=/mnt/external/cache
```

### File Corruption

**Symptoms:** Garbled output, unexpected errors

**Diagnostic:**

```bash
# Check file integrity
file .gitignore

# Validate content
head -20 .gitignore

# Check for binary data
hexdump -C .gitignore | head -10
```

**Recovery:**

```bash
# Restore from backup
gitignore restore

# Recreate from templates
rm .gitignore
gitignore init python vscode
```

## 🖥️ Platform-Specific Issues

### Linux Issues

**AppArmor/SELinux Blocking:**

```bash
# Check AppArmor status
sudo apparmor_status | grep gitignore

# Check SELinux status
sestatus

# Disable temporarily
sudo setenforce 0
```

**Systemd Journal Issues:**

```bash
# Check system logs
journalctl -f | grep gitignore

# Check for coredumps
coredumpctl list
```

### macOS Issues

**Xcode Command Line Tools:**

```bash
# Check installation
xcode-select -p

# Reinstall if needed
sudo rm -rf /Library/Developer/CommandLineTools
xcode-select --install
```

**Homebrew Library Path:**

```bash
# Check Homebrew paths
brew --prefix
brew --cellar

# Add to library path
export DYLD_LIBRARY_PATH="$(brew --prefix)/lib:$DYLD_LIBRARY_PATH"
```

**Gatekeeper Blocking:**

```bash
# Check quarantine
xattr -d com.apple.quarantine /path/to/gitignore 2>/dev/null || true

# Allow execution
spctl --add /path/to/gitignore
```

### Windows (WSL) Issues

**Path Conversion:**

```bash
# Use WSL paths
cd /mnt/c/Users/YourName/project

# Check WSL version
wsl --version
```

**File Permission Mapping:**

```bash
# Check mount options
mount | grep mnt

# Remount with metadata
sudo mount -t drvfs C: /mnt/c -o metadata,umask=22,fmask=111
```

**Network Issues:**

```bash
# Check WSL network
cat /etc/resolv.conf

# Restart WSL network
sudo service resolvconf restart
```

### FreeBSD/OpenBSD Issues

**Package Installation:**

```bash
# FreeBSD
sudo pkg install curl git gmake

# OpenBSD
doas pkg_add curl git gmake
```

**Library Paths:**

```bash
# Check library paths
echo $LD_LIBRARY_PATH

# Add local library path
export LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"
```

## ⚡ Performance Issues

### Slow Startup

**Diagnostic:**

```bash
# Time startup
time gitignore --version

# Profile execution
strace -c gitignore list 2>&1 | head -20

# Check configuration loading
time gitignore --verbose list 2>&1 | grep "Loading config"
```

**Common Causes:**

- Large configuration files
- Slow file system
- Network timeouts
- Memory pressure

**Optimizations:**

```bash
# Simplify configuration
wc -l ~/.config/gitignore/config.conf

# Use faster storage
export XDG_CONFIG_HOME=/tmp/fast-config

# Disable network features
echo "cache_enabled=false" >> ~/.config/gitignore/config.conf
```

### High Memory Usage

**Diagnostic:**

```bash
# Monitor memory usage
/usr/bin/time -v gitignore list

# Memory profiling
valgrind --tool=massif gitignore list
ms_print massif.out.*
```

**Causes:**

- Large template databases
- Memory leaks
- Inefficient algorithms

**Solutions:**

```bash
# Limit cache size
echo "cache_duration=3600" >> ~/.config/gitignore/config.conf

# Clear caches regularly
gitignore cache clear

# Use static builds
make LDFLAGS="-static"
```

### Network Performance

**Slow Sync Operations:**

```bash
# Time network operations
time gitignore sync python

# Check connection speed
curl -o /dev/null -s -w "%{speed_download}\n" https://github.com/favicon.ico

# Use faster DNS
echo "nameserver 8.8.8.8" | sudo tee /etc/resolv.conf
```

## 🔬 Advanced Diagnostics

### Debug Logging

**Enable Debug Mode:**

```bash
# Build with debug symbols
make dev

# Run with debugger
gdb --args ./gitignore --verbose init python
(gdb) run
(gdb) bt  # Show backtrace on crash
```

**Environment Debug Variables:**

```bash
# Enable libcurl debug
export CURL_DEBUG=1

# Enable malloc debugging
export MALLOC_CHECK_=2

# Enable glibc debugging
export GLIBC_DEBUG=1
```

### System Call Tracing

**Trace System Calls:**

```bash
# Trace file operations
strace -e file gitignore list

# Trace network operations
strace -e network gitignore sync python

# Trace all operations
strace gitignore init python 2>&1 | head -50
```

**Trace Process Tree:**

```bash
# Show process hierarchy
ps auxf | grep gitignore

# Monitor system resources
top -p $(pgrep gitignore)
```

### Core Dump Analysis

**Enable Core Dumps:**

```bash
# Enable core dumps
ulimit -c unlimited

# Check core dump location
cat /proc/sys/kernel/core_pattern

# Analyze core dump
gdb ./gitignore core
(gdb) bt full
```

### Memory Leak Detection

**Valgrind Analysis:**

```bash
# Comprehensive leak check
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./gitignore list

# Memory profiling
valgrind --tool=massif ./gitignore list
ms_print massif.out.*
```

### Network Debugging

**HTTP Traffic Analysis:**

```bash
# Capture HTTP traffic
tcpdump -i any port 443 -w capture.pcap

# Analyze with Wireshark
wireshark capture.pcap

# Use curl for testing
curl -v https://raw.githubusercontent.com/github/gitignore/main/Python.gitignore
```

## 🚑 Recovery Procedures

### Emergency Backup Recovery

**When gitignore is completely broken:**

```bash
# Manual backup
cp .gitignore .gitignore.manual-backup.$(date +%s)

# Use system tools
cat > .gitignore << 'EOF'
# Temporary .gitignore created during recovery
*.log
*.tmp
.DS_Store
Thumbs.db
EOF
```

### Clean System Reset

**Complete reinstallation:**

```bash
# Remove all traces
sudo make uninstall 2>/dev/null || true
rm -rf ~/.config/gitignore
rm -rf ~/.cache/gitignore
rm -f ~/.local/bin/gitignore

# Fresh installation
git clone https://github.com/mahbubhs/gitignore.git
cd gitignore
make clean
make templates
make
sudo make install

# Verify
gitignore --version
```

### Data Recovery

**Recover lost configurations:**

```bash
# Check for backups
find ~ -name "*gitignore*" -type f 2>/dev/null

# Restore from version control
git checkout HEAD~1 -- .gitignore 2>/dev/null || true

# Recreate from memory
cat > ~/.config/gitignore/config.conf << 'EOF'
verbose = false
use_color = true
auto_backup = true
cache_enabled = true
cache_duration = 86400
EOF
```

### Factory Reset

**Return to pristine state:**

```bash
# Remove all user data
rm -rf ~/.config/gitignore
rm -rf ~/.cache/gitignore

# Reset environment
unset GITIGNORE_CONFIG_DIR
unset XDG_CONFIG_HOME
unset XDG_CACHE_HOME

# Fresh start
gitignore --version
```

## 📞 Getting Help

### Self-Service Resources

**Documentation:**

```bash
# Built-in help
gitignore --help

# Manual page
man gitignore

# Online documentation
xdg-open https://github.com/mahbubhs/gitignore/docs/
```

**Community Resources:**

- **GitHub Issues:** https://github.com/mahbubhs/gitignore/issues
- **Discussions:** https://github.com/mahbubhs/gitignore/discussions
- **Stack Overflow:** Search for "gitignore tool troubleshooting"

### Reporting Bugs

**Bug Report Template:**

````markdown
## Bug Report

### System Information

- OS: [e.g., Ubuntu 22.04]
- Version: `gitignore --version`
- Installation method: [make, package manager, etc.]

### Steps to Reproduce

1. Run `gitignore init python`
2. Observe error: [describe error]
3. Expected behavior: [what should happen]

### Diagnostic Information

```bash
# Configuration
cat ~/.config/gitignore/config.conf

# Environment
echo $PATH
echo $LD_LIBRARY_PATH

# System details
uname -a
ldd $(which gitignore)
```
````

### Additional Context

- When did this start happening?
- Any recent changes to system?
- Works on other systems?

````

### Feature Requests

**Request Template:**
```markdown
## Feature Request

### Problem Statement
[Describe the problem this feature would solve]

### Proposed Solution
[Describe the feature and how it would work]

### Alternative Solutions
[Other approaches considered]

### Additional Context
[Screenshots, examples, related issues]
````

## 📚 Related Documentation

- **[Installation Guide](installation.md)** - Setup and system requirements
- **[Usage Guide](usage.md)** - Command reference and examples
- **[Configuration Guide](configuration.md)** - Customization options
- **[Building Guide](building.md)** - Development environment setup
- **[API Reference](api-reference.md)** - Internal function documentation
