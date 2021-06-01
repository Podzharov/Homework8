

#include <iostream>
#include <random>
#include <chrono>
#include <stdlib.h>

using namespace std;

//====================================================================

enum Cell
{
    EMPTY = '_',
    CROSS = 'x',
    ZERO = '0'
};

enum Progress
{
    IN_PROGRESS,
    WON_HUMAN,
    WON_AI,
    DRAW
};

void ClearScr()
{
    system("cls");
}

int32_t getRandomNum(int32_t min, int32_t max)
{
    const static auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937_64 generator(seed);
    uniform_int_distribution<int32_t> dis(min, max);
    return dis(generator);
}

struct Field
{
    Cell** ppField = nullptr;
    const size_t size = 3;
    size_t turn = 0; // чей ход, четные человек, нечетные AI
    Cell human = EMPTY;
    Cell ai = EMPTY;
    Progress progress = IN_PROGRESS;
};

struct Coord
{
    size_t y;
    size_t x;
};

//====================================================================

void InitGame(Field & f)
{
    f.ppField = new Cell * [f.size];
    for (size_t y = 0; y < f.size; y++)
    {
        f.ppField[y] = new Cell[f.size];
    }

    for (size_t y = 0; y < f.size; y++)
    {
        for (size_t x = 0; x < f.size; x++)
        {
            f.ppField[y][x] = EMPTY;
        }
    }

    if (getRandomNum(0, 1000) > 500)
    {
        f.human = CROSS;
        f.ai = ZERO;
        f.turn = 0;
    }
    else
    {
        f.human = ZERO;
        f.ai = CROSS;
        f.turn = 1;
    }
}

void DeinitGame(Field& f)
{
    for (size_t y = 0; y < f.size; y++)
    {
        delete [] f.ppField[y];
    }
    delete f.ppField;
   
}

void DrawField(const Field & f)
{
    cout << endl << "      ";
    for (size_t x = 0; x < f.size; x++)
    {
        cout << x + 1 << "     ";
    }
    cout << endl;
    for (size_t y = 0; y < f.size; y++)
    {
        cout << " " << y+1 << " |  ";
        for (size_t x = 0; x < f.size; x++)
        {
            cout << (char)f.ppField[y][x] << "  |  ";
        }
        cout << endl;
    }

    cout << endl << " Human: " << (char)f.human << endl << " Computer: " << (char)f.ai << endl;
}

Coord GetHumanCoord(const Field & f)
{
    Coord c{0};

    cout << endl;

    do {

        cout << " Enter coord X (1..3): ";
        cin >> c.x;

        cout << " Enter coord Y (1..3): ";
        cin >> c.y;

    } while (c.x == 0 || c.y == 0 || c.x > 3 || c.y > 3 || f.ppField[c.y - 1][c.x - 1] != EMPTY);

    c.y--;
    c.x--;
   
    return c;
}

Progress isWon(const Field& f)
{
    for (size_t y = 0; y < f.size; y++)
    {
        if (f.ppField[y][0] == f.ppField[y][1] && f.ppField[y][0] == f.ppField[y][2])
        {
            if (f.ppField[y][0] == f.human)
            {
                return WON_HUMAN;
            }
            else if (f.ppField[y][0] == f.ai)
            {
                return WON_AI;
            }
        }
    }

    for (size_t x = 0; x < f.size; x++)
    {
        if (f.ppField[0][x] == f.ppField[1][x] && f.ppField[0][x] == f.ppField[2][x])
        {
            if (f.ppField[0][x] == f.human)
            {
                return WON_HUMAN;
            }
            else if (f.ppField[0][x] == f.ai)
            {
                return WON_AI;
            }
        }
    }

    // Диогональ 1
    if (f.ppField[0][0] == f.ppField[1][1] && f.ppField[0][0] == f.ppField[2][2])
    {
        if (f.ppField[1][1] == f.human)
        {
            return WON_HUMAN;
        }
        else if (f.ppField[1][1] == f.ai)
        {
            return WON_AI;
        }
    }

    // Диогональ 2
    if (f.ppField[2][0] == f.ppField[1][1] && f.ppField[2][0] == f.ppField[0][2])
    {
        if (f.ppField[1][1] == f.human)
        {
            return WON_HUMAN;
        }
        else if (f.ppField[1][1] == f.ai)
        {
            return WON_AI;
        }
    }

    // Ничья
    bool draw = true;
    for (size_t y = 0; y < f.size; y++)
    {
        for (size_t x = 0; x < f.size; x++)
        {
            if (f.ppField[y][x] == EMPTY)
            {
                draw = false;
                break;
            }
        }

        if (!draw)
        {
            break;
        }
    }

    if (draw)
    {
        return DRAW;
    }

    return IN_PROGRESS;
}

