#include <DHT.h>
#include <LiquidCrystal.h>

// Pines de los sensores
#define DHTPIN1 2      // DHT11 interior
#define DHTPIN2 10     // DHT11 exterior
#define DHTTYPE DHT11

#define LDR_PIN A0     // LDR
#define RELAY_LUZ 8    // Relé luz
#define RELAY_VENT 9   // Relé ventilador o LED

// Inicializar sensores
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
LiquidCrystal lcd(12, 11, 5, 4, 7, 6); // RS, EN, D4, D5, D6, D7

// Variables
int valorLDR = 0;
float temp1 = 0.0;
float hum1 = 0.0;
float temp2 = 0.0;
float hum2 = 0.0;

int umbralLuz = 400;   // Valor de luz (0 oscuro - 1023 muy brillante)
float tempMax = 26.0;  // Temperatura máxima antes de activar ventilador

void setup() {
  Serial.begin(9600);

  dht1.begin();  // Iniciar DHT interior
  dht2.begin();  // Iniciar DHT exterior
  lcd.begin(16, 2); // Iniciar LCD 16x2

  // Configurar pines
  pinMode(LDR_PIN, INPUT);
  pinMode(RELAY_LUZ, OUTPUT);
  pinMode(RELAY_VENT, OUTPUT);

  digitalWrite(RELAY_LUZ, LOW);
  digitalWrite(RELAY_VENT, LOW);

  lcd.print("EcoSmart Light");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Leer sensores
  valorLDR = analogRead(LDR_PIN);
  temp1 = dht1.readTemperature();
  hum1 = dht1.readHumidity();
  temp2 = dht2.readTemperature();
  hum2 = dht2.readHumidity();

  // Mostrar datos en LCD
  lcd.setCursor(0, 0);
  lcd.print("In:");
  lcd.print(temp1, 0);
  lcd.print("C ");
  lcd.print(hum1, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Out:");
  lcd.print(temp2, 0);
  lcd.print("C ");
  lcd.print(hum2, 0);
  lcd.print("%");

  // Control de luz según LDR
  if (valorLDR < umbralLuz) {
    digitalWrite(RELAY_LUZ, HIGH); // Encender luz
  } else {
    digitalWrite(RELAY_LUZ, LOW);  // Apagar luz
  }

  // Control de ventilador según temperatura interior
  if (temp1 > tempMax) {
    digitalWrite(RELAY_VENT, HIGH); // Encender ventilador
  } else {
    digitalWrite(RELAY_VENT, LOW);  // Apagar ventilador
  }

  // Mostrar datos en monitor serial
  Serial.print("LDR: ");
  Serial.print(valorLDR);
  Serial.print(" | TempIn: ");
  Serial.print(temp1);
  Serial.print("C | TempOut: ");
  Serial.print(temp2);
  Serial.println("C");

  delay(2000); // Esperar 2 segundos antes de siguiente lectura
