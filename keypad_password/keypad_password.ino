#include <Keypad.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>
#include <Servo.h>

Servo htservo;  
int pos = 0; 
LiquidCrystal lcd(7,6,5,4,3,2);
char password[4];
char initial_password[4],new_password[4];
int i=0;
int LED = 13;
char key_pressed=0;
const byte rows = 4; 
const byte columns = 4; 

char hexaKeys[rows][columns] = {

{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}

};

byte row_pins[rows] = {A0,A1,A2,A3};
byte column_pins[columns] = {A4,A5,9,8};   

Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

void setup()

{
  htservo.attach(11);
  pinMode(LED, OUTPUT);
  lcd.clear();
  lcd.begin(16,2);
  lcd.setCursor(3,0);
  lcd.print("SUBSCRIBE");
  lcd.setCursor(2,1);
  lcd.print("HIREN TAILOR");
  delay(3000);

  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0,1);
  initialpassword();

}

void loop()

{
  htservo.write(pos);
  digitalWrite(LED, LOW);
  key_pressed = keypad_key.getKey();
  if(key_pressed=='#')

    change();
  if (key_pressed)
  {
    password[i++]=key_pressed;
    lcd.print(key_pressed);
  }

  if(i==4)
  {
    delay(200);
    for(int j=0;j<4;j++)

      initial_password[j]=EEPROM.read(j);
    if(!(strncmp(password, initial_password,4)))

    {

      lcd.clear();
      lcd.print("Pass Accepted");
      digitalWrite(LED, HIGH);
      pos = 120;
      htservo.write(pos);
      delay(6000);
      pos = 0;
      //delay(2000);
      lcd.setCursor(0,1);
      lcd.print("Pres # to change");
      delay(2000);
      lcd.clear();
      lcd.print("Enter Password:");
      lcd.setCursor(0,1);

      i=0;

    }

    else

    {

      digitalWrite(LED, LOW);
      lcd.clear();
      lcd.print("Wrong Password");
      lcd.setCursor(0,1);
      lcd.print("Pres # to Change");
      delay(2000);

      lcd.clear();
      lcd.print("Enter Password");
      lcd.setCursor(0,1);
      i=0;

    }

  }

}

void change()

{

  int j=0;
  lcd.clear();
  lcd.print("Current Password");
  lcd.setCursor(0,1);
  while(j<4)

  {

    char key=keypad_key.getKey();

    if(key)
    {

      new_password[j++]=key;
      lcd.print(key);
    }

    key=0;

  }

  delay(500);
  if((strncmp(new_password, initial_password, 4)))
  {

    lcd.clear();
    lcd.print("Wrong Password");
    lcd.setCursor(0,1);
    lcd.print("Try Again");
    delay(1000);

  }

  else

  {

    j=0;

    lcd.clear();
    lcd.print("New Password:");
    lcd.setCursor(0,1);

    while(j<4)

    {
      char key=keypad_key.getKey();
      if(key)

      {

        initial_password[j]=key;
        lcd.print(key);

        EEPROM.write(j,key);
        j++;
      }
    }

    lcd.print("Pass Changed");
    delay(1000);
  }

  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0,1);
  key_pressed=0;

}

void initialpassword(){

  for(int j=0;j<4;j++) 
    EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)

    initial_password[j]=EEPROM.read(j);

} 
