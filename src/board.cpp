#include "board.h"

Board2048::Board2048() {
    state = new_2D_array();
    reward = 0;
}

Board2048::Board2048(Board2048 const& other) {
    state = new_2D_array();
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            state[x][y] = other.state[x][y];
        }
    }

    reward = other.reward;
}

int** Board2048::new_2D_array() const {
	int** arr = new int*[4];
	for (int i = 0; i < 4; i++) {
		arr[i] = new int[4];
        for (int j = 0; j < 4; ++j) {
            arr[i][j] = 0;
        }
	}
	return arr;
}

void Board2048::free_state(int **board) const {
   for (int i = 0; i < 4; ++i) {
        delete [] board[i];
    }
    delete [] board;
}

int** Board2048::rotate_state(int r) const {
	r %= 4;
	int** rotated = new_2D_array();
	int temp[4][4];

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			temp[x][y] = state[x][y];
		}
	}

	bool b = true;
	for (int i = 0; i < r; i++) {
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				if (b) {			
					rotated[3 - y][x] = temp[x][y];
				} else {
					temp[3 - y][x] = rotated[x][y];
				}
			}
		}
		b = !b;
	}

	if (b) {
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				rotated[x][y] = temp[x][y];
			}
		}
	}

	return rotated;
}

void Board2048::add_random_tile() {
	int x, y;
	do {
		x = rand() % 4;
		y = rand() % 4;
	} while (state[x][y] != 0);

	if (rand() % 10 == 0) {
		state[x][y] = 2;
	} else {
		state[x][y] = 1;
	}
}

void Board2048::make_move(int action) {
    int** b = rotate_state(action);
	reward = 0;

	for (int y = 0; y < 4; y++) {
		int i = 0;
		for (int x = 0; x < 4; x++) {
			if (b[x][y] != 0) {
				if (i > 0) {
					if (b[x][y] == b[i - 1][y]) {
						b[i - 1][y] += 1;
						b[x][y] = 0;
						reward += pow(2, b[i - 1][y]);
					} else {
						int temp = b[x][y];
						b[x][y] = 0;
						b[i][y] = temp;
						i++;
					}
				} else {
					int temp = b[x][y];
					b[x][y] = 0;
					b[i][y] = temp;
					i++;
				}
			}
		}
	}

    free_state(state);
    state = b;
	b = rotate_state(4 - action);
	free_state(state);
    state = b;
}

bool Board2048::is_move_possible(int action) const {
	int** b = rotate_state(action);
	bool ans = false;

	for (int y = 0; y < 4; y++) {
		bool wasZero = false;
		for (int x = 0; x < 3; x++) {
			if (b[x][y] != 0) {
				if (wasZero || b[x][y] == b[x+1][y]) {
					ans = true;
					y = 3;
					x = 2;
				}
			} else {
				wasZero = true;
			}
		}
		if (wasZero && b[3][y] != 0) {
			ans = true;
			y = 3;
		}
	}

	free_state(b);
	return ans;
}

bool Board2048::is_game_over() const {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 3; y++) {
			if (state[x][y] == 0 || state[x][y] == state[x][y+1]) {
				return false;
			}
		}
		if (state[x][3] == 0) {
			return false;
		}
	}
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 3; x++) {
			if (state[x][y] == state[x+1][y]) {
				return false;
			}
		}
	}
	return true;
}

Board2048::~Board2048() {
    free_state(state);
 }
