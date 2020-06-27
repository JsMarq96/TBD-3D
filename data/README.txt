Juan S. Marquerie - 192631
Enlace de Youtube: https://www.youtube.com/watch?v=YBdEydwaXGs&feature=youtu.be

Cosas que habria enfocado de manera distinta:
    Aprender a usar y estructurar un proyecto en DOP ha sido divertido, entretenido, y a veces, un poco frustrante; sobretodo al principio;
    Pero, una vez encontre una estrucutra decente, el desarrollo trascurrio sin mucho problema hasta el final; en el cual tuve que incluir un
    singleton para la estructura del jugador; por la falta de flexibilidad de la estructura que eleji.

    Habria intentado buscar mas rapido una pareja, ya que ha sido una gran cantidad de trabajo para hacer, y el tiempo y las entregas han ido justas.


La IA Enemiga:
    El metodo Update del enemigo ha quedado algo spagetti, asi que lo definire aqui con msa calma:
    Es una maquina de estados con estados:
        -STOPPED: COn una posiblidad del 50%, traza una ruta a un pto cercanno aleatorio y pasa al estado ROAM.
        -ROAM: Sigue la ruta trazada por el A* de STOPPED. Cuando llega a el final, va a STOPPED.
        -RUNAFTER: Si ve al protagonista, corre hacia el. Si lo pierde de vista, pasa al estado STOPPED.
        -ATTACk: Si esta cerca del jugador, procede a atacarlo.
        -RECOVERING: Si la salud del enemigo llega a 0, se vulve transparente y se va gradualmente volviendo opaco. Cunado esto ocurre, pasa a estado STOPPED.

    El trazado de rutas se hace en una replica del mapa, en la que se rasterizan las areas de los elementos; y se mueve usando A*.
    Para saber si el jugador esta en su campo de vision, primero comporvamos si el angulo entre el jugador y el front del enemigo esta dentro del campo de vision
    y si esta, procede a rasterizar una linea en el mapa, entre el jugador y el enemigo, y si ahi algun elemento entre medias, se concluye con que no loha visto.

Problemas del desarrollo:
    No he podido hacer un buen dsseno de niveles, ya que para ello usaba Blender, y mientras lo disenaba, al actualizar el OS y el Blender, cada vez que 
    lo lanzaba se congelaba el ordenador. Probe instalarme versiones anteriores y pasar el kernel a una versiona nterior, pero no lo consegui, y depues de echarle muchas horas,
    decidi dejarlo por perdido, y ponerme con otras cosas.
