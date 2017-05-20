
#include "memefield.h"
#include <assert.h>
#include "Vei2.h"
#include "SpriteCodex.h"
#include <random>


MemeField::MemeField(int nMemes){
	assert(nMemes>0&&nMemes<width*height);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int>xDist(0,width-1);
	std::uniform_int_distribution<int>yDist(0,height-1);
	xStart=(Graphics::ScreenWidth/2)-((SpriteCodex::tileSize*width)/2);
	yStart=(Graphics::ScreenHeight/2)-((SpriteCodex::tileSize*height)/2);
	for(int nSpaw=0; nSpaw<nMemes; nSpaw++){
		Vei2 spawnPos;
		do{
			spawnPos={xDist(rng),yDist(rng)};
		} while(TileAt(spawnPos).HasMeme());
		TileAt(spawnPos).spawnMeme();
	}
}

void MemeField::Draw(Graphics & gfx) const{
	gfx.DrawRect(xStart,yStart,xStart+SpriteCodex::tileSize*width,
			   yStart+SpriteCodex::tileSize*height,SpriteCodex::baseColor);
	for(Vei2 gridPos={0,0}; gridPos.y<height; gridPos.y++){
		for(gridPos.x=0; gridPos.x<width; gridPos.x++){
			TileAt(gridPos).Draw(Vei2(xStart+gridPos.x*SpriteCodex::tileSize,yStart+gridPos.y*SpriteCodex::tileSize),gfx);
		}
	}
}

bool MemeField::checkWin(){
	bool checkAll=true;
	for(int iY=0; iY<height; iY++){
		for(int iX=0;iX<width;iX++){
			if(field[iX][iY].getState()==Tile::State::Hidden||
			   field[iX][iY].getState()==Tile::State::Flagged){
				checkAll=field[iX][iY].HasMeme()&&checkAll;
			}
		}
	}
	return checkAll;
}

void MemeField::revealAll(){
	for(int iX=0; iX<width; iX++){
		for(int iY=0; iY<height; iY++){
			if(field[iX][iY].HasMeme()){
				field[iX][iY].reveal();
			}
			if(!field[iX][iY].HasMeme()&&field[iX][iY].getState()==Tile::State::Flagged){
				field[iX][iY].falseFlag=true;
			}
		}
	}
}

void MemeField::Tile::Draw(const Vei2& screenPos,Graphics& gfx)const{
	switch(state){
		case State::Hidden:
			SpriteCodex::DrawTileButton(screenPos,gfx); 
			break;
		case State::Flagged:
			SpriteCodex::DrawTileButton(screenPos,gfx);
			SpriteCodex::DrawTileFlag(screenPos,gfx);
			if(falseFlag){
				SpriteCodex::DrawTileCross(screenPos,gfx);
			}
			break;
		case State::Revealed:
			if(!hasMeme){
				drawMemeCount(memeNum,screenPos,gfx);
			} else{
				SpriteCodex::DrawTileBomb(screenPos,gfx);
				if(deathLocation){
					SpriteCodex::DrawTileCross(screenPos,gfx);
				}
			} 
			break;
	}
}

void MemeField::Tile::spawnMeme(){
	assert(!hasMeme);
	hasMeme=true;
}

bool MemeField::Tile::HasMeme()const{
	return hasMeme;
}

void MemeField::Tile::reveal(){
	state=State::Revealed;
}

void MemeField::Tile::drawMemeCount(int const memes,const Vei2& screenPos,Graphics& gfx)const{
	switch(memes){
		case 0:
			SpriteCodex::DrawTile0(screenPos,gfx);
			break;
		case 1:
			SpriteCodex::DrawTile1(screenPos,gfx);
			break;
		case 2:
			SpriteCodex::DrawTile2(screenPos,gfx);
			break;
		case 3:
			SpriteCodex::DrawTile3(screenPos,gfx);
			break;
		case 4:
			SpriteCodex::DrawTile4(screenPos,gfx);
			break;
		case 5:
			SpriteCodex::DrawTile5(screenPos,gfx);
			break;
		case 6:
			SpriteCodex::DrawTile6(screenPos,gfx);
			break;
		case 7:
			SpriteCodex::DrawTile7(screenPos,gfx);
			break;
		case 8:
			SpriteCodex::DrawTile8(screenPos,gfx);
			break;
	}
}

