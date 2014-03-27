#include <pebble.h>

#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 151
#define FPS 60
#define INTERVAL (1000/FPS)

static Window *window;
struct Layer *window_layer;

GPoint ball;

int hor_dir = 1;
int ver_dir = 1;

int ball_radius = 5;

void render (void* callback_data){
  layer_mark_dirty(window_layer);
  app_timer_register (INTERVAL, render, NULL);
}

static void my_layer_draw(Layer *layer, GContext *ctx) {

    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_circle(ctx, ball, ball_radius);

    if (ball.x >= SCREEN_WIDTH-ball_radius || ball.x <= ball_radius) hor_dir *= -1;
    if (ball.y >= SCREEN_HEIGHT-ball_radius || ball.y <= ball_radius) ver_dir *= -1;

    ball.x += hor_dir;
    ball.y += ver_dir;

    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_circle(ctx, ball, ball_radius);

}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
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

  ball.x = 25;
  ball.y = 25;

  init();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_timer_register(INTERVAL, render, NULL);
  app_event_loop();
  deinit();
}
