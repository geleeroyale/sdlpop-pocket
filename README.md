# SDLPoP Pocket source and user-data preparation

This source includes the SDLPoP engine, user-data tool, and openfpgaOS SDK. **You must own the game for DOS and supply the actual files for PoP.**

## Prepare your game files

Requirements: Python 3 and a compatible DOS Prince of Persia game directory. After installing the binary package onto the SD card, run:

```sh
./prepare-game.sh "/path/to/Prince of Persia DOS files" "/path/to/Pocket/Assets/sdlpop/common"
```

The tool bundles your DAT files into `popdata.dat`. It refuses to overwrite an existing file. The game data remain with their respective owners; do not include the resulting bundle in an engine release.

## Rebuilding

The Pocket project is `src/sdlpop`. The source includes the exact SDK snapshot used for the app and the separately supplied musl source and patch/build script. It was built with a RISC-V `riscv64-unknown-elf` GCC toolchain for rv32imafc/ilp32f; see the SDK make rules and `PROVENANCE.txt`. The prebuilt openfpgaOS runtime and FPGA bitstream are separate upstream components: use the exact core revision and pinned submodules in PROVENANCE when rebuilding them. The desktop target is `make -C src/sdlpop USE_SDK_CONTAINER=0 app_pc`.

License and provenance details are in the included notice and license files and in the source headers. Consult the openfpgaOS core NOTICE and its applicable third-party licenses before public distribution of the bitstream. The game data remain with their respective owners.

## Project status

This port has been tested successfully on Analogue Pocket - as far as we know, it enables full gameplay. It starts directly from openFPGA; movement, sound effects, music and in-game quicksave were tested. This branch plays AdLib music through a DOSBox-derived real-time OPL emulator. The DBOPL configuration without a pre-rendered music file has been tested on one Pocket; broader compatibility and full playthroughs remain unverified. It does not currently implement Pocket sleep or system save states.

The repository deliberately omits game assets and the optional SC-55 sound bank. Review `openfpgaOS-core-NOTICE.txt` and upstream component licenses before distributing compiled runtime artifacts. See `PLAYING.md` and `PROVENANCE.txt` for details and credits.
