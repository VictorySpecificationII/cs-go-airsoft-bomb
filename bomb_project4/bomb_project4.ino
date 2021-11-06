

/////////////////////////////
//      Airsoft Bomb       //
// by Malthe Falkenstjerne //
//       Modifications     //
// by Antreas Christofi    //
/////////////////////////////

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Tone.h>
#define pound 14

Tone tone1;//speakers
Tone tone2;//buzzer

int Scount = 0; // count seconds
int Mcount = 5; // count minutes
int Hcount = 0; // count hours
int DefuseTimer = 0; // set timer to 0
int Mpenalty = 5; //59 for 1 attempt boom, less for more

boolean armed = false;
boolean planted = false;
boolean master = false;

long secMillis = 0; // store last time for second add
long interval = 1000; // interval for seconds

char password[4]; // number of characters in our password
int currentLength = 0; //defines which number we are currently writing
int i = 0; 
char entered[4];

int ledPin = 4; //red led
int ledPin2 = 3; //yellow led
int ledPin3 = 2; //green led
int armswitch_pin = 42;
int keyswitch_pin = 44;

LiquidCrystal_I2C lcd(0x27, 16, 2); // the pins we use on the LCD

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {24, 26, 28, 30}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {34, 32, 36};    // connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup(){
  pinMode(ledPin, OUTPUT); // sets the digital pin as output
  pinMode(ledPin2, OUTPUT); // sets the digital pin as output
  pinMode(ledPin3, OUTPUT); // sets the digital pin as output
  //pinMode(38, OUTPUT); // sets the digital pin as output
  pinMode(armswitch_pin, INPUT);
  pinMode(keyswitch_pin, INPUT);

  
  lcd.init();
  lcd.backlight();
  tone1.begin(38);//on pin 38
  tone2.begin(40);
  lcd.begin(16, 2);
  Serial.begin(9600);
  
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Umbrella");
  lcd.setCursor(3,1);
  lcd.print("Corporation");
  delay(3000);

  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Bio-Research");
  lcd.setCursor(4,1);
  lcd.print("Division");
  delay(3000);

  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Confirm: Point");
  lcd.setCursor(0,1);
  lcd.print("of No Return");
  delay(3000);
  
  while(digitalRead(44) == HIGH)
    Serial.println("MASTER OFF");

  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Weapon Status:");
  lcd.setCursor(0,1);
  lcd.print("====Disarmed====");
  
  while(!armed){
      Serial.println("Unarmed");
      if(digitalRead(armswitch_pin) == HIGH){
        digitalWrite(13, LOW);
      }
      if(digitalRead(armswitch_pin) == LOW){
        digitalWrite(13, HIGH);
        armed = true;
        lcd.clear();
        
        lcd.setCursor(0,0);
        lcd.print("=====NOTICE=====");
        lcd.setCursor(0,1);
        lcd.print("    Arming      ");
        delay(500);
        lcd.setCursor(0,1);
        lcd.print("    Arming.     ");
        delay(500);
        lcd.setCursor(0,1);
        lcd.print("    Arming..    ");
        delay(500);
        lcd.setCursor(0,1);
        lcd.print("    Arming...   ");
        delay(500);
        

        
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Weapon Status:");
        lcd.setCursor(0,1);
        lcd.print("=====ARMED======");
        delay(3000);
        
        lcd.clear();

        
        digitalWrite(13, LOW); 

      }
    }

  
  tone1.play(NOTE_B4, 200);
  delay(300);
  tone1.play(NOTE_B6, 200);
  
  lcd.setCursor(0,0);
  lcd.print("Enter Sequence: ");
  while (currentLength < 4)
  {
    lcd.setCursor(currentLength + 6, 1);
    lcd.cursor();
    char key = keypad.getKey();
    key == NO_KEY;
    if (key != NO_KEY)
    {
      if ((key != '*')&&(key != '#'))
      { 
      lcd.print(key);
      password[currentLength] = key;
      currentLength++;
      tone1.play(NOTE_A4, 200);
      }
    }
  }

  if (currentLength == 4)
  {
    delay(500);
    lcd.noCursor();
    lcd.clear();
    lcd.home();
    lcd.print("Defusal Code: ");
    lcd.setCursor(6,1);
    lcd.print(password[0]);
    lcd.print(password[1]);
    lcd.print(password[2]);
    lcd.print(password[3]);

    tone1.play(NOTE_E6, 200);
    delay(3000);
    lcd.clear();
    currentLength = 0;
  }
}

