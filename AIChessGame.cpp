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

int main()
{
    //std::cout << "Hello World!\n";
    GameManager gm;
    gm.play();
    
}
