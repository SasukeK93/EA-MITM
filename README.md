# EA-MITM
MITM for EA's DirtySock/ProtoSSL powered products, fairly simple, used for dumping data between secure connections.

## Usage
1. Modify EA-MITM.ini with the correct ProtoSSL offsets for your game.
2. Copy proxy DLL (dinput8.dll) to the game's directory (NOTE: This may not work on all games - such as NFS: Rivals, where a more complex method of initialization is required, or DLL injection).
3. Launch the game, all connections using ProtoSSL will be dumped to `protossl_dump.acp` and can be opened with Wireshark.

## License
Copyright (C) 2016 Baggie. All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or any 
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.