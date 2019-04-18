// Author Forest Vey
#include "TetrisGame.h"
#include <vector>
#include <iostream>


// This class encapsulates the tetris game and its drawing routines, gameplay, & control logic.
// This class was designed so with the idea of potentially instantiating 2 of them
// and have them run side by side (player vs player).
// So, anything you would need for an individual tetris game has been included here.
// Anything you might use between games (like the background, or the sprite used for 
// rendering a tetromino block) was left in main.cpp
// 
// This class is responsible for:
//   - setting up the board,
//	 - drawing game elements to the screen
//   - spawning tetrominoes,
//   - handling user input,
//   - moving and placing tetrominoes 


	// constructor
	//   assign pointers,
	//   load font from file: fonts/RedOctober.ttf
	//   setup scoreText
	//   reset the game
TetrisGame::TetrisGame(sf::RenderWindow *window, sf::Sprite *blockSprite, Point gameboardOffset, Point nextShapeOffset) {
	pWindow = new sf::RenderWindow;
	pWindow = window;

	pBlockSprite = new sf::Sprite;
	pBlockSprite = blockSprite;

	this->gameboardOffset = gameboardOffset;
	this->nextShapeOffset = nextShapeOffset;

	//TetShape t = Tetromino::getRandomShape(); // For testing a random shape on the gameboard
	//this->currentShape.Tetromino::setShape(t);
	//this->currentShape.setGridLoc(board.getSpawnLoc());

	reset();
	scoreFont.loadFromFile("fonts/RedOctober.ttf");
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(435, 325);
	}


	// destructor, set pointers to null
TetrisGame::~TetrisGame() {
	pWindow = nullptr;
	pBlockSprite = nullptr;
	}

	// draw anything to do with the game,
	// includes board, currentShape, nextShape, score
void TetrisGame::draw() {
	drawGameboard();
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(nextShape, nextShapeOffset);
	pWindow->draw(scoreText);
	}

	// Event and game loop processing
	// handles keypress events (up, left, right, down, space)
void TetrisGame::onKeyPressed(sf::Event event) {
	std::vector<Point> v = this->currentShape.getBlockLocsMappedToGrid();

	switch (event.key.code) {
	case sf::Keyboard::Up:
		if (this->currentShape.getShape() != TetShape::SHAPE_O) {
			attemptRotate(this->currentShape);
		}
		break;
	case sf::Keyboard::Down:
		if (!attemptMove(this->currentShape, 0, 1)) {
			shapePlacedSinceLastGameLoop = true;
			lock(this->currentShape);
		}
		break;
	case sf::Keyboard::Left:
		attemptMove(this->currentShape, -1, 0);
		break;
	case sf::Keyboard::Right:
		attemptMove(this->currentShape, 1, 0);
		break;
	case sf::Keyboard::Space:
		drop(this->currentShape);
		lock(this->currentShape);
		shapePlacedSinceLastGameLoop = true;
		break;
	default:
		break;
	}
	}

	// called every game loop to handle ticks & tetromino placement (locking)
void TetrisGame::processGameLoop(float secondsSinceLastLoop) {
		secondsSinceLastTick += secondsSinceLastLoop;
		if (secondsSinceLastTick > secsPerTick) {
			tick();
			secondsSinceLastTick -= secsPerTick;
		}
		if (shapePlacedSinceLastGameLoop) {
			if (spawnNextShape()) {
				pickNextShape();
				int score = board.removeCompletedRows();
				setScore(score);
				determineSecsPerTick();
			}
			else {
				std::cout << "processGameLoop resetting()" << std::endl;
				this->score = 0;
				reset();
			}
			shapePlacedSinceLastGameLoop = false;
		}
}

	// A tick() forces the currentShape to move (if there were no tick,
	// the currentShape would float in position forever). This should
	// call attemptMove() on the currentShape.  If not successful, lock() 
	// the currentShape (it can move no further), and record the fact that a
	// shape was placed (using shapePlacedSinceLastGameLoop)
void TetrisGame::tick() {
	if (attemptMove(currentShape, 0, 1)) {  }
	else{
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
	}
	}


	// reset everything for a new game (use existing functions) 
	//  - setScore to 0
	//  - determineSecondsPerTick(),
	//  - clear the gameboard,
	//  - pick & spawn next shape
	//  - pick next shape again
void TetrisGame::reset() {
	setScore(0);
	determineSecsPerTick();
	board.empty();
	pickNextShape();
	spawnNextShape();
	pickNextShape();
	}

	// assign nextShape.setShape a new random shape  
void TetrisGame::pickNextShape() {
	nextShape.setShape(Tetromino::getRandomShape());
}


	// copy the nextShape into the currentShape and set 
	//   its loc to be the gameboard's spawn loc.
	//	 - return true/false based on isPositionLegal()
