#include <pebble.h>

static Window *window;
static TextLayer *total_time_layer;
static TextLayer *slide_time_layer;

static int total_seconds;
static int slide_seconds;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "SELECT");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "UP");
  slide_seconds = 0;
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "DOWN");
  slide_seconds = 0;
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void format_seconds(int passed_seconds, char *buffer, size_t len) {
  int seconds = passed_seconds % 60;
  int minutes = (passed_seconds / 60) % 60;
  int hours = (passed_seconds / 3600);

  if(hours > 0) {
    snprintf(buffer, len, "%d:%02d:%02d", hours, minutes, seconds);
  } else {
    snprintf(buffer, len, "%d:%02d", minutes, seconds);
  }
}

static void update_text_layers(void *data) {
  total_seconds++;
  slide_seconds++;

  static char total_str[20];
  format_seconds(total_seconds, total_str, sizeof(total_str));
  text_layer_set_text(total_time_layer, total_str);

  static char slide_str[20];
  format_seconds(slide_seconds, slide_str, sizeof(slide_str));
  text_layer_set_text(slide_time_layer, slide_str);

  app_timer_register(1000, update_text_layers, NULL);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  total_time_layer = text_layer_create((GRect) { .origin = { 0, 40 }, .size = { bounds.size.w, 50 } });
  text_layer_set_font(total_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
  text_layer_set_text_alignment(total_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(total_time_layer));

  slide_time_layer = text_layer_create((GRect) { .origin = { 0, 90 }, .size = { bounds.size.w, 24 } });
  text_layer_set_font(slide_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(slide_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(slide_time_layer));

  update_text_layers(NULL);
}

static void window_unload(Window *window) {
  text_layer_destroy(total_time_layer);
}

static void init(void) {
  window = window_create();
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

  app_event_loop();
  deinit();
  return 0;
}
