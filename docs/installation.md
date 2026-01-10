# Installation Guide

This comprehensive guide covers all supported installation methods for gitignore across different platforms and environments.

## 📋 System Requirements

### Minimum Requirements

| Component            | Requirement                      | Notes                             |
| -------------------- | -------------------------------- | --------------------------------- |
| **Operating System** | Linux, macOS 10.12+, Windows 10+ | Cross-platform C application      |
| **Processor**        | x86_64, ARM64                    | Native architecture support       |
| **Memory**           | 256 MB RAM                       | Minimal runtime footprint         |
| **Storage**          | 50 MB                            | Includes source, build artifacts  |
| **Network**          | Optional                         | Required for GitHub sync features |

### Build Requirements (Source Installation)

| Component     | Version     | Purpose                    |
| ------------- | ----------- | -------------------------- |
| **GCC/Clang** | 4.8+ / 3.5+ | C compiler                 |
| **GNU Make**  | 3.81+       | Build system               |
| **libcurl**   | 7.50+       | HTTP client library        |
| **Git**       | 2.0+        | Version control (optional) |

## 🚀 Quick Installation

### Automated Script (Recommended)

The fastest way to install gitignore with zero configuration:

```bash
curl -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash
```

**What this does:**

- ✅ Downloads latest stable release
- ✅ Installs to `/usr/local/bin` (sudo required)
- ✅ Verifies installation integrity
- ✅ Sets up proper permissions
- ✅ Provides post-installation feedback

**Exit codes:**

- `0`: Success
- `1`: Download failed
- `2`: Verification failed
- `3`: Installation failed

### Platform-Specific One-Liners

#### Linux (apt-based)

```bash
# Ubuntu, Debian, Pop!_OS, etc.
sudo apt-get update && sudo apt-get install -y curl && \
curl -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash
```

#### Linux (dnf-based)

```bash
# Fedora, RHEL, CentOS
sudo dnf install -y curl && \
curl -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash
```

#### macOS

```bash
# Using curl (built-in)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh)"
```

#### Windows (PowerShell)

```powershell
# Requires curl.exe in PATH
curl.exe -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash.exe
```

## 🔧 Custom Installation Paths

### User Directory (No Root Required)

Install to your home directory without administrator privileges:

```bash
# Install to ~/.local/bin
curl -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash -s ~/.local

# Add to PATH (add to ~/.bashrc or ~/.zshrc)
export PATH="$HOME/.local/bin:$PATH"
```

### System-Wide Installation

```bash
# Install to /usr/local/bin (default)
curl -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | sudo bash

# Install to custom system path
curl -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | sudo bash -s /opt/bin
```

### Environment Variable Method

```bash
# Set custom installation prefix
export PREFIX=/opt/gitignore
curl -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | sudo bash
```

## 🏗️ Source Installation

### Prerequisites Installation

#### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install -y build-essential libcurl4-openssl-dev git
```

#### Fedora/RHEL/CentOS

```bash
sudo dnf install -y gcc make libcurl-devel git
```

#### Arch Linux

```bash
sudo pacman -S gcc make curl git
```

#### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (optional, for additional tools)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install curl git
```

