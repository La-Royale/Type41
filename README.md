# Type41
Type41 es un motor en 3D realizado con SDL2, OpenGL, glew, DevIL, GLM, Assimp e ImGui. Programado en C y C++17.

### Implementaciones
- Cargar modelos FBX (Drag and drop)
- Cargar texturas PNG y DDS (Drag and drop)
- 3 Componentes básicos para los GameObjects:
    -Transform: Permite ver y modificar la posición, rotación y escala
    -Mesh: Permite dibujar la maya en la escena y muestra información de esta 
    -Material: Permite aplicar la textura al objeto seleccionado así como mostrar información de esta y cargar la checker de cuadros para comprobar las UVs
- Cámara del editor(Renderizado y Movimiento)
- Ventanas del editor:
    - Menu principal:
        - File: 
            - Primitives: Permite crear distintos objetos primitivos como Cube, Sphere, Plane, Cylinder, Cone y Torus
            - Exit: Cierra el motor
        - Settings:
            - About: Te redirige a la página web del motor ([GitHub](https://github.com/La-Royale/Type41))
            - Windows: Permite mostrar/ocultar las diferentes ventanas del motor como Console, Configuration, Hierarchy e Inspector
    - Console: Da información sobre lo que está ocurriendo en el motor, enseñando los LOGS del mismo
    - Configuration: Da información sobre los fps, software y hardware utilizado
    - Hierarchy: Muestra los objetos que tenemos en la escena, pudiendo seleccionar uno para ver sus componentes en el Inspector
    - Inspector: Da información sobre los diferentes componentes del objeto seleccionado, pudiendo cambiar los valores del transform, ver información de la textura y aplicar una textura de cuadros a modo de checker, ver información de la mesh pudiendo enseñar tanto los triángulos como las caras del modelo.

### Controles: 
- Cámara:
    - RMB: Rotar cámara
    - W: Avanzar cámara
    - S: Retroceder cámara
    - D: Mover hacia la derecha la cámara
    - A: Mover hacia la izquierda la cámara
    - LSHIFT + W | A | S | D: Duplica la velocidad del movimiento de la cámara
    - MMB Scroll: zoom in || zoom out
    - ALT+LMB: Orbitar el objeto seleccionado
    - F: Centra la cámara a objeto seleccionado
    
## GitHub:
- https://github.com/La-Royale/Type41

## Créditos:
 - Alejandro Ibáñez Ramírez ([@AlejandroIR](https://github.com/AlejandroIR))
 - Raül Sánchez Rodríguez ([@Galycon](https://github.com/Galycon))
 - Guillem Montes Recasens ([@Guillem257](https://github.com/Guillem257))




