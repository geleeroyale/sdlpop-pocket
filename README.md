# SDLPoP Pocket source and user-data preparation

This source includes the SDLPoP engine (https://github.com/NagyD/SDLPoP), user-data tool, and openfpgaOS SDK (https://github.com/openfpgaOS/openfpgaSDK). **You must own the game for DOS and supply the actual files for PoP.**

## Prepare your game files

Requirements: a compatible DOS Prince of Persia directory; Python 3; a C compiler; `make`; and SDL2 development libraries including `sdl2-config`. On macOS, the compiler's Xcode Command Line Tools license must already be accepted and SDL2 can be installed separately, for example with Homebrew. On Debian/Ubuntu the packages `build-essential python3 libsdl2-dev` supply these prerequisites.

After installing the binary package onto the SD card, run:

```sh
./prepare-game.sh "/path/to/Prince of Persia DOS files" "/path/to/Pocket/Assets/sdlpop/common"
```

The tool bundles DAT files into `popdata.dat`, then compiles a desktop renderer and locally renders the game's own AdLib music into `popmusic.dat`. It refuses to overwrite existing data files. The output is derived from your game files; do not include it in the engine release.

Alternatively, where Docker is installed, from this source directory:

```sh
docker build -f Dockerfile.music -t sdlpop-music .
docker run --rm -v "/absolute/path/to/dos-game:/game:ro" -v "/absolute/path/to/output:/out" sdlpop-music /game /out
```

Copy the two generated `.dat` files into `Assets/sdlpop/common` on the Pocket SD card. The container does not receive your game files until the run step. Docker paths containing colons may require platform-specific handling.

## Rebuilding

The Pocket project is `src/sdlpop`. The source includes the exact SDK snapshot used for the app and the separately supplied musl source and patch/build script. It was built with a RISC-V `riscv64-unknown-elf` GCC toolchain for rv32imafc/ilp32f; see the SDK make rules and `PROVENANCE.txt`. The prebuilt openfpgaOS runtime and FPGA bitstream are separate upstream components: use the exact core revision and pinned submodules in PROVENANCE when rebuilding them. The desktop target is `make -C src/sdlpop USE_SDK_CONTAINER=0 app_pc`.

License and provenance details are in the included notice and license files and in the source headers. Consult the openfpgaOS core NOTICE and its applicable third-party licenses before public distribution of the bitstream. The game data remain with their respective owners.

## Project status

This port has been tested on an Analogue Pocket with a compatible DOS copy supplied by the player. It starts directly from openFPGA; movement, sound effects, music and in-game quicksave were tested. Music is generated locally from the player's game files and played from `popmusic.dat`. It does not currently implement Pocket sleep or system save states. Other game variants and full playthroughs remain unverified.

The repository deliberately omits game assets and the optional SC-55 sound bank. Review `openfpgaOS-core-NOTICE.txt` and upstream component licenses before distributing compiled runtime artifacts. See `PLAYING.md` and `PROVENANCE.txt` for details and credits.
