# Importar las bibliotecas necesarias
import serial
import decouple
import requests
from datetime import datetime, timedelta

# Configuración de las credenciales de ThingSpeak
ID_CANAL = decouple.config("ID_CANAL")
API_ESCRITURA = decouple.config("API_ESCRITURA")
RESPUESTA_CORRECTA = {"success": True}

# Configuración del puerto serial
PUERTO = "COM5"
VELOCIDAD = 9600
TIMEOUT = 1
INTERVALO_LECTURA = 1


# Función para obtener la hora actual
def obtener_hora_actual(segundos_adicionales=0):
    hora_actual = datetime.now() + timedelta(seconds=segundos_adicionales)
    hora_formateada = hora_actual.strftime("%Y-%m-%d %H:%M:%S")
    hora_formateada = hora_formateada + " -0500"
    return hora_formateada


# Función para formatear los datos a enviar
def formatear_datos(datos_arduino):
    datos = [
        {"created_at": obtener_hora_actual(INTERVALO_LECTURA * i), "field1": dato}
        for i, dato in enumerate(datos_arduino)
    ]
    return datos


# Iniciar el programa
try:
    # Inicializar la comunicación serial y con la API
    puerto_serial = serial.Serial(PUERTO, VELOCIDAD, timeout=TIMEOUT)
    datos_thingspeak = {"write_api_key": API_ESCRITURA, "updates": []}
    solicitud_api = f"https://api.thingspeak.com/channels/{ID_CANAL}/bulk_update.json"

    while True:
        # Leer los datos del puerto serial
        datos_seriales = puerto_serial.readline().decode().strip()

        # Verificar si se recibieron datos válidos
        if datos_seriales:
            # Formatear los datos
            lista_datos = datos_seriales.split(",")
            datos_thingspeak["updates"] = formatear_datos(lista_datos)

            # Enviar los datos a la API de ThingSpeak
            solicitud = requests.post(solicitud_api, json=datos_thingspeak)
            respuesta = solicitud.json()

            # Verificar si los datos se enviaron correctamente
            if respuesta == RESPUESTA_CORRECTA:
                print("Datos enviados correctamente")
            else:
                print("Error al enviar datos: {respuesta}")
except KeyboardInterrupt:
    # Manejar la finalizacion del programa
    print("Programa interrumpido por el usuario.")
except Exception as error:
    # Manejar cualquier otro error que ocurra durante la ejecución
    print(f"Se produjo un error: {error}.")
finally:
    # Cerrar la conexión con el puerto serial
    puerto_serial.close()
