/*
  LCD Temp Sensor logger

  Logs temps to SD Card + LCD

  The circuit:
  - LCD and RTC connected pin4 + pin5 I2c bus
  - Temps OneWire to pin2
  
  created 2018
  by Stephen McBain <http://mcbainsite.duckdns.org>
*/
#include <SPI.h>
#include <SD.h>
File dataFile;

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DS3232RTC.h>
#include <DS3231.h>
// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

//Load and setup keypad

#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;
//Define key map
char keys[ROWS][COLS] = {
  {'1','2','3','4'},
  {'5','6','7','8'},
  {'9','10','11','12'},
  {'13','14','15','16'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 6, 7, 8, 9 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 2, 3, 4, 5 };

#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 17 
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

#define I2C_ADDR 0x27  // Add your address here.
#define Rs_pin 0
#define Rw_pin 1
#define En_pin 2
#define BACKLIGHT_PIN 3
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
const int allert=25;
void setup()
{
lcd.begin (20,4); //  our LCD is a 20x4, change for your LCD if needed

// LCD Backlight ON
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);

    setSyncProvider(RTC.get);  // Library function to get the time from the RTC module.
    if(timeStatus() != timeSet) 
        Serial.println("System Time Cannot be Set. Check Connections.");
    else
        Serial.println("System Time is Set.");
// Initialize the rtc object
rtc.begin();
// Start up the sensors library 
 sensors.begin();

  //Welcome Message
  lcd.home(); // go home on LCD
  lcd.print("TheMcBain Temp V0.3");
  delay(4000);
  
//SetupSD Card
  Serial.print("\nInitializing SD card...");
  lcd.setCursor(0,2);
  lcd.print("SD Card...");
  //Change 10 below to Set CS Pin
  if (!SD.begin(10)) {
    Serial.println("SD inisalisation Failed");
    lcd.print("ERROR!");
    delay(2000);
    headderNOSD();
    return;
  } else {
    dataFile = SD.open("startup.txt", FILE_WRITE);
    dataFile.print("Startup logged.....");
    dataFile.print(rtc.getDateStr());
    dataFile.print(", ");
    dataFile.println(rtc.getTimeStr());
    dataFile.close();
    SD.remove("temps.txt");
    Serial.println("SD Ready");
    dataFile = SD.open("temps.txt", FILE_WRITE);
    dataFile.println("Temprature Log");
    dataFile.println("Written By Stephen McBain");
    dataFile.println();
    dataFile.println(rtc.getDateStr());
    dataFile.println(" ");
    dataFile.println("Data");
    dataFile.println();
    dataFile.close();
    lcd.print("Ready!");
    delay(2000);
    headder();
  } 

}

void headder()
{
  lcd.clear();
  lcd.home();
  lcd.print("Temp:    S1:");
  lcd.setCursor(9,1);
  lcd.print("S2:");
    lcd.setCursor(9,2);
  lcd.print("S3:");
    lcd.setCursor(9,3);
  lcd.print("S4:");
  lcd.setCursor(0,2);
  lcd.print("Logging");
  lcd.setCursor(0,3);
  lcd.print("Active");
}

void headderNOSD()
{
  lcd.clear();
  lcd.home();
  lcd.print("Temp:    S1:");
  lcd.setCursor(9,1);
  lcd.print("S2:");
    lcd.setCursor(9,2);
  lcd.print("S3:");
    lcd.setCursor(9,3);
  lcd.print("S4:");
  lcd.setCursor(0,2);
  lcd.print("Logging");
  lcd.setCursor(0,3);
  lcd.print("in-Active");
}

void loop()
{   
    sensors.requestTemperatures(); // Send the command to get temperature readings 
    //Sensor 1
    lcd.setCursor(12,0);
        if (sensors.getTempCByIndex(0) == -127.00){
      lcd.print("N/A  ");
    } else {
    if (sensors.getTempCByIndex(0) >= allert){
      lcd.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
         // You can have more than one DS18B20 on the same bus.  
        // 0 refers to the first IC on the wire
      lcd.print("*");} else {
      lcd.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
        // You can have more than one DS18B20 on the same bus.  
        // 0 refers to the first IC on the wire
      lcd.setCursor(17,0);
      lcd.print("   ");
      }
    }
    //Sensor 2
    lcd.setCursor(12,1);
    if (sensors.getTempCByIndex(1) == -127.00){
      lcd.print("N/A  ");
    } else {
    if (sensors.getTempCByIndex(1) >= allert){
      lcd.print(sensors.getTempCByIndex(1)); // Why "byIndex"?  
        // You can have more than one DS18B20 on the same bus.  
        // 0 refers to the first IC on the wire
      lcd.print("*");} else {
      lcd.print(sensors.getTempCByIndex(1)); // Why "byIndex"?  
        // You can have more than one DS18B20 on the same bus.  
        // 0 refers to the first IC on the wire 
      lcd.setCursor(17,1);
      lcd.print(" ");
      }    
    }
    //Sensor 3
    lcd.setCursor(12,2);
    if (sensors.getTempCByIndex(2) == -127.00){
      lcd.print("N/A  ");
    } else {
    if (sensors.getTempCByIndex(2) >= allert){
      lcd.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
        // You can have more than one DS18B20 on the same bus.  
        // 0 refers to the first IC on the wire
      lcd.print("*");} else {
      lcd.print(sensors.getTempCByIndex(2)); // Why "byIndex"?  
        // You can have more than one DS18B20 on the same bus.  
        // 0 refers to the first IC on the wire
      lcd.setCursor(17,2);
      lcd.print(" ");
      }     
    }
    //Sensor 4
    lcd.setCursor(12,3);
    if (sensors.getTempCByIndex(3) == -127.00){
      lcd.print("N/A  ");
    } else {
      if (sensors.getTempCByIndex(3) >= allert){
      lcd.print(sensors.getTempCByIndex(3)); // Why "byIndex"?  
        // You can have more than one DS18B20 on the same bus.  
        // 0 refers to the first IC on the wire
      lcd.print("*");} else {
      lcd.print(sensors.getTempCByIndex(3)); // Why "byIndex"?  
        // You can have more than one DS18B20 on the same bus.  
      // 0 refers to the first IC on the wire 
      lcd.setCursor(17,3);
      lcd.print(" ");  
      }   
    }
 if (minute() == 00 || minute() == 15 || minute() == 30 || minute() == 45)
 {
    sensors.requestTemperatures(); // Send the command to get temperature readings  
    /********************************************************************/

    dataFile = SD.open("temps.txt", FILE_WRITE);
    dataFile.print(rtc.getTimeStr());
    dataFile.print(" ");
    dataFile.print("S1:");
    dataFile.print(" ");
    dataFile.print(sensors.getTempCByIndex(0));
    dataFile.print(" ");
    dataFile.print("S2:");
    dataFile.print(" ");
    dataFile.println(sensors.getTempCByIndex(1));
    dataFile.print(" ");
    dataFile.print("S3:");
    dataFile.print(" ");
    dataFile.print(sensors.getTempCByIndex(2));
    dataFile.print(" ");
    dataFile.print("S4:");
    dataFile.print(" ");
    dataFile.println(sensors.getTempCByIndex(3));
    dataFile.close();   
  }
}

