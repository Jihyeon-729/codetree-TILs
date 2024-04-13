#include <iostream>
#include <vector>
#include <cmath>
#include <string.h>

using namespace std;

typedef struct {
	int y, x;
	int dist;
} Runner;

typedef struct {
	int y, x;
} Ext;

typedef struct {
	int y, x;
	int size;
} Square;

int n, m, k;
int answer = 0;

int board[11][11];
int board_copy[11][11];
vector<Runner> runner;
Ext ext;

// 상 하 좌 우
int dy[4] = { -1, 1, 0, 0 };
int dx[4] = { 0, 0, -1, 1 };

void input(void);
void move(void);
void rotate(void);
void sumup(void);
int get_dist(int y1, int x1, int y2, int x2);
Square find_square(void);

void print_board(void) {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << board[i][j] << " ";
		} cout << "\n";
	} cout << "\n";
}

void print_runner(void) {
	for (int i = 0; i < runner.size(); i++) {
		cout << "Runner #" << i << "at (" << runner[i].y << "," << runner[i].x << ")\n";
	} cout << "\n";
}

int main(void)
{
	ios_base::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);

	input();
	//print_board();
	while (k--) {
		//cout << "\nRound #" << k << "\n";
		move();

		if (runner.empty()) {
			break;
		}

		//cout << "After move()....\n";
		//print_runner();

		rotate();

		//cout << "After rotate()....\n";
		//print_board();
		//print_runner();
	}

	sumup();
	cout << answer << "\n";
	cout << ext.y << " " << ext.x << "\n";

	return 0;
}

void input(void) {

	runner.clear();
	runner.shrink_to_fit();
	memset(board, 0, sizeof(board));
	ext.y = 0;
	ext.x = 0;

	cin >> n >> m >> k;

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> board[i][j];
		}
	}

	Runner r;
	for (int i = 0; i < m; i++) {
		cin >> r.y >> r.x;
		r.dist = 0;
		runner.push_back(r);
	}

	cin >> ext.y >> ext.x;
	board[ext.y][ext.x] = 10;

	return;
}


void move(void) {

	vector<Runner>::iterator it;

	for (it = runner.begin(); it != runner.end(); ) {
		Runner r = (*it);
		int mind = get_dist(r.y, r.x, ext.y, ext.x);
		bool moved = false;

		for (int j = 0; j < 4; j++) {
			int ny = r.y + dy[j];
			int nx = r.x + dx[j];

			if (ny<1 || ny>n || nx<1 || nx>n) continue;
			if (1 <= board[ny][nx] && board[ny][nx] <= 9) continue;

			if (get_dist(ny, nx, ext.y, ext.x) < mind) {
				(*it).y = ny;
				(*it).x = nx;
				mind = get_dist(ny, nx, ext.y, ext.x);
				moved = true;
			}
		}

		if (moved) (*it).dist++;

		if ((*it).y == ext.y && (*it).x == ext.x) {
			answer += (*it).dist;
			it = runner.erase(it);
		}
		else it++;
	}

	return;
}

void rotate(void) {
	Square sq = find_square();
	memcpy(board_copy, board, sizeof(board));

	//cout << "Square at (" << sq.y << "," << sq.x << "), size: " << sq.size << "\n";


	int oy, ox, cy, cx;
	//보드
	for (int r = sq.y; r < sq.y + sq.size; r++) {
		for (int c = sq.x; c < sq.x + sq.size; c++) {
			oy = r - sq.y;
			ox = c - sq.x;
			cy = ox;
			cx = sq.size - oy - 1;
			cy += sq.y;
			cx += sq.x;

			board[cy][cx] = board_copy[r][c];
		}
	}
	
	//러너
	for (int i = 0; i < runner.size(); i++) {
		
		Runner r = runner[i];
		if (sq.y <= r.y && r.y < sq.y + sq.size && sq.x <= r.x && r.x < sq.x + sq.size) {

			oy = runner[i].y - sq.y;
			ox = runner[i].x - sq.x;

			cy = ox;
			cx = sq.size - oy - 1;
			cy += sq.y;
			cx += sq.x;

			runner[i].y = cy;
			runner[i].x = cx;
		}
	}

	//출구
	oy = ext.y - sq.y;
	ox = ext.x - sq.x;

	cy = ox;
	cx = sq.size - oy - 1;
	cy += sq.y;
	cx += sq.x;

	ext.y = cy;
	ext.x = cx;


	// 내구도 감소
	for (int r = sq.y; r < sq.y + sq.size; r++) {
		for (int c = sq.x; c < sq.x + sq.size; c++) {
			if (1 <= board[r][c] && board[r][c] <= 9) {
				board[r][c]--;
				if (board[r][c] <= 0)
					board[r][c] = 0;
			}
		}
	}
}

void sumup(void) {
	for (int i = 0; i < runner.size(); i++) {
		answer += runner[i].dist;
	}
	return;
}

int get_dist(int y1, int x1, int y2, int x2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

Square find_square(void) {

	Square sq;
	for (int sz = 2; sz <= n; sz++) {
		for (int r = 1; r <= n - sz + 1; r++) {
			for (int c = 1; c <= n - sz + 1; c++) {

				bool rfound = false;
				bool efound = false;
				for (int i = 0; i < runner.size(); i++) {
					int y = runner[i].y;
					int x = runner[i].x;
					if (r <= y && y < r + sz && c <= x && x < c + sz) {
						rfound = true; break;
					}
				}
				if (r <= ext.y && ext.y < r + sz && c <= ext.x && ext.x < c + sz) {
					efound = true;
				}

				if (rfound && efound) {
					sq.y = r;
					sq.x = c;
					sq.size = sz;
					return sq;
				}
			}
		}
	}

}