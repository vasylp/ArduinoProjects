#include <U8glib.h>
#include "RTDefinitions.h"
#include "RTGameData.h"
#include "Sprites.h"

U8GLIB_ST7920_128X64_4X u8g(13, 11, 10, U8G_PIN_NONE);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

static char TURTLE_X;
static char TURTLE_Y;
static unsigned char TURTLE_DIRECTION;
static unsigned char TURTLE_MOVE;
static unsigned char GAME_STATUS;

static const unsigned char MOVES_COUNT = sizeof(MOVES) / sizeof(MOVES[0]);

void debugPrint(){
	Serial.print(F("TURTLE: "));
	Serial.print(F("(")); Serial.print((int)TURTLE_X); Serial.print(F(",")); Serial.print((int)TURTLE_Y); Serial.print(") ");
	Serial.print(F("Direction: ")); Serial.print((char)TURTLE_DIRECTION);
	Serial.print(F(" Move: ")); Serial.println((char)MOVES[TURTLE_MOVE]);
}

void setup()
{
	Serial.begin(9600);
	TURTLE_X = 0;
	TURTLE_Y = BOARD_HEIGHT-1;
	TURTLE_DIRECTION = DIRECTION_NORTH;
	TURTLE_MOVE = 0;
	GAME_STATUS = GAME_RUNNING;
}

void loop()
{
	u8g.firstPage();
	do{
		if (GAME_STATUS == GAME_RUNNING){
			drawBoard();
			drawTurtle();
		}
		else{
			drawGameStatus();
		}
	} while (u8g.nextPage());

	moveTurtle();

	delay(1000);
}

void drawBoard(){
	for (char y = 0; y != BOARD_HEIGHT; ++y){
		for (char x = 0; x != BOARD_WIDTH; ++x){
			const u8g_pgm_uint8_t* sprite = NULL;
			switch (BOARD_ITEM(BOARD, x, y)){
			case W:
				sprite = WALL_SPRITE_XBM;
				break;
			case C:
				sprite = CRYSTAL_SPRITE_XBM;
				break;
			default:
				sprite = NULL;
			};
			if (sprite == NULL) continue;
			u8g.drawXBMP(BOARD_TO_COORD(x), BOARD_TO_COORD(y), SPRITE_WIDTH, SPRITE_HEIGHT, sprite);
		}
	}
}

void drawTurtle(){
	const u8g_pgm_uint8_t* sprite = TURTLE_NORTH_SPRITE_XBM;
	switch (TURTLE_DIRECTION){
	case DIRECTION_SOUTH:
		sprite = TURTLE_SOUTH_SPRITE_XBM;
		break;
	case DIRECTION_WEST:
		sprite = TURTLE_WEST_SPRITE_XBM;
		break;
	case DIRECTION_EAST:
		sprite = TURTLE_EAST_SPRITE_XBM;
		break;
	}
	u8g.drawXBMP(BOARD_TO_COORD(TURTLE_X), BOARD_TO_COORD(TURTLE_Y), SPRITE_WIDTH, SPRITE_HEIGHT, sprite);
}

void drawGameStatus(){
	u8g.setFont(u8g_font_unifont);
	switch (GAME_STATUS){
	case GAME_OUT_OF_MOVES:
		u8g.drawStr(16, 30, F("OUT OF MOVES"));
		break;
	case GAME_FAIL:
		u8g.drawStr(24, 30, F("YOU LOOSE"));
		break;
	case GAME_SUCCESS:
		u8g.drawStr(33, 30, F("YOU WIN"));
		break;
	default:
		break;
	}
}

void moveTurtle(){
	if (GAME_STATUS == GAME_RUNNING && TURTLE_MOVE >= MOVES_COUNT) {
		GAME_STATUS = GAME_OUT_OF_MOVES;
		return;
	}
	switch (MOVES[TURTLE_MOVE]){
	case LEFT:
		switch (TURTLE_DIRECTION){
		case DIRECTION_EAST:
			TURTLE_DIRECTION = DIRECTION_NORTH; break;
		case DIRECTION_NORTH:
			TURTLE_DIRECTION = DIRECTION_WEST; break;
		case DIRECTION_WEST:
			TURTLE_DIRECTION = DIRECTION_SOUTH;	break;
		case DIRECTION_SOUTH:
			TURTLE_DIRECTION = DIRECTION_EAST; break;
		}
		break;
	case RIGHT:
		switch (TURTLE_DIRECTION){
		case DIRECTION_EAST:
			TURTLE_DIRECTION = DIRECTION_SOUTH;	break;
		case DIRECTION_SOUTH:
			TURTLE_DIRECTION = DIRECTION_WEST; break;
		case DIRECTION_WEST:
			TURTLE_DIRECTION = DIRECTION_NORTH; break;
		case DIRECTION_NORTH:
			TURTLE_DIRECTION = DIRECTION_EAST; break;
		}
		break;
	case FORWARD:
	{
		char newX = TURTLE_X;
		char newY = TURTLE_Y;
		switch (TURTLE_DIRECTION){
		case DIRECTION_EAST: newX += 1;	break;
		case DIRECTION_SOUTH: newY += 1; break;
		case DIRECTION_WEST: newX -= 1; break;
		case DIRECTION_NORTH: newY -= 1; break;
		};
		if (newX < 0 || newX >= BOARD_WIDTH) {
			GAME_STATUS = GAME_FAIL;
		}
		else if (newY < 0 || newY >= BOARD_HEIGHT) {
			GAME_STATUS = GAME_FAIL;
		}
		else if (BOARD_ITEM(BOARD, newX, newY) == W){
			GAME_STATUS = GAME_FAIL;
		}
		else if (BOARD_ITEM(BOARD, newX, newY) == C){
			GAME_STATUS = GAME_SUCCESS;
		}
		TURTLE_X = newX;
		TURTLE_Y = newY;
	}
				 break;
	};
	TURTLE_MOVE += 1;
}