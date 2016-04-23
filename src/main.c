#include <pebble.h>
Window *window;
MenuLayer *s_menu_layer;
void showDetailTemp(MenuIndex* index);
// void showDetailDis();
// void showDetailTime();

uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  const uint16_t num_rows = 3;
  return num_rows;
}

void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
  static char s_buff[16];
  if((int)cell_index->row == 0){
    snprintf(s_buff, sizeof(s_buff), "Temperature");
  }
  else if((int)cell_index->row == 1){
    snprintf(s_buff, sizeof(s_buff), "Distance");
  }
  else{
    snprintf(s_buff, sizeof(s_buff), "Time");
  }
  // Draw this row's index
  menu_cell_basic_draw(ctx, cell_layer, s_buff, NULL, NULL);
}

int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  const int16_t cell_height = 44;
  return cell_height;
}

void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  // Do something in response to the button press
  //if((int)cell_index==0){
    showDetailTemp(cell_index);
//   }
//    else if((int)cell_index==1){
//      showDetailDis();
//    }
//    else{
//      showDetailTime();
//    }
}

void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window); 
  GRect bounds = layer_get_bounds(window_layer);
  // Create the MenuLayer
  s_menu_layer = menu_layer_create(bounds);

  // Let it receive click events
  menu_layer_set_click_config_onto_window(s_menu_layer, window);

  // Set the callbacks for behavior and rendering
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
    .get_num_rows = get_num_rows_callback,
    .draw_row = draw_row_callback,
    .get_cell_height = get_cell_height_callback,
    .select_click = select_callback,
  });

  // Add to the Window
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

void window_unload(Window *window) { 
 // Destroy the MenuLayer
  menu_layer_destroy(s_menu_layer);
}

void init(void) {
  window = window_create(); 
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload, });
  const bool animated = true;
  window_stack_push(window, animated); 
}

void deinit(void) { 
  window_destroy(window);
}

int main(void) { 
  init();
  app_event_loop();
  deinit(); 
}