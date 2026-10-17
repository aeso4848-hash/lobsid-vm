Lobsid-mini has 1024 commands (because of lesser commands, to make the same std program in mini, you'll need more commands) with 2 byte opcode + 2 byte nouns.
Lobsid-std has 512 commands with 3 byte opcode + 3 byte nouns.
Lobsid-ext has 8192 commands with 4 byte opcode + 4 byte nouns.

Because of noun lenghts,
mini has 256 bytes of virtual ram, (00-ff)
std has 4KiB of ram, (000-fff)
ext has 64KiB of ram. (0000-ffff)
