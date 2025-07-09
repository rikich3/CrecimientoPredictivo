#ifndef GESTOR_DATOS_H
#define GESTOR_DATOS_H

#include "../data_estructures/persona.h"
#include <QVector>
#include <QString>
#include <QMap>

class GestorDatos
{
public:
    GestorDatos();
    
    // Gestión de población
    void generarPoblacion(int tamaño = 50000);
    void cargarPoblacionDesdeCSV(const QString& rutaArchivo);
    void guardarPoblacionEnCSV(const QString& rutaArchivo);
    
    // Acceso a datos
    const QVector<Persona>& obtenerPoblacion() const { return poblacion; }
    QVector<QString> obtenerDistritos() const;
    QVector<QString> obtenerPlataformasDigitales() const;
    QVector<QString> obtenerCategoriasProductos() const;
    
    // Configuración de espacios
    void configurarEspacios();
    
private:
    QVector<Persona> poblacion;
    QMap<QString, QVector<QString>> espaciosGeograficos;
    QVector<QString> plataformasDigitales;
    QVector<QString> categoriasProductos;
    
    // Métodos auxiliares
    QString obtenerDistritoAleatorio();
    QString obtenerSexoAleatorio();
    bool obtenerAccesoInternetAleatorio(int edad);
    double obtenerProbabilidadAccesoDigital(int edad);
};

#endif // GESTOR_DATOS_H