bool TetrisGame::spawnNextShape() {
	currentShape = nextShape;
	currentShape.setGridLoc(board.getSpawnLoc());
	return isPositionLegal(currentShape);
	}



	// test if a rotation is legal on the tetromino, 
	//   if so, rotate it.
	//  To do this:
	//	 1) create a (local) temporary copy of the tetromino
	//	 2) rotate it (shape.rotateCW())
	//	 3) test if temp rotatio was legal (isPositionLegal()), 
	//      if so - rotate the original tetromino.
	//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptRotate(GridTetromino &shape) {
	GridTetromino temp = shape;
	temp.rotateCW();
	if (isPositionLegal(temp)) {
		shape.rotateCW();
		return true;
	}
	else {
		return false;
	}
	}


	// test if a move is legal on the tetromino, if so, move it.
	//  To do this:
	//	 1) create a (local) temporary copy of the current shape
	//	 2) move it (temp.move())
	//	 3) test if temp move was legal (isPositionLegal(),
	//      if so - move the original.
	//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptMove(GridTetromino &shape, int x, int y) {
	GridTetromino temp = shape;
	temp.move(x, y);
	if (isPositionLegal(temp)) {
		shape.move(x, y);
		return true;
	}
	else {
		return false;
	}
	}


	// drops the tetromino vertically as far as it can 
	//   legally go.  Use attemptMove(). This can be done in 1 line.
void TetrisGame::drop(GridTetromino &shape) {
	while (attemptMove(shape, 0, 1)) {}
}

	// copy the contents of the tetromino's mapped block locs to the grid.
	//	 1) get current blockshape locs via tetromino.getBlockLocsMappedToGrid()
	//	 2) iterate on the mapped block locs and copy the contents (color) 
	//      of each to the grid (via gameboard.setGridContent()) 
void TetrisGame::lock(const GridTetromino &shape) {
	std::vector<Point> v = shape.getBlockLocsMappedToGrid();
	for (auto point : v) {
		board.setContent(point.getX(), point.getY(), static_cast<int>(shape.getColor()));
	}
}

	// Graphics methods ==============================================

	// draw a tetris block sprite on the canvas		
	// x,y are meant to be block offsets (not pixels), which means you
	// will need to multiply them by BLOCK_WIDTH & BLOCK_HEIGHT
	//  for help: www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
	//	 1) set the block color using pBlockSprite->setTextureRect()
	//   2) set the block loc using pBlockSprite->setPosition()   
	//	 3) draw the block using Window.draw()
	// (pointers to window and sprite were passed into the constructor)
void TetrisGame::drawBlock(int x, int y, TetColor color, Point origin) {
	int blockColor = static_cast<int>(color);

	pBlockSprite->setTextureRect(sf::IntRect(blockColor * BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT));

	pBlockSprite->setPosition(origin.getX() + (x * BLOCK_WIDTH) ,origin.getY() +  (y * BLOCK_HEIGHT));
	
	pWindow->draw(*pBlockSprite);
}

	// draw the gameboard blocks on the window
	//   iterate through each row & col, use drawBlock() to 
	//   draw a block if it it isn't empty.
void TetrisGame::drawGameboard() {
	for (int y = 0; y < Gameboard::MAX_Y; y++) {
		for (int x = 0; x < Gameboard::MAX_X; x++) {
			int content = board.getContent(x, y);
			if (content != Gameboard::EMPTY_BLOCK) {
				drawBlock(x, y, static_cast<TetColor>(content), gameboardOffset);
			}
		}
	}
	}


	// draw a tetromino on the window
	//	 iterate through each mapped loc & drawBlock() for each.
	//   the origin determines a 'base point' from which to calculate block offsets
	//   If the Tetromino is on the gameboard: use gameboardOffset (otherwise you 
	//   can specify another point as the origin - for the nextShape)
void TetrisGame::drawTetromino(GridTetromino tetromino, Point origin) {
		std::vector<Point> v = tetromino.getBlockLocsMappedToGrid();
		for (auto &point : v) {
			drawBlock(point.getX(), point.getY(), tetromino.getColor(), origin);
		}
	}

	// set the score, update the score display
	// form a string "score: ##" to include the current score
	// user scoreText.setString() to display it.
void TetrisGame::setScore(int score) {
	this->score += score;
	scoreText.setString("score: " + std::to_string(this->score));
	}

	// State & gameplay/logic methods ================================

	// return true if shape is within borders (isShapeWithinBorders())
	//	 and doesn't intersect locked blocks (doesShapeIntersectLockedBlocks)
bool TetrisGame::isPositionLegal(const GridTetromino &shape) {
	if (isShapeWithinBorders(shape) && !doesShapeIntersectLockedBlocks(shape)) {
		return true;
	}
	else {
		return false;
	}
	}

	// return true if the shape is within the left, right,
	//	 and lower border of the grid. (false otherwise)
bool TetrisGame::isShapeWithinBorders(const GridTetromino &shape) {
	std::vector<Point> v = shape.getBlockLocsMappedToGrid();
	for (auto point : v) {
		if (point.getX() < 0 || point.getX() >= Gameboard::MAX_X || point.getY() >= Gameboard::MAX_Y) {
			return false;
		}
	}
	return true;
}

	// return true if the grid content at any of the shape's mapped block locs	
	//   contains anything other than Gameboard::EMPTY_BLOCK. (false otherwise)	
	//   hint Use Gameboard's areLocsEmpty() for this.
bool TetrisGame::doesShapeIntersectLockedBlocks(const GridTetromino &shape) {
	return !board.areLocsEmpty(shape.getBlockLocsMappedToGrid());
	}

	// set secsPerTick 
	//   - basic: use MAX_SECS_PER_TICK
void TetrisGame::determineSecsPerTick() {

	}
