#include <pebble.h>
Window *window_temp;
TextLayer *temp_layer;
char msg[100];
int choice;
static AppTimer *timer; // global variable to represent the timer
int sleeptime = 3000;
int isC;
int con_timer = 1;

void detailUnloadTemp(Window* win){
  text_layer_destroy(temp_layer);
  window_destroy(window_temp);
}

void out_sent_handler(DictionaryIterator *sent, void *context) {
  // outgoing message was delivered — do nothing
}

void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  // outgoing message failed
  text_layer_set_text(temp_layer, "Error out! Check internet connection."); 
}

void in_received_handler(DictionaryIterator *received, void *context) {
  // incoming message received 
  // looks for key #0 in the incoming message 
  int key = 0;
  Tuple *text_tuple = dict_find(received, key); 
  if (text_tuple) {
    if (text_tuple->value) {
      // put it in this global variable 
      strcpy(msg, text_tuple->value->cstring);
      printf("cstring=%s\n", text_tuple->value->cstring);
//       printf("value=%s\n", text_tuple->value);
      printf("cstring=%s\n", msg);
//        strcpy(msg, "Receive");
//       text_layer_set_text(temp_layer, text_tuple->value->cstring);
//       return;
    }
    else strcpy(msg, "no value!");
  text_layer_set_text(temp_layer, msg); 
  }
  else {
    text_layer_set_text(temp_layer, "no message!");
  }
}

void in_dropped_handler(AppMessageResult reason, void *context) { 
  // incoming message dropped  
  text_layer_set_text(temp_layer, "Error in!");
}

static void timer_callback(void *data) { 
  // this function is called by the timer when it wakes up
  DictionaryIterator *iter; 
  app_message_outbox_begin(&iter);
  int key = 0;
  char ms[100];

  // send the message "hello?" to the phone, using key #0 
  if(choice == 0){
    if (isC == 1) strcpy(ms,"Celsius");
    else strcpy(ms,"Farenheit");
    }
  else if(choice==1){
    strcpy(ms,"Distance");
  }
  else{
    strcpy(ms,"Time");
  }
  Tuplet value = TupletCString(key, &ms[0]);
  dict_write_tuplet(iter, &value); 
  app_message_outbox_send();
  if(con_timer == 1) timer = app_timer_register(sleeptime, timer_callback, NULL);
}

/* This is called when the select button is clicked */
void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  isC = 1;
  con_timer = 1;
  timer_callback(NULL);
}

void select_click_handler_up(ClickRecognizerRef recognizer, void *context){
  
  isC = 0;
  con_timer = 1;
  timer_callback(NULL);
}

void select_click_handler_down(ClickRecognizerRef recognizer, void *context){
  con_timer = 0;
  DictionaryIterator *iter; 
  app_message_outbox_begin(&iter);
  int key = 0;
  char ms[100];

  // send the message "hello?" to the phone, using key #0 
  if(choice == 0){
    strcpy(ms,"Standby");
  }
  else if(choice == 1){
    strcpy(ms,"Distance");
  }
  else{
    strcpy(ms,"Time");
  }
  Tuplet value = TupletCString(key, &ms[0]);
  dict_write_tuplet(iter, &value); 
  app_message_outbox_send();
}

void back_click_handler(ClickRecognizerRef recognizer, void *context){
  con_timer = 0;
  window_stack_remove(window_temp, true);
}

/* this registers the appropriate function to the appropriate button */ 
void config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, select_click_handler_up);
  window_single_click_subscribe(BUTTON_ID_DOWN, select_click_handler_down);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

void showDetailTemp(MenuIndex* index){
  
  WindowHandlers wh = {.unload = &detailUnloadTemp};
  window_temp = window_create();
  window_set_window_handlers(window_temp,wh);
  window_set_click_config_provider(window_temp,config_provider);
  app_message_register_inbox_received(in_received_handler); 
  app_message_register_inbox_dropped(in_dropped_handler); 
  app_message_register_outbox_sent(out_sent_handler); 
  app_message_register_outbox_failed(out_failed_handler); 
  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64; 
  app_message_open(inbound_size, outbound_size);
  const bool animated = true;

  Layer *window_layer = window_get_root_layer(window_temp);
  GRect bounds = layer_get_bounds(window_layer);
  temp_layer = text_layer_create((GRect) { .origin = { 0, 20 }, 
    .size = { bounds.size.w, 72 } });
	//text_layer_set_text_alignment(detailW_text, GTextAlignmentCenter); // Center the text.
	text_layer_set_font(temp_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(temp_layer, GTextAlignmentCenter); 
  
 if((int)index->row==0){
    text_layer_set_text(temp_layer, "1.termperature"); 
    choice = 0;
 }
  else if((int)index->row==1){
    text_layer_set_text(temp_layer,"2.distance");
    choice = 1;
  }
  else if((int)index->row==2){
    text_layer_set_text(temp_layer,"3.time");
    choice = 2;
  }
  layer_add_child(window_get_root_layer(window_temp), text_layer_get_layer(temp_layer));
  window_stack_push(window_temp, true);
  
}

/*void window_load(Window *window) {
Layer *window_layer = window_get_root_layer(window); 
GRect bounds = layer_get_bounds(window_layer);
hello_layer = text_layer_create((GRect) { .origin = { 0, 72 },
.size = { bounds.size.w, 20 } }); 
text_layer_set_text(hello_layer, "Hello world!"); 
text_layer_set_text_alignment(hello_layer, GTextAlignmentCenter); 
layer_add_child(window_layer, text_layer_get_layer(hello_layer));
}
void window_unload(Window *window) { text_layer_destroy(hello_layer);
}
void init(void) {
window = window_create(); 
window_set_window_handlers(window, (WindowHandlers) {
.load = window_load,
.unload = window_unload, });
const bool animated = true;
window_stack_push(window, animated); }
void deinit(void) { window_destroy(window);
}*/