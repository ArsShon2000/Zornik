FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    libboost-system-dev \
    libboost-thread-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    libboost-system1.74.0 \
    libboost-thread1.74.0 \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/CardRestApi /app/CardRestApi
COPY TARO_files ./TARO_files
COPY LENORMAND_files ./LENORMAND_files


EXPOSE 8080

CMD ["./CardRestApi"]