void loop()
{
  timer();
  char key2 = keypad.getKey(); // get the key
  
  if (key2 == '*')
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Code: ");
      
      while (currentLength < 4)
        {
          
          timer();
          
          char key2 = keypad.getKey(); 
          if (key2 == '#')
            {
              currentLength = 0;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Code: ");
            }
          else                  
          if (key2 != NO_KEY)    
            {
              
              lcd.setCursor(currentLength + 7, 0);
              lcd.cursor();
              
              lcd.print(key2);
              entered[currentLength] = key2;
              currentLength++;
              tone1.play(NOTE_C6, 200);
              delay(100);
              lcd.noCursor();
              lcd.setCursor(currentLength + 6, 0);
              lcd.print("*");
              lcd.setCursor(currentLength + 7, 0);
              lcd.cursor();
            }
        }

      if (currentLength == 4) 
        {
          if (entered[0] == password[0] && entered[1] == password[1] && entered[2] == password[2] && entered[3] == password[3])
            {
              lcd.noCursor();
              lcd.clear();
              lcd.home();
              lcd.setCursor (0,0); //99781640 marios
              lcd.print("The bomb has");
              lcd.setCursor (0,1);
              lcd.print("been defused.");

              currentLength = 0;
              digitalWrite(ledPin3, HIGH);
              delay(2500);
              lcd.setCursor(0,1);
              lcd.print("");
              delay(1000000);
            }
      else
        {
          lcd.noCursor();
          lcd.clear();
          lcd.home();
          lcd.setCursor (0,0); //99781640 marios
          lcd.print("=====WARNING=====");
          lcd.setCursor (0,1);
          lcd.print("-Wrong Password-");
          delay(3000);
          lcd.clear();
    
          if (Hcount > 0)
            {
              Hcount = Hcount - 1;
            }
    
          if (Mcount > 0)
            {
              Mcount = Mcount - Mpenalty;
            }
          if (Scount > 0)
            {
              Scount = Scount - Mpenalty;
            }
        delay(1500);
        currentLength = 0;
  
        }
      }  
    }
}

void timer()
{
  Serial.print(Scount);
  Serial.println();
  
  if (Hcount <= 0)
  {
    if ( Mcount < 0 )
    {
      lcd.noCursor();
      lcd.clear();
      lcd.home();
      lcd.print("Goodbye.");
      lcd.setCursor (0,1);
      lcd.print("--------");

      delay(1000);

      lcd.clear();
      lcd.home();
      lcd.print("The bomb");
      lcd.setCursor (0,1);
      lcd.print("has exploded.");

      
      
      while (Mcount < 0) 
      {
        digitalWrite(ledPin, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin2, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin2, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin3, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin3, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100);
      }
    } 
  }

  lcd.setCursor (0,1); // sets cursor to 2nd line
  lcd.print ("Timer:");

  if (Hcount >= 10)
    {
      lcd.setCursor (7,1);
      lcd.print (Hcount);
    }
  if (Hcount < 10) 
    {
      lcd.setCursor (7,1);
      lcd.write ("0");
      lcd.setCursor (8,1);
      lcd.print (Hcount);
    }

  lcd.print (":");

  if (Mcount >= 10)
    {
      lcd.setCursor (10,1);
      lcd.print (Mcount);
    }
  if (Mcount < 10) 
    {
      lcd.setCursor (10,1);
      lcd.write ("0");
      lcd.setCursor (11,1);
      lcd.print (Mcount);
    }
    
  lcd.print (":");

  if (Scount >= 10) 
    {
      lcd.setCursor (13,1);
      lcd.print (Scount);
    }
  if (Scount < 10) 
    {
      lcd.setCursor (13,1);
      lcd.write ("0");
      lcd.setCursor (14,1);
      lcd.print (Scount);
    }

  if (Hcount <0) 
    {
      Hcount = 0; 
    }

  if (Mcount <0) 
    {
      Hcount --; 
      Mcount = 59; 
    }

  if (Scount <1) // if 60 do this operation
    {
      Mcount --; // add 1 to Mcount
      Scount = 59; // reset Scount
    }

  if (Scount > 0) // do this oper. 59 times
    {
      unsigned long currentMillis = millis();
  
      if(currentMillis - secMillis > interval) 
        {
          tone1.play(NOTE_A4, 200);
          tone2.play(NOTE_A4, 300);
          
          secMillis = currentMillis;
          Scount --; // add 1 to Scount
          digitalWrite(ledPin2, HIGH); // sets the LED on
          delay(10); // waits for a second
          digitalWrite(ledPin2, LOW); // sets the LED off
          delay(10); // waits for a second
          //lcd.clear();
        }
    }
}
