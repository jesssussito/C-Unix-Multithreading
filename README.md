# Unix System Programming: High-Performance Multithreading Manager


## 📌 Project Overview
Este repositorio aloja una implementación avanzada en **C** diseñada para demostrar la gestión eficiente de concurrencia en sistemas Unix. El proyecto simula un entorno de procesamiento paralelo donde múltiples hilos (*threads*) operan sobre recursos compartidos bajo condiciones de alta carga.

El objetivo central es la **integridad de los datos** y la **optimización del rendimiento**, resolviendo problemas críticos de sistemas operativos como la exclusión mutua y la sincronización entre procesos.

## ⚙️ Technical Architecture

### 1. Concurrencia y Paralelismo
* **Multi-threaded Design:** Ejecución simultánea de tareas para maximizar el uso de CPU en entornos multi-core.
* **Thread Lifecycle Management:** Creación, ejecución y terminación controlada de hilos.

### 2. Sincronización y Seguridad (Thread Safety)
Para garantizar la consistencia de la memoria y evitar vulnerabilidades, se implementaron mecanismos robustos de control:
* **Mutexes (Mutual Exclusion):** Protección de secciones críticas para evitar la corrupción de datos.
* **Semáforos:** Orquestación del acceso a recursos limitados y señalización entre hilos.
* **Race Condition Mitigation:** Diseño de lógica preventiva para eliminar condiciones de carrera.

### 3. Gestión de Memoria (Low-Level Memory Management)
* Uso intensivo de punteros y asignación dinámica.
* Control estricto del *Heap* para prevenir fugas de memoria (*Memory Leaks*), validable mediante herramientas como Valgrind.
