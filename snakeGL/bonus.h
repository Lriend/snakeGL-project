#pragma once
#include <deque>
#include "model.h"

class Bonus {
public:
	virtual void affect(int &goldenApple, float &invertFor, float &fastFor, float &slowFor, std::deque<Model*> &tail, unsigned &score)=0;
	virtual void reset(int &goldenApple, float &invertFor, float &fastFor, float &slowFor) {
		goldenApple = 0;
		invertFor = 0.f;
		slowFor = 0.f;
		fastFor = 0.f;
	};
};

//----------------------------------------------------------------------------------------------

class Bunny : public Bonus {
public:
	void affect(int &goldenApple, float &invertFor, float &fastFor, float &slowFor, std::deque<Model*> &tail, unsigned &score) {
		fastFor = 5.f;
		score += 15;
	};
};

class Turtle : public Bonus {
public:
	void affect(int &goldenApple, float &invertFor, float &fastFor, float &slowFor, std::deque<Model*> &tail, unsigned &score) {
		slowFor = 5.f;
		score += 5;
	};
};

class Shroom : public Bonus {
public:
	void affect(int &goldenApple, float &invertFor, float &fastFor, float &slowFor, std::deque<Model*> &tail, unsigned &score) {
		invertFor = 2.5f;
		score += 20;
	};
};

class Apple : public Bonus {
public:
	void affect(int &goldenApple, float &invertFor, float &fastFor, float &slowFor, std::deque<Model*> &tail, unsigned &score) {
		goldenApple = 5;
		score += 10;
	};
};

class Bomb : public Bonus {
public:
	void affect(int &goldenApple, float &invertFor, float &fastFor, float &slowFor, std::deque<Model*> &tail, unsigned &score) {
		if (tail.size() > 1) {
			int tailToCut = tail.size() / 2;
			for (int i = 0; i < tailToCut; i++) tail.pop_front();
		}
		score += 3;
	};
};