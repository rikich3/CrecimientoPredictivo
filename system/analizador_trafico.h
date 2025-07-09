#ifndef ANALIZADOR_TRAFICO_H
#define ANALIZADOR_TRAFICO_H

#include "../data_estructures/persona.h"
#include "uplifting_model.h"
#include <QVector>
#include <QString>
#include <QMap>
#include <QRandomGenerator>
#include <memory>

class AnalizadorTrafico
{
public:
    AnalizadorTrafico();
    
    // Métodos principales
    int calcularTrafico(const QVector<Persona>& poblacion, 
                       const ClienteIdeal& cliente, 
                       const QString& espacio, 
                       const QString& producto,
                       const QString& tipoEspacio);
    
    // Nuevo método con filtro de uplift
    int calcularTraficoConUplift(const QVector<Persona>& poblacion, 
                                const ClienteIdeal& cliente, 
                                const QString& espacio, 
                                const QString& producto,
                                const QString& tipoEspacio,
                                double umbralInfluenciabilidad = 0.5);
    
    // Criterios de inclusión
    bool cumpleCriterioInclusion(const Persona& persona, 
                                const ClienteIdeal& cliente,
                                const QString& producto, 
                                const QString& espacio,
                                const QString& tipoEspacio);
    
    // Probabilidades demográficas
    double obtenerProbabilidadAccesoDigital(int edad);
    double obtenerProbabilidadConversion(int edad, const QString& producto);
    
    // Configuración de productos
    void configurarProductos();
    
    // Métodos para análisis de uplift
    QMap<QString, double> obtenerEstadisticasUplift(const QVector<Persona>& poblacion);
    QVector<Persona> filtrarPorInfluenciabilidad(const QVector<Persona>& poblacion, 
                                                 double umbral = 0.5);
    
private:
    QMap<QString, QVector<QString>> categoriasPorTipo;
    QVector<QString> productosDigitales;
    QVector<QString> productosVisuales;
    
    // Modelo de uplift
    std::unique_ptr<UpliftModel::UpliftTreeModel> modeloUplift;
    
    // Constantes demográficas
    static const double PROB_ACCESO_JOVENES;
    static const double PROB_ACCESO_MILLENNIALS;
    static const double PROB_ACCESO_ADULTOS;
    static const double PROB_ACCESO_MAYORES;
    
    static const double CONV_JOVENES_DIGITAL;
    static const double CONV_JOVENES_GENERAL;
    static const double CONV_MILLENNIALS_DIGITAL;
    static const double CONV_MILLENNIALS_GENERAL;
    static const double CONV_ADULTOS_DIGITAL;
    static const double CONV_ADULTOS_GENERAL;
    static const double CONV_MAYORES;
    
    static const double UMBRAL_ACCESO_MINIMO;
    static const double UMBRAL_CONVERSION_MINIMO;
    
    // Métodos auxiliares
    enum GrupoEtario { JOVENES, MILLENNIALS, ADULTOS, MAYORES };
    GrupoEtario obtenerGrupoEtario(int edad);
};

#endif // ANALIZADOR_TRAFICO_H
