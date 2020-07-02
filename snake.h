#ifndef H_SNAKE
#define H_SNAKE

#include <conio.h>
#include <queue>
#include <vector>
using namespace std;


class Snake
{
public:
	Snake(int x = 5, int y = 5) 
	{
		m_head.X = x;
		m_head.Y = y;
	}

	int getx() {return m_head.X;}
	int gety() {return m_head.Y;}
	void setx(int x) {m_head.X = x;}
	void sety(int y) {m_head.Y = y;}

	int getLength() { return m_length; }
	void addLength() { m_length++; }
	void setDir(char a)
	{
		if (a == 'i')
			m_dir = '^';
		if (a == 'j')
			m_dir = '<';
		if (a == 'l')
			m_dir = '>';
		if (a == 'k')
			m_dir = 'v';
	}

	char getDir() { return m_dir; }

	int getTailSize() { return m_body.size(); }
	void addCoord() {
		m_body.push(m_head);
	}
	void deleteCoord() {
		m_body.pop();
	}

	int getEndX() { return m_body.front().X; }
	int getEndY() { return m_body.front().Y; }

	void clearBody() {
		while (!m_body.empty()) {
			m_body.pop();
		}
		m_length = 4;
	}

	int hurtEnemy(int x, int y, Board& game) {
		int count = 0;
		while (m_body.front().X != x || m_body.front().Y != y) {
			m_coords.push_back(m_body.front());
			game.updateGrid(m_body.front().X, m_body.front().Y, ' ');
			m_body.pop();
			m_length--;
			count++;
		}
		m_coords.push_back(m_body.front());
		m_body.pop(); 
		m_length--;
		count++;
		return count;
	}

	void tailDisappear(Board& game) {
		m_coords.pop_back();
		vector<COORD>::iterator p = m_coords.begin();
		COORD temp;
		while (p != m_coords.end()) {
			temp = *p;
			game.updateGrid(temp.X, temp.Y, '&');
			p = m_coords.erase(p);
		}
	}

	bool tailGone() {
		if (m_coords.empty())
			return true;
		return false;
	}

private:
	COORD m_head;
	queue<COORD> m_body;
	vector<COORD> m_coords;
	int m_length = 4;
	char m_dir = '>';
	bool m_alive = true;

};



#endif // !H_SNAKE

