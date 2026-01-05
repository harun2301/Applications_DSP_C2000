# Uso del filtro IIR en ASM

La función iir.asm únicamente filtra una muestra, es necesario llamarla N veces desde el main.c con la muestra x[n] a filtrar y los apuntadores al inicio del arreglo de coeficientes a y b. Dichos coeficientes se calcularon con octave y se probaron, el filtro teórico sí funciona pero no jala en asm, esto se puede comprobar con los archivos "../C suppport files/iir_filter.c" y "../Matlab support files/filter_design.m" y "../Matlab support files/filter_results.m"

Para poder ejecutar bien la función, se debe cambiar el .cmd por el que se incluye en este repo, únicamente se cambiaron las secciones de RAM asignadas a la sección .data, se comprobó que sí caben todas las señales de longitud N.

En la sección de memoria xn es necesario cargar la señal en Q12, dicha señal está en el archivo "3_tones_Q12.dat". La respusta del filtro se guarda en la sección de memoria yn. Para comprobar el funcionamiento del filtro (o su no funcionamiento :'c) será necesario graficar ambas secciones de memoria en CCS.

Ayuda por favor, mucha suerte Edgar
