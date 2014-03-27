#include <pebble.h>
//#include "typedefs.h"
//#include "ball.h"

static Window *window;

int ball_x = 25;
int ball_y = 25;

struct Layer *window_layer;


typedef struct ball{
   GPoint pos;
   int hor_dir;
   int ver_dir;
   int active;
   int ereased;

   struct ball *next;
   struct ball *prev;
}ballT;

int num_balls = 0;
ballT *last =  NULL;
ballT *first = NULL;
ballT *iter = NULL;
//ball v_balls[size];

void init_ball (ballT *b, int active, ballT *previous){
  b->pos.x = rand()%139;
  b->pos.y = rand()%146;
  b->hor_dir = 1;
  b->ver_dir = 1;
  b->active = active;
  b->ereased = 0;

  b->next = NULL;
  b->prev = previous;
}

void print_ball(ballT *b, GContext *ctx){
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, b->pos, 5);
}

void clear_ball(ballT *b, GContext *ctx){
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, b->pos, 5);
  b->ereased = 0;
}

void update_pos(ballT *b){
  b->pos.x += b->hor_dir;
  b->pos.y += b->ver_dir;

  if (b->pos.x >= 144 || b->pos.x <= 0) b->hor_dir *= -1;
  if (b->pos.y >= 151 || b->pos.y <= 0) b->ver_dir *= -1; 
}


void render (void* callback_data){
  layer_mark_dirty(window_layer);
  app_timer_register (33, render, NULL);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  ballT *new = (ballT*) malloc(sizeof(ballT));

  init_ball (new, 1, iter);
  last->next = new;
  if (num_balls == 0) first = new;
  last = new;

  ++num_balls;
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    if ( last != NULL){
      ballT *aux = last->prev;
      free(iter);

      last = aux;
      last->next = NULL;
      iter = last;
    }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void my_layer_draw(Layer *layer, GContext *ctx) {
    iter = first;
    while (iter->next != NULL){
      if (iter->active){
        clear_ball(iter, ctx);
        update_pos(iter);
        print_ball(iter, ctx);
      }
      else if (iter->ereased == 0){
        clear_ball(iter, ctx);
        iter->ereased = 1;
      }
    }
}

static void window_load(Window *window) {
  //Layer *window_layer = window_get_root_layer(window);
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
