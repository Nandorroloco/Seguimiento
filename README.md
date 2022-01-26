# Seguimiento
 Circuito de control para el seguimiento de una montura equatorial 

##################
La base es un arduino y un driver UNL2003 que es un array darlington para el control de un motor unipolar.

Se adjuntan los siguientes archivos:

En el directorio "/Electronica":

.- "Seguimiento_v2.BMP"  es el circuito electrónico que se ha utilizado en un simulador de circuitos

.- "Seguimiento_v2.ino"  es el Sketch de arduino. Básicamente es un bucle que comprueba los botones (pooling) y los interpreta al soltarlos (flanco de subida) lo que evita rebotes, los leds de señalización parpadean según las opciones activas, y la interrupción de reloj ocurre 40 veces entre un paso y el siguiente de la velocidad sideral (SLOW), esto es así, para tener una variante de velocidades, desde el doble a 40 veces más rápido, en función de los ticks que se dejen pasar o actuen. La interrupción se programa en base al valor que emite la hoja de cálculo que se ha utilizado para tal propósito.

.- "Calculos Seguimiento AR.xlsx" los cálculos, para definir el valor en microsegundos de la interrupción de reloj. Está inspirada en la hoja que utiliza mi colega y amigo "AstroGlider" en su proyecto https://github.com/AstroGlider/Seguimiento_AR
que también trata de lo mismo, pero usamos técnicas distintas, en mi caso evito librerías externas y programo directamente las señales del driver. De todas maneras es interesante ver esta implementación y valorarla como propuesta.

En el directorio "/fotos": alguna foto del proyecto y cacharreo vario en el que he utilizado este circuito. 




