# Sistema de Análisis de Publicidad Efectiva

## Descripción
Sistema de escritorio desarrollado en C++ con Qt que permite analizar el tráfico potencial de publicidad en diferentes espacios (geográficos y digitales) para calcular la cantidad de clientes potenciales que podrían interesarse en un producto o servicio específico.

## Características del Sistema

### 🎯 Funcionalidades Principales
- **Análisis de Cliente Ideal**: Configuración de perfil demográfico objetivo
- **Selección de Productos**: Múltiples categorías predefinidas
- **Espacios Publicitarios**: Geográficos (distritos) y digitales (plataformas)
- **Cálculo de Tráfico**: Algoritmo basado en criterios demográficos
- **Resultados Detallados**: Interfaz visual con información completa

### 📊 Dashboard Principal
El sistema cuenta con 3 secciones principales:

#### 1. Cliente Ideal
- Rango de edad (mínima y máxima)
- Sexo (Masculino, Femenino, Cualquiera)
- Requiere acceso a internet (checkbox)

#### 2. Producto/Servicio
Categorías disponibles:
- Alimentación y Bebidas
- Ropa y Accesorios
- Electrónicos y Tecnología
- Hogar y Decoración
- Salud y Belleza
- Deportes y Fitness
- Educación y Cursos
- Servicios Financieros
- Entretenimiento Digital
- Servicios Profesionales
- Turismo y Viajes
- Automotriz
- Artesanías y Manualidades
- Joyas y Bisutería
- Software y Aplicaciones

#### 3. Región Geográfica
- **Espacios Geográficos**: Distritos de Lima (representando Arequipa)
- **Plataformas Digitales**: Facebook, Google, TikTok

## 🔬 Criterios de Inclusión

### Por Edad
- **Jóvenes (17-24 años)**: 81% acceso digital, 5% conversión general
- **Millennials (25-40 años)**: 75% acceso digital, 15% conversión general
- **Adultos (41-59 años)**: 60% acceso digital, 8% conversión general
- **Mayores (60+ años)**: 25% acceso digital, 1% conversión

### Por Sexo
- **Femenino**: Prioridad para productos visuales (25% mayor propensión)
- **Masculino**: Criterios generales por edad y producto

### Por Producto
- **Productos Digitales**: Mayor conversión para jóvenes y millennials
- **Productos Visuales**: Prioridad para segmento femenino

## 📁 Estructura del Proyecto

```
qtCreatorPublicidadEfectiva/
├── main.cpp                    # Punto de entrada
├── mainwindow.h/.cpp          # Ventana principal e interfaz
├── mainwindow.ui              # Archivo de diseño Qt
├── persona.h                  # Estructuras de datos
├── gestor_datos.h/.cpp        # Gestión de población y CSV
├── analizador_trafico.h/.cpp  # Algoritmos de análisis
├── CMakeLists.txt             # Configuración de compilación
└── README.md                  # Documentación
```

## 🛠️ Requisitos del Sistema

### Dependencias
- Qt 5.15+ o Qt 6.x
- CMake 3.16+
- Compilador C++17 compatible

### Instalación de Qt (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev cmake build-essential
```

### Instalación de Qt (Fedora/CentOS)
```bash
sudo dnf install qt6-qtbase-devel qt6-qttools-devel cmake gcc-c++
```

## 🚀 Compilación y Ejecución

### Compilar el proyecto
```bash
cd /home/rikich/repos/qtCreatorPublicidadEfectiva
mkdir build
cd build
cmake ..
make
```

### Ejecutar la aplicación
```bash
./qtCreatorPublicidadEfectiva
```

## 💾 Gestión de Datos

### Formato CSV
Los datos de población se almacenan en:
`~/.local/share/qtCreatorPublicidadEfectiva/poblacion_arequipa.csv`

### Estructura del CSV
```csv
Edad,Sexo,AccesoInternet,Distrito
25,Femenino,1,Miraflores
32,Masculino,1,San Isidro
```

### Población Simulada
- 50,000 registros generados automáticamente
- Distribución demográfica realista
- Probabilidades basadas en datos del mercado peruano

## 🎨 Interfaz de Usuario

### Características Visuales
- **Diseño Moderno**: Interfaz limpia con colores corporativos
- **Responsive**: Adaptable a diferentes tamaños de pantalla
- **Splash Screen**: Pantalla de carga inicial
- **Validación**: Controles automáticos de entrada
- **Feedback Visual**: Estados de botones y campos

### Experiencia de Usuario
1. **Configuración Intuitiva**: Formulario paso a paso
2. **Validación en Tiempo Real**: Verificación automática
3. **Resultados Claros**: Modal con información detallada
4. **Facilidad de Uso**: Tooltips y ayuda contextual

## 📈 Algoritmo de Análisis

### Proceso de Cálculo
1. **Filtrado Demográfico**: Edad, sexo, acceso a internet
2. **Ubicación**: Verificación de espacio geográfico/digital
3. **Criterios de Acceso**: Probabilidad de acceso digital (≥30%)
4. **Criterios de Conversión**: Probabilidad de compra (≥5%)
5. **Factores Especiales**: Productos visuales y género

### Supuestos del Sistema
- Persona que cumple criterios + está en tráfico = compra garantizada
- Acceso a internet implica acceso a todas las plataformas digitales
- Datos de Lima representan características de Arequipa

## 🔧 Personalización

### Agregar Nuevos Productos
Editar `gestor_datos.cpp` en el método `configurarEspacios()`:
```cpp
categoriasProductos.append("Nueva Categoría");
```

### Modificar Criterios Demográficos
Editar constantes en `analizador_trafico.cpp`:
```cpp
const double PROB_ACCESO_JOVENES = 0.81;
```

### Agregar Nuevos Espacios
Editar el mapa `espaciosGeograficos` en `gestor_datos.cpp`.

## 📊 Reportes y Análisis

### Información Mostrada
- **Clientes Potenciales**: Número total calculado
- **Parámetros de Búsqueda**: Resumen de criterios
- **Población Base**: Tamaño de muestra analizada
- **Contexto**: Información sobre metodología

### Formato de Resultados
Modal emergente con:
- Número destacado de clientes potenciales
- Detalles del análisis realizado
- Información metodológica
- Opciones para nuevo análisis

## 🤝 Contribuciones

### Desarrollo Futuro
- Exportación de resultados a PDF/Excel
- Gráficos y visualizaciones avanzadas
- Análisis comparativo entre espacios
- Integración con APIs de redes sociales
- Histórico de análisis realizados

### Arquitectura Extensible
- Separación clara de responsabilidades
- Interfaces bien definidas
- Código modular y reutilizable
- Documentación completa

## 📞 Soporte

Para soporte técnico o consultas sobre el funcionamiento del sistema, revisar:
1. Esta documentación
2. Comentarios en el código fuente
3. Logs de aplicación en modo debug

## 📄 Licencia

Este proyecto es desarrollado para uso académico y comercial según los términos establecidos por el desarrollador.
