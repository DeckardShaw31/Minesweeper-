#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <chrono>
#include <windows.h>

using namespace std;

#define max_mine 99
#define max_side 25
#define max_move 526

int kichthuoc;
int mine;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clear() {
    system("cls");
}

bool phuhop(int hang, int cot) {
    return (hang >= 0) && (hang < kichthuoc) && (cot >= 0) && (cot < kichthuoc);
}

bool lamin(int hang, int cot, char board[][max_side]) {
    return (board[hang][cot] == '*');
}

void make_move(int *x, int *y) {
    while (true) {
        cout << "\nChon nuoc di cua ban: [hang] [cot] -> ";
        cin >> *x >> *y;
        if ((*x < kichthuoc) && (*y < kichthuoc)) {
            return;
        }
    }
}

void printboard(char bangcuatoi[][max_side]) {
    clear();
    cout << "\n\n\t\t\t    ";

    for (int i = 0; i < kichthuoc; i++) {
        if (i > 9)
            cout << i / 10 << " ";
        else
            cout << "  ";
    }

    cout << "\n\t\t\t    ";

    for (int i = 0; i < kichthuoc; i++)
        cout << i % 10 << " ";

    cout << "\n\n";

    for (int i = 0; i < kichthuoc; i++) {
        cout << "\t\t\t    ";
        for (int j = 0; j < kichthuoc; j++) {
            if (bangcuatoi[i][j] == '*') {
                setColor(14); // mìn màu vàng
                cout << bangcuatoi[i][j] << " ";
                setColor(15); // đen thành trắng
            } else if (isdigit(bangcuatoi[i][j])) {
                int digit = bangcuatoi[i][j] - '0';
                if (digit == 1) {
                    setColor(10); // 1 là xanh lá
                } else if (digit == 2) {
                    setColor(9); // xanh dương là 2
                } else if (digit == 3) {
                    setColor(12); // đỏ là 3
                }
                cout << bangcuatoi[i][j] << " ";
                setColor(15); // đen thành trắng
            } else {
                cout << bangcuatoi[i][j] << " ";
            }
        }
        cout << " " << i << "\n";
    }
    return;
}


int countadjacent(int hang, int cot, int min[][2], char bangthat[][max_side]) {
    int count = 0;

    if (phuhop(hang - 1, cot) && lamin(hang - 1, cot, bangthat))
        count++;

    if (phuhop(hang + 1, cot) && lamin(hang + 1, cot, bangthat))
        count++;

    if (phuhop(hang, cot + 1) && lamin(hang, cot + 1, bangthat))
        count++;

    if (phuhop(hang, cot - 1) && lamin(hang, cot - 1, bangthat))
        count++;

    if (phuhop(hang - 1, cot - 1) && lamin(hang - 1, cot - 1, bangthat))
        count++;

    if (phuhop(hang - 1, cot + 1) && lamin(hang - 1, cot + 1, bangthat))
        count++;

    if (phuhop(hang + 1, cot - 1) && lamin(hang + 1, cot - 1, bangthat))
        count++;

    if (phuhop(hang + 1, cot + 1) && lamin(hang + 1, cot + 1, bangthat))
        count++;

    return count;
}

