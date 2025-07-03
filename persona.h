#ifndef PERSONA_H
#define PERSONA_H

#include <QString>

// Estructura para representar una persona
struct Persona {
    int edad;
    QString sexo;
    bool accesoInternet;
    QString distrito;
    
    Persona(int e = 0, const QString& s = "", bool ai = false, const QString& d = "") 
        : edad(e), sexo(s), accesoInternet(ai), distrito(d) {}
    
    // Métodos para comparación y conversión
    bool operator==(const Persona& other) const {
        return edad == other.edad && sexo == other.sexo && 
               accesoInternet == other.accesoInternet && distrito == other.distrito;
    }
    
    QString toString() const {
        return QString("%1,%2,%3,%4").arg(edad).arg(sexo).arg(accesoInternet ? 1 : 0).arg(distrito);
    }
};

// Estructura para el perfil del cliente ideal
struct ClienteIdeal {
    int edadMin;
    int edadMax;
    QString sexo;
    bool requiereInternet;
    
    ClienteIdeal(int eMin = 18, int eMax = 65, const QString& s = "Cualquiera", bool ri = true)
        : edadMin(eMin), edadMax(eMax), sexo(s), requiereInternet(ri) {}
    
    bool cumpleRequisitos(const Persona& persona) const {
        if (persona.edad < edadMin || persona.edad > edadMax) return false;
        if (sexo != "Cualquiera" && persona.sexo != sexo) return false;
        if (requiereInternet && !persona.accesoInternet) return false;
        return true;
    }
};

// Estructura para el resultado del análisis
struct ResultadoAnalisis {
    int clientesPotenciales;
    QString espacio;
    QString producto;
    QString tipoEspacio;
    ClienteIdeal clienteIdeal;
    QString detalles;
    
    ResultadoAnalisis(int cp = 0, const QString& e = "", const QString& p = "", 
                     const QString& te = "", const ClienteIdeal& ci = ClienteIdeal())
        : clientesPotenciales(cp), espacio(e), producto(p), tipoEspacio(te), clienteIdeal(ci) {}
};

#endif // PERSONA_H
