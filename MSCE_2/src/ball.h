#include <Pebble.h>
#include "typedefs.h"
#include "ball.c"

void init (ball& b, int active, ball *previous);

void print_ball(ball& b, GContext *ctx);

void clear_ball(ball& b, GContext *ctx);

void update_pos(ball& b);