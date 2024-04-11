#include <iostream>
#include <utility>
#include <queue>

using namespace std;

#define pii pair<int, int>

typedef struct {
	int power;
	int time;
} Board;

int n, m, k;
Board board[11][11];

int dy[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
int dx[8] = { 1, 0, -1, 0, 1, -1, -1, 1 };

pii find_weakest(void);
pii find_strongest(void);
bool lazor_attack(pii a, pii d);
void bomb_attack(pii a, pii d);
bool check_broken(void);
void repair(void);
void answer(void);

void print_board(void) {
	cout << "\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << board[i][j].power << " ";
		} cout << "\n";
	} cout << "\n";
}

int main(void)
{
	ios_base::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);

	cin >> n >> m >> k;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> board[i][j].power;
			board[i][j].time = k + 1;
		}
	}

	while (k--) {
		pair<int, int> attacker = find_weakest();
		pair<int, int> defender = find_strongest();
		/*
		cout << "Turn #" << k << ", a:(" << attacker.first << "," << attacker.second << "), d:(" << defender.first << "," << defender.second << ")\n";
		*/
		board[attacker.first][attacker.second].power += (n + m);
		board[attacker.first][attacker.second].time = k;

		bool lazor_success = lazor_attack(attacker, defender);
		if (!lazor_success) bomb_attack(attacker, defender);

		bool one_left = check_broken();
		if (one_left) break;

		//print_board();

		repair();

		//print_board();
	}

	answer();

	return 0;
}

pair<int, int> find_weakest(void) {
	pair<int, int> p;
	p.first = 0; p.second = 0;
	int time = 1001;
	int power = 5001;
	int r = -1, c = -1;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (board[i][j].power != 0) {
				if (power == board[i][j].power) {
					if (time == board[i][j].time) {
						if (r + c == i + j) {
							if (c < j) {
								p.first = i;
								p.second = j;
								power = board[i][j].power;
								time = board[i][j].time;
								r = i; c = j;
							}
						}
						else if (r + c < i + j) {
							p.first = i;
							p.second = j;
							power = board[i][j].power;
							time = board[i][j].time;
							r = i; c = j;
						}
					}
					else if (time > board[i][j].time) {
						p.first = i;
						p.second = j;
						power = board[i][j].power;
						time = board[i][j].time;
						r = i; c = j;
					}
				}
				else if (power > board[i][j].power) {
					p.first = i;
					p.second = j;
					power = board[i][j].power;
					time = board[i][j].time;
					r = i; c = j;
				}
			}
		}
	}

	return p;
}

pair<int, int> find_strongest(void) {
	pii p;
	p.first = 0; p.second = 0;
	int time = -1;
	int power = -1;
	int r = 11, c = 11;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (board[i][j].power != 0) {
				if (power == board[i][j].power) {
					if (time == board[i][j].time) {
						if (r + c == i + j) {
							if (c > j) {
								p.first = i;
								p.second = j;
								power = board[i][j].power;
								time = board[i][j].time;
								r = i; c = j;
							}
						}
						else if (r + c > i + j) {
							p.first = i;
							p.second = j;
							power = board[i][j].power;
							time = board[i][j].time;
							r = i; c = j;
						}
					}
					else if (time < board[i][j].time) {
						p.first = i;
						p.second = j;
						power = board[i][j].power;
						time = board[i][j].time;
						r = i; c = j;
					}
				}
				else if (power < board[i][j].power) {
					p.first = i;
					p.second = j;
					power = board[i][j].power;
					time = board[i][j].time;
					r = i; c = j;
				}
			}
		}
	}

	return p;
}

bool lazor_attack(pii a, pii d) {

	bool lazor_success = false;
	bool visited[11][11] = { 0, };

	queue<vector<pii>> q;
	vector<pii> v;

	v.push_back(a);
	q.push(v);

	while (!q.empty()) {
		vector<pii> path = q.front();
		q.pop();

		pii p = path[path.size() - 1];

		for (int i = 0; i < 4; i++) {
			int ny = (p.first + dy[i] + n)%n;
			int nx = (p.second + dx[i] + m)%m;
			
			vector<pii> newpath = path;

			// 부서진 포탑이거나 탐색한 적 있다면 다음칸 탐색
			if (board[ny][nx].power == 0) continue;
			if (visited[ny][nx]) continue;

			visited[ny][nx] = true;
			newpath.push_back({ ny, nx });
			q.push(newpath);
			/*
			for (int i = 0; i < path.size() - 1; i++) {
				int y = newpath[i].first;
				int x = newpath[i].second;
				cout << "(" << y << "," << x << ") -> ";
			} cout << "\n";
			*/
			if (ny == d.first && nx == d.second) {
				lazor_success = true;
				v = newpath;
				break;
			}
		}

		if (lazor_success) break;
	}

	if (lazor_success) {
		int power = board[a.first][a.second].power;
		for (int i = 1; i < v.size() - 1; i++) {
			int y = v[i].first;
			int x = v[i].second;
			board[y][x].power -= (power / 2);
			board[y][x].time = k;
		}
		board[d.first][d.second].power -= power;
		board[d.first][d.second].time = k;
		return true;
	}

	else return false;
}

void bomb_attack(pii a, pii d) {

	int power = board[a.first][a.second].power;

	for (int i = 0; i < 8; i++) {
		int y = (d.first + dy[i] + n)%n;
		int x = (d.second + dx[i]+ m)%m;
		
		if (board[y][x].power == 0) continue;
		if (y == a.first && x == a.second) continue;

		board[y][x].power -= (power / 2);
		board[y][x].time = k;
	}
	board[d.first][d.second].power -= power;
	board[d.first][d.second].time = k;
}

bool check_broken(void) {
	int not_broken = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (board[i][j].power <= 0)
				board[i][j].power = 0;
			else not_broken++;
		}
	}
	if (not_broken == 1) return true;
	else return false;
}

void repair(void) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (board[i][j].power == 0) continue;
			if (board[i][j].time == k) continue;
			board[i][j].power += 1;
		}
	}

}

void answer(void) {
	int power = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (board[i][j].power > power)
				power = board[i][j].power;
		}
	}
	cout << power << "\n";
}