# Game of Life

<p align="center">
    <img src=".github/assets/demo.gif" width="1300" alt="Demo">
</p>

## Requirements

- C++17 compatible compiler
- `make` or `cmake` for building
- POSIX-compliant system (e.g., Linux, macOS)
- A terminal that support ANSI escape codes

## Build

### Building with Make

1. Clone the repository
    ```bash
    git clone https://github.com/Frost-Phoenix/game-of-life.git
    cd game-of-life
    ```
2. Build the project
    ```bash
    make release
    ```
3. Run the executable
    ```bash
    ./bin/game-of-life
    ```

### Building with CMake

1. Clone the repository
    ```bash
    git clone https://github.com/Frost-Phoenix/game-of-life.git
    cd game-of-life
    ```
2. Create a build directory
    ```bash
    mkdir build && cd build
    ```
3. Run CMake and build
    ```bash
    cmake ..
    make
    ```
4. Run the executable
    ```bash
    ./bin/game-of-life
    ```
    
## Usage

```
$ game-of-life --help

Game of Life - A terminal-based Conway's Game of Life simulation

USAGE: game-of-life [OPTIONS]

OPTIONS:
    -f, --fps <FPS>                  Set the speed of the simulation [default: 10]
    -r, --randomness <RANDOMNESS>    Set the percentage of randomness [default: 25%]
    -c, --color <COLOR_ID>           Set the cells color [default: 7 white]
    -h, --help                       Show this help message and exit

KEYBINDS:
    [q]            Quit

  Default:
    [s]            Step the simulation
    [p]            Toggle pause status
    [i]            Enter Insert mode
    [(shift) =]    +1 (+5) FPS
    [(shift) -]    -1 (-5) FPS

  Insert:
    [c]            Clear the grid
    [t]            Toggle cell under the cursor
    [r]            Generate random grid
    [h,j,k,l]      Move the cursor
    [Arrows]       Move the cursor
    [Escape]       Quit insert mode

Colors:
    0: Black        2: Green        4: Blue         6: Cyan
    1: Red          3: Yellow       5: Magenta      7: White

Note: Grid size is determine by the terminal size
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.