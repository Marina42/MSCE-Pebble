#include <pebble.h>

static Window *window;

int ball_x = 25;
int ball_y = 25;

int hor_dir = 1;
int ver_dir = 1;

struct Layer *window_layer;



void render (void* callback_data){
  layer_mark_dirty(window_layer);
  app_timer_register (33, render, NULL);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void my_layer_draw(Layer *layer, GContext *ctx) {
    GPoint ball;
    ball.x = ball_x;
    ball.y = ball_y;

    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_circle(ctx, ball, 5);

    ball_x += hor_dir;
    ball_y += ver_dir;

    if (ball_x >= 139 || ball_x <= 5) hor_dir *= -1;
    if (ball_y >= 146 || ball_y <= 5) ver_dir *= -1; 

    //GPoint ball;
    ball.x = ball_x;
    ball.y = ball_y;

    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_circle(ctx, ball, 5);
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
  window_set_click_config_provider(window, click_config_provider);
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
  init();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_timer_register(33, render, NULL);
  app_event_loop();
  deinit();
}
