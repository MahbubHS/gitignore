# Troubleshooting Guide

Common issues and solutions for gitignore.

## 🚨 Quick Diagnosis

### Check Installation

```bash
# Verify gitignore is installed
which gitignore

# Check version
gitignore --version

# Test basic functionality
gitignore --help
```

### Check Permissions

```bash
# Check binary permissions
ls -la $(which gitignore)

# Check config directory permissions
ls -la ~/.config/gitignore/

# Check current directory permissions
ls -la .gitignore 2>/dev/null || echo "No .gitignore found"
```

## 🆘 Common Issues

### "Command not found"

**Symptoms:**

```bash
$ gitignore --version
bash: gitignore: command not found
```

**Solutions:**

1. **Check PATH:**

   ```bash
   echo $PATH
   which gitignore
   ```

2. **Add to PATH:**

   ```bash
   # Add to ~/.bashrc or ~/.zshrc
   export PATH="$HOME/.local/bin:$PATH"
   source ~/.bashrc
   ```

3. **Reinstall:**
   ```bash
   # If installed with make
   sudo make uninstall
   make PREFIX=~/.local install
   ```

### "Permission denied"

**Symptoms:**

```bash
$ gitignore init python
mkdir: cannot create directory '.config': Permission denied
```

**Solutions:**

1. **Fix directory permissions:**

   ```bash
   chmod 755 ~/.config
   ```

2. **Use sudo for system operations:**

   ```bash
   sudo gitignore init python
   ```

3. **Install to user directory:**
   ```bash
   make PREFIX=~/.local install
   ```

### "Template not found"

**Symptoms:**

```bash
$ gitignore init nonexistent
✗ Error: Template 'nonexistent' not found
```

**Solutions:**

1. **List available templates:**

   ```bash
   gitignore list
   ```

2. **Check spelling:**

   ```bash
   gitignore list py  # Shows python-related
   ```

3. **Use custom templates:**
   ```bash
   # Create custom template
   mkdir -p ~/.config/gitignore/templates
   echo "*.custom" > ~/.config/gitignore/templates/mytemplate.gitignore
   gitignore init mytemplate
   ```

### "Network error" (GitHub sync)

**Symptoms:**

```bash
$ gitignore sync python
✗ Error: Network error - Failed to download template
```

**Solutions:**

1. **Check internet connection:**

   ```bash
   curl -I https://github.com
   ```

2. **Use cached templates:**

   ```bash
   # Disable cache temporarily
   gitignore cache clear
   ```

3. **Use built-in templates instead:**
   ```bash
   gitignore init python  # Uses built-in
   ```

### "libcurl not found"

**Symptoms:**

```bash
$ make
/usr/bin/ld: cannot find -lcurl
```

**Solutions:**

1. **Install libcurl development headers:**

   ```bash
   # Ubuntu/Debian
   sudo apt-get install libcurl4-openssl-dev

   # macOS
   brew install curl

   # Fedora
   sudo dnf install libcurl-devel
   ```

2. **Check library path:**
   ```bash
   pkg-config --libs libcurl
   ```

### "Templates not generated"

**Symptoms:**

```bash
$ make
src/templates.c: No such file or directory
```

**Solutions:**

1. **Generate templates first:**

   ```bash
   make templates
   ```

2. **Check templates directory:**

   ```bash
   ls -la templates/
   ```

3. **Regenerate templates:**
   ```bash
   make regen-templates
   ```

## 🔧 Build Issues

### Compiler Errors

**"undefined reference" errors:**

```bash
# Check libcurl installation
pkg-config --libs libcurl

# Rebuild with verbose output
make V=1
```

**Header file not found:**

```bash
# Install development headers
sudo apt-get install libc6-dev

# Check include paths
gcc -E -Wp,-v -xc /dev/null
```

### Makefile Issues

**"No rule to make target":**

```bash
# Check Makefile syntax
make -n

# Clean and rebuild
make clean
make
```

**Variables not set:**

```bash
# Check Makefile variables
make show-prefix

# Override variables
make CC=gcc CFLAGS="-O2 -Wall"
```

## ⚙️ Configuration Issues

### Config File Not Loading

**Symptoms:**

- Settings not applied
- Default behavior used

**Solutions:**

1. **Check config file location:**

   ```bash
   ls -la ~/.config/gitignore/config.conf
   ```

2. **Validate syntax:**

   ```bash
   cat ~/.config/gitignore/config.conf
   # Should be key=value format
   ```

3. **Check permissions:**

   ```bash
   chmod 644 ~/.config/gitignore/config.conf
   ```

