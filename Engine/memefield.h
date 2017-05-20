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
		enum class State{
			Hidden,
			Flagged,
			Revealed
		};
	private: State state=State::Hidden;
	};
	static constexpr int width=20;
	static constexpr int height=16;
	Tile field[width][height];
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
};
