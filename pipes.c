#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#define NUM_PIPES 10

typedef enum { ALIVE, DEAD } PipeStatus;

typedef struct {
    int x, y;
    int direction;
    int color;
    PipeStatus status;
} Pipe;

void init_pipe(Pipe *pipe, int rows, int cols) {
    int edge = rand() % 4;
    switch (edge) {
        case 0: pipe->x = rand() % cols; pipe->y = 0; pipe->direction = 2; break; // Top edge -> down
        case 1: pipe->x = cols - 1; pipe->y = rand() % rows; pipe->direction = 3; break; // Right edge -> left
        case 2: pipe->x = rand() % cols; pipe->y = rows - 1; pipe->direction = 0; break; // Bottom edge -> up
        case 3: pipe->x = 0; pipe->y = rand() % rows; pipe->direction = 1; break; // Left edge -> right
    }
    pipe->color = (rand() % 15) + 1;
    pipe->status = ALIVE;
}

const char *get_pipe_char(int dir_arrival, int dir_departure) {
    if (dir_arrival == dir_departure) {
        return (dir_arrival == 0 || dir_arrival == 2) ? "|" : "-";
    }
    return "+";
}

int main(int argc, char *argv[]) {
    int speed = 50;
    int bend_chance = 50;
    int opt;

    while ((opt = getopt(argc, argv, "s:b:")) != -1) {
        switch (opt) {
            case 's': speed = atoi(optarg); break;
            case 'b': bend_chance = atoi(optarg); break;
            default:
                fprintf(stderr, "Usage: %s [-s speed] [-b bend_chance]\n", argv[0]);
                return 1;
        }
    }

    long delay = (speed > 0) ? (1000000 / speed) : 1000000;

    setlocale(LC_ALL, "");
    initscr();
    curs_set(0);
    noecho();
    start_color();
    nodelay(stdscr, TRUE);

    for (int i = 1; i <= 15; i++) {
        init_pair(i, i, COLOR_BLACK);
    }

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    char **screen_grid = malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        screen_grid[i] = malloc(cols * sizeof(char));
        memset(screen_grid[i], 0, cols * sizeof(char));
    }
    long filled_cells = 0;
    long total_cells = (long)rows * cols;

    srand(time(NULL));

    Pipe pipes[NUM_PIPES];
    for (int i = 0; i < NUM_PIPES; i++) {
        init_pipe(&pipes[i], rows, cols);
    }

    while (getch() == ERR) {
        for (int i = 0; i < NUM_PIPES; i++) {
            if (pipes[i].status == ALIVE) {
                int next_direction = pipes[i].direction;
                if (rand() % bend_chance == 0) {
                    int random_direction = rand() % 4;
                    if (random_direction != (pipes[i].direction + 2) % 4) { // Prevent 180-degree turns
                        next_direction = random_direction;
                    }
                }

                if (pipes[i].y >= 0 && pipes[i].y < rows && pipes[i].x >= 0 && pipes[i].x < cols) {
                    if (screen_grid[pipes[i].y][pipes[i].x] == 0) {
                        screen_grid[pipes[i].y][pipes[i].x] = 1;
                        filled_cells++;
                    }
                    attron(COLOR_PAIR(pipes[i].color) | A_BOLD);
                    mvprintw(pipes[i].y, pipes[i].x, "%s", get_pipe_char(pipes[i].direction, next_direction));
                    attroff(COLOR_PAIR(pipes[i].color) | A_BOLD);
                }

                pipes[i].direction = next_direction;

                switch (pipes[i].direction) {
                    case 0: pipes[i].y--; break;
                    case 1: pipes[i].x++; break;
                    case 2: pipes[i].y++; break;
                    case 3: pipes[i].x--; break;
                }

                if (pipes[i].x < 0 || pipes[i].x >= cols || pipes[i].y < 0 || pipes[i].y >= rows) {
                    pipes[i].status = DEAD;
                }
            } else { // DEAD
                if (rand() % 100 == 0) {
                    init_pipe(&pipes[i], rows, cols);
                }
            }
        }

        if (filled_cells >= total_cells) {
            clear();
            for (int i = 0; i < rows; i++) {
                memset(screen_grid[i], 0, cols * sizeof(char));
            }
            filled_cells = 0;
        }

        refresh();
        usleep(delay);
    }

    for (int i = 0; i < rows; i++) {
        free(screen_grid[i]);
    }
    free(screen_grid);

    endwin();
    return 0;
}