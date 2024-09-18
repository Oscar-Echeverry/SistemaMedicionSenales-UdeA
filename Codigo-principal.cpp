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

//almacenar los valores de la señal
float *valores; //muestras de la señal
int numMuestras = 100;

void setup() {
    lcd.begin(16, 2);           //LCD de 16x2
    pinMode(pinDeSenal, INPUT);
    pinMode(botonInicio, INPUT);
    pinMode(botonInfo, INPUT);

    lcd.setCursor(0, 0);
    lcd.print("   Bienvenido   ");
    lcd.setCursor(0, 1);
    lcd.print("  Estoy Listo  ");

    valores = new float[numMuestras]; //memoria para las muestras
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
        delay(500);

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
        lcd.clear();
        lcd.setCursor(0, 0);

        // Leer la señal y determinar la forma de la onda
        leerSenal();
        String formaOnda = identificarForma();

        // forma de la onda
        lcd.print("  Forma de la  ");
        lcd.setCursor(0, 1);
        lcd.print("  onda: ");
        lcd.print(formaOnda);

        delay(500);
    }
}

// Función para leer las muestras de la señal
void leerSenal() {
    for (int i = 0; i < numMuestras; i++) {
        valores[i] = analogRead(pinDeSenal) * (5.0 / 1023.0); // Lee y convierte el valor a voltaje
    }
}

// Función para identificar la forma de la onda
String identificarForma() {
    bool esTriangular = true;
    bool esCuadrada = true;
    bool esSenoidal = true;

    // Analizamos los valores para determinar si es triangular
    for (int i = 1; i < numMuestras - 1; i++) {
        if (!((valores[i-1] < valores[i] && valores[i] > valores[i+1]) || (valores[i-1] > valores[i] && valores[i] < valores[i+1]))) {
            esTriangular = false;
            break;
        }
    }

    // Analizamos los valores para determinar si es cuadrada
    for (int i = 1; i < numMuestras - 1; i++) {
        if (!(valores[i] == valores[i-1] || valores[i] == 0 || valores[i] == 5)) {
            esCuadrada = false;
            break;
        }
    }

    // Analizamos los valores para determinar si es senoidal
    int cambios = 0;
    for (int i = 1; i < numMuestras; i++) {
        if ((valores[i-1] < valores[i] && valores[i] > valores[i+1]) || (valores[i-1] > valores[i] && valores[i] < valores[i+1])) {
            cambios++;
        }
    }
    if (cambios > 5) esSenoidal = false;

    if (esTriangular)
        return "Triangular";
    else if (esCuadrada)
        return "Cuadrada";
    else if (esSenoidal)
        return "Senoidal";
    else
        return "Desconocida";
}
