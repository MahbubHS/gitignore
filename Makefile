# Makefile for gitignore tool v2.0

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -I.
LDFLAGS = -lcurl
PREFIX = /data/data/org.smartide.code/files/usr
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man/man1

TARGET = gitignore
SRCDIR = src
SOURCES = main.c help.c init.c sync.c utils.c features.c global_backup.c cache_config.c
OBJECTS = $(SOURCES:%.c=$(SRCDIR)/%.o)
HEADERS = gitignore.h

# Template files to embed
TEMPLATE_DIR = templates
TEMPLATES = $(wildcard $(TEMPLATE_DIR)/*.gitignore)

.PHONY: all clean install uninstall templates dirs test package

all: dirs $(TARGET)

dirs:
	@mkdir -p $(SRCDIR) $(TEMPLATE_DIR) man

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo ""
	@echo "✓ Build complete: $(TARGET)"
	@echo "  Install with: sudo make install"

$(SRCDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Create template directory with built-in templates
templates:
	@echo "Creating built-in templates..."
	@mkdir -p $(TEMPLATE_DIR)
	
	@echo "# Python" > $(TEMPLATE_DIR)/python.gitignore
	@echo "__pycache__/" >> $(TEMPLATE_DIR)/python.gitignore
	@echo "*.py[cod]" >> $(TEMPLATE_DIR)/python.gitignore
	@echo "*\$$py.class" >> $(TEMPLATE_DIR)/python.gitignore
	@echo "*.so" >> $(TEMPLATE_DIR)/python.gitignore
	@echo ".Python" >> $(TEMPLATE_DIR)/python.gitignore
	@echo "venv/" >> $(TEMPLATE_DIR)/python.gitignore
	@echo "env/" >> $(TEMPLATE_DIR)/python.gitignore
	@echo ".env" >> $(TEMPLATE_DIR)/python.gitignore
	@echo "*.egg-info/" >> $(TEMPLATE_DIR)/python.gitignore
	@echo "dist/" >> $(TEMPLATE_DIR)/python.gitignore
	@echo "build/" >> $(TEMPLATE_DIR)/python.gitignore
	
	@echo "# Node.js" > $(TEMPLATE_DIR)/node.gitignore
	@echo "node_modules/" >> $(TEMPLATE_DIR)/node.gitignore
	@echo "npm-debug.log*" >> $(TEMPLATE_DIR)/node.gitignore
	@echo "yarn-debug.log*" >> $(TEMPLATE_DIR)/node.gitignore
	@echo "yarn-error.log*" >> $(TEMPLATE_DIR)/node.gitignore
	@echo ".npm" >> $(TEMPLATE_DIR)/node.gitignore
	@echo "dist/" >> $(TEMPLATE_DIR)/node.gitignore
	@echo "build/" >> $(TEMPLATE_DIR)/node.gitignore
	@echo ".next/" >> $(TEMPLATE_DIR)/node.gitignore
	@echo ".nuxt/" >> $(TEMPLATE_DIR)/node.gitignore
	@echo "package-lock.json" >> $(TEMPLATE_DIR)/node.gitignore
	
	@echo "# Rust" > $(TEMPLATE_DIR)/rust.gitignore
	@echo "target/" >> $(TEMPLATE_DIR)/rust.gitignore
	@echo "Cargo.lock" >> $(TEMPLATE_DIR)/rust.gitignore
	@echo "**/*.rs.bk" >> $(TEMPLATE_DIR)/rust.gitignore
	@echo "*.pdb" >> $(TEMPLATE_DIR)/rust.gitignore
	
	@echo "# C" > $(TEMPLATE_DIR)/c.gitignore
	@echo "*.o" >> $(TEMPLATE_DIR)/c.gitignore
	@echo "*.a" >> $(TEMPLATE_DIR)/c.gitignore
	@echo "*.so" >> $(TEMPLATE_DIR)/c.gitignore
	@echo "*.out" >> $(TEMPLATE_DIR)/c.gitignore
	@echo "*.exe" >> $(TEMPLATE_DIR)/c.gitignore
	@echo "*.dylib" >> $(TEMPLATE_DIR)/c.gitignore
	
	@echo "# C++" > $(TEMPLATE_DIR)/cpp.gitignore
	@echo "*.o" >> $(TEMPLATE_DIR)/cpp.gitignore
	@echo "*.obj" >> $(TEMPLATE_DIR)/cpp.gitignore
	@echo "*.exe" >> $(TEMPLATE_DIR)/cpp.gitignore
	@echo "*.out" >> $(TEMPLATE_DIR)/cpp.gitignore
	@echo "*.a" >> $(TEMPLATE_DIR)/cpp.gitignore
	@echo "*.so" >> $(TEMPLATE_DIR)/cpp.gitignore
	@echo "*.dylib" >> $(TEMPLATE_DIR)/cpp.gitignore
	
	@echo "# Java" > $(TEMPLATE_DIR)/java.gitignore
	@echo "*.class" >> $(TEMPLATE_DIR)/java.gitignore
	@echo "*.jar" >> $(TEMPLATE_DIR)/java.gitignore
	@echo "*.war" >> $(TEMPLATE_DIR)/java.gitignore
	@echo "*.ear" >> $(TEMPLATE_DIR)/java.gitignore
	@echo "target/" >> $(TEMPLATE_DIR)/java.gitignore
	@echo ".gradle/" >> $(TEMPLATE_DIR)/java.gitignore
	@echo "build/" >> $(TEMPLATE_DIR)/java.gitignore
	
	@echo "# Go" > $(TEMPLATE_DIR)/go.gitignore
	@echo "*.exe" >> $(TEMPLATE_DIR)/go.gitignore
	@echo "*.test" >> $(TEMPLATE_DIR)/go.gitignore
	@echo "*.out" >> $(TEMPLATE_DIR)/go.gitignore
	@echo "vendor/" >> $(TEMPLATE_DIR)/go.gitignore
	
	@echo "# VS Code" > $(TEMPLATE_DIR)/vscode.gitignore
	@echo ".vscode/" >> $(TEMPLATE_DIR)/vscode.gitignore
	@echo "*.code-workspace" >> $(TEMPLATE_DIR)/vscode.gitignore
	
	@echo "# macOS" > $(TEMPLATE_DIR)/macos.gitignore
	@echo ".DS_Store" >> $(TEMPLATE_DIR)/macos.gitignore
	@echo ".AppleDouble" >> $(TEMPLATE_DIR)/macos.gitignore
	@echo ".LSOverride" >> $(TEMPLATE_DIR)/macos.gitignore
	
	@echo "# Linux" > $(TEMPLATE_DIR)/linux.gitignore
	@echo "*~" >> $(TEMPLATE_DIR)/linux.gitignore
	@echo ".directory" >> $(TEMPLATE_DIR)/linux.gitignore
	
	@echo "# Windows" > $(TEMPLATE_DIR)/windows.gitignore
	@echo "Thumbs.db" >> $(TEMPLATE_DIR)/windows.gitignore
	@echo "ehthumbs.db" >> $(TEMPLATE_DIR)/windows.gitignore
	@echo "Desktop.ini" >> $(TEMPLATE_DIR)/windows.gitignore
	
	@echo "✓ Created $(shell ls -1 $(TEMPLATE_DIR)/*.gitignore | wc -l) templates"

install: $(TARGET)
	@echo "Installing gitignore..."
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)
	@if [ -f man/gitignore.1 ]; then \
		install -d $(MANDIR); \
		install -m 644 man/gitignore.1 $(MANDIR); \
		echo "  ✓ Manual page installed"; \
	fi
	@echo "  ✓ Binary installed to $(BINDIR)/$(TARGET)"
	@echo ""
	@echo "Run 'gitignore --help' to get started!"

