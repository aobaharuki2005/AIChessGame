#include <iostream>
#include <stack>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <time.h>
#include <algorithm>
using namespace sf;
using namespace std;

int _size_ = 56;         // kich thuoc ban co
Vector2f offset(28, 28); // vecto tinh tien, dung de di chuyen quan co

int board[8][8] = // cau hinh ban co
    {
        -1, -2, -3, -4, -5, -3, -2, -1,
        -6, -6, -6, -6, -6, -6, -6, -6,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        6, 6, 6, 6, 6, 6, 6, 6,
        1, 2, 3, 4, 5, 3, 2, 1};

typedef struct QuanCo
{ // cau truc du lieu cho cac quan co
    Sprite s;
    int index, cost;
};

class GameManager
{
public:
    QuanCo f[33];              // luu cac quan co
    Vector2f positiveMove[32]; // toa do nuoc hop le
    int positiveCount;         // so nuoc hop le
    stack<Vector2f> posS;      // danh sach nuoc da di
    stack<int> nS;             // index cac quan da di

    void Move(int n, Vector2f oldPos, Vector2f newPos); // danh co
    void Undo();                                        // quay lai nuoc truoc
    void Create();                                      // khoi tao van co
    void Play();                                        // xu li thao tac trong tro choi

    /* Tim cac nuoc di hop le cua tung quan */
    void PositiveXe(int n, int x, int y, int grid[9][9]);
    void PositiveTuong(int n, int x, int y, int grid[9][9]);
    void PositiveMa(int n, int x, int y, int grid[9][9]);
    void PositiveVua(int n, int x, int y, int grid[9][9]);
    void PositiveTot(int n, int x, int y, int grid[9][9]);

    void IncreasePositive(int i, int j); // tang so nuoc di hop le them 1
    
    void PositiveMoving(int n);          // tim cac nuoc di hop le 
    /* Phai bo sung tinh nang kiem tra chieu trong ham nay, de ngan ngua nhung nuoc di khong hop le */
    
    int CostMove();                      // ham danh gia nuoc di
    // int Minimax(int depth, bool luot);   // Chien luoc MiniMax
    int Alpha_Beta(int depth, bool luot, int alpha, int beta); // Thuat toan cat tia alpha-beta
    Vector2f getNextMove(bool luot);                           // tra ve nuoc di tot nhat
};

void GameManager::Create()
{
    positiveCount = 0; // so nuoc hop le ban dau = 0
    int k = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; i < 8; j++)
        {
            int n = board[i][j];
            if (n == 0)
                continue;
            int x = abs(n) - 1;
            int y = (n > 0) ? 1 : 0;
            f[k].index = n;
            f[k].s.setTextureRect(IntRect(_size_ * x, _size_ * y, _size_, _size_));
            f[k].s.setPosition(_size_ * j + offset.x, _size_ * i + offset.y);

            // khoi tao chi so cost
            int v = 0;
            int g;
            g = abs(f[k].index);
            if (g == 1)
                v = 50;
            else if (g == 2 || g == 3)
                v = 30;
            else if (g == 4)
                v = 90;
            else if (g == 5)
                v = 900;
            else if (g == 6)
                v = 10;

            f[k].cost = f[k].index / g * v;
            k++;
        }
}

