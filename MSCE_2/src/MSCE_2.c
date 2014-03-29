#include <pebble.h>
#include <time.h>
#include <stdlib.h>
#include <stddef.h>

#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 151
#define FPS 60
#define INTERVAL (1000/FPS)
#define INITIAL_SIZE 16

static Window *window;
struct Layer *window_layer;

typedef struct Ball Ball;

struct Ball 
{
  GPoint pos;
  GPoint speed;
  int radius;
};

typedef struct 
{
  Ball *array;
  size_t used;
  size_t size;
}Ball_array;

Ball_array array;

void initArray (size_t init_size)
{
  array.array = (Ball *)malloc(init_size * sizeof(Ball));
  array.used = 0;
  array.size = init_size;
}

void render (void* callback_data)
{
  layer_mark_dirty(window_layer);
  app_timer_register (INTERVAL, render, NULL);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  Ball ball;

  ball.pos.x = rand()%5 - 2;
  ball.pos.y = rand()%5 -2;

  ball.radius = rand()%10+1;

  ball.pos.x = rand()%(SCREEN_WIDTH-2*ball.radius)+ball.radius;
  ball.pos.y = rand()%(SCREEN_HEIGHT-2*ball.radius)+ball.radius;
  
  if (array.used == array.size) 
  {
    array.size *= 2;
    array.array = (Ball *)realloc(array.array, array.size * sizeof(Ball));
  }
  array.array[array.used++] = ball;
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
  --array.used;
}

static void click_config_provider(void *context) 
{
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void my_layer_draw(Layer *layer, GContext *ctx) 
{
  int n = array.used;
  int i;
  for (i = 0; i < n; ++i)
  {
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_circle(ctx, array.array[i].pos, array.array[i].radius);

    if (array.array[i].pos.x >= SCREEN_WIDTH - array.array[i].radius || array.array[i].pos.x <= array.array[i].radius) 
        array.array[i].speed.x *= -1;
    if (array.array[i].pos.y >= SCREEN_HEIGHT - array.array[i].radius || array.array[i].pos.y <= array.array[i].radius) 
        array.array[i].speed.y *= -1;

    array.array[i].pos.x += array.array[i].speed.x;
    array.array[i].pos.y += array.array[i].speed.y;

    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_circle(ctx, array.array[i].pos, array.array[i].radius);
  }
}

static void window_load(Window *window) {}

static void window_unload(Window *window) {}
  
static void init(void) 
{
  window = window_create();
  window_layer = window_get_root_layer (window);
  layer_set_update_proc(window_layer, my_layer_draw);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  initArray(INITIAL_SIZE);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_timer_register(INTERVAL, render, NULL);
  app_event_loop();
  deinit();

}
