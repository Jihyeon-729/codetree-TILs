#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

typedef struct {
	int y, x, d;
	int curd;
} Person;

int t;
int n, m, h, k;
int arr[100][100];
bool tree[100][100] = {0, };
int dir[100][100];
int rdir[100][100];

long long answer = 0;

// 좌우상하
int dy[4] = {0, 0, -1, 1};
int dx[4] = {-1, 1, 0, 0};

Person chaser;
vector<Person> runner;

void input(void);
void make_snail(void);
void run(void);
void chase(void);
void catchh(void);
int get_dist(int fy, int fx, int ty, int tx);

void print_snail(void) {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << dir[i][j] << " ";
		} cout << "\n";
	}cout << "\n";
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << rdir[i][j] << " ";
		} cout << "\n";
	}cout << "\n";
}

int main(void)
{
	input();
	make_snail();
	//print_snail();
	for(t=1; t<=k; t++) {
		run();
		chase();
		catchh();
	}

	cout << answer << "\n";

	return 0;
}

void input(void) {
	cin >> n >> m >> h >> k;

	Person r;
	runner.resize(m + 1);
	for (int i = 1; i <= m; i++) {
		cin >> r.y >> r.x >> r.d;
		r.d--;
		r.curd = (r.d * 2) + 1;
		runner[i] = r;
	}

	int y, x;
	for (int i = 0; i < h; i++) {
		cin >> y >> x;
		tree[y][x] = true;
	}

	chaser.y = (n + 1) / 2;
	chaser.x = (n + 1) / 2;
	chaser.d = 1;
	chaser.curd = 2;

	return;
}

void make_snail(void) {

	int y = chaser.y;
	int x = chaser.x;

	// 0 1 2 3 좌 우 상 하
	for (int sz = 0; sz <= n / 2; sz++) {
		for (x; x < chaser.x + sz; x++) {
			dir[y][x] = 1;
		}
		for (y; y < chaser.y + sz; y++) {
			dir[y][x] = 3;
		}
		for (x; x > chaser.x - sz; x--) {
			dir[y][x] = 0;
		}
		for (y; y >= chaser.y - sz; y--) {
			dir[y][x] = 2;
		}
	}

	y = 1; 
	x = 1;

	for (int sz = n / 2; sz > 0; sz--) {
		for (y; y < chaser.y + sz; y++) {
			rdir[y][x] = 3;
		}
		for (x; x < chaser.x + sz; x++) {
			rdir[y][x] = 1;
		}
		for (y; y > chaser.y - sz; y--) {
			rdir[y][x] = 2;
		}
		for (x; x > chaser.x - sz + 1; x--) {
			rdir[y][x] = 0;
		}
		rdir[y++][x] = 3;
	}
	rdir[y][x] = 2;

	return;
}

void run(void) {
	for (int i = 1; i < runner.size(); i++) {
		//술래와의 거리가 3 이하일 때만
		if (get_dist(chaser.y, chaser.x, runner[i].y, runner[i].x) <= 3) {
			int ny = runner[i].y + dy[runner[i].curd];
			int nx = runner[i].x + dx[runner[i].curd];

			if (ny<1 || ny>n || nx<1 || nx>n) {
				runner[i].curd = (runner[i].curd + 1) % 2;
				ny = runner[i].y + dy[runner[i].curd];
				nx = runner[i].x + dx[runner[i].curd];
			}

			//cout << "runner moved (" << runner[i].y << "," << runner[i].x << ") -> ";
			//if (chaser.y == ny && chaser.x == nx) continue;
			if (chaser.y == ny && chaser.x == nx) {
				//cout << "\n";
				continue;
			}
			else {
				runner[i].y = ny;
				runner[i].x = nx;
			}

			//cout << "(" << ny << "," << nx << ")\n";
			
		}
	}
}

void chase(void) {

	int y, x, cd;

	cd = chaser.curd;/*
	if (chaser.d == -1) {
		cd = rdir[chaser.y][chaser.x];
	} else if (chaser.d == 1) {
		cd = dir[chaser.y][chaser.x];
	}*/

	//cout << "Chaser moved (" << chaser.y << "," << chaser.x << ") -> ";

	y = (chaser.y += dy[cd]);
	x = (chaser.x += dx[cd]);

	//cout << "(" << chaser.y << "," << chaser.x << ")\n";

	if (y == 1 && x == 1)
		chaser.d = -1;
	else if (y == (n + 1) / 2 && x == (n + 1) / 2)
		chaser.d = 1;

	if (chaser.d == -1)
		chaser.curd = rdir[y][x];
	else if (chaser.d == 1)
		chaser.curd = dir[y][x];

	return;
}

void catchh(void) {
	int len;
	if (chaser.curd == 0) {
		len = chaser.x;
	}
	else if (chaser.curd == 1) {
		len = n - chaser.x + 1;
	}
	else if (chaser.curd == 2) {
		len = chaser.y;
	}
	else if (chaser.curd == 3) {
		len = n - chaser.y + 1;
	}

	int count = 0;

	int cy, cx;
	for (int i = 0; i < len; i++) {
		cy = chaser.y + i * dy[chaser.curd];
		cx = chaser.x + i * dx[chaser.curd];
		
		if (tree[cy][cx]) continue;

		vector<Person>::iterator it;
		int size = runner.size();

		for (it = runner.begin(); it != runner.end(); ) {
			if ((*it).y == cy && (*it).x == cx) {
				count++;
				it = runner.erase(it);
			}
			else it++;
		}
	}

	answer += (t * count);

	return;
}

int get_dist(int fy, int fx, int ty, int tx) {
	return abs(fy - fx) + abs(ty - tx);
}