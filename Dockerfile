FROM ubuntu:22.04
LABEL authors="arjunvarma"

# Install system dependencies required for building CMake and your project
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    libssl-dev \
    wget \
    curl \
    libncurses5-dev \
    libreadline-dev \
    zlib1g-dev \
    libbz2-dev \
    liblzma-dev \
    xz-utils \
    && rm -rf /var/lib/apt/lists/*

# Download and build CMake 3.31.2
RUN wget https://github.com/Kitware/CMake/releases/download/v3.31.2/cmake-3.31.2.tar.gz && \
    tar -xzf cmake-3.31.2.tar.gz && \
    cd cmake-3.31.2 && \
    ./bootstrap && \
    make -j$(nproc) && \
    make install && \
    cd .. && rm -rf cmake-3.31.2 cmake-3.31.2.tar.gz

# Set working directory
WORKDIR /

# Copy project files
COPY app app
COPY CMakeLists.txt .
COPY main.cpp .

# Build project
RUN cmake -S . -B build && cmake --build build

# Run the binary
CMD ["./build/Quant_Engine"]
