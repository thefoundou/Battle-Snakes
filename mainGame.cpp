#include "board.h"
#include "snake.h"
using namespace std;

const char ARROW_LEFT = 'h';
const char ARROW_RIGHT = 'l';
const char ARROW_UP = 'k';
const char ARROW_DOWN = 'j';

void slow(int a);
void move(Snake& s, char q);
char findDir(int x, int y, Snake e, Board game);
bool check(int x, int y, Board g);
bool checkForEnemy(int x, int y, Board g);

int main() {
	SetColor(15);
	cout << "***********BATTLE SNAKES***********" << endl;
	cout << "a. Start Game" << endl;
	cout << "b. Instructions" << endl;

	char a = ' ';
	while (a != 'a' && a != 'b')
		cin >> a;

	if (a == 'b') {
		system("cls");
		cout << "This is your Snake: "; SetColor(10); cout << "@@@@>"; SetColor(15); cout << "." << endl << endl;
		cout << "You control your Snake with the following controls:" << endl;
		cout << "'l': Turn Right" << endl;
		cout << "'j': Turn Left" << endl;
		cout << "'i': Turn Up" << endl;
		cout << "'k': Turn Down" << endl << endl;

		cout << "Like the original Snake game, you will be collecting food for points." << endl;
		cout << "Food shows up on the board as '";
		SetColor(12); cout << "A"; SetColor(15); cout << "'." << endl << endl;

		cout << "Portals appear on the edges of the map between '|'s and '-'s" << endl;
		cout << "Enter a portal to come out on the opposite side of the map" << endl << endl;

		cout << "While collecting food, you will have to avoid the enemy Snake." << endl;
		cout << "The enemy Snake will also be trying to get the food and looks like ";
		SetColor(14); cout << "*0000"; SetColor(15); cout << "." << endl << endl;

		cout << "You can temporarily freeze the enemy Snake by collecting the '"; 
		SetColor(11); cout << 'F'; SetColor(15); cout << "'s that randomly appear." << endl << endl;

		cout << "When the enemy Snake gets big enough, a '+' may appear. If you collect this item" << endl;
		cout << "you will go into battle mode for a short period of time." << endl << endl;


		cout << "In battle mode, if you run into the enemy Snake's tail, it will cut off the tail" << endl;
		cout << "segment from your snake to the end of the tail and the enemy Snake will lose as" << endl;
		cout << "many points as segments you cut off." << endl << endl;

		cout << "These segments will turn into '&' which can be picked up for points without increasing" << endl;
		cout << "your tail length." << endl << endl;

		cout << "The final score will subtract the enemy's points from your points." << endl << endl;
		cout << "Press 'l' start game...";
	

		while (!_kbhit()) {}

	}

	Board game;
	Snake s;
	Snake e(55, 12);
	e.setDir('j');

	char q = 'l';
	char exit = ' ';
	char effects = 'n';
	int freezeTime = 0;
	int attackTime = 0;
	bool hasFreeze = true;
	bool hasAttack = false;

	cout << "SCORE: 0" << endl;
	game.updateGrid(s.getx(), s.gety(), s.getDir());

	game.display('n', 0, 0);

	while (q != 'a') {

		int score = 0;
		int Enscore = 0;
		freezeTime = 0;
		attackTime = 0;
		hasFreeze = true;
		hasAttack = false;

		// Player Snake
		s.setx(5);
		s.sety(5);
		s.setDir('l');
		s.clearBody();
		game.resetBoard();
		q = 'l';

		// Enemy Snake
		e.setx(55);
		e.sety(12);
		e.setDir('j');
		e.clearBody();

		// ADD PICKUP ITEMS
		game.addFood();
		game.addFreeze();

		for (;;) {
			slow(score);

			if (_kbhit())
				q = _getch();

			if (q == 'a')
			{
				cout << "Done" << endl;
				break;
				//cin >> q;
			}

			if (q == 'f') {
				freezeTime = 1000;
				game.addAttack();
				hasAttack = true;
				q = 'l';
			}

			if (!e.tailGone())
				e.tailDisappear(game);

			game.updateGrid(s.getx(), s.gety(), ' ');

			game.updateGrid(e.getx(), e.gety(), ' ');

			// TAIL //
			game.updateGrid(s.getx(), s.gety(), '@');
			s.addCoord();

			if (freezeTime == 0) {
				// Enemy TAIL //
				game.updateGrid(e.getx(), e.gety(), '0');
				e.addCoord();
			}


			if (s.getTailSize() == s.getLength() + 1) {
				if (s.getEndX() == game.getFoodx() && s.getEndY() == game.getFoody())
					game.updateGrid(s.getEndX(), s.getEndY(), 'A');
				else if (hasFreeze == true && s.getEndX() == game.getFreezex() && s.getEndY() == game.getFreezey())
					game.updateGrid(s.getEndX(), s.getEndY(), 'F');
				else if (hasAttack == true && s.getEndX() == game.getAttackx() && s.getEndY() == game.getAttacky())
					game.updateGrid(s.getEndX(), s.getEndY(), '+');
				else
					game.updateGrid(s.getEndX(), s.getEndY(), ' ');
				s.deleteCoord();
			}

			if (e.getTailSize() == e.getLength() + 1) {
				if (e.getEndX() == game.getFoodx() && e.getEndY() == game.getFoody())
					game.updateGrid(e.getEndX(), e.getEndY(), 'A');
				else if (e.getEndX() == game.getFreezex() && e.getEndY() == game.getFreezey())
					game.updateGrid(e.getEndX(), e.getEndY(), 'F');
				else if (hasAttack == true && e.getEndX() == game.getAttackx() && e.getEndY() == game.getAttacky())
					game.updateGrid(e.getEndX(), e.getEndY(), '+');
				else
					game.updateGrid(e.getEndX(), e.getEndY(), ' ');
				e.deleteCoord();
			}

			// MOVE ENEMY SNAKE
			if (freezeTime != 0)
				freezeTime--;
			else {
				move(e, findDir(game.getFoodx(), game.getFoody(), e, game));
			}
			if (attackTime != 0)
				attackTime--;

			// MOVE PLAYER SNAKE
			move(s, q);

			// EATING A FOOD //
			if (game.checkFood(s.getx(), s.gety())) {
				s.addLength();
				score++;
			}

			if (game.checkFood(e.getx(), e.gety())) {
				e.addLength();
				Enscore++;
			}

			// EATING A FREEZE //
			if (game.checkFreeze(s.getx(), s.gety())) {
				// FILL WITH FREEZE MECHANIC
				game.updateGrid(game.getFreezex(), game.getFreezey(), ' ');
				game.deleteFreeze();
				freezeTime = 25;
				hasFreeze = false;
			}

			// RANDOMLY ADDING A FREEZE
			if (hasFreeze == false) {
				int c = randInt(0, 100);
				if (c == 50) {
					game.addFreeze();
					hasFreeze = true;
				}
			}

			// EATING AN ATTACK
			if (hasAttack == true && game.checkAttack(s.getx(), s.gety())) {
				game.updateGrid(game.getAttackx(), game.getAttacky(), ' ');
				game.deleteAttack();
				attackTime = 50;
				hasAttack = false;
			}

			// ADDING AN ATTACK
			if (hasAttack == false) {
				if (e.getTailSize() >= 10) {
					int x = randInt(0, 100 - 2 * (Enscore - 5));
					if (x < 0)
						x = 0;
					if (x == 0) {
						game.addAttack();
						hasAttack = true;
					}
				}
			}

			// DYING BY RUNNING INTO SOMETHING

			if (game.getPos(s.getx(), s.gety()) == '@' ||
				game.getPos(s.getx(), s.gety()) == '#' ||
				game.getPos(s.getx(), s.gety()) == '-' ||
				game.getPos(s.getx(), s.gety()) == '|') {
				exit = 'd';
				break;
			}

			// DIFFERING ATTACK TIME AND NOT ATTACK TIME
			if (attackTime == 0 && game.getPos(s.getx(), s.gety()) == '0') {
				exit = 'd';
				break;
			}

			if (attackTime != 0 && game.getPos(s.getx(), s.gety()) == '0') {
				Enscore = Enscore - e.hurtEnemy(s.getx(), s.gety(), game);
				if (Enscore < 0)
					Enscore = 0;
			}

			if (game.getPos(s.getx(), s.gety()) == '&')
				score++;


			// TYING BY RUNNING INTO HEAD OF OTHER SNAKE
			if (game.getPos(s.getx(), s.gety()) == '*') {
				exit = 't';
				break;
			}

			// Our Snake //
			game.updateGrid(s.getx(), s.gety(), s.getDir());

			// Enemy Snake //
			game.updateGrid(e.getx(), e.gety(), '*');

			system("cls");
			SetColor(15);
			cout << "SCORE: " << score << "					       ENEMY SCORE: " << Enscore << endl;

			// INTERPRET COUNTERS
			if (freezeTime != 0 && attackTime == 0)
				effects = 'a';
			if (attackTime != 0 && freezeTime == 0)
				effects = 'b';
			if (attackTime != 0 && freezeTime != 0)
				effects = 'c';
			if (attackTime == 0 && freezeTime == 0)
				effects = 'n';

			game.display(effects, freezeTime, attackTime);


		}

		system("cls");
		SetColor(12);
		if (score - Enscore < 0)
			cout << "GAME OVER!" << endl;
		if (score - Enscore > 0)
			cout << "YOU WIN!" << endl;
		if (score - Enscore == 0)
			cout << "IT IS A TIE!" << endl;
		SetColor(15);
		cout << "FINAL SCORE: " << score - Enscore << endl;
		cout << "Retry?" << endl << "a: Yes" << endl << "b: No" << endl;
		char a = ' ';
		while (a != 'a' && a != 'b') {
			cin >> a;
		}

		if (a == 'a')
			continue;
		else
			break;

	}

}

