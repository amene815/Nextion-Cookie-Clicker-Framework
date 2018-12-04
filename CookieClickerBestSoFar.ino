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
String upgrades = "Upgrades";
int upgrades_id = 2;

//Components

//Clicker components

String clicker_to_buildings = clicker +  "." + "BuildingsDoor";
int clicker_to_buildings_id = 8;
String clicker_to_upgrades = clicker +  "." + "UpgradesDoor";
int clicker_to_upgrades_id = 3;

String cookie = clicker + "." + "Cookie";
int cookie_id = 1;
String num_cookies = clicker + "." + "Number";
int num_cookies_id = 2;

//Buildings components

String buildings_to_clicker = buildings + "." + "ClickerDoor";
int buildings_to_clicker_id = 1;
String buildings_to_upgrades = buildings + "." + "UpgradesDoor";
int buildings_to_upgrades_id = 21;

String finger = buildings + "." + "Finger";
int finger_id = 2;
String num_fingers = buildings + "." + "num_f";
int num_fingers_id = 12;
String finger_price = buildings + "." + "cost_f";
int finger_price_id = 11;

String monster = buildings + "." + "C Monster";
int monster_id = 3;
String num_monsters = buildings + "." + "num_c";
int num_monsters_id = 14;
String monster_price = buildings + "." + "cost_m";
int monster_price_id = 13;

String oven = buildings + "." + "Oven";
int oven_id = 4;
String num_ovens = buildings + "." + "num_o";
int num_ovens_id = 16;
String oven_price = buildings + "." + "cost_o";
int oven_price_id = 15;

String printer = buildings + "." + "Three Printer";
int printer_id = 5;
String num_printers = buildings + "." + "num_3";
int num_printers_id = 18;
String printer_price = buildings + "." + "cost_3";
int printer_price_id = 17;

//Upgrades components

String upgrades_to_clicker = upgrades + "." + "ClickerDoor";
int upgrades_to_clicker_id = 1;
String upgrades_to_buildings = upgrades + "." + "BuildingsDoor";
int upgrades_to_buildings_id = 12;

String two = "Upgrade1";
int two_id = 2;
String strong = "Upgrade2";
int strong_id = 3;
String hungry = "Upgrade3";
int hungry_id = 4;
String colorful = "Upgrade4";
int colorful_id = 5;

//Events
int touch_press = 101;

//Arduino Stuff

bool show_messages = true;
int how_far_neg = 0;
int neg_cutoff = 65535 + how_far_neg;
int money;

float price_scaler = 1.1;

int finger_base_cost = 10;
int monster_base_cost = 100;
int oven_base_cost = 1000;
int printer_base_cost = 10000;

int finger_cost = finger_base_cost;
int monster_cost = monster_base_cost;
int oven_cost = oven_base_cost;
int printer_cost = printer_base_cost;

void change_val(String component, int amount) {
  myNextion.sendCommand((component + ".val+=" + String(amount)).c_str());
}

void set_val(String component, int value) {
  myNextion.sendCommand((component + ".val=" + String(value)).c_str());
}

void go_to_page(String page) {
  myNextion.sendCommand(("page " + page).c_str());
}

/*
int get_val(String component) {
  return myNextion.getComponentValue(component);
}
*/
//Don't use ^, does some weird stuff

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
  set_val(num_fingers, 0);
  set_val(num_monsters, 0);
  set_val(num_ovens, 0);
  set_val(num_printers, 0);
  set_val(num_cookies, 0);
  set_val(finger_price, finger_cost);
  set_val(monster_price, monster_cost);
  set_val(oven_price, oven_cost);
  set_val(printer_price, printer_cost);
  myNextion.sendCommand((cookie + ".pic=0").c_str());
  myNextion.sendCommand((cookie + ".pic2=1").c_str());
}

void loop() {

  if (nextion_input()) {
    
    //CLICKER
    if (page == clicker_id) {

      if (id == cookie_id) {
        if (event == touch_press) {
          if (show_messages) {
            Serial.println("Cookie pressed.");
          }
          myNextion.sendCommand((num_cookies + ".val+=" + String(1)).c_str());
          //Serial.println(myNextion.getComponentValue(num_cookies));
        }
      }
  
      if (id == clicker_to_buildings_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Buildings door pressed.");
          go_to_page(buildings);
        }
      }

      if (id == clicker_to_upgrades_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Upgrades door pressed.");
          go_to_page(upgrades);
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

      if (id == buildings_to_upgrades_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Upgrades door pressed.");
          go_to_page(upgrades);
        }
      }

      if (id == finger_id) {
        if (event == touch_press) {
          if (show_messages) {
            Serial.println("Finger button pressed.");
            Serial.println("Money: " + String(money));
            Serial.println("Finger cost: " + String(finger_cost));
          }
          if (money >= finger_cost) {
            if (show_messages) Serial.println("Bought a finger.");
            change_val(num_cookies, -finger_cost);
            delay(20);
            myNextion.sendCommand((num_fingers + ".val+=" + String(1)).c_str());
            finger_cost = round(pow(price_scaler, myNextion.getComponentValue(num_fingers)) * finger_base_cost);
            set_val(finger_price, finger_cost);
          }
        }
      }

      if (id == monster_id) {
        if (event == touch_press) {
          if (show_messages) {
            Serial.println("Monster button pressed.");
            Serial.println("Money: " + String(money));
          }
          if (money >= monster_cost) {
            if (show_messages) Serial.println("Bought a monster.");
            change_val(num_cookies, -monster_cost);
            change_val(num_monsters, 1);
            monster_cost = round(pow(price_scaler, myNextion.getComponentValue(num_monsters)) * monster_base_cost);
            set_val(monster_price, monster_cost);
          }
        }
      }

      if (id == oven_id) {
        if (event == touch_press) {
          if (show_messages) {
            Serial.println("Oven button pressed.");
            Serial.println("Money: " + String(money));
          }
          if (money >= oven_cost) {
            if (show_messages) Serial.println("Bought an oven.");
            change_val(num_cookies, -oven_cost);
            change_val(num_ovens, 1);
            oven_cost = round(pow(price_scaler, myNextion.getComponentValue(num_ovens)) * oven_base_cost);
            set_val(oven_price, oven_cost);
          }
        }
      }

      if (id == printer_id) {
        if (event == touch_press) {
          if (show_messages) {
            Serial.println("Printer button pressed.");
            Serial.println("Money: " + money);
          }
          if (money >= printer_cost) {
            if (show_messages) Serial.println("Bought a printer.");
            change_val(num_cookies, -finger_cost);
            change_val(num_printers, 1);
            printer_cost = round(pow(price_scaler, myNextion.getComponentValue(num_printers)) * printer_base_cost);
            set_val(printer_price, printer_cost);
          }
        }
      }
            
    } else if (page == upgrades_id) {
      
      if (id == upgrades_to_clicker_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Clicker door pressed.");
          go_to_page(clicker);
        }
      }

      if (id == upgrades_to_buildings_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Buildings door pressed.");
          go_to_page(buildings);
        }
      }

      if (id == colorful_id) {
        if (event == touch_press) {
          if (show_messages) Serial.println("Monster cookie button pressed.");
          myNextion.sendCommand((cookie + ".pic=3").c_str());
          myNextion.sendCommand((cookie + ".pic2=2").c_str());
        }
      }
      
    }
  }
}
