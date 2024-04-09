#include <iostream>
#include <string.h>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

typedef struct {
	int y, x;
	int h, w;
	int health;
	int damage;
	bool out;
} Knight;

typedef struct {
	int status;
	int idx;
} Board;

int L, N, Q;
Board board[42][42];
Knight knight[31];

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

void init(void);
void get_knight_info(void);
void get_command(void);
bool movable(int idx, int dir);
vector<int> move(int idx, int dir);
void damage(vector<int> &moved, int idx, int dir);
void answer(void);

void print_boardstatus(void)
{
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cout << board[i][j].status << " ";
		} cout << "\n";
	} cout << "\n";
}

void print_boardidx(void)
{
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cout << board[i][j].idx << " ";
		} cout << "\n";
	} cout << "\n";
}

void print_knightdamage(void)
{
	for (int i = 1; i <= N; i++) {
		cout << "Knight #" << i << " - damage: " << knight[i].damage << "\n";
	} cout << "\n";
}

void print_knighthealth(void) 
{
	for (int i = 1; i <= N; i++) {
		cout << "Knight #" << i << " - health: " << knight[i].health << "\n";
	} cout << "\n";
}

int main(void)
{
	ios_base::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);

	init();
	get_knight_info();
	get_command();
	answer();

	return 0;
}


void init(void)
{
	cin >> L >> N >> Q;

	memset(board, sizeof(board), 0);
	for (int i = 0; i <= L + 1; i++) {
		for (int j = 0; j <= L + 1; j++) {
			if (1 <= i && i <= L && 1 <= j && j <= L)
				cin >> board[i][j].status;
			else board[i][j].status = 2;
			board[i][j].idx = 0;
		}
	}

	memset(knight, sizeof(knight), 0);
	return;
}


void get_knight_info(void)
{
	int r, c, h, w, k;
	for (int i = 1; i <= N; i++) {
		cin >> r >> c >> h >> w >> k;
		knight[i].y = r;
		knight[i].x = c;
		knight[i].h = h;
		knight[i].w = w;
		knight[i].health = k;
		knight[i].out = false;
		knight[i].damage = 0;

		for (int j = r; j < r + h; j++) {
			for (int k = c; k < c + w; k++) {
				board[j][k].idx = i;
			}
		}
	}
}

void get_command(void)
{
	int i, d;
	while (Q--) {
		cin >> i >> d;
		vector<int> moved = move(i, d);
		damage(moved, i, d);
		/* print_boardidx();
		print_knightdamage();
		print_knighthealth();*/
	}
}

bool movable(int idx, int dir)
{
	Knight kn = knight[idx];

	// out
	if (knight[idx].out) return false;

	// wall
	int ny = kn.y + dy[dir];
	int nx = kn.x + dx[dir];
	for (int i = ny; i < ny + kn.h; i++) {
		for (int j = nx; j < nx + kn.w; j++) {
			if (i<1 || i>L || j<1 || j>L) return false;
			if (board[i][j].status == 2) return false;
		}
	}

	return true;
}

vector<int> move(int idx, int dir)
{
	vector<int> moved;
	if (!movable(idx, dir)) return moved;

	queue<int> q;
	q.push(idx);
	moved.push_back(idx);
	while (!q.empty()) {
		int num = q.front();
		q.pop();

		int ny = knight[num].y + dy[dir];
		int nx = knight[num].x + dx[dir];
		int h = knight[num].h;
		int w = knight[num].w;

		for (int i = ny; i < ny + h; i++) {
			for (int j = nx; j < nx + w; j++) {
				// other knight exists
				if (board[i][j].idx != num && board[i][j].idx > 0) {
					if (movable(board[i][j].idx, dir)) {
						q.push(board[i][j].idx);
						if (find(moved.begin(), moved.end(), board[i][j].idx) == moved.end())
							moved.push_back(board[i][j].idx);
					}
					else {
						moved.clear();
						return moved;
					}
				}
			}
		}
	}

	for (int i = 0; i < moved.size(); i++) {
		Knight kn = knight[moved[i]];
		int y = kn.y, x = kn.x;
		int h = kn.h, w = kn.w;
		
		for (int r = y; r < y + h; r++) {
			for (int c = x; c < x + w; c++) {
				board[r][c].idx = 0;
			}
		}

		knight[moved[i]].y += dy[dir];
		knight[moved[i]].x += dx[dir];
	}

	for (int i = 0; i < moved.size(); i++) {
		Knight kn = knight[moved[i]];
		int y = kn.y, x = kn.x;
		int h = kn.h, w = kn.w;

		for (int r = y; r < y + h; r++) {
			for (int c = x; c < x + w; c++) {
				board[r][c].idx = moved[i];
			}
		}
	}

	return moved;
}

void damage(vector<int> &moved, int idx, int dir)
{
	for (int i = 0; i < moved.size(); i++) {
		
		if (moved[i] == idx) continue;
		
		Knight kn = knight[moved[i]];
		int y = kn.y, x = kn.x;
		int h = kn.h, w = kn.w;
		int cnt = 0;

		for (int r = y; r < y + h; r++) {
			for (int c = x; c < x + w; c++) {
				if (board[r][c].status == 1)
					cnt++;
			}
		}

		knight[moved[i]].health -= cnt;
		if (knight[moved[i]].health <= 0) {
			knight[moved[i]].health = 0;
			knight[moved[i]].out = true;
			for (int r = y; r < y + h; r++) {
				for (int c = x; c < x + w; c++) {
					board[r][c].idx = 0;
				}
			}
		}
		knight[moved[i]].damage += cnt;
	}
}

void answer(void)
{
	int sum = 0;
	for (int i = 0; i <= N; i++) {
		if (knight[i].out) continue;
		sum += knight[i].damage;
	}
	cout << sum << "\n";
}