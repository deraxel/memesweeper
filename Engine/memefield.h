#pragma once
#include "Graphics.h"

class MemeField{
private:
	class Tile{
	private:
		bool hasMeme=false;
	public:
		void Draw(const Vei2& screenPos,Graphics& gfx) const;
		void spawnMeme();
		bool HasMeme() const;
		void reveal();
		void plantFlag();
		void captureFlag();
		int memeNum=0;
		bool deathLocation=false;
		bool falseFlag=false;
		void drawMemeCount(int const memes,const Vei2 & screenPos,Graphics& gfx) const;
		enum class State{
			Hidden,
			Flagged,
			Revealed
		};
		State getState()const;
	private: State state=State::Hidden;
	};
	int xStart;
	int yStart;
	static constexpr int width=40;
	static constexpr int height=32;
	Tile field[width][height];
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	void internalSpread(const Vei2& gridPos);
public:
	void revealAll();
	void plantFlag(const Vei2& mousePos);
	bool isMeme(const Vei2& mousePos);
	int neighborBombs(const Vei2 & gridPos) const;
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
	bool checkWin();
	bool gameOverWin=false;

};
