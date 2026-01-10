#!/bin/bash
# install.sh - Smart installation script with PREFIX detection

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

# Smart PREFIX detection
# Priority: 1. Command line arg, 2. Environment variable, 3. Default
if [ -n "$1" ]; then
    INSTALL_PREFIX="$1"
elif [ -n "$PREFIX" ]; then
    INSTALL_PREFIX="$PREFIX"
else
    INSTALL_PREFIX="/usr/local"
fi

REPO_URL="https://github.com/mahbubhs/gitignore"
BINARY_NAME="gitignore"
INSTALL_DIR="$INSTALL_PREFIX/bin"
MANDIR="$INSTALL_PREFIX/share/man/man1"
VERSION="2.0.0"

print_banner() {
    echo -e "${CYAN}${BOLD}"
    echo "╔════════════════════════════════════════╗"
    echo "║                                        ║"
    echo "║     GITIGNORE TOOL INSTALLER v2.0      ║"
    echo "║                                        ║"
    echo "╚════════════════════════════════════════╝"
    echo -e "${NC}"
}

print_success() {
    echo -e "${GREEN}✓${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

print_info() {
    echo -e "${BLUE}ℹ${NC} $1"
}

show_prefix_info() {
    echo -e "${CYAN}${BOLD}Installation Configuration:${NC}"
    echo -e "  PREFIX:    ${BOLD}$INSTALL_PREFIX${NC}"
    echo -e "  Binary:    ${BOLD}$INSTALL_DIR/$BINARY_NAME${NC}"
    echo -e "  Man page:  ${BOLD}$MANDIR/gitignore.1${NC}"
    echo ""
    
    if [ "$INSTALL_PREFIX" != "/usr/local" ]; then
        print_warning "Using custom PREFIX: $INSTALL_PREFIX"
        echo ""
    fi
}

check_requirements() {
    print_info "Checking requirements..."
    
    local missing_deps=()
    
    if ! command -v gcc &> /dev/null && ! command -v clang &> /dev/null; then
        missing_deps+=("gcc or clang")
    fi
    
    if ! command -v make &> /dev/null; then
        missing_deps+=("make")
    fi
    
    if ! command -v curl &> /dev/null; then
        missing_deps+=("curl")
    fi
    
    if ! ldconfig -p 2>/dev/null | grep -q libcurl; then
        missing_deps+=("libcurl-dev")
    fi
    
    if [ ${#missing_deps[@]} -gt 0 ]; then
        print_error "Missing dependencies: ${missing_deps[*]}"
        echo ""
        print_info "Install missing dependencies:"
        echo ""
        
        if [ -f /etc/debian_version ]; then
            echo "  ${BOLD}sudo apt-get update${NC}"
            echo "  ${BOLD}sudo apt-get install -y build-essential libcurl4-openssl-dev${NC}"
        elif [ -f /etc/redhat-release ]; then
            echo "  ${BOLD}sudo dnf install -y gcc make libcurl-devel${NC}"
        elif [ "$(uname)" == "Darwin" ]; then
            echo "  ${BOLD}brew install curl${NC}"
        else
            echo "  Please install: ${missing_deps[*]}"
        fi
        
        echo ""
        read -p "Would you like to continue anyway? (y/N) " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            exit 1
        fi
    else
        print_success "All requirements met"
    fi
}

detect_os() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        OS=$ID
        VER=$VERSION_ID
    elif [ "$(uname)" == "Darwin" ]; then
        OS="macos"
        VER=$(sw_vers -productVersion)
    else
        OS=$(uname -s)
        VER=$(uname -r)
    fi
    
    print_info "Detected OS: $OS $VER"
}

clone_or_download() {
    print_info "Downloading source code..."
    
    local temp_dir=$(mktemp -d)
    cd "$temp_dir"
    
    if command -v git &> /dev/null; then
        print_info "Cloning from GitHub..."
        if git clone --depth 1 "$REPO_URL.git" gitignore 2>/dev/null; then
            cd gitignore
            print_success "Repository cloned"
            return 0
        else
            print_warning "Git clone failed, trying alternative method..."
        fi
    fi
    
    print_info "Downloading tarball..."
    if curl -sSL "$REPO_URL/archive/refs/heads/main.tar.gz" -o gitignore.tar.gz; then
        tar -xzf gitignore.tar.gz
        cd gitignore-main 2>/dev/null || cd gitignore-master 2>/dev/null || {
            print_error "Failed to extract archive"
            exit 1
        }
        print_success "Source downloaded"
    else
        print_error "Failed to download source code"
        print_info "Please visit: $REPO_URL"
        exit 1
    fi
}

build_from_source() {
    print_info "Building from source..."
    
    # Generate templates
    if make templates &> build.log; then
        print_success "Templates generated"
    else
        print_error "Failed to generate templates"
        cat build.log
        exit 1
    fi
    
    # Build with PREFIX
    if make PREFIX="$INSTALL_PREFIX" >> build.log 2>&1; then
        print_success "Build successful"
    else
        print_error "Build failed"
        cat build.log
        exit 1
    fi
}

install_binary() {
    print_info "Installing binary..."
    
    # Check if we need sudo
    if [ -w "$INSTALL_DIR" ]; then
        SUDO=""
    else
        SUDO="sudo"
        print_warning "Requires sudo privileges for installation to $INSTALL_PREFIX"
    fi
    
    # Install with PREFIX
    if $SUDO make PREFIX="$INSTALL_PREFIX" install &> /dev/null; then
        print_success "Installed to $INSTALL_DIR/$BINARY_NAME"
    else
        print_error "Installation failed"
        
        # Try alternative installation
        print_info "Trying alternative installation method..."
        $SUDO install -d "$INSTALL_DIR"
        if $SUDO cp "$BINARY_NAME" "$INSTALL_DIR/"; then
            $SUDO chmod +x "$INSTALL_DIR/$BINARY_NAME"
            print_success "Binary installed manually"
            
            # Try to install man page
            if [ -f man/gitignore.1 ]; then
                $SUDO install -d "$MANDIR"
                $SUDO cp man/gitignore.1 "$MANDIR/"
                print_success "Man page installed"
            fi
        else
            print_error "Could not install binary"
            exit 1
        fi
    fi
}

update_path() {
    # Check if INSTALL_DIR is in PATH
    if [[ ":$PATH:" != *":$INSTALL_DIR:"* ]]; then
        print_warning "Installation directory not in PATH"
        echo ""
        print_info "Add to your PATH by adding this to ~/.bashrc or ~/.zshrc:"
        echo ""
        echo "  ${BOLD}export PATH=\"$INSTALL_DIR:\$PATH\"${NC}"
        echo ""
        
        read -p "Would you like to add it now? (y/N) " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            local shell_rc="$HOME/.bashrc"
            if [ -f "$HOME/.zshrc" ]; then
                shell_rc="$HOME/.zshrc"
            fi
            
            echo "" >> "$shell_rc"
            echo "# Added by gitignore installer" >> "$shell_rc"
            echo "export PATH=\"$INSTALL_DIR:\$PATH\"" >> "$shell_rc"
            
            print_success "Added to $shell_rc"
            print_info "Restart your shell or run: source $shell_rc"
        fi
    fi
}

setup_config() {
    print_info "Setting up configuration..."
    
    local config_dir="$HOME/.config/gitignore"
    
    mkdir -p "$config_dir"/{templates,cache,backups}
    
    if [ ! -f "$config_dir/config.conf" ]; then
        cat > "$config_dir/config.conf" << 'EOF'
# gitignore configuration file

auto_backup=true
cache_enabled=true
cache_duration=86400
verbose=false
use_color=true
EOF
        print_success "Configuration created"
    else
        print_info "Configuration already exists, skipping"
    fi
    
    if [ ! -f "$config_dir/templates/auto.gitignore" ]; then
        cat > "$config_dir/templates/auto.gitignore" << 'EOF'
# Auto template
.DS_Store
*.log
*.tmp
.env
node_modules/
__pycache__/
EOF
        print_success "Auto template created"
    fi
}

verify_installation() {
    print_info "Verifying installation..."
    
    if command -v "$BINARY_NAME" &> /dev/null; then
        local installed_version=$($BINARY_NAME --version | head -1 | awk '{print $NF}')
        print_success "Installation verified"
        print_info "Installed version: $installed_version"
        
        if $BINARY_NAME list &> /dev/null; then
            print_success "Basic functionality test passed"
        else
            print_warning "Basic functionality test failed"
        fi
    else
        print_error "Verification failed - command not found"
        print_info "Binary location: $INSTALL_DIR/$BINARY_NAME"
        update_path
    fi
}

show_completion() {
    echo ""
    echo -e "${GREEN}${BOLD}════════════════════════════════════════${NC}"
    echo -e "${GREEN}${BOLD}    Installation Complete! 🎉${NC}"
    echo -e "${GREEN}${BOLD}════════════════════════════════════════${NC}"
    echo ""
    echo "Installation details:"
    echo "  PREFIX:   ${BOLD}$INSTALL_PREFIX${NC}"
    echo "  Binary:   ${BOLD}$INSTALL_DIR/$BINARY_NAME${NC}"
    echo "  Man page: ${BOLD}$MANDIR/gitignore.1${NC}"
    echo ""
    echo "Quick start:"
    echo "  ${BOLD}gitignore --help${NC}          # Show help"
    echo "  ${BOLD}gitignore auto${NC}            # Auto-detect project"
    echo "  ${BOLD}gitignore init python${NC}     # Create Python .gitignore"
    echo "  ${BOLD}gitignore -t${NC}              # Interactive mode"
    echo ""
    echo "Configuration:"
    echo "  ${CYAN}$HOME/.config/gitignore/${NC}"
    echo ""
    echo "Documentation:"
    echo "  ${BLUE}$REPO_URL${NC}"
    echo ""
}

uninstall() {
    print_info "Uninstalling gitignore from $INSTALL_PREFIX..."
    
    if [ -w "$INSTALL_DIR" ]; then
        SUDO=""
    else
        SUDO="sudo"
    fi
    
    if $SUDO rm -f "$INSTALL_DIR/$BINARY_NAME"; then
        print_success "Binary removed from $INSTALL_DIR"
    fi
    
    if $SUDO rm -f "$MANDIR/gitignore.1"; then
        print_success "Manual page removed from $MANDIR"
    fi
    
    read -p "Remove configuration files? (y/N) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        rm -rf "$HOME/.config/gitignore"
        print_success "Configuration removed"
    fi
    
    print_success "Uninstallation complete"
}

main() {
    # Parse arguments
    case "${1:-}" in
        --uninstall|-u)
            print_banner
            uninstall
            exit 0
            ;;
        --help|-h)
            echo "Usage: $0 [OPTIONS] [PREFIX]"
            echo ""
            echo "Options:"
            echo "  -h, --help       Show this help message"
            echo "  -u, --uninstall  Uninstall gitignore"
            echo ""
            echo "Examples:"
            echo "  $0                          # Install to /usr/local (default)"
            echo "  $0 ~/.local                 # Install to ~/.local"
            echo "  PREFIX=~/.local $0          # Install using environment variable"
            echo ""
            echo "Environment Variables:"
            echo "  PREFIX       Installation prefix (default: /usr/local)"
            echo ""
            exit 0
            ;;
    esac
    
    print_banner
    show_prefix_info
    
    detect_os
    check_requirements
    
    original_dir=$(pwd)
    
    clone_or_download
    build_from_source
    install_binary
    
    cd "$original_dir"
    
    setup_config
    verify_installation
    show_completion
}

# Handle Ctrl+C
trap 'echo -e "\n${RED}Installation cancelled${NC}"; exit 1' INT

# Run main
main "$@"