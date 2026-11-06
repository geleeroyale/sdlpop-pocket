#!/bin/sh
# Bundle the player's own Prince of Persia data for SDLPoP Pocket.
set -eu
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 /path/to/dos-game /path/to/Assets/sdlpop/common" >&2
  exit 2
fi
command -v python3 >/dev/null 2>&1 || { echo "Missing prerequisite: python3" >&2; exit 1; }
script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd -P)
game_dir=$(CDPATH= cd -- "$1" && pwd -P)
mkdir -p "$2"
out_dir=$(CDPATH= cd -- "$2" && pwd -P)
if [ -e "$out_dir/popdata.dat" ]; then
  echo "Output already exists; refusing to overwrite popdata.dat." >&2
  exit 1
fi
work_dir=$(mktemp -d "${TMPDIR:-/tmp}/sdlpop-assets.XXXXXXXX")
trap 'rm -rf "$work_dir"' EXIT HUP INT TERM
python3 "$script_dir/src/sdlpop/tools/make_assets.py" "$game_dir" "$work_dir/popdata.dat"
cp "$work_dir/popdata.dat" "$out_dir/popdata.dat"
echo "Prepared game data in $out_dir"
