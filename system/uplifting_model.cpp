#include "uplifting_model.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <QVector>

namespace UpliftModel {

// Implementación de UpliftNode
bool UpliftNode::evaluateCondition(const Persona& persona) const {
    if (decision.isNumeric) {
        double value = getFeatureValue(persona, decision.feature);
        return value >= decision.threshold;
    } else {
        std::string category = getFeatureCategory(persona, decision.feature);
        return category == decision.category;
    }
}

double UpliftNode::getFeatureValue(const Persona& persona, const std::string& feature) const {
    if (feature == "edad") {
        return static_cast<double>(persona.edad);
    } else if (feature == "ingresos") {
        return persona.ingresos;
    } else if (feature == "influenciabilidad_digital") {
        return persona.influenciabilidad_digital;
    } else if (feature == "gasto_promedio") {
        return persona.gasto_promedio;
    }
    return 0.0; // Valor por defecto
}

std::string UpliftNode::getFeatureCategory(const Persona& persona, const std::string& feature) const {
    if (feature == "sexo") {
        return persona.sexo.toStdString();
    } else if (feature == "ubicacion") {
        return persona.ubicacion.toStdString();
    } else if (feature == "distrito") {
        return persona.distrito.toStdString();
    }
    return ""; // Valor por defecto
}

double UpliftNode::evaluate(const Persona& persona) const {
    if (isLeaf) {
        return upliftScore;
    }
    
    if (evaluateCondition(persona)) {
        return right ? right->evaluate(persona) : 0.0;
    } else {
        return left ? left->evaluate(persona) : 0.0;
    }
}

// Implementación de UpliftTreeModel
UpliftTreeModel::UpliftTreeModel() {
    buildPredefinedTree();
}

void UpliftTreeModel::buildPredefinedTree() {
    /*
     * Estructura del árbol de uplift basado en análisis de comportamiento del consumidor:
     * 
     * Raíz: ¿Edad >= 25?
     *   NO (< 25):
     *     ¿Influenciabilidad digital >= 0.7?
     *       SÍ: Puntuación alta (0.8) - Jóvenes muy influenciables
     *       NO: ¿Ingresos >= 30000?
     *         SÍ: Puntuación media-alta (0.6) - Jóvenes con recursos
     *         NO: Puntuación baja (0.3) - Jóvenes con pocos recursos
     *   SÍ (>= 25):
     *     ¿Sexo == "Femenino"?
     *       SÍ: ¿Gasto promedio >= 500?
     *         SÍ: Puntuación muy alta (0.9) - Mujeres con alto poder adquisitivo
     *         NO: Puntuación media (0.5) - Mujeres con gasto moderado
     *       NO: ¿Ingresos >= 50000?
     *         SÍ: Puntuación alta (0.7) - Hombres con altos ingresos
     *         NO: Puntuación media-baja (0.4) - Hombres con ingresos moderados
     */
    
    // Crear la raíz del árbol
    root = std::make_unique<UpliftNode>(Decision("edad", 25.0));
    
    // Rama izquierda (edad < 25)
    root->left = std::make_unique<UpliftNode>(Decision("influenciabilidad_digital", 0.7));
    
    // Jóvenes muy influenciables
    root->left->right = std::make_unique<UpliftNode>(0.8);
    
    // Jóvenes menos influenciables - evaluar ingresos
    root->left->left = std::make_unique<UpliftNode>(Decision("ingresos", 30000.0));
    root->left->left->right = std::make_unique<UpliftNode>(0.6); // Con recursos
    root->left->left->left = std::make_unique<UpliftNode>(0.3);  // Sin recursos
    
    // Rama derecha (edad >= 25)
    root->right = std::make_unique<UpliftNode>(Decision("sexo", "Femenino"));
    
    // Mujeres adultas - evaluar gasto promedio
    root->right->right = std::make_unique<UpliftNode>(Decision("gasto_promedio", 500.0));
    root->right->right->right = std::make_unique<UpliftNode>(0.9); // Alto gasto
    root->right->right->left = std::make_unique<UpliftNode>(0.5);  // Gasto moderado
    
    // Hombres adultos - evaluar ingresos
    root->right->left = std::make_unique<UpliftNode>(Decision("ingresos", 50000.0));
    root->right->left->right = std::make_unique<UpliftNode>(0.7); // Altos ingresos
    root->right->left->left = std::make_unique<UpliftNode>(0.4);  // Ingresos moderados
}

double UpliftTreeModel::evaluateInfluenciability(const Persona& persona) const {
    if (!root) {
        return 0.0;
    }
    return root->evaluate(persona);
}

std::vector<Persona> UpliftTreeModel::filterByInfluenciability(
    const std::vector<Persona>& personas, 
    double minInfluenciability) const {
    
    std::vector<Persona> filtered;
    filtered.reserve(personas.size());
    
    for (const auto& persona : personas) {
        double score = evaluateInfluenciability(persona);
        if (score >= minInfluenciability) {
            filtered.push_back(persona);
        }
    }
    
    return filtered;
}

std::map<std::string, double> UpliftTreeModel::getModelStatistics(const std::vector<Persona>& personas) const {
    std::map<std::string, double> stats;
    
    if (personas.empty()) {
        return stats;
    }
    
    std::vector<double> scores = evaluateBatch(personas);
    
    // Calcular estadísticas básicas
    double sum = 0.0;
    double min_score = scores[0];
    double max_score = scores[0];
    
    for (double score : scores) {
        sum += score;
        min_score = std::min(min_score, score);
        max_score = std::max(max_score, score);
    }
    
    double mean = sum / scores.size();
    
    // Calcular desviación estándar
    double variance = 0.0;
    for (double score : scores) {
        variance += (score - mean) * (score - mean);
    }
    variance /= scores.size();
    double std_dev = std::sqrt(variance);
    
    // Calcular mediana
    std::vector<double> sorted_scores = scores;
    std::sort(sorted_scores.begin(), sorted_scores.end());
    double median = (sorted_scores.size() % 2 == 0) ?
        (sorted_scores[sorted_scores.size()/2 - 1] + sorted_scores[sorted_scores.size()/2]) / 2.0 :
        sorted_scores[sorted_scores.size()/2];
    
    // Contar personas por rango de influenciabilidad
    int high_influence = 0;    // >= 0.7
    int medium_influence = 0;  // 0.4 - 0.7
    int low_influence = 0;     // < 0.4
    
    for (double score : scores) {
        if (score >= 0.7) {
            high_influence++;
        } else if (score >= 0.4) {
            medium_influence++;
        } else {
            low_influence++;
        }
    }
    
    stats["total_personas"] = static_cast<double>(personas.size());
    stats["media"] = mean;
    stats["mediana"] = median;
    stats["desviacion_estandar"] = std_dev;
    stats["minimo"] = min_score;
    stats["maximo"] = max_score;
    stats["alta_influenciabilidad"] = static_cast<double>(high_influence);
    stats["media_influenciabilidad"] = static_cast<double>(medium_influence);
    stats["baja_influenciabilidad"] = static_cast<double>(low_influence);
    stats["porcentaje_alta"] = (static_cast<double>(high_influence) / personas.size()) * 100.0;
    stats["porcentaje_media"] = (static_cast<double>(medium_influence) / personas.size()) * 100.0;
    stats["porcentaje_baja"] = (static_cast<double>(low_influence) / personas.size()) * 100.0;
    
    return stats;
}

void UpliftTreeModel::printTreeStructure() const {
    std::cout << "\n=== ESTRUCTURA DEL ÁRBOL DE UPLIFT ===" << std::endl;
    std::cout << "Raíz: ¿Edad >= 25?" << std::endl;
    std::cout << "  NO (< 25):" << std::endl;
    std::cout << "    ¿Influenciabilidad digital >= 0.7?" << std::endl;
    std::cout << "      SÍ: Puntuación 0.8 (Jóvenes muy influenciables)" << std::endl;
    std::cout << "      NO: ¿Ingresos >= 30000?" << std::endl;
    std::cout << "        SÍ: Puntuación 0.6 (Jóvenes con recursos)" << std::endl;
    std::cout << "        NO: Puntuación 0.3 (Jóvenes sin recursos)" << std::endl;
    std::cout << "  SÍ (>= 25):" << std::endl;
    std::cout << "    ¿Sexo == Femenino?" << std::endl;
    std::cout << "      SÍ: ¿Gasto promedio >= 500?" << std::endl;
    std::cout << "        SÍ: Puntuación 0.9 (Mujeres alto poder adquisitivo)" << std::endl;
    std::cout << "        NO: Puntuación 0.5 (Mujeres gasto moderado)" << std::endl;
    std::cout << "      NO: ¿Ingresos >= 50000?" << std::endl;
    std::cout << "        SÍ: Puntuación 0.7 (Hombres altos ingresos)" << std::endl;
    std::cout << "        NO: Puntuación 0.4 (Hombres ingresos moderados)" << std::endl;
    std::cout << "==========================================\n" << std::endl;
}

std::vector<double> UpliftTreeModel::evaluateBatch(const std::vector<Persona>& personas) const {
    std::vector<double> scores;
    scores.reserve(personas.size());
    
    for (const auto& persona : personas) {
        scores.push_back(evaluateInfluenciability(persona));
    }
    
    return scores;
}

// Implementación de las funciones de testing
namespace Testing {

std::vector<Persona> generateTestPersonas(int count) {
    std::vector<Persona> personas;
    personas.reserve(count);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> age_dist(18, 65);
    std::uniform_real_distribution<> income_dist(20000, 100000);
    std::uniform_real_distribution<> digital_influence_dist(0.1, 1.0);
    std::uniform_real_distribution<> expense_dist(100, 2000);
    std::uniform_int_distribution<> gender_dist(0, 1);
    
    QVector<QString> ubicaciones = {"Madrid", "Barcelona", "Valencia", "Sevilla", "Bilbao"};
    std::uniform_int_distribution<> location_dist(0, ubicaciones.size() - 1);
    
    for (int i = 0; i < count; ++i) {
        QString sexo = (gender_dist(gen) == 0) ? "Masculino" : "Femenino";
        QString ubicacion = ubicaciones[location_dist(gen)];
        
        Persona persona(
            i + 1,                                    // id
            age_dist(gen),                           // edad
            sexo,                                    // sexo
            true,                                    // accesoInternet (por defecto true)
            ubicacion,                               // distrito
            income_dist(gen),                        // ingresos
            ubicacion,                               // ubicacion (igual a distrito)
            digital_influence_dist(gen),             // influenciabilidad_digital
            expense_dist(gen)                        // gasto_promedio
        );
        
        personas.push_back(persona);
    }
    
    return personas;
}

void runUpliftModelTests() {
    std::cout << "\n=== EJECUTANDO PRUEBAS DEL MODELO DE UPLIFT ===" << std::endl;
    
    UpliftTreeModel model;
    
    // Mostrar estructura del árbol
    model.printTreeStructure();
    
    // Generar datos de prueba
    std::cout << "Generando 500 personas de prueba..." << std::endl;
    auto personas = generateTestPersonas(500);
    
    // Obtener estadísticas del modelo
    auto stats = model.getModelStatistics(personas);
    
    std::cout << "\n=== ESTADÍSTICAS DEL MODELO ===" << std::endl;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Total de personas: " << static_cast<int>(stats["total_personas"]) << std::endl;
    std::cout << "Puntuación media: " << stats["media"] << std::endl;
    std::cout << "Puntuación mediana: " << stats["mediana"] << std::endl;
    std::cout << "Desviación estándar: " << stats["desviacion_estandar"] << std::endl;
    std::cout << "Puntuación mínima: " << stats["minimo"] << std::endl;
    std::cout << "Puntuación máxima: " << stats["maximo"] << std::endl;
    
    std::cout << "\n=== DISTRIBUCIÓN POR NIVEL DE INFLUENCIABILIDAD ===" << std::endl;
    std::cout << "Alta (>= 0.7): " << static_cast<int>(stats["alta_influenciabilidad"]) 
              << " (" << stats["porcentaje_alta"] << "%)" << std::endl;
    std::cout << "Media (0.4-0.7): " << static_cast<int>(stats["media_influenciabilidad"]) 
              << " (" << stats["porcentaje_media"] << "%)" << std::endl;
    std::cout << "Baja (< 0.4): " << static_cast<int>(stats["baja_influenciabilidad"]) 
              << " (" << stats["porcentaje_baja"] << "%)" << std::endl;
    
    // Prueba de filtrado
    std::cout << "\n=== PRUEBA DE FILTRADO ===" << std::endl;
    auto filtered_high = model.filterByInfluenciability(personas, 0.7);
    auto filtered_medium = model.filterByInfluenciability(personas, 0.5);
    
    std::cout << "Personas con alta influenciabilidad (>= 0.7): " << filtered_high.size() << std::endl;
    std::cout << "Personas con influenciabilidad media-alta (>= 0.5): " << filtered_medium.size() << std::endl;
    
    std::cout << "Reducción del " << ((1.0 - static_cast<double>(filtered_high.size()) / personas.size()) * 100.0) 
              << "% con filtro estricto" << std::endl;
    std::cout << "Reducción del " << ((1.0 - static_cast<double>(filtered_medium.size()) / personas.size()) * 100.0) 
              << "% con filtro moderado" << std::endl;
    
    // Analizar distribución de puntuaciones
    auto scores = model.evaluateBatch(personas);
    analyzeScoreDistribution(scores);
    
    // Comparar resultados
    compareFilterResults(personas, filtered_high);
    
    std::cout << "\n=== PRUEBAS COMPLETADAS ===" << std::endl;
}

void analyzeScoreDistribution(const std::vector<double>& scores) {
    std::cout << "\n=== ANÁLISIS DE DISTRIBUCIÓN DE PUNTUACIONES ===" << std::endl;
    
    std::map<double, int> score_counts;
    for (double score : scores) {
        score_counts[score]++;
    }
    
    std::cout << "Distribución de puntuaciones únicas:" << std::endl;
    for (const auto& pair : score_counts) {
        double percentage = (static_cast<double>(pair.second) / scores.size()) * 100.0;
        std::cout << "  " << std::fixed << std::setprecision(1) << pair.first 
                  << ": " << pair.second << " personas (" << percentage << "%)" << std::endl;
    }
}

void compareFilterResults(const std::vector<Persona>& original, 
                         const std::vector<Persona>& filtered) {
    std::cout << "\n=== COMPARACIÓN ANTES/DESPUÉS DEL FILTRO ===" << std::endl;
    
    // Calcular promedios de edad
    double avg_age_original = 0.0;
    double avg_age_filtered = 0.0;
    
    for (const auto& p : original) {
        avg_age_original += p.edad;
    }
    avg_age_original /= original.size();
    
    for (const auto& p : filtered) {
        avg_age_filtered += p.edad;
    }
    if (!filtered.empty()) {
        avg_age_filtered /= filtered.size();
    }
    
    // Calcular promedios de ingresos
    double avg_income_original = 0.0;
    double avg_income_filtered = 0.0;
    
    for (const auto& p : original) {
        avg_income_original += p.ingresos;
    }
    avg_income_original /= original.size();
    
    for (const auto& p : filtered) {
        avg_income_filtered += p.ingresos;
    }
    if (!filtered.empty()) {
        avg_income_filtered /= filtered.size();
    }
    
    // Contar distribución por género
    int male_original = 0, female_original = 0;
    int male_filtered = 0, female_filtered = 0;
    
    for (const auto& p : original) {
        if (p.sexo == "Masculino") male_original++;
        else female_original++;
    }
    
    for (const auto& p : filtered) {
        if (p.sexo == "Masculino") male_filtered++;
        else female_filtered++;
    }
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Edad promedio - Original: " << avg_age_original 
              << ", Filtrado: " << avg_age_filtered << std::endl;
    std::cout << "Ingresos promedio - Original: " << avg_income_original 
              << ", Filtrado: " << avg_income_filtered << std::endl;
    
    std::cout << "Distribución por género:" << std::endl;
    std::cout << "  Original - Hombres: " << male_original << ", Mujeres: " << female_original << std::endl;
    std::cout << "  Filtrado - Hombres: " << male_filtered << ", Mujeres: " << female_filtered << std::endl;
    
    if (!filtered.empty()) {
        double male_retention = (static_cast<double>(male_filtered) / male_original) * 100.0;
        double female_retention = (static_cast<double>(female_filtered) / female_original) * 100.0;
        
        std::cout << "Tasa de retención:" << std::endl;
        std::cout << "  Hombres: " << male_retention << "%" << std::endl;
        std::cout << "  Mujeres: " << female_retention << "%" << std::endl;
    }
}

} // namespace Testing

} // namespace UpliftModel
