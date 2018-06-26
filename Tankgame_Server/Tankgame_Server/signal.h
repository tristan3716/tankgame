#ifndef _SIGNAL_H
#define _SIGNAL_H

typedef enum _SIGNAL {
	DEFAULT_INFORMATION = 11, PING, MATCHED, JOIN_USER,
	SET_DEFAULT_POSITION = 21,
	TANK_MOVE = 31, SHOT, MOVE_BULLET, EXPLOSION, DESTROY, WINNER,
	CLIENT_ACTIVE_CLOSE = 41
} SIGNAL;

typedef enum _DIRECT { UP=21, DOWN, LEFT, RIGHT, DIE } DIRECT;

#endif _SIGNAL_H