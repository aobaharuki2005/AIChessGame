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
    //cout << "Hello World!" << endl;
    RenderWindow window(VideoMode(504, 504), "The Chess! Alpha Beta Pruning");  // tao cua so hien thi choi game
    
    Texture t1, t2, t3; // load cac file hinh anh
    t1.loadFromFile("figures.png");
    t2.loadFromFile("board1.png");
    t3.loadFromFile("no.png");

    for (int i = 0; i < 32; i++) f[i].s.setTexture(t1);
    Sprite sBoard(t2);
    Sprite sPositive(t3);

    create();

    bool LuotChoi = true;   // true = nguoi, false = may
    Vector2f oldPos, newPos;    // luu vi tri click lan1 va lan2
    int n = 0, click = 0, count = 0;    // n: luu chi so trong mang f, count: bien thay the positiveMove, positiveMove la bien toan cuc nen thay doi lien tuc
    Vector2i pos;  // vi tri chuot khi click;

    while (window.isOpen()) {   // ca tro choi la mot vong lap, dieu kien la cua so con mo
        Event e;    // bat su kien click, close

        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) window.close();    // sk dong cua so
            if (e.type == Event::KeyPressed) {  // sk bam phim cach
                if (e.key.code == Keyboard::BackSpace) undo();
            }
            if (e.type == Event::MouseButtonPressed) {  // sk bam chuot
                if (e.key.code == Mouse::Left) {
                    pos = Mouse::getPosition(window) - Vector2i(offset);
                    click++;
                }
            }

            if (LuotChoi == true) {
                if (click == 1) {
                    bool isMove = false;    // kiem tra xem click dau co hop le khong
                    for (int i = 16; i < 32; i++) {
                        if (f[i].s.getGlobalBounds().contains(pos.x + offset.y, pos.y + offset.y)) {
                            isMove = true;
                            n = i;
                            f[n].s.setColor(Color::Green);
                            oldPos = f[n].s.getPosition();
                        }
                    }
                }

                if (click == 2) {   // neu click thu 2 trong vung co the di thi doi luot choi, khong quay lai
                    f[n].s.setColor(Color::White);
                    int x = pos.x / _size_;
                    int y = pos.y / _size_;
                    newPos = Vector2f(x * _size_, y * _size_) + offset;

                    // chi di chuyen trong vung positiveMove
                    for (int i = 0; i < count; i++) {
                        if (positiveMove[i] == newPos) {
                            move(n, oldPos, newPos);
                            LuotChoi = !LuotChoi;
                        }
                    }

                    count = 0, click = 0;   //reset
                }

                else {  // may choi
                    newPos = getNextMove(LuotChoi);
                    int c = nS.top();
                    nS.pop();   // xoa khoi stack, vi ham move tu nhet trong stack roi
                    move(c, oldPos, newPos);
                    LuotChoi = !LuotChoi;
                    click = 0;  //reset
                }

                // draw
                window.draw(sBoard);
                for (int i = 0; i < count; i++) window.draw(sPositive); // to mau cac nuoc co the di
                for (int i = 0; i < 32; i++) window.draw(f[i].s);    // ve cac quan co
                window.display();   // Hien thi ra man hinh
            }
        }

    }



}

int main()
{
    GameManager gm;
    gm.play();
    
}
