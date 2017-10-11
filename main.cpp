#include <iostream>
#include <stdio.h>
#include <stdlib.h>   
#include <curses.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>§
#include <thread>
using namespace std;

// Global variables
bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

void Setup() {
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}

void Draw() {
	system("clear");
	for (int i = 0; i < width + 2; i++) {
		cout << "#";
	}
	cout << endl;

	for (int i = 0; i < height; i ++) {
		for (int j = 0; j < width; j++) {
			if (j == 0) {
				cout << "#";
			}
			if (i == y && j == x) {
				cout << "O";
			} else if (i == fruitY && j == fruitX) {
				cout << "F";
			} else {
				bool print = false;
				for (int k = 0; k < nTail; k++) {
					if (tailX[k] == j && tailY[k] == i) {
						cout << "o";
						print = true;
					}
				}
				if (!print) {
					cout << " ";
				}
			}
			if (j == width - 1) {
				cout << "#";
			}
		}
		cout << endl;
	}

	for (int i = 0; i < width + 2; i++) {
		cout << "#";
	}
	cout << endl;
	cout << "Score: " << score << endl;
}

int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}

void Input() {
	if (kbhit()) {
		switch(getchar()) {
			case 'a' :
				dir = LEFT;
				break;
			case 'd' :
				dir = RIGHT;
				break;
			case 'w' :
				dir = UP;
				break;
			case 's' :
				dir = DOWN;
				break;
			case 'x' :
			gameOver = true;
			break;
		}
	}
}

void Logic() {
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++) {
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch (dir) {
		case LEFT :
			x--;
			break;
		case RIGHT :
			x++;
			break;
		case UP :
			y--;
			break;
		case DOWN :
			y++;
			break;
		default :
			break;
	}
	if (x >= width) x = 0; else if (x < 0) x = width - 1;
	if (y >= height) y = 0; else if (y < 0) y = height - 1;
	for (int i = 0; i < nTail; i++) {
		if (tailX[i] == x && tailY[i] == y) {
			gameOver = true;
		}
	}
	if (x == fruitX && y == fruitY) {
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}
}

int controlls() {
	while (!gameOver) {
		Input();
	}
	return 0;
}

int main() {
	std::thread parallelThread(controlls);
	parallelThread.detach();

	Setup();
	while (!gameOver) {
		Draw();
		Logic();
		usleep(80000);
	} 
	return 0;
}
