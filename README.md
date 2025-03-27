# SuperTux GLUA Game Client

This is a modified version of the SuperTux game that implements functionality to make it possible to play in a tournament. This fork implements the following features:

- Player authentication: players enter an access code to be able to play in the tournament and appear in the leaderboard.
- Remove distractions: the game is simplified to only show menus relevant to the tournament.
- Level controls: players cannot replay levels.
- Statistics: the game keeps track of the player's progress and shows it in the leaderboard.

The objective of this fork is to provide all these features while changing the original game as little as possible. This way, the game can be easily updated to newer versions of SuperTux.

## Installation && Documentation

Important documentation and instructions for SuperTux are contained in the [original repository](https://github.com/SuperTux/supertux).

## Development

> For more complete information on how to build this project on your system, follow the [original repository](https://github.com/SuperTux/supertux/blob/master/INSTALL.md).

### Prerequisites

> Future maintainers, please help us keep this list up-to-date as the project evolves.

#### Arch Linux

```bash
sudo pacman -S cmake base-devel libogg libvorbis openal sdl2 sdl2_image freetype2 libraqm curl openssl glew harfbuzz fribidi glm zlib
```

#### EMSCRIPTEN

```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

### Building for Linux

To build the game for Linux, you need to follow these steps after cloning and entering the repository:

```bash
git submodule update --init --recursive

mkdir build && cd build

# The flag -O1 reduces recompliation time
cmake .. -DWARNINGS=ON -DCMAKE_CXX_FLAGS="-O1"

# This will compile the gam using all available cores
make -j $(nproc || sysctl -n hw.ncpu || echo 2)

# Then run the game
./supertux2
```

### Building for the Web

To build the game for the web, you need to follow these steps after cloning and entering the repository. just make sure you have the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) installed, added to your PATH, and activated.

```bash
git submodule update --init --recursive
mkdir build && cd build

emsdk activate latest
emcmake cmake -DCMAKE_BUILD_TYPE=Release ..
rsync -aP ../data/ data/
emmake make -j$(nproc || sysctl -n hw.ncpu || echo 2)
cp template.html supertux2.html
emrun supertux2.html
```

SuperTux will be available at [http://localhost:6931/supertux2.html](http://localhost:6931/supertux2.html).

## Community

In case you need help, feel free to reach out using the following means:

- **Discord:** You can join our [Discord server](https://discord.com/invite/AcvtHWz) to get in touch with us.

- **E-Mail:** Contact us using [glua@ua.pt](mailto:glua@ua.pt)
