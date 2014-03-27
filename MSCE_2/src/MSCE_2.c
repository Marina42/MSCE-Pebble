#include <pebble.h>
#include <time.h>
#include <stdlib.h>
#include <stddef.h>

#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 151
#define FPS 60
#define INTERVAL (1000/FPS)

static Window *window;
struct Layer *window_layer;

typedef struct Ball Ball;

struct Ball {
  GPoint pos;
  GPoint speed;
  int radius;
  Ball* next;
  Ball* prev;
};

Ball* front_ball;
Ball* back_ball;

void render (void* callback_data){
  layer_mark_dirty(window_layer);
  app_timer_register (INTERVAL, render, NULL);
}

static void my_layer_draw(Layer *layer, GContext *ctx) {

  Ball *ball = front_ball;
  int i = 0;

  while (ball) {

    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_circle(ctx, ball->pos, ball->radius);

    if (ball->pos.x >= SCREEN_WIDTH-ball->radius || ball->pos.x <= ball->radius) ball->speed.x *= -1;
    if (ball->pos.y >= SCREEN_HEIGHT-ball->radius || ball->pos.y <= ball->radius) ball->speed.y *= -1;

    ball->pos.x += ball->speed.x;
    ball->pos.y += ball->speed.y;

    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_circle(ctx, ball->pos, ball->radius);

    ball = ball->next;

  }

}

static void window_load(Window *window) {
  window_layer = window_get_root_layer(window);
}

static void window_unload(Window *window) {
  
}

static void init(void) {

  window = window_create();
  window_layer = window_get_root_layer (window);
  layer_set_update_proc(window_layer, my_layer_draw);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {

  srand(time(NULL));

  int i = 0;

  for (i = 0; i < 100; ++i) {

    Ball *b = (Ball *) malloc(sizeof(Ball));

    b->speed.x = rand()%5 - 2;
    b->speed.y = rand()%5 - 2;

    b->radius = rand()%10+1;

    b->pos.x = rand()%(SCREEN_WIDTH-2*b->radius)+b->radius;
    b->pos.y = rand()%(SCREEN_HEIGHT-2*b->radius)+b->radius;

    b->next = NULL;
    b->prev = NULL;

    if (front_ball == NULL) {
      front_ball = b;
      back_ball = b;
    } else {
      back_ball->next = b;
      b->prev = back_ball;
      back_ball = b;
    }

  }

  init();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_timer_register(INTERVAL, render, NULL);
  app_event_loop();
  deinit();
}
