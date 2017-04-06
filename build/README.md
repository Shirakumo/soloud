## Building It

1. Install [GENie](https://github.com/bkaradzic/genie). On Arch it's in the AUR.
2. `genie --os=linux --platform=x64 gmake`
3. `cd gmake && make config=release64`
4. Libs will be in `lib/`, binaries in `bin/`.

Obviously substitute os, platform, and config if necessary
