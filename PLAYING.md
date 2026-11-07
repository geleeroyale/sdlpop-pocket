# SDLPoP for Analogue Pocket — 0.1.0 release candidate

An openfpgaOS port of SDLPoP, the DOS-based Prince of Persia engine. It needs a player's own compatible DOS game files; game files are not included. The engine package also excludes the optional SC-55 sample bank.

## Install

1. Merge the `Assets`, `Cores`, and `Platforms` folders into the root of the Pocket SD card, without replacing unrelated directories.
2. Download the accompanying source/tools archive. Follow its README to run `prepare-game.sh` using your own DOS game's directory and the SD card's `Assets/sdlpop/common` directory. It creates `popdata.dat`, which must be present.
3. Safely eject the SD card. On the Pocket, select openFPGA → Computer → Prince of Persia. Press a button to pass the SDLPoP information screen.

## Controls and saves

- D-pad: move, climb, duck and jump.
- B or X: action (walk carefully, grab, and fight); hold it with a direction as needed.
- Start or Select: game menu. Use **Quicksave** and **Quickload** there.

The game also has its original in-game save. The Pocket's system save states are not supported by the runtime. Do not rely on a save until you have checked it on your device.

The internal core identifier remains `SDLPoP.experimental` for compatibility with saves from the prototype. The displayed title is Prince of Persia.

## Status and credits

Gameplay, text, effects and music were tested on one Analogue Pocket with one DOS data set. The game's AdLib music is synthesized in real time using a DOSBox-derived OPL emulator. The setup without a pre-rendered music file has been tested on one Pocket. Other game variants, full-length playthroughs, dock behavior, and all save paths have not been comprehensively tested. Optional PNG replacement assets, screenshots, and controller rumble are not supported.

SDLPoP is by Dávid Nagy and contributors; it uses a DOSBox-derived DBOPL emulator on Pocket and Nuked OPL3 by Alexey Khokholov on desktop, along with other credited libraries. The hardware runtime and SDK are from openfpgaOS by ThinkElastic and contributors. See LICENSES and the accompanying source archive. This is an unofficial community port, not an Ubisoft or Analogue product.

Before public redistribution, review `LICENSES/openfpgaOS-core-NOTICE.txt` and the other notices. The runtime includes Analogue Pocket Framework, Intel FPGA IP, and third-party components under their own terms. The source archive identifies the exact upstream runtime revision and its pinned dependencies.
