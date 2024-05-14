// Incluye la librería para el sensor de carga HX711
#include "HX711.h"  

// Define el tamaño máximo de la lista de mediciones
#define MAX_LIST_SIZE 10 

// Define los pines para el motor paso a paso y los pines para el sensor de carga
const int stepPin = 3;
const int dirPin = 4;
const int start = 7;
const int LOADCELL_DOUT_PIN = A4;
const int LOADCELL_SCK_PIN = A5;

// Define un listado de prueba
long lista[MAX_LIST_SIZE];
long mock[] = {
  0, 0, 0, 0, 0, 100, 210, 1220, 1420, 1480
};

// Define el número de elementos en la lista
int numElementos = 0;

// Objeto para el sensor de carga HX711
HX711 hx711;

// Funcion de configuración
void setup() {
  // Configura los pines del motor y el sensor
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, INPUT);
  digitalWrite(8, HIGH);

  Serial.begin(9600);  // Inicia la comunicación serial a 9600 baudios
  hx711.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);  // Inicializa el sensor HX711
}

// Función de bucle
void loop() {
  // Establece la dirección del motor
  digitalWrite(dirPin, HIGH);  

  // Si se activa el pin 7
  if (digitalRead(7) == HIGH) {

    // Realiza una serie de movimientos del motor
    for (int j = 0; j < 9; j++) {
      // Realiza una serie de pasos del motor
      for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(2000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(2000);
      }

      // Lee la medición del sensor de carga o usa un valor de prueba y lo agrega a la lista
      if (hx711.is_ready()) {
        long reading = hx711.read();
        agregarElemento(mock[j]);
      } else {
        agregarElemento(mock[j]);
      }
      
      // Espera antes de la próxima medición
      delay(1000);  
    }

    // Cambia la dirección del motor
    digitalWrite(dirPin, LOW);  

    // Realiza una serie de movimientos hacia atrás del motor sin agregar mediciones
    for (int j = 0; j < 5; j++) {
      for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(2000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(2000);
      }
    }
    
    // Imprime y limpia la lista de mediciones
    imprimirYLimpiarLista();  
  }
}

// Función para agregar un elemento a la lista
void agregarElemento(long valor) {
  if (numElementos < MAX_LIST_SIZE) {
    lista[numElementos] = valor;
    numElementos++;
  }
}

// Función para imprimir y limpiar la lista de mediciones
void imprimirYLimpiarLista() {
  for (int i = 0; i < numElementos; i++) {
    Serial.print(lista[i]);
    Serial.print(",");
  }
  numElementos = 0;
}
