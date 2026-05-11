#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// ===== PIN CONFIG =====
#define ONE_WIRE_BUS 10
#define BUZZER_PIN 8
#define FAN_PIN 9
#define LED_PIN 11
#define LCD_LIGHT 13   // 🔥 LCD backlight control

// LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// OneWire setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ===== SETTINGS =====
float thresholdTemp = 35.0;

void setup() {
  Serial.begin(9600);

  sensors.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LCD_LIGHT, OUTPUT);  // 🔥 backlight pin

  // Ensure all OFF at start
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(LCD_LIGHT, LOW); // backlight OFF initially

  lcd.begin(16, 2);
  lcd.print("Fire System");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read temperature
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  Serial.print("Temp: ");
  Serial.println(temp);

  // Sensor error
  if (temp == -127.0) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error   ");
    lcd.setCursor(0, 1);
    lcd.print("Check Wiring   ");

    digitalWrite(FAN_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LCD_LIGHT, LOW);

    delay(1000);
    return;
  }

  // Display temp
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print((char)223);
  lcd.print("C   ");

  // ===== LOGIC =====
  if (temp >= thresholdTemp) {
    // 🔥 FIRE ALERT
    lcd.setCursor(0, 1);
    lcd.print("ALERT! FIRE!!! ");

    digitalWrite(FAN_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(LCD_LIGHT, HIGH); // 💡 ON

  } else {
    // ✅ NORMAL
    lcd.setCursor(0, 1);
    lcd.print("Status: Normal ");

    digitalWrite(FAN_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LCD_LIGHT, LOW); // 💡 OFF
  }

  delay(500);
}