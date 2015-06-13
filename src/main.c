#include <pebble.h>

#define KEY_BGMIN 0
#define KEY_FGMIN 1
#define KEY_BGHOUR 2
#define KEY_FGHOUR 3
#define KEY_RIM 4
  
int bgMin=1;
int fgMin=2;
int bgHour=3;
int fgHour=4;
int fgRim=5;


static Window *s_main_window;
static TextLayer *s_time_layer;


static void update_text()
{
  static char buffer[50];

  APP_LOG(APP_LOG_LEVEL_DEBUG, "update_text() called");
  snprintf(buffer, sizeof(buffer), "%d, %d, %d, %d, %d", bgMin, fgMin, bgHour, fgHour, fgRim);
  text_layer_set_text(s_time_layer, buffer);
}

static void in_recv_handler(DictionaryIterator *iterator, void *context)
{
  Tuple *t=NULL;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "in_recv_handler() called");
  t = dict_read_first(iterator);
  while(t != NULL)
  {
    switch(t->key)
    {
    case KEY_BGMIN:
      APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_BGMIN");
      bgMin = (int)t->value->int16;             
      break;
    case KEY_FGMIN:
      APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_FGMIN");
      fgMin = (int)t->value->int16;      
      break;
    case KEY_BGHOUR:
      APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_BGHOUR");
      bgHour = (int)t->value->int16;      
      break;      
    case KEY_FGHOUR:
      APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_FGHOUR");
      fgHour = (int)t->value->int16;      
      break;  
    case KEY_RIM:
      APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_RIM");
      fgRim = (int)t->value->int16;      
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Unknown key! :-(");
      break;
    }
  t = dict_read_next(iterator);
  }
  
  //Update text
  update_text();
}

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  update_text();

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  /* TODO use AppSync instead? */
  app_message_register_inbox_received(in_recv_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum()); 
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
