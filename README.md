# TP3 - CDYM

## Descripción

Este proyecto es un ejercicio práctico que utiliza Microchip Studio y Proteus para configurar y simular un microcontrolador ATMEGA328P con un sensor `DHT11`, el sensor `RTC` y la comunicación mediante el protocolo `I2C` y la `terminal serie` mediante el `UART`

## Instrucciones

### Paso 1: Abrir y compilar el proyecto en Microchip Studio

1. Abre el archivo `TP3.atsln` en Microchip Studio.
2. En la barra superior, selecciona la opción `Build`.
3. Luego, selecciona `Build TP3`.

### Paso 2: Configurar y simular el proyecto en Proteus

1. Abre el archivo `dht11YLCD.pdsprj` en Proteus.
2. Haz doble clic en el ATMEGA328P y realiza la siguiente configuración:
    - En `Program file`, selecciona el icono de la carpeta y navega hasta donde tengas tu repositorio. Entra en la carpeta `TP3`, luego en `TP3`, `Debug` y selecciona el archivo `TP3.elf`.
    - Configura `CLKDIV8` como `unprogrammed`.
    - Configura `CKSEL Fuses` como `(0000) ext. Clock`.
    - En `Advanced Properties`, selecciona `Clock Frequency` y elige `16MHz`.
3. Cierra la configuración.
4. Haz clic en el botón de `Play` en la parte inferior izquierda para iniciar la simulación.
5. Si no se abre la terminal virtual al hacer clic en `Play`, entra en el apartado de `Debug` que se encuentra en la parte superior y selecciona `Terminal Virtual`.


