#include <iostream>
#include <stack>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <time.h>
#include <algorithm>
using namespace sf;
using namespace std;

int _size_ = 56;    // kich thuoc ban co
Vector2f offset(28,28);     // dieu chinh chon quan, khi di chuyen

int board[8][8] =   // cau hinh ban co
{
    -1, -2, -3, -4, -5, -3, -2, -1,
    -6, -6, -6, -6, -6, -6, -6, -6,
     0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,
     6,  6,  6,  6,  6,  6,  6,  6,
     1,  2,  3,  4,  5,  3,  2,  1
};

typedef struct QuanCo {     // cau truc du lieu cho cac quan co
    Sprite s;
    int index, cost;
};


class GameManager {
public:
    QuanCo f[33];    // mang luu cac quan co
    Vector2f positiveMove[32];   // vi tri cac nuoc co the di chuyen
    int positiveCount;  // so nuoc mot quan co co the di chuyen
    stack<Vector2f> posS;   // luu vi tri cac nuoc da di
    stack<int> nS;  // luu index cac quan da di, tuong ung voi vi tri ben tren

    void move(int n, Vector2f oldPos, Vector2f newPos);     // di chuyen quan co (danh co)
    void undo();    // quay lai nuoc truoc
    void create();  // khoi tao mang quan co va cac gia tri can thiet
    void play();    // xu li moi thao tac trong game

    void PositiveXe(int n, int x, int y, int grid[9][9]);    // Cac nuoc quan xe co the di duoc
    void PositiveTuong(int n, int x, int y, int grid[9][9]);
    void PositiveMa(int n, int x, int y, int grid[9][9]);
    void PositiveVua(int n, int x, int y, int grid[9][9]);
    void PositiveTot(int n, int x, int y, int grid[9][9]);
    
    void IncreasePositive(int i, int j); // tang them 1 nuoc co the di o vi tri i, j
    void PositiveMoving(int n);  // tim cac nuoc di co the ung voi index n
    int CostMove(); // tinh diem nuoc di (ham danh gia)
    // int Minimax(int depth, bool luot);   // Chien luoc MiniMax
    int Alpha_Beta(int depth, bool luot, int alpha, int beta);   // ham cat tia alpha
    Vector2f getNextMove(bool luot);    // tra ve nuoc di tot nhat theo chien luoc 

};

// Ham khoi tao game co
void GameManager::create() {    // gan cac gia tri can thiet vao danh sach quan co
    positiveCount = 0;  // so nuoc co the di ban dau = 0 (chua chon gi ca)
    int k = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; i < 8; j++) {
            int n = board[i][j];
            if (n == 0) continue;
            int x = abs(n) - 1;
            int y = (n > 0) ? 1 : 0;
            f[k].index = n;
            f[k].s.setTextureRect(IntRect(_size_ * x, _size_ * y, _size_, _size_));
            f[k].s.setPosition(_size_ * j + offset.x, _size_ * i + offset.y);

            // khoi tao chi so cost
            int v = 0;
            int g;
            g = abs(f[k].index);
            if (g == 1) v = 50;
            else if (g == 2 || g == 3) v = 30;
            else if (g == 4) v = 90;
            else if (g == 5) v = 900;
            else if (g == 6) v = 10;

            f[k].cost = f[k].index / g * v;
            k++;
        }

}

void GameManager::play() {
    cout << "Hello World!" << endl;
}

int main()
{
    GameManager gm;
    gm.play();
    
}
