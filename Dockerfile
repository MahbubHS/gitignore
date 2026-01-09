# Multi-stage build for gitignore tool
FROM gcc:latest AS builder

# Install dependencies
RUN apt-get update && apt-get install -y \
    libcurl4-openssl-dev \
    make \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /build

# Copy source files
COPY . .

# Build
RUN make templates && make

# Production stage
FROM debian:bookworm-slim

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libcurl4 \
    ca-certificates \
    git \
    && rm -rf /var/lib/apt/lists/*

# Copy binary from builder
COPY --from=builder /build/gitignore /usr/local/bin/gitignore
COPY --from=builder /build/templates /usr/local/share/gitignore/templates

# Create config directory
RUN mkdir -p /root/.config/gitignore/{templates,cache,backups}

# Set working directory
WORKDIR /workspace

# Set entrypoint
ENTRYPOINT ["/usr/local/bin/gitignore"]

# Default command
CMD ["--help"]

# Labels
LABEL org.opencontainers.image.title="gitignore"
LABEL org.opencontainers.image.description="Professional .gitignore file generator"
LABEL org.opencontainers.image.version="2.0.0"
LABEL org.opencontainers.image.authors="gitignore contributors"
LABEL org.opencontainers.image.url="https://github.com/mahbubhs/gitignore"
LABEL org.opencontainers.image.source="https://github.com/mahbubhs/gitignore"
LABEL org.opencontainers.image.licenses="MIT"