
#if !defined RT_DEFS
#define RT_DEFS

#define BOARD_WIDTH 16
#define BOARD_HEIGHT 8

#define E ' ' // Empty field
#define W 'W' // Wall
#define I 'I' // Ice Box
#define C 'C' // Crystal

typedef const char* BoardType;

#define BOARD_TO_COORD(x) (x << 3)

#define BOARD_ITEM(board, x, y) (*(board + BOARD_WIDTH*y + x))

#define DIRECTION_NORTH 'N'
#define DIRECTION_SOUTH 'S'
#define DIRECTION_WEST 'W'
#define DIRECTION_EAST 'E'

#define GAME_RUNNING 0
#define GAME_OUT_OF_MOVES 1
#define GAME_FAIL 2
#define GAME_SUCCESS 3

#define IDLE 'N'
#define FORWARD 'F'
#define LEFT 'L'
#define RIGHT 'R'


#endif // RT_DEFS