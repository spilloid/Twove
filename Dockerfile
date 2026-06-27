# syntax=docker/dockerfile:1
#
# Build every game to WebAssembly, then serve the bundles with nginx.
#
#   docker build -t vge-web .
#   docker run --rm -p 8080:80 vge-web      # → http://localhost:8080
#
# This doubles as a build test: stage 1 fails the build if the WASM target
# doesn't compile.

# ── Stage 1: compile pong / tictactoe / quoridor to WASM ─────────────────────
# Pin a specific emsdk tag for reproducible builds.
FROM emscripten/emsdk:3.1.74 AS build
WORKDIR /src
COPY . .
RUN emcmake cmake -B build-web -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build-web -j"$(nproc)"

# ── Stage 2: dead-simple static server for the .wasm bundles ─────────────────
FROM nginx:1.27-alpine
COPY docker/nginx.conf /etc/nginx/conf.d/default.conf
COPY docker/index.html /usr/share/nginx/html/index.html
# Each game is a self-contained set: <game>.{html,js,wasm,data}
COPY --from=build /src/build-web/*.html /usr/share/nginx/html/
COPY --from=build /src/build-web/*.js   /usr/share/nginx/html/
COPY --from=build /src/build-web/*.wasm /usr/share/nginx/html/
COPY --from=build /src/build-web/*.data /usr/share/nginx/html/
EXPOSE 80