Coord GetAICoord(Field& f)
{
    Coord c{0};

    for (size_t y = 0; y < f.size; y++)
    {
        for (size_t x = 0; x < f.size; x++)
        {
            if (f.ppField[y][x] == EMPTY)
            {
                f.ppField[y][x] == f.ai;
                if (isWon(f) == WON_AI)
                {
                    f.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                f.ppField[y][x] == EMPTY;
            }
        }
    }

    for (size_t y = 0; y < f.size; y++)
    {
        for (size_t x = 0; x < f.size; x++)
        {
            if (f.ppField[y][x] == EMPTY)
            {
                f.ppField[y][x] == f.human;
                if (isWon(f) == WON_HUMAN)
                {
                    f.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                f.ppField[y][x] == EMPTY;
            }
        }
    }

    // Центр
    if (f.ppField[1][1] == EMPTY)
    {
        return { 1, 1 };
    }
    
    Coord buf[4];
    size_t num = 0;

    // Углы
    if (f.ppField[0][0] == EMPTY)
    {
        buf[num] = { 0, 0 };
        num++;
    }
    if (f.ppField[2][2] == EMPTY)
    {
        buf[num] = { 2, 2 };
        num++;
    }
    if (f.ppField[0][2] == EMPTY)
    {
        buf[num] = { 0, 2 };
        num++;
    }
    if (f.ppField[2][0] == EMPTY)
    {
        buf[num] = { 2, 0 };
        num++;
    }
    if (num != 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }

    num = 0;

    if (f.ppField[0][1] == EMPTY)
    {
        buf[num] = { 0, 1 };
        num++;
    }
    if (f.ppField[2][1] == EMPTY)
    {
        buf[num] = { 2, 1 };
        num++;
    }
    if (f.ppField[1][0] == EMPTY)
    {
        buf[num] = { 1, 0 };
        num++;
    }
    if (f.ppField[1][2] == EMPTY)
    {
        buf[num] = { 1, 2 };
        num++;
    }
    if (num != 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }

    return c;
}

void Congrats(Field & f)
{
    cout << endl;
    if (f.progress == WON_HUMAN)
    {
        cout << " Human won!" << endl;
    }
    else if (f.progress == WON_AI)
    {
        cout << " Computer won!" << endl;
    }
    else if (f.progress == DRAW)
    {
        cout << " Draw" << endl;
    }
}

//====================================================================

int main()
{
    Field f;
    InitGame(f);

    ClearScr();
    DrawField(f);

    do {

        if (f.turn % 2 == 0)
        {
            Coord c = GetHumanCoord(f);
            f.ppField[c.y][c.x] = f.human;

        }
        else
        {
            Coord c = GetAICoord(f);
            f.ppField[c.y][c.x] = f.ai;
        }
        
        f.turn++;
        ClearScr();
        DrawField(f);

        f.progress = isWon(f);
                
    } while (f.progress == IN_PROGRESS);

    Congrats(f);

    DeinitGame(f);
           
    return 0;
}

//====================================================================