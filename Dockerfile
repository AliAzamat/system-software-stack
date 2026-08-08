# A pinned, reproducible cross-compile toolchain for the ARM DPU. Pinning the
# base image and tool versions means every build — laptop or CI — is identical.
FROM debian:12-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        gcc-aarch64-linux-gnu \
        make \
        device-tree-compiler \
        openssl \
        python3 \
        pyflakes3 \
    && rm -rf /var/lib/apt/lists/*

# Match the Makefiles' cross settings so the container builds ARM out of the box.
ENV CROSS_COMPILE=aarch64-linux-gnu-
ENV ARCH=arm64
WORKDIR /src