char getCharacter()
{
	if (_kbhit()) {
		int c = _getch();
		if (c != 0xE0 && c != 0x00)  // first of the two sent by arrow keys
			return c;
		else
		{
			c = _getch();
			switch (c)
			{
			case 'K':  return ARROW_LEFT;
			case 'M':  return ARROW_RIGHT;
			case 'H':  return ARROW_UP;
			case 'P':  return ARROW_DOWN;
			default:   return '?';
			}
		}
	}
}

void slow(int a) {
	for (int i = 0; i < 3000000 - 150000*a; i++) {

	}
}

void move(Snake& s, char q) {



	// Move Snake
	switch (q) {
	case 'l':
		if (s.getDir() == '<')
			q = 'j';
		break;
	case 'j':
		if (s.getDir() == '>')
			q = 'l';
		break;
	case 'i':
		if (s.getDir() == 'v')
			q = 'k';
		break;
	case 'k':
		if (s.getDir() == '^')
			q = 'i';
		break;
	default:
		break;
	}

	s.setDir(q);

	// MOVING THROUGH HORIZONTAL GATES

	if (s.getDir() == '<' && s.getx() == 0 && s.gety() == 9) {
		s.setx(69);
		s.sety(9);
		return;
	}

	if (s.getDir() == '>' && s.getx() == 69 && s.gety() == 9) {
		s.setx(0);
		s.sety(9);
		return;
	}

	// MOVING THROUGH THE VERTICAL GATES

	if (s.getDir() == '^' && s.getx() == 34 && s.gety() == 0) {
		s.setx(34);
		s.sety(17);
		return;
	}

	if (s.getDir() == 'v' && s.getx() == 34 && s.gety() == 17) {
		s.setx(34);
		s.sety(0);
		return;
	}

	switch (q) {
	case 'i':
		// UP
		s.sety(s.gety() - 1);
		break;
	case 'j':
		// LEFT
		s.setx(s.getx() - 1);
		break;
	case 'l':
		// RIGHT
		s.setx(s.getx() + 1);
		break;
	case 'k':
		// DOWN
		s.sety(s.gety() + 1);
		break;
	}

}

