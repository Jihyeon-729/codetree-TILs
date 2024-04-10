#include <iostream>
#include <algorithm>
#include <string.h>

using namespace std;

typedef struct {
	int y, x;
} Location;

typedef struct {
	int y, x;
	int dist;
	bool out;
} Runner;

typedef struct {
	int y, x;
	int size;
} Square;

int n, m, k;
Location ext;

int board_status[11][11];
int board_runner[11][11];
int board_copy[11][11];
Runner runner[11];

int dy[4] = { -1, 1, 0, 0 };
int dx[4] = { 0, 0, -1, 1 };

void input(void);
void solve(void);
void answer(void);

void move(int idx);
int getd(int fy, int fx, int ty, int tx);

void rotate(Square sq);
Square find_square(void);
bool found(int r, int c, int size);

bool all_out(void);

void print_boardstatus(void) {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << board_status[i][j] << " ";
		} cout << "\n";
	} cout << "\n";
}

void print_boardrunner(void) {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << board_runner[i][j] << " ";
		} cout << "\n";
	} cout << "\n";
}

int main()
{
	input();
	solve();
	answer();

	return 0;
}

void input(void) {
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> board_status[i][j];
			board_runner[i][j] = 0;
		}
	}
	for (int i = 1; i <= m; i++) {
		Runner r;
		cin >> r.y >> r.x;
		r.dist = 0;
		r.out = false;
		runner[i] = r;
		board_runner[r.y][r.x] = 1;

	}
	cin >> ext.y >> ext.x;
	board_status[ext.y][ext.x] = 10;
}

void solve(void) {
	while (k--) {
		//cout << "------------Round #" << k << "------------\n";
		for (int i = 1; i <= m; i++) {
			if(!runner[i].out) move(i);
		}
		//print_boardstatus();
		//print_boardrunner();

		Square sq = find_square();
		//cout << "Sqaure at (" << sq.y << "," << sq.x << "), size: " << sq.size << "\n";
		rotate(sq);

		if (all_out()) break;
		
		//print_boardstatus();
		//print_boardrunner();
	}
}

void answer(void) {
	int sum = 0;
	for (int i = 1; i <= m; i++) {
		sum += runner[i].dist;
	}
	cout << sum << "\n";
	cout << ext.y << " " << ext.x << "\n";
}

void move(int idx) {
	int y = runner[idx].y;
	int x = runner[idx].x;
	int ny, nx, dir = -1;
	int mind = getd(y, x, ext.y, ext.x);

	for (int i = 0; i < 4; i++) {
		ny = y + dy[i];
		nx = x + dx[i];

		if (ny<1 || ny >n || nx<1 || nx>n) continue;
		if (1 <= board_status[ny][nx] && board_status[ny][nx] <= 9) continue;

		int nd = getd(ny, nx, ext.y, ext.x);
		if (nd < mind) {
			mind = nd;
			dir = i;
		}
	}

	if (dir == -1) return; 

	//cout << "Runner #" << idx << " moved (" << y << "," << x << ") -> (";

	board_runner[y][x]--;
	runner[idx].y += dy[dir];
	runner[idx].x += dx[dir];
	runner[idx].dist++;

	y = runner[idx].y;
	x = runner[idx].x;

	//cout << ny << "," << nx << ")\n";

	if (!(y == ext.y && x == ext.x))
		board_runner[y][x]++;
	else {
		runner[idx].y = -1;
		runner[idx].x = -1;
		runner[idx].out = true;
	}

	return;
}

int getd(int fy, int fx, int ty, int tx) {
	return abs(fy - ty) + abs(fx - tx);
}

void rotate(Square sq) {
	// board_status 복사
	memset(board_copy, sizeof(board_copy), 0);
	memcpy(board_copy, board_status, sizeof(board_status));

	for (int r = 0; r < sq.size; r++) {
		for (int c = 0; c < sq.size; c++) {
			if (1 <= board_copy[r + sq.y][c + sq.x] && board_copy[r + sq.y][c + sq.x] <= 9)
				board_status[c + sq.y][sq.size - r - 1 + sq.x] = board_copy[r + sq.y][c + sq.x] - 1;
			else board_status[c + sq.y][sq.size - r - 1 + sq.x] = board_copy[r + sq.y][c + sq.x];
		}
	}

	// board_runner 복사
	memset(board_copy, sizeof(board_copy), 0);
	memcpy(board_copy, board_runner, sizeof(board_runner));

	for (int r = 0; r < sq.size; r++) {
		for (int c = 0; c < sq.size; c++) {
			board_runner[c + sq.y][sq.size - r - 1 + sq.x] = board_copy[r + sq.y][c + sq.x];
		}
	}

	for (int i = 1; i <= m; i++) {
		int y = runner[i].y;
		int x = runner[i].x;
		if (sq.y <= y && y < sq.y + sq.size && sq.x <= x && x < sq.x + sq.size) {
			y -= sq.y;
			x -= sq.x;
			runner[i].y = x + sq.y;
			runner[i].x = sq.size - y - 1 + sq.x;
		}
	}

	// exit 회전
	int ey = ext.y - sq.y;
	int ex = ext.x - sq.x;
	ext.y = ex + sq.y;
	ext.x = sq.size - ey - 1 + sq.x;

	//cout << "Exit now at (" << ext.y << "," << ext.x << ")\n";
	return;
}

Square find_square(void) {
	for (int size = 0; size < n; size++) {
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				if (found(i, j, size + 1)) {
					Square sq;
					sq.y = i; sq.x = j;
					sq.size = size + 1;
					return sq;
				}
			}
		}
	}
}

bool found(int r, int c, int size) {
	bool ext_found=false, man_found=false;
	for (int i = r; i < r + size; i++) {
		for (int j = c; j < c + size; j++) {
			if (board_runner[i][j] > 0) man_found = true;
			if (board_status[i][j] == 10) ext_found = true;
			if (man_found && ext_found) return true;
		}
	}
	return false;
}

bool all_out(void) {
	for (int i = 1; i <= m; i++) {
		if (!runner[i].out) return false;
	}
	return true;
}