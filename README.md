PDIfich
=======

Para utiliar QT CREATOR
=======

* sudo apt-get install qtcreator
* Crear proyecto
* Editar el makefile agregando en la linea -lX11 -lfftw3
      LIBS          = $(SUBLIBS) -lQt5Core -lpthread -lX11 -lfftw3
* Click en Project (en la barra de la izquierda)
* En el título de *Build Steps* hacer click en el signo de prohibido de qmake para que no lo vuelva a generar
* Para debuguear editar el archivo 
* sudo sublime /proc/sys/kernel/yama/ptrace_scope y poner 0
y
* sudo sublime /etc/sysctl.d/10-ptrace.conf y poner en 0


And that's all, HAVE A NICE DAY
 