//Codigo Arduino Esteban y Oscar 
#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd(0); // Pantalla LCD en I2C
const int pinDeSenal = A0;
const int botonInicio = 2;
const int botonInfo = 3;
bool adquiriendo = false; // Estado de adquisición de datos
int ultimoDato = 0;
float voltaje = 0.0;
float frecuencia = 0.0;
unsigned long tiempoAnterior = 0;
unsigned long tiempoActual = 0;
float periodo = 0.0;
unsigned long tiempoLecturaAnterior = 0; // Control de tiempo de lectura

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
    adquiriendo = !adquiriendo; // Cambiar el estado de adquisición
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

  // Proceso de adquisición de datos
  if (adquiriendo) {
    unsigned long tiempoActual = millis();
    
    // Solo ejecuta la lectura si ha pasado 1 segundo desde la última lectura
    if (tiempoActual - tiempoLecturaAnterior >= 1000) {
      // Leer el valor analógico
      ultimoDato = analogRead(pinDeSenal);
    
      // Convertir a voltaje(Amplitud)
      voltaje = (ultimoDato * 5.0) / 1023.0;

      // Calcular el periodo y la frecuencia (simplificado, usando millis())
      periodo = (tiempoActual - tiempoAnterior) / 1000.0; // En segundos
      if (periodo > 0) {
        frecuencia = 1.0 / periodo;
      }
      tiempoAnterior = tiempoActual;

      // Mostrar Amplitud y frecuencia en la pantalla LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Amplitud: ");
      lcd.print(voltaje, 2); // 2 decimales para el voltaje
      lcd.setCursor(0, 1);
      lcd.print("Frecuencia: ");
      lcd.print(frecuencia, 2); // 2 decimales para la frecuencia

      // Actualizar el tiempo de la última lectura
      tiempoLecturaAnterior = tiempoActual;
    }
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
}