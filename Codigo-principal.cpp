//Codigo Arduino Esteban y Oscar 
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0); // Pantalla LCD en I2C
const int pinDeSenal = A0;
const int botonInicio = 2;
const int botonInfo = 3;
bool adquiriendo = false; // Estado de adquisición de datos
int ultimoDato = 0;

void setup() {
  lcd.begin(16, 2);           //LCD de 16x2
  pinMode(pinDeSenal, INPUT); 
  pinMode(botonInicio, INPUT);
  pinMode(botonInfo, INPUT); 
  
  lcd.setCursor(0, 0);
  lcd.print("   Bienvenido   ");
  lcd.setCursor(0, 1);
  lcd.print("  Estoy Listo  ");
}

void loop() {
  // estados de los botones directamente
  bool botonInicioPresionado = digitalRead(botonInicio) == HIGH;
  bool botonInfoPresionado = digitalRead(botonInfo) == HIGH; 
  // Control del botón de inicio/parada
  if (botonInicioPresionado) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Inicio   ");
    // Cambiar el estado de adquisición
    adquiriendo = !adquiriendo;
    delay(500);  // Evitar múltiples detecciones de una sola pulsación

    // Mostrar el estado actual de adquisición en la pantalla
    lcd.clear();
    if (adquiriendo) {
      lcd.setCursor(0, 0);
      lcd.print("Recolectando");
      lcd.setCursor(0, 1);
      lcd.print("   Datos   ");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("  Detenido  ");
    }
  }

  //leer del puerto analógico
  if (adquiriendo) {
    ultimoDato = analogRead(pinDeSenal);  // Lee el valor de A0
  }

  // Control del botón de información
  if (botonInfoPresionado) {
    lcd.clear();             // Limpiar pantalla
    lcd.setCursor(0, 0);
    lcd.print("  Ultimo dato:  ");
    lcd.setCursor(0, 1);
    lcd.print(ultimoDato);   //el último dato adquirido
    
    delay(500);  // Evitar múltiples detecciones de una sola pulsación
  }

  delay(100);  // Retardo para evitar demasiadas lecturas rápidas
}