char findDir(int x, int y, Snake e, Board game) {
	int xDist = abs(e.getx() - x);
	int yDist = abs(e.gety() - y);

	int TotDist = xDist + yDist;

	int xDistright = abs((e.getx() + 1) - x);
	int xDistleft = abs((e.getx() - 1) - x);
	int yDistup = abs((e.gety() - 1) - y);
	int yDistdown = abs((e.gety() + 1) - y);

	int TotDistRight = xDistright + yDist;
	int TotDistLeft = xDistleft + yDist;
	int TotDistUp = xDist + yDistup;
	int TotDistDown = xDist + yDistdown;

	if (TotDistRight < TotDist && checkForEnemy(e.getx() + 1, e.gety(), game)) {
		if (game.getPos(e.getx() + 1, e.gety()) == '0') {
			if (checkForEnemy(e.getx(), e.gety() - 1, game)) {
				return 'i';
			}
			if (checkForEnemy(e.getx(), e.gety() + 1, game)) {
				return 'k';
			}
		}
		else
			return 'l';
	}

	if (TotDistLeft < TotDist && checkForEnemy(e.getx() - 1, e.gety(), game)) {
		if (game.getPos(e.getx() - 1, e.gety()) == '0') {
			if (checkForEnemy(e.getx(), e.gety() - 1, game)) {
				return 'i';
			}
			if (checkForEnemy(e.getx(), e.gety() + 1, game)) {
				return 'k';
			}
		}
		else
			return 'j';
	}

	if (TotDistUp < TotDist && checkForEnemy(e.getx(), e.gety() - 1, game)) {
		if (game.getPos(e.getx(), e.gety() - 1) == '0') {
			if (checkForEnemy(e.getx() + 1, e.gety(), game)) {
				return 'l';
			}
			if (checkForEnemy(e.getx() - 1, e.gety(), game)) {
				return 'j';
			}
		}
		else
			return 'i';
	}

	if (TotDistDown < TotDist && checkForEnemy(e.getx(), e.gety() + 1, game)) {
		if (game.getPos(e.getx(), e.gety() + 1) == '0') {
			if (checkForEnemy(e.getx() + 1, e.gety(), game)) {
				return 'l';
			}
			if (checkForEnemy(e.getx() - 1, e.gety(), game)) {
				return 'j';
			}
		}
		else
			return 'k';
	}
	
	return 'a';
}

bool check(int x, int y, Board g) {
	if (g.getPos(x, y) == '#' ||
		g.getPos(x, y) == '@' ||
		g.getPos(x, y) == '0' ||
		g.getPos(x, y) == '>' ||
		g.getPos(x, y) == '<' || 
		g.getPos(x, y) == '^' ||
		g.getPos(x, y) == 'v' ||
		g.getPos(x, y) == '*' )
		return false;
	return true;
}

bool checkForEnemy(int x, int y, Board g) {
	if (g.getPos(x, y) == '#' ||
		g.getPos(x, y) == '@' ||
		g.getPos(x, y) == '>' ||
		g.getPos(x, y) == '<' ||
		g.getPos(x, y) == '^' ||
		g.getPos(x, y) == 'v')
		return false;
	return true;
}