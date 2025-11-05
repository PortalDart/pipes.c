# Terminal Pipes
<img width="1876" height="893" alt="Screenshot 2025-11-05 at 20 22 55" src="https://github.com/user-attachments/assets/c2127688-c4ba-4094-ac98-ad5c14839cc1" />

A classic "pipes" screensaver animation running in your terminal, written in C using the `ncurses` library.

## Description

This program creates a colorful, animated display of pipes growing and turning randomly across the terminal screen. It's a fun little program to watch and a good example of using the `ncurses` library for terminal graphics.

## Compilation

To compile the program, you need a C compiler (like `gcc`) and the `ncurses` library installed.

On most Linux distributions, you can install `ncurses` with:
```bash
sudo apt-get install libncurses-dev
```
or
```bash
sudo dnf install ncurses-devel
```

On macOS, `ncurses` is usually included with the command-line tools.

Once you have the dependencies, compile the program using:
```bash
gcc pipes.c -o pipes -lncurses
```

## Usage

Run the compiled program with:
```bash
./pipes
```

The animation will run until you press any key.

### Options

You can customize the animation with the following command-line flags:

*   `-s <speed>`: Sets the animation speed. Higher numbers are faster. Default is `50`.
    ```bash
    ./pipes -s 100
    ```
*   `-b <bend_chance>`: Sets the chance for a pipe to bend. A lower number means more frequent bends. Default is `50`.
    ```bash
    ./pipes -b 10
    ```

You can combine the flags:
```bash
./pipes -s 200 -b 5
```
