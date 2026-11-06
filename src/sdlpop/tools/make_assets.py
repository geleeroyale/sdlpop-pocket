#!/usr/bin/env python3
"""Pack the user's existing DOS DAT files for the Pocket port."""
import argparse
import pathlib
import struct

MAGIC = b'POPDAT01'
REQUIRED = {'PRINCE.DAT', 'TITLE.DAT', 'KID.DAT', 'LEVELS.DAT', 'VPALACE.DAT', 'VDUNGEON.DAT', 'PV.DAT', 'GUARD.DAT', 'GUARD1.DAT', 'GUARD2.DAT', 'FAT.DAT', 'SKEL.DAT', 'VIZIER.DAT', 'SHADOW.DAT', 'IBM_SND1.DAT', 'IBM_SND2.DAT'}
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('game_folder', type=pathlib.Path)
p.add_argument('output', type=pathlib.Path)
a = p.parse_args()
files = {}
for item in a.game_folder.iterdir():
    if item.is_file() and item.suffix.lower() == '.dat':
        name = item.name.upper()
        if len(name.encode('ascii')) > 15 or name in files:
            raise SystemExit(f'Unsupported or duplicate name: {item.name}')
        files[name] = item.read_bytes()
missing = REQUIRED - files.keys()
if missing:
    raise SystemExit('Missing game archives: ' + ', '.join(sorted(missing)))
entries = sorted(files.items())
position = 12 + 24 * len(entries)
header = bytearray(MAGIC + struct.pack('<I', len(entries)))
body = bytearray()
for name, data in entries:
    header += struct.pack('<16sII', name.encode('ascii'), position, len(data))
    body += data
    position += len(data)
a.output.write_bytes(header + body)
print(f'Packed {len(entries)} DAT files into {a.output} ({position} bytes)')
