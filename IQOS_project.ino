#include <SoftwareSerial.h>

#define LED_pin 11        // живлення світлодіода
#define IR_pin 17         // живлення фототранзистора
#define IR_sens 14        // сигнал фототранзистора
#define Button_pin 2      // сигнал кнопки
#define Rx_Pin 5          // пін Bluetoth TX
#define Tx_Pin 6          // пін Bluetoth RX

int prev_button_state;
int button_state;
unsigned long start_time;
unsigned long end_time;
int value;
int max_value;
int result;
int minimum;
int maximum;
boolean bt_flag;

SoftwareSerial Bluetooth(Rx_Pin, Tx_Pin);

void setup() {
  
  Serial.begin(9600);
  Bluetooth.begin(9600);
  delay(500);
  
  pinMode(LED_pin, OUTPUT);
  pinMode(IR_pin, OUTPUT);
  pinMode(Button_pin, INPUT_PULLUP);
  
  
  digitalWrite(LED_pin, 1);
  digitalWrite(IR_pin, 1);
}

void loop(){
  
  if (button_state == 0){
    minimum = analogRead(IR_sens) + 5;
    maximum = minimum + 100;
  }

  button_state = !digitalRead(Button_pin);
  
  if ((button_state == 1) && (prev_button_state == 0)){
    start_time = millis();
  }
  if (button_state == 1){
    if ((millis() - start_time) > 500){
      max_value = analogRead(IR_sens);
      bt_flag = true;
      if(button_state == 1){
        value = analogRead(IR_sens);
        if (value > max_value){
          max_value = value;
          end_time = millis();
        }
        button_state = !digitalRead(Button_pin);
      }
      max_value = max_value - 3;
      if (max_value < minimum){
        max_value = minimum;
      }
      if (max_value > maximum){
        max_value = maximum;
      }
      result = map(max_value, minimum, maximum, 0, 100);
      Serial.println(result);
    }
  }
  Bluetooth.println(result);
  result = 0;
  prev_button_state = button_state;
  delay(30);
}
