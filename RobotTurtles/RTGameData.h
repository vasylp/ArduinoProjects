
#if !defined RT_GAME_DATA_H
#define RT_GAME_DATA_H

#include "RTDefinitions.h"

static BoardType BOARD = 
	"................"
	"................"
	".....C.........."
	"...W.W.........."
	"WWW...W...W....."
	"................"
	"W..............."
	"..W....W.......W"
;


static const char MOVES[] = {
	RIGHT, FORWARD, LEFT, FORWARD, 
	FORWARD, RIGHT, FORWARD, 
	FORWARD, LEFT, 
	FORWARD, RIGHT, FORWARD, 
	LEFT, FORWARD, FORWARD, 
	RIGHT, FORWARD
};

// Or ti could be written like this:
// static const char MOVES[] = "RFLFFRFFLFRFLFFRF";

#endif // RT_GAME_DATA_H