void GameManager::Play()
{
    RenderWindow window(VideoMode(504, 504), "The Chess! Alpha Beta Pruning");

    Texture t1, t2, t3;
    t1.loadFromFile("figures.png");
    t2.loadFromFile("board1.png");
    t3.loadFromFile("no.png");

    for (int i = 0; i < 32; i++)
        f[i].s.setTexture(t1); // set texture mac dinh cho cac o ban co
    Sprite sBoard(t2);
    Sprite sPositive(t3);

    Create();

    bool LuotChoi = true;    // true = nguoi, false = may
    Vector2f oldPos, newPos; // luu vi tri click lan1 (chon quan) va lan2 (danh)
    int n = 0;               // n: luu chi so trong mang f
    int click = 0;
    int count = 0;
    Vector2i pos; // toa do chuot khi click

    while (window.isOpen())
    {
        Event e; // bat su kien click, close

        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close(); // su kien dong cua so
            if (e.type == Event::KeyPressed)
            { // su kien bam phim cach
                if (e.key.code == Keyboard::BackSpace)
                    Undo();
            }
            if (e.type == Event::MouseButtonPressed)
            { // sk bam chuot
                if (e.key.code == Mouse::Left)
                {
                    pos = Mouse::getPosition(window) - Vector2i(offset);
                    click++;
                }
            }

            if (LuotChoi == true)
            {
                if (click == 1)
                {
                    bool isMove = false; // kiem tra xem click dau co hop le khong
                    for (int i = 16; i < 32; i++)
                    {
                        if (f[i].s.getGlobalBounds().contains(pos.x + offset.y, pos.y + offset.y))
                        {
                            isMove = true;
                            n = i;
                            f[n].s.setColor(Color::Green);
                            oldPos = f[n].s.getPosition();
                        }
                    }
                }

                if (click == 2)
                { // neu click thu 2 trong vung co the di thi danh co va trao lai quyen choi
                    f[n].s.setColor(Color::White);
                    int x = pos.x / _size_;
                    int y = pos.y / _size_;
                    newPos = Vector2f(x * _size_, y * _size_) + offset;

                    // chi di chuyen trong vung positiveMove
                    for (int i = 0; i < count; i++)
                    {
                        if (positiveMove[i] == newPos)
                        {
                            Move(n, oldPos, newPos);
                            LuotChoi = !LuotChoi; // trao quyen choi
                        }
                    }

                    count = 0, click = 0; // reset
                }

                else
                { // may choi
                    newPos = getNextMove(LuotChoi);
                    int c = nS.top();
                    nS.pop(); // xoa khoi stack, vi ham move tu nhet trong stack roi
                    Move(c, oldPos, newPos);
                    LuotChoi = !LuotChoi;
                    click = 0; // reset
                }

                // draw
                window.draw(sBoard);
                for (int i = 0; i < count; i++)
                    window.draw(sPositive); // to mau cac nuoc co the di
                for (int i = 0; i < 32; i++)
                    window.draw(f[i].s); // ve cac quan co
                window.display();        // Hien thi ra man hinh
            }
        }
    }
}

void GameManager::IncreasePositive(int i, int j)
{
    positiveMove[positiveCount] = Vector2f(i * _size_, j * _size_) + offset;
    positiveCount++;
}

void GameManager::PositiveXe(int n, int x, int y, int grid[9][9])
{
    for (int i = x + 1; i < 8; i++) // huong bac
    {
        if (grid[i][y] != 0) // neu gap vat can
        {
            if (grid[i][y] * grid[x][y] < 0) // neu la quan doi phuong
                IncreasePositive(i, y);
            break;
        }
        IncreasePositive(i, y);
    }
    for (int i = x - 1; i >= 0; i--) // huong nam
    {
        if (grid[i][y] != 0)
        {
            if (grid[i][y] * grid[x][y] < 0)
                IncreasePositive(i, y);
            break;
        }
        IncreasePositive(i, y);
    }
    for (int j = y + 1; j < 8; j++) // huong tay
    {
        if (grid[x][j] != 0)
        {
            if (grid[x][j] * grid[x][y] < 0)
                IncreasePositive(x, j);
            break;
        }
        IncreasePositive(x, j);
    }
    for (int j = y - 1; j >= 0; j--) // huong dong
    {
        if (grid[x][j] != 0)
        {
            if (grid[x][j] * grid[x][y] < 0)
                IncreasePositive(x, j);
            break;
        }
        IncreasePositive(x, j);
    }
}

void GameManager::PositiveTuong(int n, int x, int y, int grid[9][9])
{
    for (int i = x + 1, j = y + 1; (i < 8) && (j < 8); i++, j++)
    {   // cheo goc phan tu thu nhat
        if (grid[i][j] != 0)    // neu gap vat can
        {   
            // neu la quan doi phuong
            if (grid[i][j] * grid[x][y] < 0) IncreasePositive(i, j);
            break;
        }
        IncreasePositive(i, j);
    }
    
    for (int i = x + 1, j = y - 1; (i < 8) && (j > 0); i++, j++)
    {   // cheo goc phan tu thu hai
        if (grid[i][j] != 0)
        {
            if (grid[i][j] * grid[x][y] < 0) IncreasePositive(i, j);
            break;
        }
        IncreasePositive(i, j);
    }
    
    for (int i = x - 1, j = y + 1; (x > 0) && (j < 8); i++, j++)
    {   // cheo goc phan tu thu ba
        if (grid[i][j] != 0)
        {
            if (grid[i][j] * grid[x][y] < 0) IncreasePositive(i, j);
            break;
        }
        IncreasePositive(i, j);
    }

    for (int i = x - 1, j = y - 1; (x > 0) && (j > 0); i++, j++)
    {   // cheo goc phan tu thu tu
        if (grid[i][j] != 0)
        {
            if (grid[i][j] * grid[x][y] < 0) IncreasePositive(i, j);
            break;
        }
        IncreasePositive(i, j);
    }
}

