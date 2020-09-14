#include "IRremote.h"
#include <LiquidCrystal.h>
#include <DHT.h>
#include <DHT_U.h>
#include <elapsedMillis.h>

int buzzer = 4;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int receiver=6;
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/*-----( Function )-----*/
String translateIR(){
  switch(results.value){
    case 0xFFA25D: return "POWER";
    case 0xFFE21D: return "FUNC/STOP";
    case 0xFF629D: return "VOL+";
    case 0xFF22DD: return "FAST BACK";
    case 0xFF02FD: return "PAUSE";
    case 0xFFC23D: return "FAST FORWARD";
    case 0xFFE01F: return "DOWN";
    case 0xFFA857: return "VOL-";
    case 0xFF906F: return "UP";
    case 0xFF9867: return "EQ";
    case 0xFFB04F: return "ST/REPT";
    case 0xFF6897: return "0";
    case 0xFF30CF: return "1";
    case 0xFF18E7: return "2";
    case 0xFF7A85: return "3";
    case 0xFF10EF: return "4";
    case 0xFF38C7: return "5";
    case 0xFF5AA5: return "6";
    case 0xFF42BD: return "7";
    case 0xFF4AB5: return "8";
    case 0xFF52AD: return "9";
    case 0xFFFFFFFF: return "REPEAT"; 
    default: 
      return "None!";
  }
}

void startBuzzer(){
  digitalWrite(buzzer,HIGH);
}

void stopBuzzer(){
  digitalWrite(buzzer,LOW);
}

void setTempAndHumidity(){
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  if(isnan(h)||isnan(t)){
    lcd.println("Failed");
    return;
  }
  lcd.println("Temp"+String(t));
  lcd.setCursor(0,1);
  lcd.println("Humidity"+String(h));
}

boolean inputingNumbers=false;
String timerTime="";
long timerExact;
boolean countingDown;
String outputLCD="Waiting";

void loop() {
  if (irrecv.decode(&results)){
    String input = translateIR();
    if(input=="VOL+"){
      startBuzzer();
    }
    if(input=="VOL-"){
      stopBuzzer();
    }
    if(input=="FUNC/STOP"){
      elapsedMillis counting;
      lcd.clear();
      setTempAndHumidity();
      while(counting<10000){
      }
    }
    if(inputingNumbers && (input=="0" || input=="1" || input=="2" || input=="3" ||input=="4" || input=="5" || input=="6" || input=="7" ||input=="8" || input=="9")){
      timerTime=timerTime+input;
      outputLCD+=input;
    }else{
      //the following are shortcuts
      if(input=="1"){
        outputLCD="  POMODORO  ";
        timerExact=25*60UL*1000+millis();
        countingDown=true;
      }else if(input=="2"){
        outputLCD=" SHORT BREAK ";
        timerExact=5*60UL*1000+millis();
        countingDown=true;
      }else if(input=="3"){
        outputLCD=" LONG BREAK  ";
        timerExact=30*60UL*1000+millis();
        countingDown=true;
      }
    }
    if(input=="ST/REPT"){
        inputingNumbers=!inputingNumbers;
        if(inputingNumbers){//start listening to the numbers 
          timerTime="";
          outputLCD="listening";
        }else{
          outputLCD="       Timer    ";
          if(!(timerTime=="")){
            timerExact=timerTime.toInt()*1000+millis();
            countingDown=true;
          }
        }
    }
    
    irrecv.resume(); // receive the next value
  }
  if(countingDown){
    if(millis()>=timerExact){
      outputLCD="Waiting";
      countingDown=false;
      startBuzzer(); 
    }
    long current=(timerExact-millis())/1000;
    int minute = current/60;
    int second=current-minute*60;
    lcd.setCursor(0,1);
    lcd.println("======"+String(minute)+":"+String(second)+"======");
  }
  lcd.setCursor(0,0);
  lcd.println(outputLCD);
  //lcd.println(getTempAndHumidity());  
  delay(500);
  lcd.clear();
}

void setup() {
  pinMode(buzzer,OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
  lcd.begin(16, 2);
  dht.begin();
}
