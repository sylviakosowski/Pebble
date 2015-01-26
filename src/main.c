#include <pebble.h>

static Window *s_main_window;

static BitmapLayer *s_image_layer;
static GBitmap *defaultsad;
static GBitmap *bestfriend;
static GBitmap *feedme;
static GBitmap *hugme;
static GBitmap *loveme;
static GBitmap *tellmestory;
static GBitmap *wannaplay;

static GBitmap * currentImage;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  if(tick_time->tm_sec == 0) {
    vibes_short_pulse();
    bitmap_layer_set_bitmap(s_image_layer, hugme);
  }
  
  /*
  if(tick_time->tm_sec % 60 == 0) {
    //Every minute, make it vibrate
    vibes_short_pulse();
    //currentImage = gbitmap_create_with_resource(RESOURCE_ID_HUG_ME);
    //s_image_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
    bitmap_layer_set_bitmap(s_image_layer, hugme);
  } else {
    //currentImage = gbitmap_create_with_resource(RESOURCE_ID_DEFAULT_SAD);
  }*/
}

//Handles the time
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

//Handles accelerometer stuff
static void tap_handler(AccelAxisType axis, int32_t direction) {

}

static void main_window_load(Window *window) {
  
  currentImage = gbitmap_create_with_resource(RESOURCE_ID_DEFAULT_SAD);
  hugme = gbitmap_create_with_resource(RESOURCE_ID_HUG_ME);
  
  s_image_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_image_layer, currentImage);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_image_layer));

}

static void main_window_unload(Window *window) {
  
  // Destroy GBitmap
  gbitmap_destroy(currentImage);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_image_layer);
}

static void init() {
  //Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  //Register with Accelerometer tap event service
  accel_tap_service_subscribe(tap_handler);
  
  //Create main Window element and assign to pointer
  s_main_window = window_create();
  
  //Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  //Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  update_time();
}

static void deinit() {
  //Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
