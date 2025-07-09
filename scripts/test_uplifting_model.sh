#!/usr/bin/env bash

# test_uplifting_model.sh
# Script para compilar y ejecutar las pruebas del modelo de uplift

echo "=== SCRIPT DE PRUEBAS DEL MODELO DE UPLIFT ==="
echo "Compilando el proyecto..."

# Ir al directorio del proyecto
cd "$(dirname "$0")"

# Crear directorio de build si no existe
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configurar y compilar con CMake
cmake .. -DCMAKE_BUILD_TYPE=Debug
if [ $? -ne 0 ]; then
    echo "Error: Falló la configuración de CMake"
    exit 1
fi

make -j$(nproc)
if [ $? -ne 0 ]; then
    echo "Error: Falló la compilación"
    exit 1
fi

echo "Compilación exitosa!"
echo ""

# Ejecutar las pruebas si el binario existe
if [ -f "./qtCreatorPublicidadEfectiva" ]; then
    echo "Ejecutando el programa principal (incluye pruebas del modelo de uplift)..."
    echo "Nota: Las pruebas se ejecutan automáticamente al iniciar la aplicación"
    echo ""
    
    # Crear un archivo temporal para ejecutar solo las pruebas
    cat > test_mode.txt << EOF
MODO_PRUEBA_UPLIFT=1
EOF
    
    # Ejecutar el programa
    ./qtCreatorPublicidadEfectiva
    
    # Limpiar archivo temporal
    rm -f test_mode.txt
else
    echo "Error: No se encontró el ejecutable qtCreatorPublicidadEfectiva"
    exit 1
fi

echo ""
echo "=== PRUEBAS COMPLETADAS ==="
