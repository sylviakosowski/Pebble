#include <pebble.h>

static Window *s_main_window;

static BitmapLayer *s_image_layer;
static GBitmap *bestFriend;
static GBitmap *feedMe;
static GBitmap *hugMe;
static GBitmap *loveMe;
static GBitmap *tellMeStory;
static GBitmap *playWithMe;

bool isDemanding;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  
  if(tick_time->tm_min == 0) {
    vibes_long_pulse();
    bitmap_layer_set_bitmap(s_image_layer, loveMe);
    isDemanding = true;
  } else if(tick_time->tm_min == 15) {
    vibes_long_pulse();
    bitmap_layer_set_bitmap(s_image_layer, hugMe);
    isDemanding = true;
  } else if(tick_time->tm_min == 30) {
    vibes_long_pulse();
    bitmap_layer_set_bitmap(s_image_layer, tellMeStory);
    isDemanding = true;
  } else if(tick_time->tm_min == 45) {
    vibes_long_pulse();
    bitmap_layer_set_bitmap(s_image_layer, feedMe);
    isDemanding = true;
  } else if(tick_time->tm_sec == 0) {
    vibes_short_pulse();
    bitmap_layer_set_bitmap(s_image_layer, playWithMe);
    isDemanding = true;
  }
  
}

//Handles the time
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

//Handles accelerometer stuff
static void tap_handler(AccelAxisType axis, int32_t direction) {
  if(isDemanding) {
    if(direction != 0) {
      isDemanding = false;
      bitmap_layer_set_bitmap(s_image_layer, bestFriend);
    }
  }
}

static void main_window_load(Window *window) {
  
  bestFriend = gbitmap_create_with_resource(RESOURCE_ID_BEST_FRIEND);
  feedMe = gbitmap_create_with_resource(RESOURCE_ID_FEED_ME);
  hugMe = gbitmap_create_with_resource(RESOURCE_ID_HUG_ME);
  loveMe = gbitmap_create_with_resource(RESOURCE_ID_LOVE_ME);
  tellMeStory = gbitmap_create_with_resource(RESOURCE_ID_TELL_ME_STORY);
  playWithMe =  gbitmap_create_with_resource(RESOURCE_ID_PLAY_WITH_ME);
  
  s_image_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_image_layer, bestFriend);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_image_layer));

}

static void main_window_unload(Window *window) {
  
  // Destroy GBitmaps
  gbitmap_destroy(bestFriend);
  gbitmap_destroy(feedMe);
  gbitmap_destroy(hugMe);
  gbitmap_destroy(loveMe);
  gbitmap_destroy(tellMeStory);
  gbitmap_destroy(playWithMe);
  
  // Destroy BitmapLayer
  bitmap_layer_destroy(s_image_layer);
}

static void init() {
  isDemanding = false;
  
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
