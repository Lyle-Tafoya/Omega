#!/usr/bin/env bash
# Build the SDL2/gamepad version and bundle it for Steam Deck & other Linux handhelds.
#   tools/package_handheld.sh [path/to/DawnLikeAtlas/renamed]
# Produces dist/omega-handheld.tar.gz containing:
#   omega, assets/, lib/, omega.sh, README-HANDHELD.md, CREDITS.md, COPYING
set -euo pipefail
cd "$(dirname "$0")/.."

if [[ $# -ge 1 ]]; then
  python3 tools/build_atlas.py "$1" assets
fi

cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release -DOMEGA_SDL=ON
cmake --build build-release -j"$(nproc)"

out=dist/omega-handheld
rm -rf "$out"
mkdir -p "$out"
cp build-release/omega "$out/"
cp -r lib "$out/lib"
mkdir -p "$out/assets"
cp assets/tiles.rgba assets/tiles.txt assets/font.rgba assets/font.txt "$out/assets/"
cp docs/README-HANDHELD.md docs/CREDITS.md COPYING "$out/"
cat > "$out/omega.sh" <<'EOF'
#!/bin/sh
# Launcher: add this to Steam as a non-Steam game.
cd "$(dirname "$(readlink -f "$0")")"
export OMEGA_FULLSCREEN=1
exec ./omega "$@"
EOF
chmod +x "$out/omega.sh" "$out/omega"
tar -C dist -czf dist/omega-handheld.tar.gz omega-handheld
echo "wrote dist/omega-handheld.tar.gz"
