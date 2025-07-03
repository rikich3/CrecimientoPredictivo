#include "analizador_trafico.h"

// Constantes demográficas
const double AnalizadorTrafico::PROB_ACCESO_JOVENES = 0.81;
const double AnalizadorTrafico::PROB_ACCESO_MILLENNIALS = 0.75;
const double AnalizadorTrafico::PROB_ACCESO_ADULTOS = 0.60;
const double AnalizadorTrafico::PROB_ACCESO_MAYORES = 0.25;

const double AnalizadorTrafico::CONV_JOVENES_DIGITAL = 0.15;
const double AnalizadorTrafico::CONV_JOVENES_GENERAL = 0.05;
const double AnalizadorTrafico::CONV_MILLENNIALS_DIGITAL = 0.25;
const double AnalizadorTrafico::CONV_MILLENNIALS_GENERAL = 0.15;
const double AnalizadorTrafico::CONV_ADULTOS_DIGITAL = 0.12;
const double AnalizadorTrafico::CONV_ADULTOS_GENERAL = 0.08;
const double AnalizadorTrafico::CONV_MAYORES = 0.01;

const double AnalizadorTrafico::UMBRAL_ACCESO_MINIMO = 0.30;
const double AnalizadorTrafico::UMBRAL_CONVERSION_MINIMO = 0.05;

AnalizadorTrafico::AnalizadorTrafico()
{
    configurarProductos();
}

void AnalizadorTrafico::configurarProductos()
{
    // Productos digitales (mayor conversión digital)
    productosDigitales = {
        "Electrónicos y Tecnología",
        "Educación y Cursos",
        "Entretenimiento Digital",
        "Servicios Financieros",
        "Software y Aplicaciones"
    };
    
    // Productos visuales (prioridad femenina)
    productosVisuales = {
        "Ropa y Accesorios",
        "Hogar y Decoración",
        "Salud y Belleza",
        "Artesanías y Manualidades",
        "Joyas y Bisutería"
    };
}

int AnalizadorTrafico::calcularTrafico(const QVector<Persona>& poblacion,
                                      const ClienteIdeal& cliente,
                                      const QString& espacio,
                                      const QString& producto,
                                      const QString& tipoEspacio)
{
    int contador = 0;
    
    for (const auto& persona : poblacion) {
        if (cumpleCriterioInclusion(persona, cliente, producto, espacio, tipoEspacio)) {
            contador++;
        }
    }
    
    return contador;
}

bool AnalizadorTrafico::cumpleCriterioInclusion(const Persona& persona,
                                               const ClienteIdeal& cliente,
                                               const QString& producto,
                                               const QString& espacio,
                                               const QString& tipoEspacio)
{
    // 1. Verificar si cumple el perfil del cliente ideal
    if (!cliente.cumpleRequisitos(persona)) {
        return false;
    }
    
    // 2. Verificar ubicación/espacio
    if (tipoEspacio == "Espacio Geográfico") {
        if (persona.distrito != espacio) {
            return false;
        }
    } else { // Plataforma Digital
        if (!persona.accesoInternet) {
            return false;
        }
    }
    
    // 3. Aplicar criterios específicos por edad
    double probAcceso = obtenerProbabilidadAccesoDigital(persona.edad);
    if (probAcceso < UMBRAL_ACCESO_MINIMO) {
        return false;
    }
    
    // 4. Aplicar criterios de conversión
    double probConversion = obtenerProbabilidadConversion(persona.edad, producto);
    if (probConversion < UMBRAL_CONVERSION_MINIMO) {
        return false;
    }
    
    // 5. Criterio especial para productos visuales y sexo femenino
    if (productosVisuales.contains(producto) && persona.sexo == "Femenino") {
        return true; // Prioridad para productos visuales y mujeres
    }
    
    return true;
}

double AnalizadorTrafico::obtenerProbabilidadAccesoDigital(int edad)
{
    GrupoEtario grupo = obtenerGrupoEtario(edad);
    
    switch (grupo) {
        case JOVENES:
            return PROB_ACCESO_JOVENES;
        case MILLENNIALS:
            return PROB_ACCESO_MILLENNIALS;
        case ADULTOS:
            return PROB_ACCESO_ADULTOS;
        case MAYORES:
            return PROB_ACCESO_MAYORES;
        default:
            return 0.50;
    }
}

double AnalizadorTrafico::obtenerProbabilidadConversion(int edad, const QString& producto)
{
    GrupoEtario grupo = obtenerGrupoEtario(edad);
    bool esDigital = productosDigitales.contains(producto);
    
    switch (grupo) {
        case JOVENES:
            return esDigital ? CONV_JOVENES_DIGITAL : CONV_JOVENES_GENERAL;
        case MILLENNIALS:
            return esDigital ? CONV_MILLENNIALS_DIGITAL : CONV_MILLENNIALS_GENERAL;
        case ADULTOS:
            return esDigital ? CONV_ADULTOS_DIGITAL : CONV_ADULTOS_GENERAL;
        case MAYORES:
            return CONV_MAYORES;
        default:
            return 0.05;
    }
}

AnalizadorTrafico::GrupoEtario AnalizadorTrafico::obtenerGrupoEtario(int edad)
{
    if (edad >= 17 && edad <= 24) return JOVENES;
    if (edad >= 25 && edad <= 40) return MILLENNIALS;
    if (edad >= 41 && edad <= 59) return ADULTOS;
    return MAYORES;
}
