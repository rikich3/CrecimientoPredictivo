#ifndef UPLIFTING_MODEL_H
#define UPLIFTING_MODEL_H

#include <vector>
#include <memory>
#include <map>
#include <string>
#include "../data_estructures/persona.h"

namespace UpliftModel {

// Estructura para representar una decisión en el árbol
struct Decision {
    std::string feature;  // Característica a evaluar (ej: "edad", "ingresos")
    double threshold;     // Umbral para la decisión
    bool isNumeric;       // Si es característica numérica o categórica
    std::string category; // Para características categóricas
    
    Decision(const std::string& f, double t) 
        : feature(f), threshold(t), isNumeric(true) {}
    
    Decision(const std::string& f, const std::string& c) 
        : feature(f), threshold(0.0), isNumeric(false), category(c) {}
};

// Nodo del árbol de uplift
class UpliftNode {
public:
    Decision decision;
    std::unique_ptr<UpliftNode> left;   // Hijo izquierdo (no cumple condición)
    std::unique_ptr<UpliftNode> right;  // Hijo derecho (cumple condición)
    
    // Para nodos hoja
    bool isLeaf;
    double upliftScore;  // Puntuación de influenciabilidad (0.0 - 1.0)
    
    // Constructor para nodo interno
    UpliftNode(const Decision& d) 
        : decision(d), left(nullptr), right(nullptr), isLeaf(false), upliftScore(0.0) {}
    
    // Constructor para nodo hoja
    UpliftNode(double score) 
        : decision("", 0.0), left(nullptr), right(nullptr), isLeaf(true), upliftScore(score) {}
    
    // Evalúa una persona y devuelve el uplift score
    double evaluate(const Persona& persona) const;
    
private:
    // Función auxiliar para evaluar condiciones
    bool evaluateCondition(const Persona& persona) const;
    
    // Extrae el valor de una característica de la persona
    double getFeatureValue(const Persona& persona, const std::string& feature) const;
    std::string getFeatureCategory(const Persona& persona, const std::string& feature) const;
};

// Clase principal del modelo de uplift
class UpliftTreeModel {
private:
    std::unique_ptr<UpliftNode> root;
    
public:
    UpliftTreeModel();
    ~UpliftTreeModel() = default;
    
    // Construye el árbol de decisión predefinido
    void buildPredefinedTree();
    
    // Evalúa una persona y devuelve su puntuación de influenciabilidad
    double evaluateInfluenciability(const Persona& persona) const;
    
    // Filtra una lista de personas basándose en el umbral de influenciabilidad
    std::vector<Persona> filterByInfluenciability(
        const std::vector<Persona>& personas, 
        double minInfluenciability = 0.5) const;
    
    // Obtiene estadísticas del modelo sobre un conjunto de personas
    std::map<std::string, double> getModelStatistics(const std::vector<Persona>& personas) const;
    
    // Métodos para testing y debugging
    void printTreeStructure() const;
    std::vector<double> evaluateBatch(const std::vector<Persona>& personas) const;
};

// Funciones auxiliares para testing
namespace Testing {
    // Genera personas de prueba con diferentes perfiles
    std::vector<Persona> generateTestPersonas(int count = 100);
    
    // Ejecuta pruebas del modelo
    void runUpliftModelTests();
    
    // Analiza la distribución de puntuaciones
    void analyzeScoreDistribution(const std::vector<double>& scores);
    
    // Compara resultados antes y después del filtro de uplift
    void compareFilterResults(const std::vector<Persona>& original, 
                             const std::vector<Persona>& filtered);
}

} // namespace UpliftModel

#endif // UPLIFTING_MODEL_H