void GameManager::PositiveMa(int n, int x, int y, int grid[9][9]) // xet 8 vi tri co the di theo hinh chu L
{
    if ((grid[x + 2][y + 1] == 0 || grid[x][y] * grid[x + 2][y + 1] < 0) && x + 2 < 8 && y + 1 < 8)
        IncreasePositive(x + 2, y + 1);
    if ((grid[x + 2][y - 1] == 0 || grid[x][y] * grid[x + 2][y - 1] < 0) && y - 1 >= 0 && x + 2 < 8)
        IncreasePositive(x + 2, y - 1);
    if ((grid[x - 2][y + 1] == 0 || grid[x][y] * grid[x - 2][y + 1] < 0) && x - 2 >= 0 && y + 1 < 8)
        IncreasePositive(x - 2, y + 1);
    if ((grid[x - 2][y - 1] == 0 || grid[x][y] * grid[x - 2][y - 1] < 0) && x - 2 >= 0 && y - 1 >= 0)
        IncreasePositive(x - 2, y - 1);
    if ((grid[x + 1][y + 2] == 0 || grid[x][y] * grid[x + 1][y + 2] < 0) && x + 1 < 8 && y + 2 < 8)
        IncreasePositive(x + 1, y + 2);
    if ((grid[x - 1][y + 2] == 0 || grid[x][y] * grid[x - 1][y + 2] < 0) && x - 1 >= 0 && y + 2 < 8)
        IncreasePositive(x - 1, y + 2);
    if ((grid[x + 1][y - 2] == 0 || grid[x][y] * grid[x + 1][y - 2] < 0) && y - 2 >= 0 && x + 1 < 8)
        IncreasePositive(x + 1, y - 2);
    if ((grid[x - 1][y - 2] == 0 || grid[x][y] * grid[x - 1][y - 2] < 0) && x - 1 >= 0 && y - 2 >= 0)
        IncreasePositive(x - 1, y - 2);
}

void GameManager::PositiveVua(int n, int x, int y, int grid[9][9]) // xet 8 vi tri xung quanh no
{
    if ((grid[x + 1][y] == 0 || grid[x][y] * grid[x + 1][y] < 0) && x + 1 < 8)
        IncreasePositive(x + 1, y);
    if ((grid[x - 1][y] == 0 || grid[x][y] * grid[x - 1][y] < 0) && x - 1 >= 0)
        IncreasePositive(x - 1, y);
    if ((grid[x + 1][y + 1] == 0 || grid[x][y] * grid[x + 1][y + 1] < 0) && x + 1 < 8 && y + 1 < 8)
        IncreasePositive(x + 1, y + 1);
    if ((grid[x - 1][y + 1] == 0 || grid[x][y] * grid[x - 1][y + 1] < 0) && x - 1 >= 0 && y + 1 < 8)
        IncreasePositive(x - 1, y + 1);
    if ((grid[x][y + 1] == 0 || grid[x][y] * grid[x][y + 1] < 0) && y + 1 < 8)
        IncreasePositive(x, y + 1);
    if ((grid[x - 1][y - 1] == 0 || grid[x][y] * grid[x - 1][y - 1] < 0) && x - 1 >= 0 && y - 1 >= 0)
        IncreasePositive(x - 1, y - 1);
    if ((grid[x + 1][y - 1] == 0 || grid[x][y] * grid[x + 1][y - 1] < 0) && y - 1 >= 0 && x + 1 < 8)
        IncreasePositive(x + 1, y - 1);
    if ((grid[x][y - 1] == 0 || grid[x][y] * grid[x][y - 1] < 0) && y - 1 >= 0)
        IncreasePositive(x, y - 1);
}

void GameManager::PositiveTot(int n, int x, int y, int grid[9][9])
{
    int k = grid[x][y] / abs(grid[x][y]); // 1 hoac -1
    if ((y == 1 || y == 6) && grid[x][y - k] == 0 && grid[x][y - 2 * k] == 0 && y - 2 * k >= 0 && y - 2 * k < 8)
        IncreasePositive(x, y - 2 * k);
    if (grid[x][y - k] == 0 && y - k >= 0 && y - k < 8)
        IncreasePositive(x, y - k);
    if (grid[x + 1][y - k] * grid[x][y] < 0 && y - k >= 0 && y - k < 8 && x + 1 < 8)
        IncreasePositive(x + 1, y - k);
    if (grid[x - 1][y - k] * grid[x][y] < 0 && y - k >= 0 && y - k < 8 && x - 1 >= 0)
        IncreasePositive(x - 1, y - k);
}

int main()
{
    GameManager gm;
    gm.Play();
    return 0;
}