4. **Reset configuration:**
   ```bash
   rm ~/.config/gitignore/config.conf
   # Restart with defaults
   ```

### Cache Problems

**Symptoms:**

- Old templates used
- Sync not working

**Solutions:**

1. **Clear cache:**

   ```bash
   gitignore cache clear
   ```

2. **Check cache directory:**

   ```bash
   ls -la ~/.config/gitignore/cache/
   ```

3. **Disable cache:**
   ```bash
   echo "cache_enabled=false" >> ~/.config/gitignore/config.conf
   ```

## 📁 File System Issues

### .gitignore Not Created

**Symptoms:**

```bash
$ gitignore init python
✓ Template applied successfully
$ ls .gitignore
ls: cannot access '.gitignore': No such file or directory
```

**Solutions:**

1. **Check write permissions:**

   ```bash
   touch .gitignore
   ls -la .gitignore
   ```

2. **Check disk space:**

   ```bash
   df -h .
   ```

3. **Use verbose mode:**
   ```bash
   gitignore --verbose init python
   ```

### Backup/Restore Issues

**Symptoms:**

- Backups not created
- Restore fails

**Solutions:**

1. **Check backup directory:**

   ```bash
   ls -la ~/.config/gitignore/backups/
   ```

2. **List available backups:**

   ```bash
   gitignore backups
   ```

3. **Manual backup:**
   ```bash
   cp .gitignore .gitignore.backup
   ```

## 🌐 Platform-Specific Issues

### Linux Issues

**AppArmor/SELinux blocking:**

```bash
# Check SELinux status
sestatus

# Temporarily disable for testing
setenforce 0
```

**Missing libraries:**

```bash
# Check dynamic linking
ldd $(which gitignore)

# Reinstall dependencies
sudo apt-get install --reinstall libcurl4-openssl-dev
```

### macOS Issues

**Xcode Command Line Tools:**

```bash
# Install/reinstall
xcode-select --install

# Accept license
sudo xcodebuild -license accept
```

**Homebrew issues:**

```bash
# Update Homebrew
brew update

# Reinstall curl
brew reinstall curl
```

### Windows (WSL) Issues

**Path conversion:**

```bash
# Use WSL paths
cd /mnt/c/Users/YourName/project

# Check WSL version
wsl --version
```

**File permission mapping:**

```bash
# Check mount options
mount | grep mnt

# Remount with metadata
sudo mount -t drvfs C: /mnt/c -o metadata
```

## 🔍 Debug Mode

### Enable Debug Output

```bash
# Verbose mode
gitignore --verbose init python

# Debug configuration
echo "verbose=true" >> ~/.config/gitignore/config.conf

# Debug build
make dev
gdb ./gitignore
```

### Log Analysis

```bash
# Check system logs
journalctl -f | grep gitignore

# Network debugging
curl -v https://raw.githubusercontent.com/github/gitignore/main/Python.gitignore
```

### Environment Inspection

```bash
# Show environment
env | grep -i git

# Check library paths
echo $LD_LIBRARY_PATH

# Check pkg-config
pkg-config --list-all | grep curl
```

## 🚑 Emergency Recovery

### Restore from Backup

```bash
# List backups
gitignore backups

# Restore specific backup
gitignore restore backup_2024-01-10_14-30-00

# Manual restore
cp ~/.config/gitignore/backups/backup_2024-01-10_14-30-00 .gitignore
```

### Clean Reinstall

```bash
# Remove everything
sudo make uninstall
rm -rf ~/.config/gitignore
rm -rf /tmp/gitignore-*

# Fresh install
git clone https://github.com/mahbubhs/gitignore.git
cd gitignore
make clean
make templates
make
sudo make install
```

### Factory Reset

```bash
# Remove all user data
rm -rf ~/.config/gitignore

# Reset PATH
unset GITIGNORE_CONFIG_DIR

# Fresh start
gitignore --version
```

## 📞 Getting Help

### Self-Help Resources

1. **Check documentation:**

   ```bash
   gitignore --help
   man gitignore
   ```

2. **Run diagnostics:**

   ```bash
   gitignore --verbose --version
   ```

3. **Search issues:**
   - GitHub Issues: https://github.com/mahbubhs/gitignore/issues

### Reporting Issues

When reporting bugs, include:

```bash
# System information
uname -a
gitignore --version

# Configuration
cat ~/.config/gitignore/config.conf

# Error reproduction
gitignore --verbose init python 2>&1 | head -50
```

## 📚 Related Topics

- [Installation](installation.md) - Setup issues
- [Configuration](configuration.md) - Config problems
- [Building](building.md) - Build issues
