# Installation Guide

This guide covers all the ways to install gitignore on your system.

## 📥 Quick Install (Recommended)

The easiest way to install gitignore is using our automated installer:

```bash
curl -sSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash
```

This will:
- Download the latest release
- Install to `/usr/local/bin` (requires sudo)
- Make gitignore available system-wide

## 🔧 Custom Installation Location

### Install to User Directory (No Root Required)

```bash
# Install to ~/.local/bin
curl -sSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash -s ~/.local

# Add to PATH if needed
export PATH="$HOME/.local/bin:$PATH"
```

### Using Environment Variables

```bash
# Set custom prefix
export PREFIX=~/.local
curl -sSL https://raw.githubusercontent.com/mahbubhs/gitignore/main/install.sh | bash
```

## 🏗️ Build from Source

For developers or custom builds, you can compile from source.

### Prerequisites

**Required Dependencies:**
- GCC or Clang compiler
- Make build system
- libcurl development libraries

**Install on Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential libcurl4-openssl-dev
```

**Install on macOS:**
```bash
# Using Homebrew
brew install curl

# Or using Xcode Command Line Tools
xcode-select --install
```

**Install on Fedora/RHEL:**
```bash
sudo dnf install gcc make libcurl-devel
```

**Install on Arch Linux:**
```bash
sudo pacman -S gcc make curl
```

### Build Steps

1. **Clone the repository:**
   ```bash
   git clone https://github.com/mahbubhs/gitignore.git
   cd gitignore
   ```

2. **Generate templates:**
   ```bash
   make templates
   ```

3. **Build the binary:**
   ```bash
   make
   ```

4. **Install system-wide:**
   ```bash
   sudo make install
   ```

5. **Verify installation:**
   ```bash
   gitignore --version
   ```

### Custom Installation Path

```bash
# Install to custom location
make PREFIX=/opt/gitignore install

# Or set environment variable
export PREFIX=/usr/local
make install
```

## 📦 Distribution Packages

### Snap Package (Linux)

```bash
sudo snap install gitignore --edge
```

### Homebrew (macOS/Linux)

```bash
brew install gitignore
```

*Note: Homebrew formula coming soon. For now, use source build.*

### Docker

```bash
# Run without installing
docker run --rm -v $(pwd):/workspace ghcr.io/mahbubhs/gitignore:latest gitignore --help

# Or build your own
docker build -t gitignore .
```

## 🔍 Verification

After installation, verify everything works:

```bash
# Check version
gitignore --version

# Show help
gitignore --help

# List available templates
gitignore list
```

## 🆘 Troubleshooting Installation

### Common Issues

**"Command not found" after installation:**
- Check if the installation directory is in your PATH
- Try running with full path: `/usr/local/bin/gitignore --version`

**Build fails with curl errors:**
- Install libcurl development headers
- On Ubuntu: `sudo apt-get install libcurl4-openssl-dev`

**Permission denied during install:**
- Use `sudo make install` for system-wide installation
- Or install to user directory: `make PREFIX=~/.local install`

**Templates not working:**
- Ensure you ran `make templates` before `make`
- Check that `templates/` directory exists and has `.gitignore` files

### Uninstalling

To remove gitignore:

```bash
# If installed with make
sudo make uninstall

# Or manually remove the binary
rm /usr/local/bin/gitignore
rm /usr/local/share/man/man1/gitignore.1
```

## 📋 System Requirements

- **Operating System:** Linux, macOS, Windows (via WSL)
- **Architecture:** x86_64, ARM64
- **Memory:** Minimal (templates compiled into binary)
- **Storage:** ~2MB for binary + templates

## 🚀 Next Steps

Once installed, check out:
- [Quick Start Guide](quick-start.md) - Get up and running
- [Usage Guide](usage.md) - Learn all commands
- [Configuration](configuration.md) - Customize behavior