#ifndef PERSONA_H
#define PERSONA_H

#include <QString>

// Estructura para representar una persona
struct Persona {
    int id;                              // ID único de la persona
    int edad;                            // Edad de la persona
    QString sexo;                        // Sexo: "Masculino" o "Femenino"
    bool accesoInternet;                 // Acceso a internet
    QString distrito;                    // Distrito/ubicación geográfica
    
    // Nuevos campos para el modelo de uplift
    double ingresos;                     // Ingresos anuales
    QString ubicacion;                   // Ubicación general (puede ser igual a distrito)
    double influenciabilidad_digital;   // Factor de influenciabilidad digital (0.0-1.0)
    double gasto_promedio;              // Gasto promedio mensual
    
    Persona(int i = 0, int e = 0, const QString& s = "", bool ai = false, const QString& d = "",
            double ing = 30000.0, const QString& ub = "", double inf_dig = 0.5, double gasto = 300.0) 
        : id(i), edad(e), sexo(s), accesoInternet(ai), distrito(d), 
          ingresos(ing), ubicacion(ub.isEmpty() ? d : ub), influenciabilidad_digital(inf_dig), gasto_promedio(gasto) {}
    
    // Métodos para comparación y conversión
    bool operator==(const Persona& other) const {
        return id == other.id || (edad == other.edad && sexo == other.sexo && 
               accesoInternet == other.accesoInternet && distrito == other.distrito);
    }
    
    QString toString() const {
        return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9")
            .arg(id).arg(edad).arg(sexo).arg(accesoInternet ? 1 : 0)
            .arg(distrito).arg(ingresos).arg(ubicacion)
            .arg(influenciabilidad_digital).arg(gasto_promedio);
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
