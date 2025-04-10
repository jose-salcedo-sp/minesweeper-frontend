#include "minesweeper.h"
#define SIZE 8

void generate_bomb_map(char bomb_map[SIZE][SIZE], char bomb_count) {
  srand(time(NULL));

  int bombs_placed = 0;
  while (bombs_placed < bomb_count) {
    int x = rand() % SIZE;
    int y = rand() % SIZE;
    if (bomb_map[y][x] == 0) {
      bomb_map[y][x] = 1;
      bombs_placed++;
    }
  }
}

void generate_unreveiled_map(char map[SIZE][SIZE]) {
  memset(map, 'u', SIZE * SIZE * sizeof(char));
}

char count_adjacent_bombs(char bomb_map[SIZE][SIZE], char x, char y) {
  int count = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int new_x = x + j;
      int new_y = y + i;
      if (new_x >= 0 && new_x < SIZE && new_y >= 0 && new_y < SIZE &&
          bomb_map[new_y][new_x] == 1) {
        count++;
      }
    }
  }
  return count;
}

void reveal_empty_cells(char bomb_map[SIZE][SIZE], char game_map[SIZE][SIZE],
                        char x, char y, int *revealed_count) {
  if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || game_map[y][x] != 'u') {
    return;
  }

  int adjacent_bombs = count_adjacent_bombs(bomb_map, x, y);
	(*revealed_count)++;

  if (adjacent_bombs == 0) {
    game_map[y][x] = 'e';
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        if (i != 0 || j != 0) {
          reveal_empty_cells(bomb_map, game_map, x + i, y + j, revealed_count);
        }
      }
    }
  } else {
    game_map[y][x] = '0' + adjacent_bombs;
  }
}

BoardStatus process_move(char bomb_map[SIZE][SIZE], char game_map[SIZE][SIZE],
                         BoardMove move, int *revealed_count) {
  int x = move.x;
  int y = move.y;

  if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
    return ONGOING;
  }

  if (move.action == 'r') {
    if (bomb_map[y][x] == 1) {
      return DEFEAT;
    } else {
      reveal_empty_cells(bomb_map, game_map, x, y, revealed_count);
    }
  }

  if (move.action == 'f') {
    game_map[y][x] = 'f';
  }

  int unrevealed_count = 0;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (game_map[j][i] == 'u' && bomb_map[j][i] == 0) {
        unrevealed_count++;
      }
    }
  }
  if (unrevealed_count == 0) {
    return VICTORY;
  }

  return ONGOING;
}

char *map_to_string(char map[SIZE][SIZE]) {
  char *result = (char *)malloc((SIZE * SIZE + 1) * sizeof(char));
  int index = 0;

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      char c = map[i][j];
      char visual =
          (c == 1) ? '*' : (c == 0 ? '.' : c); // convert bomb=*, empty=.
      result[index++] = visual;
    }
    printf("\n");
  }
  result[index] = '\0';
  return result;
}
