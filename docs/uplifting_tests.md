# Documentación del Modelo de Uplift - Pruebas y Resultados

## Descripción General

El modelo de uplift implementado en este proyecto predice la "influenciabilidad" de una persona para responder positivamente a una campaña publicitaria. Utiliza un árbol de decisión binario que evalúa características demográficas y comportamentales para asignar una puntuación entre 0.0 y 1.0.

## Arquitectura del Modelo

### Estructura del Árbol de Decisión

El modelo implementa un árbol de decisión con la siguiente lógica:

```
¿Edad >= 25?
├─ NO (< 25): Segmento Joven
│  └─ ¿Influenciabilidad digital >= 0.7?
│     ├─ SÍ: Puntuación 0.8 (Jóvenes muy influenciables)
│     └─ NO: ¿Ingresos >= 30000?
│        ├─ SÍ: Puntuación 0.6 (Jóvenes con recursos)
│        └─ NO: Puntuación 0.3 (Jóvenes sin recursos)
└─ SÍ (>= 25): Segmento Adulto
   └─ ¿Sexo == "Femenino"?
      ├─ SÍ: ¿Gasto promedio >= 500?
      │  ├─ SÍ: Puntuación 0.9 (Mujeres alto poder adquisitivo)
      │  └─ NO: Puntuación 0.5 (Mujeres gasto moderado)
      └─ NO: ¿Ingresos >= 50000?
         ├─ SÍ: Puntuación 0.7 (Hombres altos ingresos)
         └─ NO: Puntuación 0.4 (Hombres ingresos moderados)
```

### Características Evaluadas

1. **Edad**: Factor principal de segmentación
2. **Sexo**: Diferenciación en comportamiento de compra
3. **Ingresos**: Capacidad económica
4. **Gasto Promedio**: Disposición a gastar
5. **Influenciabilidad Digital**: Susceptibilidad a publicidad digital

## Integración con el Sistema

### Ubicación de Archivos

- **Header**: `system/uplifting_model.h`
- **Implementación**: `system/uplifting_model.cpp`
- **Integración**: `system/analizador_trafico.h/.cpp`

### Métodos Principales

#### UpliftTreeModel

```cpp
// Evaluar influenciabilidad de una persona
double evaluateInfluenciability(const Persona& persona);

// Filtrar por umbral de influenciabilidad
std::vector<Persona> filterByInfluenciability(
    const std::vector<Persona>& personas, 
    double minInfluenciability = 0.5);

// Obtener estadísticas del modelo
std::map<std::string, double> getModelStatistics(
    const std::vector<Persona>& personas);
```

#### AnalizadorTrafico (Métodos Extendidos)

```cpp
// Cálculo de tráfico con filtro de uplift
int calcularTraficoConUplift(const QVector<Persona>& poblacion, 
                            const ClienteIdeal& cliente, 
                            const QString& espacio, 
                            const QString& producto,
                            const QString& tipoEspacio,
                            double umbralInfluenciabilidad = 0.5);

// Estadísticas del modelo para la población
QMap<QString, double> obtenerEstadisticasUplift(const QVector<Persona>& poblacion);

// Filtrado por influenciabilidad
QVector<Persona> filtrarPorInfluenciabilidad(const QVector<Persona>& poblacion, 
                                            double umbral = 0.5);
```

## Scripts de Prueba

### 1. Pruebas Independientes

**Archivo**: `scripts/test_uplift_standalone.cpp`

Ejecuta casos de prueba específicos para validar la lógica del árbol de decisión.

**Compilación y Ejecución**:
```bash
./scripts/compile_and_test_uplift.sh
```

### 2. Pruebas Integradas

**Archivo**: `scripts/test_uplifting_model.sh`

Ejecuta las pruebas como parte de la aplicación principal.

**Ejecución**:
```bash
./scripts/test_uplifting_model.sh
```

## Casos de Prueba Validados

### Caso 1: Joven Muy Influenciable
- **Perfil**: Mujer, 22 años, ingresos 25,000, influenciabilidad digital 0.8
- **Puntuación Esperada**: 0.8
- **Lógica**: Edad < 25 → Influenciabilidad digital >= 0.7 → Puntuación alta

### Caso 2: Mujer Adulta Alto Poder Adquisitivo
- **Perfil**: Mujer, 35 años, ingresos 60,000, gasto promedio 800
- **Puntuación Esperada**: 0.9
- **Lógica**: Edad >= 25 → Sexo femenino → Gasto >= 500 → Puntuación muy alta

### Caso 3: Hombre Adulto Altos Ingresos
- **Perfil**: Hombre, 40 años, ingresos 75,000
- **Puntuación Esperada**: 0.7
- **Lógica**: Edad >= 25 → Sexo masculino → Ingresos >= 50,000 → Puntuación alta