uninstall:
	rm -f $(BINDIR)/$(TARGET)
	rm -f $(MANDIR)/gitignore.1
	@echo "✓ gitignore uninstalled"

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf $(TEMPLATE_DIR)
	@echo "✓ Clean complete"

# Test targets
test: $(TARGET)
	@echo "Running tests..."
	@./$(TARGET) --version
	@./$(TARGET) list
	@echo "✓ Basic tests passed"

# Development targets
dev: CFLAGS += -g -DDEBUG
dev: clean all

# Package for distribution
package: all
	@echo "Creating distribution package..."
	@mkdir -p dist/gitignore-$(VERSION)
	@cp $(TARGET) dist/gitignore-$(VERSION)/
	@cp README.md LICENSE dist/gitignore-$(VERSION)/
	@cp -r $(TEMPLATE_DIR) dist/gitignore-$(VERSION)/
	@cd dist && tar -czf gitignore-$(VERSION).tar.gz gitignore-$(VERSION)
	@echo "✓ Package created: dist/gitignore-$(VERSION).tar.gz"

# Help target
help:
	@echo "Gitignore Tool - Makefile targets:"
	@echo ""
	@echo "  make              - Build the project"
	@echo "  make templates    - Create built-in templates"
	@echo "  make install      - Install to system (requires sudo)"
	@echo "  make uninstall    - Remove from system"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make test         - Run basic tests"
	@echo "  make dev          - Build with debug symbols"
	@echo "  make package      - Create distribution package"
	@echo "  make help         - Show this help"

VERSION = 2.0.0

.SILENT: templates help