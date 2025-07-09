# Estructura del Proyecto - Publicidad Efectiva

## 📁 Organización de Directorios

```
qtCreatorPublicidadEfectiva/
├── 📁 data_estructures/          # Estructuras de datos y gestión
│   ├── persona.h                 # Definiciones de estructuras (Persona, ClienteIdeal, ResultadoAnalisis)
│   ├── gestor_datos.h           # Interfaz del gestor de datos
│   └── gestor_datos.cpp         # Implementación del gestor de datos
│
├── 📁 system/                    # Lógica del sistema
│   ├── analizador_trafico.h     # Interfaz del analizador de tráfico
│   └── analizador_trafico.cpp   # Implementación de algoritmos de análisis
│
├── 📁 ui/                        # Interfaz de usuario
│   ├── mainwindow.h             # Interfaz de la ventana principal
│   ├── mainwindow.cpp           # Implementación de la UI
│   └── mainwindow.ui            # Archivo de diseño Qt
│
├── 📁 build/                     # Archivos de compilación
├── main.cpp                     # Punto de entrada de la aplicación
├── CMakeLists.txt              # Configuración de compilación
├── build.sh                    # Script de compilación
├── datos_ejemplo.csv           # Datos de ejemplo
├── README.md                   # Documentación principal
└── ESTILOS.md                  # Guía de estilos
```

## 🎯 Responsabilidades por Módulo

### 📊 data_estructures/
**Propósito**: Gestión de datos y estructuras fundamentales

#### persona.h
- **Struct Persona**: Representa un individuo con edad, sexo, acceso a internet y distrito
- **Struct ClienteIdeal**: Define el perfil del cliente objetivo
- **Struct ResultadoAnalisis**: Contiene los resultados del análisis de tráfico

#### gestor_datos.h/.cpp
- **Gestión de población**: Generación, carga y guardado de datos CSV
- **Configuración de espacios**: Distritos, plataformas digitales, categorías de productos
- **Métodos auxiliares**: Generación aleatoria de datos demográficos

### ⚙️ system/
**Propósito**: Lógica de negocio y algoritmos del sistema

#### analizador_trafico.h/.cpp
- **Cálculo de tráfico**: Algoritmo principal de análisis
- **Criterios de inclusión**: Filtros demográficos y de consumo
- **Probabilidades**: Acceso digital y conversión por grupo etario
- **Configuración de productos**: Categorización digital vs visual

### 🎨 ui/
**Propósito**: Interfaz de usuario y experiencia visual

#### mainwindow.h/.cpp
- **Interfaz principal**: Configuración de widgets y layouts
- **Gestión de eventos**: Slots para interacciones del usuario
- **Estilos CSS**: Configuración visual y temas
- **Diálogos**: Splash screen y ventana de resultados

## 🔄 Flujo de Datos

```
1. main.cpp
   └── Inicializa MainWindow

2. MainWindow (ui/)
   ├── Crea GestorDatos (data_estructures/)
   ├── Crea AnalizadorTrafico (system/)
   └── Configura interfaz

3. Usuario interactúa con UI
   └── MainWindow procesa entrada

4. MainWindow llama a AnalizadorTrafico
   ├── AnalizadorTrafico usa datos de GestorDatos
   └── Aplica algoritmos de análisis

5. Resultados se muestran en UI
   └── Modal con información detallada
```

## 🔧 Dependencias entre Módulos

### data_estructures/
- **Sin dependencias externas** (solo Qt básico)
- **Provee estructuras** para otros módulos

### system/
- **Depende de**: `data_estructures/persona.h`
- **Provee**: Algoritmos de análisis

### ui/
- **Depende de**: 
  - `data_estructures/persona.h`
  - `data_estructures/gestor_datos.h`
  - `system/analizador_trafico.h`
- **Provee**: Interfaz de usuario

## 📝 Ventajas de esta Estructura

### 1. **Separación de Responsabilidades**
- Cada módulo tiene una función específica
- Facilita el mantenimiento y debugging
- Permite trabajar en paralelo en diferentes componentes

### 2. **Reutilización de Código**
- Las estructuras de datos son independientes
- Los algoritmos pueden usarse en diferentes contextos
- La UI está desacoplada de la lógica de negocio

### 3. **Escalabilidad**
- Fácil agregar nuevas estructuras de datos
- Simple extender algoritmos de análisis
- Posible crear múltiples interfaces (CLI, web, etc.)

### 4. **Testabilidad**
- Cada módulo puede probarse independientemente
- Fácil crear mocks para testing
- Algoritmos aislados de la UI

## 🚀 Compilación con Nueva Estructura

### Usando el script:
```bash
./build.sh
```

### Manual:
```bash
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=/home/rikich/Qt/6.9.0/gcc_64 ..
make
```

### Ejecución:
```bash
cd build
./qtCreatorPublicidadEfectiva
```

## 🔮 Extensiones Futuras

### Nuevos Módulos Posibles:
- **`database/`**: Integración con bases de datos
- **`network/`**: APIs externas y sincronización
- **`reports/`**: Generación de reportes avanzados
- **`config/`**: Configuraciones y preferencias
- **`tests/`**: Pruebas unitarias y de integración

### Mejoras por Módulo:
- **data_estructures/**: Cacheo, índices, persistencia avanzada
- **system/**: Machine learning, predicciones, optimizaciones
- **ui/**: Gráficos, dashboards, temas múltiples

## 📋 Checklist de Migración

- ✅ Estructuras de datos movidas a `data_estructures/`
- ✅ Lógica de sistema movida a `system/`
- ✅ Interfaz movida a `ui/`
- ✅ Includes actualizados con rutas relativas
- ✅ CMakeLists.txt actualizado
- ✅ main.cpp actualizado
- ✅ Compilación exitosa
- ✅ Funcionalidad preservada

¡La reestructuración está completa y el proyecto mantiene toda su funcionalidad!