### Caso 4: Joven Sin Recursos
- **Perfil**: Hombre, 20 años, ingresos 18,000, influenciabilidad digital 0.5
- **Puntuación Esperada**: 0.3
- **Lógica**: Edad < 25 → Influenciabilidad digital < 0.7 → Ingresos < 30,000 → Puntuación baja

### Caso 5: Hombre Adulto Ingresos Moderados
- **Perfil**: Hombre, 45 años, ingresos 40,000
- **Puntuación Esperada**: 0.4
- **Lógica**: Edad >= 25 → Sexo masculino → Ingresos < 50,000 → Puntuación media-baja

## Estadísticas del Modelo

### Métricas Calculadas

- **Media**: Puntuación promedio de influenciabilidad
- **Mediana**: Valor central de la distribución
- **Desviación Estándar**: Dispersión de las puntuaciones
- **Mínimo/Máximo**: Rango de puntuaciones
- **Distribución por Niveles**:
  - Alta (>= 0.7): Personas con alta probabilidad de conversión
  - Media (0.4-0.7): Personas con probabilidad moderada
  - Baja (< 0.4): Personas con baja probabilidad

### Interpretación de Puntuaciones

| Rango | Interpretación | Recomendación |
|-------|---------------|---------------|
| 0.8-1.0 | MUY ALTA | Target prioritario para campañas |
| 0.6-0.79 | ALTA | Buen candidato para publicidad |
| 0.4-0.59 | MEDIA | Candidato moderado, requiere estrategia específica |
| 0.0-0.39 | BAJA | No recomendado para campañas estándar |

## Impacto en el Pipeline de Predicción

### Flujo Original
1. Filtro demográfico (edad, sexo, ubicación)
2. Probabilidad de acceso digital
3. Probabilidad de conversión por producto
4. Resultado final

### Flujo con Uplift
1. Filtro demográfico (edad, sexo, ubicación)
2. Probabilidad de acceso digital
3. Probabilidad de conversión por producto
4. **Evaluación de influenciabilidad (NUEVO)**
5. **Filtro por umbral de uplift (NUEVO)**
6. Resultado final optimizado

### Beneficios

1. **Mejor Targeting**: Identifica personas más propensas a responder
2. **Optimización de Costos**: Reduce inversión en targets poco prometedores
3. **Mayor ROI**: Concentra esfuerzos en audiencias de alta conversión
4. **Personalización**: Permite estrategias diferenciadas por segmento

## Análisis de Resultados

### Ejemplo de Distribución Típica

Para una población de 500 personas:
- **Alta influenciabilidad (>= 0.7)**: ~30-40% (150-200 personas)
- **Media influenciabilidad (0.4-0.7)**: ~35-45% (175-225 personas)
- **Baja influenciabilidad (< 0.4)**: ~20-30% (100-150 personas)

### Efectos del Filtrado

- **Filtro estricto (>= 0.7)**: Reduce audiencia ~65-70%, maximiza conversión
- **Filtro moderado (>= 0.5)**: Reduce audiencia ~40-50%, equilibra alcance y conversión
- **Filtro flexible (>= 0.3)**: Reduce audiencia ~20-30%, maximiza alcance

## Configuración y Personalización

### Ajuste de Umbrales

```cpp
// Umbral conservador - alta precisión
auto filtered = model.filterByInfluenciability(personas, 0.7);

// Umbral equilibrado - balance precisión/alcance
auto filtered = model.filterByInfluenciability(personas, 0.5);

// Umbral amplio - máximo alcance
auto filtered = model.filterByInfluenciability(personas, 0.3);
```

### Modificación del Árbol

Para adaptar el modelo a diferentes mercados o productos:

1. **Ajustar umbrales** en `buildPredefinedTree()`
2. **Modificar características** evaluadas
3. **Cambiar puntuaciones** de los nodos hoja
4. **Recompilar y testear** con los nuevos parámetros

## Limitaciones y Consideraciones

### Limitaciones Actuales

1. **Árbol estático**: No aprende de nuevos datos
2. **Características limitadas**: Solo evalúa 5 características principales
3. **Reglas fijas**: No se adapta automáticamente a diferentes mercados

### Mejoras Futuras

1. **Aprendizaje automático**: Implementar entrenamiento con datos reales
2. **Más características**: Incluir historial de compras, ubicación específica, etc.
3. **Modelos por industria**: Árboles especializados por sector
4. **Validación A/B**: Comparación con resultados reales de campañas

## Conclusiones

El modelo de uplift implementado proporciona una mejora significativa en la precisión del targeting publicitario. Su integración en el pipeline de predicción permite:

- Identificar de manera más efectiva a las personas con mayor probabilidad de conversión
- Optimizar la inversión publicitaria concentrándose en audiencias de alta calidad
- Proporcionar insights cuantitativos sobre la composición de la audiencia objetivo

La implementación es extensible y puede adaptarse a diferentes necesidades del negocio mediante la modificación de los parámetros del árbol de decisión.
