# Makefile for gitignore tool v2.0 - SMART VERSION

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -I.
LDFLAGS = -lcurl

# Smart PREFIX detection from environment or default
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man/man1

TARGET = gitignore
SRCDIR = src
SOURCES = main.c help.c init.c sync.c utils.c features.c global_backup.c cache_config.c templates.c
OBJECTS = $(SOURCES:%.c=$(SRCDIR)/%.o)
HEADERS = gitignore.h

TEMPLATE_DIR = templates
TEMPLATE_GEN = scripts/generate_templates.sh

VERSION = 2.0.0

.PHONY: all clean install uninstall dirs test package templates help

all: templates dirs $(TARGET)

# Show detected PREFIX
show-prefix:
	@echo "Installation prefix: $(PREFIX)"
	@echo "Binary will be installed to: $(BINDIR)"
	@echo "Man page will be installed to: $(MANDIR)"
	@echo ""
	@echo "To change prefix, use: make PREFIX=/your/path install"
	@echo "Or set environment: export PREFIX=/your/path"

dirs:
	@mkdir -p $(SRCDIR) man scripts

# Generate templates.c from templates/ directory
templates: $(TEMPLATE_DIR)/*.gitignore $(TEMPLATE_GEN)
	@echo "Generating templates.c from templates/ directory..."
	@chmod +x $(TEMPLATE_GEN)
	@$(TEMPLATE_GEN)

# If template generator doesn't exist, create it
$(TEMPLATE_GEN):
	@echo "Creating template generator script..."
	@mkdir -p scripts
	@echo "Template generator not found. Please ensure scripts/generate_templates.sh exists"

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo ""
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "✓ Build complete: $(TARGET)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo ""
	@echo "Installation prefix: $(PREFIX)"
	@echo "  Binary: $(BINDIR)/$(TARGET)"
	@echo "  Man page: $(MANDIR)/gitignore.1"
	@echo ""
	@echo "Install with: make install"
	@echo "Or with custom prefix: make PREFIX=/your/path install"
	@echo ""

$(SRCDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET) show-prefix
	@echo ""
	@echo "Installing gitignore to $(PREFIX)..."
	@echo ""
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)
	@echo "  ✓ Binary installed to $(BINDIR)/$(TARGET)"
	@if [ -f man/gitignore.1 ]; then \
		install -d $(MANDIR); \
		install -m 644 man/gitignore.1 $(MANDIR); \
		echo "  ✓ Manual page installed to $(MANDIR)/gitignore.1"; \
	fi
	@echo ""
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "✓ Installation complete!"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo ""
	@echo "Run: gitignore --help"
	@echo ""

uninstall:
	@echo "Uninstalling from $(PREFIX)..."
	rm -f $(BINDIR)/$(TARGET)
	rm -f $(MANDIR)/gitignore.1
	@echo "✓ Uninstalled from $(PREFIX)"

clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "✓ Clean complete"

# Clean everything including generated templates.c
distclean: clean
	@echo "Removing generated files..."
	rm -f $(SRCDIR)/templates.c
	@echo "✓ Distribution clean complete"

# Regenerate templates
regen-templates:
	@echo "Regenerating templates.c..."
	@rm -f $(SRCDIR)/templates.c
	@$(MAKE) templates

# Test targets
test: $(TARGET)
	@echo "Running tests..."
	@echo ""
	@./$(TARGET) --version
	@echo ""
	@./$(TARGET) list
	@echo ""
	@echo "✓ Basic tests passed"

# Development build with debug symbols
dev: CFLAGS += -g -DDEBUG
dev: clean all

# Package for distribution
package: all
	@echo "Creating distribution package..."
	@mkdir -p dist/gitignore-$(VERSION)
	@cp $(TARGET) README.md LICENSE dist/gitignore-$(VERSION)/
	@cp -r $(TEMPLATE_DIR) dist/gitignore-$(VERSION)/
	@cp -r scripts dist/gitignore-$(VERSION)/
	@cd dist && tar -czf gitignore-$(VERSION).tar.gz gitignore-$(VERSION)
	@echo "✓ Package created: dist/gitignore-$(VERSION).tar.gz"

# Create initial template directory structure
setup-templates:
	@echo "Setting up templates directory..."
	@mkdir -p $(TEMPLATE_DIR)
	@if [ ! -f $(TEMPLATE_DIR)/python.gitignore ]; then \
		echo "Creating sample Python template..."; \
		echo "# Python" > $(TEMPLATE_DIR)/python.gitignore; \
		echo "__pycache__/" >> $(TEMPLATE_DIR)/python.gitignore; \
		echo "*.py[cod]" >> $(TEMPLATE_DIR)/python.gitignore; \
		echo "venv/" >> $(TEMPLATE_DIR)/python.gitignore; \
		echo ".env" >> $(TEMPLATE_DIR)/python.gitignore; \
	fi
	@if [ ! -f $(TEMPLATE_DIR)/node.gitignore ]; then \
		echo "Creating sample Node template..."; \
		echo "# Node.js" > $(TEMPLATE_DIR)/node.gitignore; \
		echo "node_modules/" >> $(TEMPLATE_DIR)/node.gitignore; \
		echo "*.log" >> $(TEMPLATE_DIR)/node.gitignore; \
		echo ".env" >> $(TEMPLATE_DIR)/node.gitignore; \
	fi
	@echo "✓ Template directory setup complete"
	@echo "  Add your .gitignore templates to $(TEMPLATE_DIR)/"
	@echo "  Then run: make templates"

# Help target
help:
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "  Gitignore Tool - Makefile Targets"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo ""
	@echo "BUILD TARGETS:"
	@echo "  make                   - Build the project"
	@echo "  make templates         - Generate templates.c from templates/"
	@echo "  make regen-templates   - Regenerate templates.c"
	@echo "  make dev               - Build with debug symbols"
	@echo ""
	@echo "INSTALLATION:"
	@echo "  make install           - Install to $(PREFIX)"
	@echo "  make PREFIX=/path install - Install to custom location"
	@echo "  make uninstall         - Remove installation"
	@echo "  make show-prefix       - Show current PREFIX"
	@echo ""
	@echo "CLEANING:"
	@echo "  make clean             - Remove build artifacts"
	@echo "  make distclean         - Remove all generated files"
	@echo ""
	@echo "TEMPLATES:"
	@echo "  make setup-templates   - Create initial template directory"
	@echo "  make templates         - Generate templates.c"
	@echo ""
	@echo "OTHER:"
	@echo "  make test              - Run basic tests"
	@echo "  make package           - Create distribution package"
	@echo "  make help              - Show this help"
	@echo ""
	@echo "ENVIRONMENT VARIABLES:"
	@echo "  PREFIX                 - Installation prefix (default: /usr/local)"
	@echo "  CC                     - C compiler (default: gcc)"
	@echo ""
	@echo "EXAMPLES:"
	@echo "  make                                    # Build with default prefix"
	@echo "  make PREFIX=~/.local install            # Install to home directory"
	@echo "  export PREFIX=/opt/gitignore && make install"
	@echo ""
	@echo "ADDING NEW TEMPLATES:"
	@echo "  1. Create templates/yourname.gitignore"
	@echo "  2. Add your patterns to the file"
	@echo "  3. Run: make regen-templates"
	@echo "  4. Run: make"
	@echo ""
	@echo "Current PREFIX: $(PREFIX)"
	@echo ""

.SILENT: help show-prefix