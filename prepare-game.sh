#!/bin/sh
# Build user-supplied Prince of Persia assets for SDLPoP Pocket.
set -eu
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 /path/to/dos-game /path/to/Assets/sdlpop/common" >&2
  exit 2
fi
for cmd in python3 make cc sdl2-config; do
  command -v "$cmd" >/dev/null 2>&1 || { echo "Missing prerequisite: $cmd" >&2; exit 1; }
done
script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd -P)
game_dir=$(CDPATH= cd -- "$1" && pwd -P) || exit 1
mkdir -p "$2"
out_dir=$(CDPATH= cd -- "$2" && pwd -P)
if [ -e "$out_dir/popdata.dat" ] || [ -e "$out_dir/popmusic.dat" ]; then
  echo "Output already exists; refusing to overwrite popdata.dat or popmusic.dat." >&2
  exit 1
fi
work_dir=$(mktemp -d "${TMPDIR:-/tmp}/sdlpop-assets.XXXXXXXX")
trap 'rm -rf "$work_dir"' EXIT HUP INT TERM
python3 "$script_dir/src/sdlpop/tools/make_assets.py" "$game_dir" "$work_dir/popdata.dat"
echo "Building the desktop music renderer..."
make -C "$script_dir/src/sdlpop" USE_SDK_CONTAINER=0 app_pc
renderer="$script_dir/src/sdlpop/app_pc"
echo "Rendering your game's AdLib music; this can take a few minutes..."
(cd "$work_dir" && SDL_AUDIODRIVER=dummy SDLPOP_RENDER_MUSIC="$work_dir/popmusic.dat" "$renderer")
[ -s "$work_dir/popmusic.dat" ] || { echo "Music generation failed." >&2; exit 1; }
cp "$work_dir/popdata.dat" "$out_dir/popdata.dat"
cp "$work_dir/popmusic.dat" "$out_dir/popmusic.dat"
echo "Prepared game data in $out_dir"
