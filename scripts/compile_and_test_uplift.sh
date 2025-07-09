#!/usr/bin/env bash

# compile_and_test_uplift.sh
# Script para compilar y ejecutar las pruebas independientes del modelo de uplift

echo "=== COMPILADOR Y EJECUTOR DE PRUEBAS INDEPENDIENTES DEL MODELO DE UPLIFT ==="

# Ir al directorio del proyecto
cd "$(dirname "$0")/.."

echo "Compilando pruebas independientes del modelo de uplift..."

# Compilar el programa de pruebas independiente simplificado
g++ -std=c++17 \
    scripts/test_uplift_standalone_simple.cpp \
    -o scripts/test_uplift_standalone

if [ $? -ne 0 ]; then
    echo "Error: Falló la compilación de las pruebas independientes"
    exit 1
fi

echo "Compilación exitosa!"
echo ""

echo "Ejecutando pruebas independientes..."
echo "====================================="

# Ejecutar las pruebas
./scripts/test_uplift_standalone

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ Todas las pruebas se ejecutaron correctamente"
else
    echo ""
    echo "✗ Las pruebas encontraron errores"
    exit 1
fi

# Limpiar archivos temporales
rm -f ./scripts/test_uplift_standalone

echo ""
echo "=== PRUEBAS INDEPENDIENTES COMPLETADAS ==="
