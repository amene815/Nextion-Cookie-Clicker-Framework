#include <SoftwareSerial.h>
#include <Nextion.h>

//Nextion Stuff:

SoftwareSerial nextion(3, 2);     // Nextion TX to pin 2 and RX to pin 3 of Arduino
Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps
char charstring[80];
char * pch;
int field, event, page, id;
String message;

//Pages
String clicker = "Clicker";
int clicker_id = 0;
String buildings = "Buildings";
int buildings_id = 1;
String finish = "EndGame";
int finish_id = 3;

//Components

//Clicker components

String clicker_to_buildings = clicker +  "." + "BuildingsDoor";
int clicker_to_buildings_id = 7;

String cookie = clicker + "." + "Cookie";
int cookie_id = 1;
String num_cookies = clicker + "." + "Number";
int num_cookies_id = 2;

//Buildings components

String buildings_to_clicker = buildings + "." + "ClickerDoor";
int buildings_to_clicker_id = 1;

String finger = buildings + "." + "Finger";
int finger_id = 2;
String number_fingers = buildings + "." + "num_f";
int number_fingers_id = 12;
String finger_price = buildings + "." + "cost_f";
int finger_price_id = 11;

String monster = buildings + "." + "C Monster";
int monster_id = 3;
String number_monsters = buildings + "." + "num_c";
int number_monsters_id = 14;
String monster_price = buildings + "." + "cost_m";
int monster_price_id = 13;

String oven = buildings + "." + "Oven";
int oven_id = 4;
String number_ovens = buildings + "." + "num_o";
int number_ovens_id = 16;
String oven_price = buildings + "." + "cost_o";
int oven_price_id = 15;

String printer = buildings + "." + "Three Printer";
int printer_id = 5;
String number_printers = buildings + "." + "num_3";
int number_printers_id = 18;
String printer_price = buildings + "." + "cost_3";
int printer_price_id = 17;

//Events
int touch_press = 101;

//Arduino Stuff

bool show_messages = true;
int money;
long start_time;
float game_length = 1.5;

float price_scaler = 1.1;

int num_fingers;
int num_monsters;
int num_ovens;
int num_printers;

int finger_base_cost = 10;
int monster_base_cost = 42;
int oven_base_cost = 100;
int printer_base_cost = 1000;

int finger_cost;
int monster_cost;
int oven_cost;
int printer_cost;

void change_val(String component, int amount) {
  myNextion.sendCommand((component + ".val+=" + String(amount)).c_str());
}

void set_val(String component, int value) {
  myNextion.sendCommand((component + ".val=" + String(value)).c_str());
}

void go_to_page(String page) {
  myNextion.sendCommand(("page " + page).c_str());
}