#### Windows (MSYS2/MinGW)

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-curl mingw-w64-x86_64-make git
```

### Build Process

1. **Clone the repository:**

   ```bash
   git clone https://github.com/mahbubhs/gitignore.git
   cd gitignore
   ```

2. **Generate template database:**

   ```bash
   make templates
   ```

   This creates `src/templates.c` from `templates/*.gitignore` files.

3. **Compile the application:**

   ```bash
   make
   ```

   This produces the `gitignore` binary.

4. **Install (optional):**

   ```bash
   sudo make install
   ```

5. **Verify installation:**
   ```bash
   ./gitignore --version
   ./gitignore --help
   ```

### Build Customization

#### Debug Build

```bash
make CFLAGS="-g -O0 -Wall -Wextra" LDFLAGS="-g"
```

#### Optimized Build

```bash
make CFLAGS="-O3 -march=native -flto" LDFLAGS="-flto"
```

#### Cross-Compilation

```bash
# ARM64 cross-compilation
make CC=aarch64-linux-gnu-gcc
```

#### Custom Compiler

```bash
make CC=clang
```

## 📦 Package Manager Installation

### Snap (Linux)

```bash
sudo snap install gitignore --edge
sudo snap connect gitignore:home  # Optional: access home directory
```

### Homebrew (macOS/Linux)

```bash
brew tap mahbubhs/gitignore
brew install gitignore
```

_Note: Homebrew formula pending official publication._

### Docker

```bash
# Pull and run
docker run --rm -v $(pwd):/workspace ghcr.io/mahbubhs/gitignore:latest --help

# Build locally
docker build -t gitignore .
```

### Nix (Linux/macOS)

```nix
# shell.nix
{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  buildInputs = with pkgs; [
    (import (fetchTarball "https://github.com/mahbubhs/gitignore/archive/main.tar.gz") {})
  ];
}
```

## 🔍 Post-Installation Verification

### Basic Verification

```bash
# Check installation
which gitignore
gitignore --version

# Test core functionality
gitignore --help
gitignore list
```

### Comprehensive Testing

```bash
# Create test environment
mkdir /tmp/gitignore-test && cd /tmp/gitignore-test

# Test template initialization
gitignore init python
cat .gitignore

# Test auto-detection (create package.json first)
echo '{}' > package.json
gitignore auto

# Test pattern addition
gitignore *.log .env

# Cleanup
cd / && rm -rf /tmp/gitignore-test
```

### Integration Testing

```bash
# Test with Git
git init /tmp/test-repo
cd /tmp/test-repo
gitignore init rust
git status
```

## 🆘 Troubleshooting Installation

### Common Installation Issues

#### "curl: command not found"

```bash
# Install curl first
# Ubuntu/Debian
sudo apt-get install curl

# macOS
# curl is built-in, check PATH
which curl
```

#### "Permission denied" during installation

```bash
# Use sudo for system directories
curl -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | sudo bash

# Or install to user directory
curl -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash -s ~/.local
```

#### "make: command not found"

```bash
# Install build tools
# Ubuntu/Debian
sudo apt-get install build-essential

# macOS
xcode-select --install
```

#### "libcurl not found" during build

```bash
# Install development headers
# Ubuntu/Debian
sudo apt-get install libcurl4-openssl-dev

# Check library path
pkg-config --libs libcurl
```

### Build Troubleshooting

#### Compilation errors

```bash
# Check compiler version
gcc --version

# Clean and rebuild
make clean
make

# Verbose build output
make V=1
```

#### Template generation fails

```bash
# Check template files exist
ls -la templates/

# Check script permissions
chmod +x scripts/generate_templates.sh

# Manual generation
./scripts/generate_templates.sh
```

### Runtime Issues

#### "Command not found" after installation

```bash
# Check PATH
echo $PATH
which gitignore

# Add to PATH
export PATH="/usr/local/bin:$PATH"
```

#### "Library not found" errors

```bash
# Check dynamic linking
ldd $(which gitignore)

# Reinstall dependencies
sudo apt-get install --reinstall libcurl4-openssl-dev
```

## 🔄 Upgrading

### Automated Upgrade

```bash
# Re-run installation script
curl -fsSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash
```

### Manual Upgrade

```bash
# Update source
cd /path/to/gitignore/source
git pull
make clean
make templates
make
sudo make install
```

### Checking Version

```bash
# Current version
gitignore --version

# Check for updates
curl -s https://api.github.com/repos/mahbubhs/gitignore/releases/latest | grep '"tag_name"' | cut -d'"' -f4
```

## 🗑️ Uninstallation

### Script-based Installation

```bash
# Remove binary
sudo rm /usr/local/bin/gitignore
sudo rm /usr/local/share/man/man1/gitignore.1

# Remove user data
rm -rf ~/.config/gitignore
```

### Source-based Installation

```bash
# From source directory
sudo make uninstall

# Manual cleanup
rm -rf ~/.config/gitignore
```

### Complete Cleanup

```bash
# Remove all traces
sudo rm -f /usr/local/bin/gitignore
sudo rm -f /usr/local/share/man/man1/gitignore.1
rm -rf ~/.config/gitignore
rm -rf ~/.cache/gitignore  # If exists
```

## 📊 Installation Analytics

| Method               | Platforms         | Root Required | Offline Capable |
| -------------------- | ----------------- | ------------- | --------------- |
| **Automated Script** | Linux, macOS, WSL | Optional      | No              |
| **Source Build**     | All               | Optional      | Yes             |
| **Package Managers** | Limited           | Varies        | Varies          |
| **Docker**           | All               | No            | No              |

## 📚 Next Steps

Once installed, explore:

- **[Quick Start Guide](quick-start.md)** - Basic usage patterns
- **[Usage Reference](usage.md)** - Complete command documentation
- **[Configuration Guide](configuration.md)** - Customize behavior
- **[Template Reference](templates.md)** - Available templates

## 🔗 Related Documentation

- **[Building Guide](building.md)** - Development setup
- **[Contributing Guide](contributing.md)** - Development workflow
- **[Troubleshooting](troubleshooting.md)** - Common issues
