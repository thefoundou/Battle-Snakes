#ifndef H_BOARD
#define H_BOARD

#include <iostream>
#include <windows.h>
#include <random>
using namespace std;

static random_device rd;
static default_random_engine generator(rd());

int randInt(int lowest, int highest);

void SetColor(int value) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value);
}

class Board
{
public:
	Board() {
		//SetColor(6);
		for (int i = 0; i <= 69; i++) {
			for (int j = 0; j <= 17; j++) {
				m_grid[j][i] = '#';
			}

		}
	//	SetColor(15);
		for (int i = 1; i < 69; i++) {
			for (int j = 1; j < 17; j++) {
				m_grid[j][i] = ' ';
			}
		}

		// SET PORTAL SPACES

		m_grid[9][0] = ' ';
		m_grid[8][0] = '-';
		m_grid[10][0] = '-';

		m_grid[9][69] = ' ';
		m_grid[8][69] = '-';
		m_grid[10][69] = '-';

		m_grid[0][34] = ' ';
		m_grid[0][33] = '|';
		m_grid[0][35] = '|';

		m_grid[17][34] = ' ';
		m_grid[17][33] = '|';
		m_grid[17][35] = '|';
	}

	void resetBoard() {
		for (int i = 0; i <= 69; i++) {
			for (int j = 0; j <= 17; j++) {
				m_grid[j][i] = '#';
			}

		}

		for (int i = 1; i < 69; i++) {
			for (int j = 1; j < 17; j++) {
				m_grid[j][i] = ' ';
			}
		}

		// SET PORTAL SPACES

		m_grid[9][0] = ' ';
		m_grid[8][0] = '-';
		m_grid[10][0] = '-';

		m_grid[9][69] = ' ';
		m_grid[8][69] = '-';
		m_grid[10][69] = '-';

		m_grid[0][34] = ' ';
		m_grid[0][33] = '|';
		m_grid[0][35] = '|';

		m_grid[17][34] = ' ';
		m_grid[17][33] = '|';
		m_grid[17][35] = '|';
	}


	void display(char effects, int freeze, int attack) {
		for (int i = 0; i <= 17; i++) {
			for (int j = 0; j <= 69; j++) {
				char a = m_grid[i][j];
				switch (a) {
				case 'A':
					SetColor(12);
					cout << m_grid[i][j];
					SetColor(15);
					break;
				case 'F':
					SetColor(11);
					cout << m_grid[i][j];
					SetColor(15);
					break;
				case '@':
				case '>':
				case '<':
				case 'v':
				case '^':
					SetColor(10);
					cout << m_grid[i][j];
					SetColor(15);
					break;
				case '*':
				case '0':

					switch (effects) {
					case 'a':
						if (freeze % 2 == 0) {
							SetColor(14);
							cout << m_grid[i][j];
							SetColor(15);
						}
						else {
							cout << ' ';
						}
						break;

					case 'b':
						SetColor(15);
						cout << m_grid[i][j];
						SetColor(15);
						break;

					case 'c':

						if (freeze % 2 == 0) {
							SetColor(15);
							cout << m_grid[i][j];
						}
						else {
							cout << ' ';
						}
						break;
				
					default:

						SetColor(14);
						cout << m_grid[i][j];
						SetColor(15);
						break;
					}
					break;
					
				default:
					cout << m_grid[i][j];
					break;
				}
				
			}
			cout << endl;
		}
	}

	void updateGrid(int x, int y, char a) {
		m_grid[y][x] = a;
	}

	char getPos(int x, int y) {
		return m_grid[y][x];
	}

	void addFood() {
		int x = randInt(1, 68);
		int y = randInt(1, 17);

		while (m_grid[y][x] == '#' || 
			m_grid[y][x] == '-' ||
			m_grid[y][x] == '|' ||
			m_grid[y][x] == '@') {
			x = randInt(1, 68);
			y = randInt(1, 17);
		}

		m_grid[y][x] = 'A';

		m_food = Food(x, y);
	}

	void addFreeze() {
		int x = randInt(1, 68);
		int y = randInt(1, 17);

		while (m_grid[y][x] == '#' ||
			m_grid[y][x] == '-' ||
			m_grid[y][x] == '|' ||
			m_grid[y][x] == 'A' ||
			m_grid[y][x] == '@') {
			x = randInt(1, 68);
			y = randInt(1, 17);
		}

		m_grid[y][x] = 'F';

		m_freeze = new Food(x, y);
	}


	void addAttack() {
		int x = randInt(1, 68);
		int y = randInt(1, 17);

		while (m_grid[y][x] == '#' ||
			m_grid[y][x] == '-' ||
			m_grid[y][x] == '|' ||
			m_grid[y][x] == 'A' ||
			m_grid[y][x] == 'F' ||
			m_grid[y][x] == '@') {
			x = randInt(1, 68);
			y = randInt(1, 17);
		}

		m_grid[y][x] = '+';

		m_attack = new Food(x, y);
	}

	void deleteFreeze() {
		m_grid[m_freeze->y][m_freeze->x] = ' ';
		delete m_freeze;
	}

	void deleteAttack() {
		m_grid[m_attack->y][m_attack->x] = ' ';
		delete m_attack;
	}

	bool checkFood(int x, int y) {
		if (m_food.x == x && m_food.y == y) {
			addFood();
			
			return true;
		}
		return false;
	}

	bool checkFreeze(int x, int y) {
		if (m_freeze->x == x && m_freeze->y == y) {
			return true;
		}
		return false;
	}

	bool checkAttack(int x, int y) {
		if (m_attack->x == x && m_attack->y == y) {
			return true;
		}
		return false;
	}

	int getFoodx() { return m_food.x; }
	int getFoody() { return m_food.y; }

	int getFreezex() { return m_freeze->x; }
	int getFreezey() { return m_freeze->y; }

	int getAttackx() { return m_attack->x; }
	int getAttacky() { return m_attack->y; }


private:
	char m_grid[18][70];

	struct Food
	{
		int x;
		int y;

	public:
		Food(int x = 5, int y = 10)
			:x(x),y(y)	{}
	};

	Food m_food;
	Food* m_freeze;
	Food* m_attack;
};

int randInt(int lowest, int highest)
{
	if (highest < lowest)
		swap(highest, lowest);
	uniform_int_distribution<> distro(lowest, highest);
	return distro(generator);
}








#endif // !H_BOARD

