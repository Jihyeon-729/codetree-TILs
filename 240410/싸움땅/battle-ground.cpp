#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef struct {
    int y, x, d, s;
    int gun;
    int point;
} Player;

typedef struct {
    int player;
    priority_queue<int> guns;
} Board;

int n, m, k;

Player players[31];
vector<vector<Board>> board;

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

void input(void);
void solve(void);
void answer(void);
int move(int idx);
void move_loser(int idx);
void fight(int attacker, int defender);
void get_gun(int idx);

void print_player(void) {
    for (int i = 1; i <= m; i++) {
        cout << "Player #" << i << " at (" << players[i].y << "," << players[i].x << ")\n";
        
        cout << "d, s, gun, point: " << players[i].d << "," << players[i].s << "," << players[i].gun << "," << players[i].point << "\n";
    }
}

void print_boardplayer(void) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cout << board[i][j].player << " ";
        }cout << "\n";
    }
    cout << "\n";
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    input();
    solve();
    answer();

    return 0;
}

void input(void) {
    cin >> n >> m >> k;

    priority_queue<int> pq;
    vector<Board> v;

    // 행별 열 삽입
    for (int i = 0; i <= n; i++) {
        Board b;
        b.player = 0;
        v.push_back(b);
    }
    // 행 삽입
    for (int i = 0; i <= n; i++) {
        board.push_back(v);
    }

    int num;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> num;
            if(num != 0)
                board[i][j].guns.push(num);
        }
    }

    int y, x, d, s;
    for (int i = 1; i <= m; i++) {
        cin >> y >> x >> d >> s;
        players[i].y = y;
        players[i].x = x;
        players[i].d = d;
        players[i].s = s;
        players[i].gun = 0;
        players[i].point = 0;
        board[y][x].player = i;
    }
}

void solve(void)
{
    while (k--) {
        //cout << "------------Round " << k << "------------\n";
        for (int i = 1; i <= m; i++) {
            int encountered = move(i);
            if(encountered != 0)
                fight(i, encountered);
            else {
                get_gun(i);
            }

            //print_player();
            //print_boardplayer();
        }
    }
}

void answer(void) {
    for (int i = 1; i <= m; i++) {
        cout << players[i].point << " ";
    } cout << "\n";
}

int move(int idx) {
    Player p = players[idx];

    board[p.y][p.x].player = 0;

    int ny = p.y + dy[p.d];
    int nx = p.x + dx[p.d];
    // 격자를 벗어난다면
    if (ny<1 || ny>n || nx<1 || nx>n) {
        players[idx].d += 2;
        players[idx].d %= 4;
        ny = p.y + dy[players[idx].d];
        nx = p.x + dx[players[idx].d];
    }
    players[idx].y = ny;
    players[idx].x = nx;
    //cout << "Player #" << idx << " moved to (" << ny << "," << nx << ")\n";


    return board[ny][nx].player;
}

void move_loser(int idx) {
    Player p = players[idx];

    int dir = p.d;

    for (int i = 0; i < 4; i++) {
        dir = (dir + i) % 4;
        int ny = p.y + dy[dir];
        int nx = p.x + dx[dir];

        if (ny<1 || ny>n || nx<1 || nx>n) continue;
        if (board[ny][nx].player != 0) continue;

        players[idx].y = ny;
        players[idx].x = nx;
        players[idx].d = dir;
        get_gun(idx);

        //cout << "Loser #" << idx << " moved to (" << ny << "," << nx << ")\n";

        break;
    }

    return;
}

void fight(int attacker, int defender) {
    // 싸움
    Player a = players[attacker];
    Player d = players[defender];

    int win=0, lose=0;
    
    if (a.s + a.gun == d.s + d.gun) {
        win = a.s > d.s ? attacker : defender;
        lose = win == attacker ? defender : attacker;
    } else {
        win = a.s + a.gun > d.s + d.gun ? attacker : defender;
        lose = win == attacker ? defender : attacker;
    }

    Player w = players[win];
    Player l = players[lose];

    board[w.y][w.x].player = win;
    players[win].point += (w.s + w.gun) - (l.s + l.gun);

    // 진 플레이어
    if (l.gun != 0) {
        // 총 내려놓고
        board[l.y][l.x].guns.push(l.gun);
        players[lose].gun = 0;
    }
    // 이동
    move_loser(lose);

    // 이긴 플레이어
    get_gun(win);
}

void get_gun(int idx)
{
    Player p = players[idx];
    board[p.y][p.x].player = idx;

    // 칸 총이 없다면
    if (board[p.y][p.x].guns.empty()) return;

    // 칸에 있고 플레이어도 있다면
    int newgun = board[p.y][p.x].guns.top();
    if (p.gun != 0) {
        if (newgun > p.gun) {
            board[p.y][p.x].guns.pop();
            board[p.y][p.x].guns.push(p.gun);
            players[idx].gun = newgun;
        }
    }
    else {  // 칸에 있고 플레이어 없다면
        board[p.y][p.x].guns.pop();
        players[idx].gun = newgun;
    }
}