bool playminesuntil(char bangcuatoi[][max_side], char bangthat[][max_side], int min[][2], int hang, int cot, int *moves_left) {
    if (bangcuatoi[hang][cot] != '-') {
        return false;
    }

    int i, j;
    if (bangthat[hang][cot] == '*') {
        bangcuatoi[hang][cot] = '*';
        for (i = 0; i < mine; i++)
            bangcuatoi[min[i][0]][min[i][1]] = '*';

        printboard(bangcuatoi);
        cout << "\nBan thua roi !\n";

        this_thread::sleep_for(chrono::seconds(10));

        return true;
    } else {
        int count = countadjacent(hang, cot, min, bangthat);
        (*moves_left)--;

        bangcuatoi[hang][cot] = count + '0';

        if (!count) {
            if (phuhop(hang - 1, cot) && !lamin(hang - 1, cot, bangthat))
                playminesuntil(bangcuatoi, bangthat, min, hang - 1, cot, moves_left);

            if (phuhop(hang + 1, cot) && !lamin(hang + 1, cot, bangthat))
                playminesuntil(bangcuatoi, bangthat, min, hang + 1, cot, moves_left);

            if (phuhop(hang, cot + 1) && !lamin(hang, cot + 1, bangthat))
                playminesuntil(bangcuatoi, bangthat, min, hang, cot + 1, moves_left);

            if (phuhop(hang, cot - 1) && !lamin(hang, cot - 1, bangthat))
                playminesuntil(bangcuatoi, bangthat, min, hang, cot - 1, moves_left);

            if (phuhop(hang - 1, cot + 1) && !lamin(hang - 1, cot + 1, bangthat))
                playminesuntil(bangcuatoi, bangthat, min, hang - 1, cot + 1, moves_left);

            if (phuhop(hang - 1, cot - 1) && !lamin(hang - 1, cot - 1, bangthat))
                playminesuntil(bangcuatoi, bangthat, min, hang - 1, cot - 1, moves_left);

            if (phuhop(hang + 1, cot + 1) && !lamin(hang + 1, cot + 1, bangthat))
                playminesuntil(bangcuatoi, bangthat, min, hang + 1, cot + 1, moves_left);

            if (phuhop(hang + 1, cot - 1) && !lamin(hang + 1, cot - 1, bangthat))
                playminesuntil(bangcuatoi, bangthat, min, hang + 1, cot - 1, moves_left);
        }
        return false;
    }
}

void placemines(int min[][2], char bangthat[][max_side]) {
    bool mark[max_side * max_side];

    memset(mark, false, sizeof(mark));

    for (int i = 0; i < mine;) {
        int random = rand() % (kichthuoc * kichthuoc);
        int x = random / kichthuoc;
        int y = random % kichthuoc;

        if (!mark[random]) {
            min[i][0] = x;
            min[i][1] = y;

            bangthat[min[i][0]][min[i][1]] = '*';
            mark[random] = true;
            i++;
        }
    }
}

void initialise(char bangthat[][max_side], char bangcuatoi[][max_side]) {
    srand(time(NULL));

    int i, j;
    for (i = 0; i < kichthuoc; i++)
        for (j = 0; j < kichthuoc; j++) {
            bangcuatoi[i][j] = bangthat[i][j] = '-';
        }
    return;
}

void cheatmines(char bangthat[][max_side]) {
    cout << "Vi tri cua min la: \n";
    printboard(bangthat);
    return;
}

void thaydoimin(int hang, int cot, char board[][max_side]) {
    for (int i = 0; i < kichthuoc; i++) {
        for (int j = 0; j < kichthuoc; j++) {
            if (board[i][j] != '*') {
                board[i][j] = '*';
                board[hang][cot] = '-';
                return;
            }
        }
    }
}

void play() {
    bool gameover = false;

    char bangthat[max_side][max_side], bangcuatoi[max_side][max_side];

    int moves_left = kichthuoc * kichthuoc - mine;
    int x, y;
    int min[max_mine][2];

    initialise(bangthat, bangcuatoi);

    placemines(min, bangthat);

    int currentmoveindex = 0;

    while (gameover == false) {
        cout << "Bang hien tai : \n";
        printboard(bangcuatoi);
        make_move(&x, &y);

        if (currentmoveindex == 0) {
            if (lamin(x, y, bangthat))
                thaydoimin(x, y, bangthat);
        }

        currentmoveindex++;

        gameover = playminesuntil(bangcuatoi, bangthat, min, x, y, &moves_left);

        if ((gameover == false) && (moves_left == 0)) {
            cout << "\nBan thang roi !\n";
            gameover = true;
        }
    }
}

void dokho() {
    clear();
    cout << "\n\t\t\t\t\t\t\t\tDo min";
    cout << "\n\n\t\t\t\t\t\tChon do kho : ";
    cout << "\n\n\t\t\t\t\t\t0.De\n\t\t\t\t\t\t1.Vua\n\t\t\t\t\t\t2.Kho";
    cout << "\n\n\t\t\t\t\t\tChon (0 - 2) : ";
    int choice;
    cin >> choice;
    if (choice == 0) {
        kichthuoc = 9;
        mine = 10;
    } else if (choice == 1) {
        kichthuoc = 16;
        mine = 40;
    } else if (choice == 2) {
        kichthuoc = 24;
        mine = 99;
    } else
        exit(0);
}

int main() {
    dokho();
    play();
    return 0;
}
