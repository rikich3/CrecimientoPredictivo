#include "gestor_datos.h"
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QRegularExpression>

GestorDatos::GestorDatos()
{
    configurarEspacios();
}

void GestorDatos::configurarEspacios()
{
    // Configurar espacios geográficos (distritos de Lima representando Arequipa)
    espaciosGeograficos["Lima"] = {
        "Miraflores", "San Isidro", "Barranco", "Surco", "La Molina",
        "San Borja", "Magdalena", "Pueblo Libre", "Jesús María",
        "Lince", "Breña", "Cercado de Lima", "Rímac", "San Martín de Porres",
        "Los Olivos", "Independencia", "Comas", "Puente Piedra",
        "Carabayllo", "Ate", "Santa Anita", "El Agustino", "San Juan de Lurigancho",
        "Lurigancho", "Chaclacayo", "Villa El Salvador", "Villa María del Triunfo",
        "San Juan de Miraflores", "Surquillo", "Chorrillos"
    };
    
    // Configurar plataformas digitales
    plataformasDigitales = {"Facebook", "Google", "TikTok"};
    
    // Configurar categorías de productos
    categoriasProductos = {
        "Alimentación y Bebidas",
        "Ropa y Accesorios", 
        "Electrónicos y Tecnología",
        "Hogar y Decoración",
        "Salud y Belleza",
        "Deportes y Fitness",
        "Educación y Cursos",
        "Servicios Financieros",
        "Entretenimiento Digital",
        "Servicios Profesionales",
        "Turismo y Viajes",
        "Automotriz",
        "Artesanías y Manualidades",
        "Joyas y Bisutería",
        "Software y Aplicaciones"
    };
}

void GestorDatos::generarPoblacion(int tamaño)
{
    QRandomGenerator *random = QRandomGenerator::global();
    poblacion.clear();
    poblacion.reserve(tamaño);
    
    for (int i = 0; i < tamaño; ++i) {
        int edad = random->bounded(15, 81); // Edad entre 15 y 80
        QString sexo = obtenerSexoAleatorio();
        bool accesoInternet = obtenerAccesoInternetAleatorio(edad);
        QString distrito = obtenerDistritoAleatorio();
        
        // Generar valores aleatorios para los nuevos campos del modelo de uplift
        double ingresos = 25000 + (QRandomGenerator::global()->generateDouble() * 75000); // 25k-100k
        double influenciabilidad = 0.2 + (QRandomGenerator::global()->generateDouble() * 0.8); // 0.2-1.0
        double gasto = 150 + (QRandomGenerator::global()->generateDouble() * 1850); // 150-2000
        
        poblacion.append(Persona(i + 1, edad, sexo, accesoInternet, distrito, 
                               ingresos, distrito, influenciabilidad, gasto));
    }
}

void GestorDatos::cargarPoblacionDesdeCSV(const QString& rutaArchivo)
{
    QFile archivo(rutaArchivo);
    
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo:" << rutaArchivo;
        return;
    }
    
    QTextStream in(&archivo);
    poblacion.clear();
    
    // Saltar encabezado si existe
    if (!in.atEnd()) {
        QString primeraLinea = in.readLine();
        QRegularExpression regex("^\\d+");
        if (!regex.match(primeraLinea).hasMatch()) {
            // Es un encabezado, no procesarlo
        } else {
            // Es datos, procesarlo
            in.seek(0);
        }
    }
    
    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        if (linea.isEmpty()) continue;
        
        QStringList datos = linea.split(',');
        
        if (datos.size() >= 4) {
            int id = poblacion.size() + 1; // Generar ID secuencial
            int edad = datos[0].toInt();
            QString sexo = datos[1].trimmed();
            bool accesoInternet = (datos[2].toInt() == 1);
            QString distrito = datos[3].trimmed();
            
            // Si el CSV tiene más campos, usarlos; si no, generar valores por defecto
            double ingresos = (datos.size() > 4) ? datos[4].toDouble() : 
                              25000 + (QRandomGenerator::global()->generateDouble() * 75000);
            QString ubicacion = (datos.size() > 5) ? datos[5].trimmed() : distrito;
            double influenciabilidad = (datos.size() > 6) ? datos[6].toDouble() :
                                      0.2 + (QRandomGenerator::global()->generateDouble() * 0.8);
            double gasto = (datos.size() > 7) ? datos[7].toDouble() :
                          150 + (QRandomGenerator::global()->generateDouble() * 1850);
            
            poblacion.append(Persona(id, edad, sexo, accesoInternet, distrito, 
                                   ingresos, ubicacion, influenciabilidad, gasto));
        }
    }
    
    archivo.close();
    qDebug() << "Cargadas" << poblacion.size() << "personas desde CSV";
}

void GestorDatos::guardarPoblacionEnCSV(const QString& rutaArchivo)
{
    // Crear directorio si no existe
    QFileInfo info(rutaArchivo);
    QDir dir = info.dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    QFile archivo(rutaArchivo);
    
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo crear el archivo:" << rutaArchivo;
        return;
    }
    
    QTextStream out(&archivo);
    
    // Escribir encabezado
    out << "Edad,Sexo,AccesoInternet,Distrito\n";
    
    // Escribir datos
    for (const auto& persona : poblacion) {
        out << persona.toString() << "\n";
    }
    
    archivo.close();
    qDebug() << "Guardadas" << poblacion.size() << "personas en CSV";
}

QVector<QString> GestorDatos::obtenerDistritos() const
{
    return espaciosGeograficos.value("Lima");
}

QVector<QString> GestorDatos::obtenerPlataformasDigitales() const
{
    return plataformasDigitales;
}

QVector<QString> GestorDatos::obtenerCategoriasProductos() const
{
    return categoriasProductos;
}

QString GestorDatos::obtenerDistritoAleatorio()
{
    QVector<QString> distritos = espaciosGeograficos["Lima"];
    int indice = QRandomGenerator::global()->bounded(distritos.size());
    return distritos[indice];
}

QString GestorDatos::obtenerSexoAleatorio()
{
    return (QRandomGenerator::global()->bounded(2) == 0) ? "Masculino" : "Femenino";
}

bool GestorDatos::obtenerAccesoInternetAleatorio(int edad)
{
    double probabilidad = obtenerProbabilidadAccesoDigital(edad);
    return QRandomGenerator::global()->generateDouble() < probabilidad;
}

double GestorDatos::obtenerProbabilidadAccesoDigital(int edad)
{
    if (edad >= 17 && edad <= 24) return 0.81;
    if (edad >= 25 && edad <= 40) return 0.75;
    if (edad >= 41 && edad <= 59) return 0.60;
    if (edad >= 60) return 0.25;
    return 0.50;
}
