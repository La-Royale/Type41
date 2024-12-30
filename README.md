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

### Nuevas Implementaciones
- La escena Street Environment se carga automáticamente al inicio.  
- Inspector:  
   - Jerarquía: eliminar, reasignar como hijo, crear objetos vacíos y crear hijos, además Se puede renombrar y eliminar los GameObjects. 
   - Transformación: trasladar, rotar y escalar objetos.  
   - Malla: seleccionar o arrastrar cualquier malla importada.  
   - Textura: seleccionar o arrastrar cualquier textura importada.  
   - Cámara: es un componente con configuraciones que se pueden modificar.  

- Los GameObjects pueden seleccionarse en el mundo utilizando el ratón. El raycasting esta configurado para selecionar el GameObject mas cercano.
- Todas las mallas utilizan un volumen delimitador (AABB) y se pueden descartar mediante Frustum Culling. Este proceso debe visualizarse en el editor (debug de raycasting y cajas).  
- El usuario puede Iniciar / Pausar / Detener la simulación y recuperar su estado original, además los GameObjects pueden ser estáticos o dinámicos. 

- Gestión de Recursos 
   - Hay una ventana de “Assets” que muestra todos los recursos del usuario (puede ser una visualización sencilla con un treeview).  
   - El usuario puede arrastrar/importar nuevos archivos, y la ventana reacciona adecuadamente.  
   - Los archivos pueden eliminarse a través de la ventana, y las entradas en /Library se eliminan.  
   - Al iniciar, todos los recursos no gestionados dentro de “Assets” se generan en Library.  
   - La carpeta /Library debe regenerarse a partir de /Assets + contenido meta.  
   - Todos los recursos utilizan conteo de referencias (por ejemplo, una textura o malla solo está en memoria una vez, independientemente de cuántos GameObjects la utilicen). Este proceso debe ser visible en el editor (mostrando el contador de referencias, etc.).  

#### Opcionalmente:  

- Ventana de exploración de assets:  
   - El usuario debe poder navegar por carpetas y ver una representación de los archivos.  
   - El usuario debe poder eliminar archivos a través de la ventana (y la carpeta Library se actualiza en consecuencia).  

- Implementación básica de opciones de importación:  
   - Texturas: filtrado, wrapping, invertir (X/Y).  
   - Modelos: escala global, ejes, ignorar cámaras/luces.  
   - Las opciones de importación deben guardarse como información .meta para que /Library se genere correctamente.  

### Controles: 
- Cámara:
    - RMB: Rotar cámara
    - W: Avanzar cámara
    - S: Retroceder cámara
    - D: Mover hacia la derecha la cámara
    - A: Mover hacia la izquierda la cámara
    - LSHIFT + W | A | S | D: Duplica la velocidad del movimiento de la cámara
    - MMB Scroll: zoom in || zoom out
    - MMB Desplazamiento de la cámara
    - ALT+LMB: Orbitar el objeto seleccionado
    - F: Centra la cámara a objeto seleccionado
    
## GitHub:
- https://github.com/La-Royale/Type41

## Créditos:
 - Alejandro Ibáñez Ramírez ([@AlejandroIR](https://github.com/AlejandroIR))
 - Raül Sánchez Rodríguez ([@Galycon](https://github.com/Galycon))
 - Guillem Montes Recasens ([@Guillem257](https://github.com/Guillem257))




