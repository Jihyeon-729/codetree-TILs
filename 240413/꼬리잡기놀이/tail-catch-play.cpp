#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

typedef struct {
	int y, x;
} Location;

int n, m, k;
int rnd;
int answer = 0;

Location hd, tl;

int board[20][20];
vector<Location> head;

// 우 상 좌 하
int dy[4] = { 0, -1, 0, 1 };
int dx[4] = { 1, 0, -1, 0 };

void input(void);
void move(void); 
Location throwball(void);
int score(Location fth, int depth, vector<vector<bool>> &visited);
void changedir(void);
void forward(Location cur, Location nxt);

void print_board(void) {
	cout << "\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << board[i][j] << " ";
		} cout << "\n";
	} cout << "\n";
}

int main(void)
{
	ios_base::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);

	input();
	for (rnd = 0; rnd < k; rnd++) {
		move();
		//print_board();
		Location fth = throwball();
		//cout << "Crash at (" << fth.y << "," << fth.x << ")\n";

		vector<vector<bool>> visited(20, vector<bool>(20, 0));
		answer += score(fth, 1, visited);
		
		changedir();
		//print_board();

		//cout << "answer : " << answer << "\n";
	}
	cout << answer << "\n";

	return 0;
}

void input(void) {

	memset(board, 0, sizeof(board));
	head.clear();
	head.shrink_to_fit();

	Location l;
	cin >> n >> m >> k;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
			if (board[i][j] == 1) {
				l.y = i;
				l.x = j;
				head.push_back(l);
			}
		}
	}
}

void move(void) {
	for (int i = 0; i < head.size(); i++) {
		Location h = head[i];
		for (int j = 0; j < 4; j++) {
			Location n;
			n.y = h.y + dy[j];
			n.x = h.x + dx[j];
			if (board[n.y][n.x] == 4) {
				forward(h, n);
				head[i] = n;
			}
		}
	}
}

Location throwball(void) {	
	int dir;
	dir = (rnd / n) % 4;

	if (dir == 0) {
		for (int r = (rnd % n); r <= (rnd % n); r++) {
			for (int c = 0; c <= (n-1); c++) {
				if (board[r][c] != 0 && board[r][c] != 4) {
					Location l;
					l.y = r;
					l.x = c;
					return l;
				}
			}
		}
	}
	else if (dir == 1) {
		for (int r = n - 1; r >= 0; r--) {
			for (int c = (rnd%n); c <= (rnd%n); c++) {
				if (board[r][c] != 0 && board[r][c] != 4) {
					Location l;
					l.y = r;
					l.x = c;
					return l;
				}
			}
		}
	}
	else if (dir == 2) {
		for (int r = (n - 1) - (rnd%n); r >= (n - 1) - (rnd % n); r--) {
			for (int c = (n-1); c >= 0; c--) {
				if (board[r][c] != 0 && board[r][c] != 4) {
					Location l;
					l.y = r;
					l.x = c;
					return l;
				}
			}
		}
	}
	else if (dir == 3) {
		for (int r = 0; r <= (n - 1); r++) {
			for (int c = (n - 1) - (rnd % n); c >= (n - 1) - (rnd % n); c--) {
				if (board[r][c] != 0 && board[r][c] != 4) {
					Location l;
					l.y = r;
					l.x = c;
					return l;
				}
			}
		}
	}
}

int score(Location cur, int depth, vector<vector<bool>> &visited) {

	int sc = 0;
	visited[cur.y][cur.x] = true;

	if (board[cur.y][cur.x] == 1) {
		hd = cur;
		return (depth * depth);
	}

	if (board[cur.y][cur.x] == 3) {
		tl = cur;
	}

	for (int i = 0; i < 4; i++) {
		Location nxt;
		nxt.y = cur.y + dy[i];
		nxt.x = cur.x + dx[i];

		// 현재 칸에서 진행한 칸이거나, 이동선이 아니거나
		if (nxt.y < 0 || nxt.y >= n || nxt.x < 0 || nxt.x >= n)
			continue;
		if (visited[nxt.y][nxt.x]) continue;
		if (board[nxt.y][nxt.x] == 0) continue;
		if (board[nxt.y][nxt.x] == 4) continue;
		if (board[nxt.y][nxt.x] == 3) {
			tl = nxt;
			cout << "Tail (" << tl.y << "," << tl.x << ")\n";
			continue;
		}

		int ret = score(nxt, depth + 1, visited);
		if (ret != 0) {
			sc = ret;
			break;
		}
	}

	return sc;
}

void changedir(void) {
	for (int i = 0; i < head.size(); i++) {
		if (head[i].y == hd.y && head[i].x == hd.x) {
			head[i] = tl;
			board[hd.y][hd.x] = 3;
			board[tl.y][tl.x] = 1;
		}
	}
}

void forward(Location cur, Location frt) {

	bool notover = true;
	board[frt.y][frt.x] = board[cur.y][cur.x];
	for (int j = 0; j < 4; j++) {
		Location bhd;
		bhd.y = cur.y + dy[j];
		bhd.x = cur.x + dx[j];

		// 현재 칸에서 진행한 칸이거나, 이동선이 아니거나, 뒷사람이 없으면
		if (bhd.y < 0 || bhd.y >= n || bhd.x < 0 || bhd.x >= n) continue;
		if (bhd.y == frt.y && bhd.x == frt.x) continue;
		if (board[bhd.y][bhd.x] == 0) continue;
		if (board[bhd.y][bhd.x] == 4) {
			board[cur.y][cur.x] = 4;
			return;
		}
		
		forward(bhd, cur);
	}
}