MemeField::Tile::State MemeField::Tile::getState() const{
	return state;
}

void MemeField::plantFlag(const Vei2 & mousePos){
	Vei2 gridPos((mousePos.x-xStart)/SpriteCodex::tileSize,(mousePos.y-yStart)/SpriteCodex::tileSize);
	if(TileAt(gridPos).getState()==Tile::State::Hidden){
		TileAt(gridPos).plantFlag();
	} else if(TileAt(gridPos).getState()==Tile::State::Flagged){
		TileAt(gridPos).captureFlag();
	}

}

void MemeField::Tile::plantFlag(){
	state=State::Flagged;
}

void MemeField::Tile::captureFlag(){
	state=State::Hidden;
}

bool MemeField::isMeme(const Vei2& mousePos){
	if(mousePos.x>=xStart && mousePos.y>=yStart && mousePos.x<(xStart+(width*SpriteCodex::tileSize))&&mousePos.y<(yStart+(height*SpriteCodex::tileSize))){
		Vei2 gridPos((mousePos.x-xStart)/SpriteCodex::tileSize,(mousePos.y-yStart)/SpriteCodex::tileSize);
		if(TileAt(gridPos).getState()==Tile::State::Hidden)
		internalSpread(gridPos);
		TileAt(gridPos).deathLocation=TileAt(gridPos).HasMeme();
		return TileAt(gridPos).HasMeme();
	}
	return false;
}

int MemeField::neighborBombs(const Vei2& gridPos)const{
	int number=0;
	if(field[gridPos.x-1][gridPos.y-1].HasMeme()&&
	   gridPos.x-1>=0&&gridPos.y-1>=0){
		number++;
	}
	if(field[gridPos.x][gridPos.y-1].HasMeme()&&gridPos.y-1>=0){
		number++;
	}
	if(field[gridPos.x+1][gridPos.y-1].HasMeme()&&
	   gridPos.y-1>=0&&gridPos.x+1<width){
		number++;
	}
	if(field[gridPos.x-1][gridPos.y].HasMeme()&&
	   gridPos.x-1>=0){
		number++;
	}
	if(field[gridPos.x+1][gridPos.y].HasMeme()&&
	   gridPos.x+1<width){
		number++;
	}
	if(field[gridPos.x-1][gridPos.y+1].HasMeme()&&
	   gridPos.x-1>=0&&gridPos.y+1<height){
		number++;
	}
	if(field[gridPos.x][gridPos.y+1].HasMeme()&&
	   gridPos.y+1<height){
		number++;
	}
	if(field[gridPos.x+1][gridPos.y+1].HasMeme()&&
	   gridPos.y+1<height&&gridPos.x+1<width){
		number++;
	}	
	return number;
}

MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos){
	return field[gridPos.x][gridPos.y];
}

const MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos)const{
	return field[gridPos.x][gridPos.y];
}

void MemeField::internalSpread(const Vei2 & gridPos){
	TileAt(gridPos).memeNum=neighborBombs(gridPos);
	TileAt(gridPos).reveal();
	Vei2 selGridStart(0,0);
	Vei2 selGridEnd(0,0);

	if(neighborBombs(gridPos)==0){
		if(gridPos.x-1<0){
			selGridStart.x=0;
		} else{
			selGridStart.x=gridPos.x-1;
		}
		if(gridPos.y-1<0){
			selGridStart.y=0;
		} else{
			selGridStart.y=gridPos.y-1;
		}
		if(gridPos.x+2>width){
			selGridEnd.x=width-1;
		} else{
			selGridEnd.x=gridPos.x+1;
		}
		if(gridPos.y+2>height){
			selGridEnd.y=height-1;
		} else{
			selGridEnd.y=gridPos.y+1;
		}
		for(int iX=selGridStart.x; iX<=selGridEnd.x; iX++){
			for(int iY=selGridStart.y; iY<=selGridEnd.y; iY++){
				if(field[iX][iY].getState()==Tile::State::Hidden){
					internalSpread(Vei2(iX,iY));
				}
			}
		}
	}
}
