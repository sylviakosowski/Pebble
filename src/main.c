#include <pebble.h>

static Window *s_main_window;

static void main_window_load(Window *window) {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

static void main_window_unload(Window *window) {

}

static void init() {
  
}

static void deinit() {
  
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