boolean nextion_input() {
  event = 0;
  page = 0;
  id = 0;
  field = 0;
  
  message = myNextion.listen();                  //check for message
  message.toCharArray(charstring,80);
  
  if(message != ""){ // if a message is received...                      //...print it out
    //Serial.println(message);
    pch = strtok (charstring," ");
    while (pch != NULL) {
       field++;    
       if (field == 1) event = strtol(pch, NULL, 16);
       if (field == 2) page = strtol(pch, NULL, 16);
       if (field == 3) id = strtol(pch, NULL, 16);
       pch = strtok (NULL, " ");
    }

    if (show_messages) {
      Serial.print("Event: ");
      Serial.println(event);
      Serial.print("Page: ");
      Serial.println(page);
      Serial.print("ID: ");
      Serial.println(id);
    }

    return true;
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  myNextion.init();
  num_fingers = 0;
  num_monsters = 0;
  num_ovens = 0;
  num_printers = 0;
  set_val(number_fingers, 0);
  set_val(number_monsters, 0);
  set_val(number_ovens, 0);
  set_val(number_printers, 0);
  set_val(num_cookies, 0);
  finger_cost = finger_base_cost;
  monster_cost = monster_base_cost;
  oven_cost = oven_base_cost;
  printer_cost = printer_base_cost;
  set_val(finger_price, finger_cost);
  set_val(monster_price, monster_cost);
  set_val(oven_price, oven_cost);
  set_val(printer_price, printer_cost);
  myNextion.sendCommand((cookie + ".pic=0").c_str());
  myNextion.sendCommand((cookie + ".pic2=1").c_str());
  start_time = millis();
}

void loop() {

  if (millis() - start_time >= 60000 * game_length) {
    go_to_page(finish);
    num_fingers = 0;
    num_monsters = 0;
    num_ovens = 0;
    num_printers = 0;
    set_val(number_fingers, 0);
    set_val(number_monsters, 0);
    set_val(number_ovens, 0);
    set_val(number_printers, 0);
    set_val(num_cookies, 0);
    finger_cost = finger_base_cost;
    monster_cost = monster_base_cost;
    oven_cost = oven_base_cost;
    printer_cost = printer_base_cost;
    set_val(finger_price, finger_cost);
    set_val(monster_price, monster_cost);
    set_val(oven_price, oven_cost);
    set_val(printer_price, printer_cost);
    myNextion.sendCommand((cookie + ".pic=0").c_str());
    myNextion.sendCommand((cookie + ".pic2=1").c_str());
    while (!nextion_input()) {
      continue;
    }
    start_time = millis();
    go_to_page(clicker);
    
  } else if (nextion_input()) {
    
    //CLICKER
    if (page == clicker_id) {

      if (id == cookie_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Cookie pressed.");
          change_val(num_cookies, 1);
        }
      }
  
      if (id == clicker_to_buildings_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Buildings door pressed.");
          go_to_page(buildings);
        }
      }

    //BUILDINGS
    } else if (page == buildings_id) {
      
      money = myNextion.getComponentValue(num_cookies);
      Serial.println(money);

      if (id == buildings_to_clicker_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Clicker door pressed.");
          go_to_page(clicker);
        }
      }

      if (id == finger_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Finger button pressed.");
          if (money >= finger_cost) {
            if (show_messages) Serial.println("Bought a finger.");
            num_fingers += 1;
            change_val(num_cookies, -finger_cost);
            change_val(number_fingers, 1);
            finger_cost = round(pow(price_scaler, num_fingers) * finger_base_cost);
            set_val(finger_price, finger_cost);
            if (num_fingers == 1 && num_monsters >= 1 && num_ovens >= 1 && num_printers >= 1) {
              myNextion.sendCommand((cookie + ".pic=3").c_str());
              myNextion.sendCommand((cookie + ".pic2=2").c_str());
            }
            go_to_page(clicker);
          }
        }
      }

      if (id == monster_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Monster button pressed.");
          if (money >= monster_cost) {
            if (show_messages) Serial.println("Bought a monster.");
            num_monsters += 1;
            change_val(num_cookies, -monster_cost);
            change_val(number_monsters, 1);
            monster_cost = round(pow(1.0 / price_scaler, num_monsters) * monster_base_cost);
            set_val(monster_price, monster_cost);
            if (num_fingers >= 1 && num_monsters == 1 && num_ovens >= 1 && num_printers >= 1) {
              myNextion.sendCommand((cookie + ".pic=3").c_str());
              myNextion.sendCommand((cookie + ".pic2=2").c_str());
            }
            go_to_page(clicker);
          }
        }
      }

      if (id == oven_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Oven button pressed.");
          if (money >= oven_cost) {
            if (show_messages) Serial.println("Bought an oven.");
            num_ovens += 1;
            change_val(num_cookies, -oven_cost);
            change_val(number_ovens, 1);
            oven_cost = round(pow(price_scaler, num_ovens) * oven_base_cost);
            set_val(oven_price, oven_cost);
            if (num_fingers >= 1 && num_monsters >= 1 && num_ovens == 1 && num_printers >= 1) {
              myNextion.sendCommand((cookie + ".pic=3").c_str());
              myNextion.sendCommand((cookie + ".pic2=2").c_str());
            }
            go_to_page(clicker);
          }
        }
      }

      if (id == printer_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Printer button pressed.");
          if (money >= printer_cost | money == -42) {
            if (show_messages) Serial.println("Bought a printer.");
            num_printers += 1;
            change_val(num_cookies, -printer_cost);
            change_val(number_printers, 1);
            printer_cost = round(pow(price_scaler, num_printers) * printer_base_cost);
            set_val(printer_price, printer_cost);
            if (num_fingers >= 1 && num_monsters >= 1 && num_ovens >= 1 && num_printers == 1) {
              myNextion.sendCommand((cookie + ".pic=3").c_str());
              myNextion.sendCommand((cookie + ".pic2=2").c_str());
            }
            go_to_page(clicker);
          }
        }
      }
      
    }
  }
}
