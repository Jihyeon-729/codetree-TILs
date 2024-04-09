#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <queue>
#include <set>

using namespace std;

typedef struct {
	int y, x;
} Location;

typedef struct {
	int dist;
	int dir;
	int y, x;
} Info;

int n, m;
int t = 0;
int cnt_onboard = 0;
int cnt_arrived = 0;

int board[15][15];
Location store[30];
Location man[30];
bool arrived[30] = { 0, };

int dy[4] = { -1, 0, 0, 1 };
int dx[4] = { 0, -1, 1, 0 };

void input(void);
void moveto_store(void);
void moveto_basecamp(int idx);
void check_arrival(void);

void print_board(void)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << board[i][j] << " ";
		} cout << "\n";
	} cout << "\n";
}

void print_man(void)
{
	for (int i = 0; i < cnt_onboard; i++) {
		cout << "man #" << i << " at (" << man[i].y << "," << man[i].x << "), ";
		cout << "store #" << i << " at (" << store[i].y << "," << store[i].x << ")\n";
	} cout << "\n";
}

int main(void)
{
	ios_base::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);

	input();
	while (1) {
		t++;
		if (cnt_onboard > 0) {
			moveto_store();
			check_arrival();
		}
		if (t <= m) moveto_basecamp(t-1);

		//print_board();
		//print_man();

		if (cnt_arrived == m) break;
	}
	cout << t << "\n";

	return 0;
}

void input(void) {
	cin >> n >> m;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	for (int i = 0; i < m; i++) {
		cin >> store[i].y >> store[i].x;
		store[i].y--;
		store[i].x--;
	}
}

void moveto_store(void) {
	for (int i = 1; i <= cnt_onboard; i++) {
		// 격자에 있는 i번 사람
		// 편의점에 도착한 사람이 아니라면
		// i번 편의점까지 가장 가까워지는 방향(위치) 찾고
		// 좌표 이동

		Location person = man[i];
		Location goal = store[i];

		// 이미 편의점에 도착한 사람이 아니라면
		if (!(person.y == goal.y && person.x == goal.x)) {

			queue<vector<Info>> q;

			// 시작 탐색좌표 큐에 집어넣기
			for (int j = 0; j < 4; j++) {
				// (탐색방향, 위치)
				vector<Info> v(2);

				int ny = person.y + dy[j];
				int nx = person.x + dx[j];

				// inbound and able to move to
				if (0 <= ny && ny < n && 0 <= nx && nx < n) {
					if (board[ny][nx] == 0 || board[ny][nx] == 1) {
						v[0].dir = j;
						v[0].dist = 1;
						v[0].y = ny;
						v[0].x = nx;
						v[1].dir = j;
						v[1].dist = 1;
						v[1].y = ny;
						v[1].x = nx;
						q.push(v);
					}
				}
			}

			int mindist = 300;
			vector<int> nextdir;

			while (!q.empty()) {

				vector<Info> vi = q.front();
				q.pop();

				// store found
				int size = vi[1].dist;
				//if(mindist < size) continue;


				Info curr = vi[1];
				if (curr.y == goal.y && curr.x == goal.x) {
					if (mindist >= size) {
						mindist = size;
						nextdir.push_back(vi[0].dir);
					}
				}

				for (int j = 0; j < 4; j++) {
					int ny = curr.y + dy[j];
					int nx = curr.x + dx[j];

					// inbound and able to move to
					if (0 <= ny && ny < n && 0 <= nx && nx < n) {
						if (board[ny][nx] == 0 || board[ny][nx] == 1) {
							if (mindist >= size) {
								vector<Info> newvi;
								newvi = vi;
								newvi[1].dir = j;
								newvi[1].dist = vi[1].dist + 1;
								newvi[1].y = ny;
								newvi[1].x = nx;
								q.push(newvi);
							}
						}
					}
				}
			}

			// i번 편의점까지 가장 가까워지는 방향(위치) 찾고
			sort(nextdir.begin(), nextdir.end());
			int dir = nextdir[0];

			// 위치 갱신
			man[i].y += dy[dir];
			man[i].x += dx[dir];
		}
	}
}

void check_arrival(void) {
	for (int i = 0; i < cnt_onboard; i++) {
		Location person = man[i];
		Location goal = store[i];

		// 편의점에 도착했다면
		if (!arrived[i] && (person.y == goal.y && person.x == goal.x)) {
			arrived[i] = true;
			board[person.y][person.x] = 2;
			cnt_arrived++;
		}
	}
}

void moveto_basecamp(int idx) {

	cnt_onboard++;

	queue<pair<Location, int>> q;
	Location sloc = store[idx];
	q.push(make_pair(sloc, 0));

	vector<Location> nearest;

	int mindist = 300;

	while (!q.empty()) {
		pair<Location, int> curr = q.front();
		q.pop();

		// nearest basecamp found
		if (board[curr.first.y][curr.first.x] == 1) {
			if (mindist >= curr.second) {
				mindist = curr.second;
				nearest.push_back(curr.first);
			}
		}

		for (int i = 0; i < 4; i++) {
			int ny = curr.first.y + dy[i];
			int nx = curr.first.x + dx[i];

			// inbound and able to move to
			if (0 <= ny && ny < n && 0 <= nx && nx < n) {
				if (board[ny][nx] == 0 || board[ny][nx] == 1) {
					if (curr.second <= mindist) {
						Location next;
						next.y = ny;
						next.x = nx;
						q.push(make_pair(next, curr.second + 1));
					}
				}
			}
		}
	}

	int y = nearest[0].y;
	int x = nearest[0].x;
	//cout << "(" << y << "," << x << "), ";

	for (int i = 1; i < nearest.size(); i++) {
		//cout << "(" << nearest[i].y << "," << nearest[i].x << "), ";
		if (y == nearest[i].y) {
			if (x > nearest[i].x) {
				x = nearest[i].x;
			}
		}
		else {
			if (y > nearest[i].y) {
				y = nearest[i].y;
				x = nearest[i].x;
			}
		}
	};

	man[idx].y = y;
	man[idx].x = x;
	
	//cout << "Final: (" << y << "," << x << ")\n";

	board[y][x] = 2;
	return;
}