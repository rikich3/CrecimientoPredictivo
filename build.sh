#!/bin/bash

# Script de compilación para Publicidad Efectiva
# Uso: ./build.sh

echo "🔨 Compilando Publicidad Efectiva..."

# Configurar variables
QT_PATH="/home/rikich/Qt/6.9.0/gcc_64"
PROJECT_DIR="/home/rikich/repos/qtCreatorPublicidadEfectiva"
BUILD_DIR="$PROJECT_DIR/build"

# Limpiar build anterior si existe
if [ -d "$BUILD_DIR" ]; then
    echo "🧹 Limpiando build anterior..."
    rm -rf "$BUILD_DIR"
fi

# Crear directorio de build
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configurar CMake con Qt
echo "⚙️ Configurando CMake..."
cmake -DCMAKE_PREFIX_PATH="$QT_PATH" ..

# Compilar
echo "🚀 Compilando..."
make -j$(nproc)

# Verificar si la compilación fue exitosa
if [ $? -eq 0 ]; then
    echo "✅ Compilación exitosa!"
    echo "📋 Para ejecutar la aplicación:"
    echo "   cd $BUILD_DIR"
    echo "   ./qtCreatorPublicidadEfectiva"
else
    echo "❌ Error en la compilación"
    exit 1
fi
