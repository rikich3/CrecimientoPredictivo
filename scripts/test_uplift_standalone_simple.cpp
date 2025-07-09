#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <map>
#include <memory>

// Versión simplificada de la estructura Persona para pruebas independientes
struct PersonaTest {
    int id;
    int edad;
    std::string sexo;
    double ingresos;
    std::string ubicacion;
    double influenciabilidad_digital;
    double gasto_promedio;
    
    PersonaTest() : id(0), edad(0), sexo(""), ingresos(0.0), ubicacion(""), 
                   influenciabilidad_digital(0.0), gasto_promedio(0.0) {}
};

// Implementación simplificada del modelo de uplift sin dependencias de Qt
namespace UpliftModelTest {

struct Decision {
    std::string feature;
    double threshold;
    bool isNumeric;
    std::string category;
    
    Decision(const std::string& f, double t) 
        : feature(f), threshold(t), isNumeric(true) {}
    
    Decision(const std::string& f, const std::string& c) 
        : feature(f), threshold(0.0), isNumeric(false), category(c) {}
};

class UpliftNode {
public:
    Decision decision;
    std::unique_ptr<UpliftNode> left;
    std::unique_ptr<UpliftNode> right;
    
    bool isLeaf;
    double upliftScore;
    
    UpliftNode(const Decision& d) 
        : decision(d), left(nullptr), right(nullptr), isLeaf(false), upliftScore(0.0) {}
    
    UpliftNode(double score) 
        : decision("", 0.0), left(nullptr), right(nullptr), isLeaf(true), upliftScore(score) {}
    
    double evaluate(const PersonaTest& persona) const {
        if (isLeaf) {
            return upliftScore;
        }
        
        if (evaluateCondition(persona)) {
            return right ? right->evaluate(persona) : 0.0;
        } else {
            return left ? left->evaluate(persona) : 0.0;
        }
    }
    
private:
    bool evaluateCondition(const PersonaTest& persona) const {
        if (decision.isNumeric) {
            double value = getFeatureValue(persona, decision.feature);
            return value >= decision.threshold;
        } else {
            std::string category = getFeatureCategory(persona, decision.feature);
            return category == decision.category;
        }
    }
    
    double getFeatureValue(const PersonaTest& persona, const std::string& feature) const {
        if (feature == "edad") {
            return static_cast<double>(persona.edad);
        } else if (feature == "ingresos") {
            return persona.ingresos;
        } else if (feature == "influenciabilidad_digital") {
            return persona.influenciabilidad_digital;
        } else if (feature == "gasto_promedio") {
            return persona.gasto_promedio;
        }
        return 0.0;
    }
    
    std::string getFeatureCategory(const PersonaTest& persona, const std::string& feature) const {
        if (feature == "sexo") {
            return persona.sexo;
        } else if (feature == "ubicacion") {
            return persona.ubicacion;
        }
        return "";
    }
};

class UpliftTreeModel {
private:
    std::unique_ptr<UpliftNode> root;
    
public:
    UpliftTreeModel() {
        buildPredefinedTree();
    }
    
    void buildPredefinedTree() {
        root = std::make_unique<UpliftNode>(Decision("edad", 25.0));
        
        root->left = std::make_unique<UpliftNode>(Decision("influenciabilidad_digital", 0.7));
        root->left->right = std::make_unique<UpliftNode>(0.8);
        root->left->left = std::make_unique<UpliftNode>(Decision("ingresos", 30000.0));
        root->left->left->right = std::make_unique<UpliftNode>(0.6);
        root->left->left->left = std::make_unique<UpliftNode>(0.3);
        
        root->right = std::make_unique<UpliftNode>(Decision("sexo", "Femenino"));
        root->right->right = std::make_unique<UpliftNode>(Decision("gasto_promedio", 500.0));
        root->right->right->right = std::make_unique<UpliftNode>(0.9);
        root->right->right->left = std::make_unique<UpliftNode>(0.5);
        
        root->right->left = std::make_unique<UpliftNode>(Decision("ingresos", 50000.0));
        root->right->left->right = std::make_unique<UpliftNode>(0.7);
        root->right->left->left = std::make_unique<UpliftNode>(0.4);
    }
    
    double evaluateInfluenciability(const PersonaTest& persona) const {
        if (!root) {
            return 0.0;
        }
        return root->evaluate(persona);
    }
    
    std::vector<PersonaTest> filterByInfluenciability(
        const std::vector<PersonaTest>& personas, 
        double minInfluenciability = 0.5) const {
        
        std::vector<PersonaTest> filtered;
        filtered.reserve(personas.size());
        
        for (const auto& persona : personas) {
            double score = evaluateInfluenciability(persona);
            if (score >= minInfluenciability) {
                filtered.push_back(persona);
            }
        }
        
        return filtered;
    }
    
    std::map<std::string, double> getModelStatistics(const std::vector<PersonaTest>& personas) const {
        std::map<std::string, double> stats;
        
        if (personas.empty()) {
            return stats;
        }
        
        std::vector<double> scores = evaluateBatch(personas);
        
        double sum = 0.0;
        double min_score = scores[0];
        double max_score = scores[0];
        
        for (double score : scores) {
            sum += score;
            min_score = std::min(min_score, score);
            max_score = std::max(max_score, score);
        }
        
        double mean = sum / scores.size();
        
        double variance = 0.0;
        for (double score : scores) {
            variance += (score - mean) * (score - mean);
        }
        variance /= scores.size();
        double std_dev = std::sqrt(variance);
        
        std::vector<double> sorted_scores = scores;
        std::sort(sorted_scores.begin(), sorted_scores.end());
        double median = (sorted_scores.size() % 2 == 0) ?
            (sorted_scores[sorted_scores.size()/2 - 1] + sorted_scores[sorted_scores.size()/2]) / 2.0 :
            sorted_scores[sorted_scores.size()/2];
        
        int high_influence = 0;
        int medium_influence = 0;
        int low_influence = 0;
        
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
    
    std::vector<double> evaluateBatch(const std::vector<PersonaTest>& personas) const {
        std::vector<double> scores;
        scores.reserve(personas.size());
        
        for (const auto& persona : personas) {
            scores.push_back(evaluateInfluenciability(persona));
        }
        
        return scores;
    }
    
    void printTreeStructure() const {
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
};

std::vector<PersonaTest> generateTestPersonas(int count = 100) {
    std::vector<PersonaTest> personas;
    personas.reserve(count);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> age_dist(18, 65);
    std::uniform_real_distribution<> income_dist(20000, 100000);
    std::uniform_real_distribution<> digital_influence_dist(0.1, 1.0);
    std::uniform_real_distribution<> expense_dist(100, 2000);
    std::uniform_int_distribution<> gender_dist(0, 1);
    
    std::vector<std::string> ubicaciones = {"Madrid", "Barcelona", "Valencia", "Sevilla", "Bilbao"};
    std::uniform_int_distribution<> location_dist(0, ubicaciones.size() - 1);
    
    for (int i = 0; i < count; ++i) {
        PersonaTest persona;
        persona.id = i + 1;
        persona.edad = age_dist(gen);
        persona.sexo = (gender_dist(gen) == 0) ? "Masculino" : "Femenino";
        persona.ingresos = income_dist(gen);
        persona.ubicacion = ubicaciones[location_dist(gen)];
        persona.influenciabilidad_digital = digital_influence_dist(gen);
        persona.gasto_promedio = expense_dist(gen);
        
        personas.push_back(persona);
    }
    
    return personas;
}

void runUpliftModelTests() {
    std::cout << "\n=== EJECUTANDO PRUEBAS DEL MODELO DE UPLIFT ===" << std::endl;
    
    UpliftTreeModel model;
    model.printTreeStructure();
    
    std::cout << "Generando 500 personas de prueba..." << std::endl;
    auto personas = generateTestPersonas(500);
    
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
    
    auto filtered_high = model.filterByInfluenciability(personas, 0.7);
    auto filtered_medium = model.filterByInfluenciability(personas, 0.5);
    
    std::cout << "\n=== PRUEBA DE FILTRADO ===" << std::endl;
    std::cout << "Personas con alta influenciabilidad (>= 0.7): " << filtered_high.size() << std::endl;
    std::cout << "Personas con influenciabilidad media-alta (>= 0.5): " << filtered_medium.size() << std::endl;
    
    std::cout << "Reducción del " << ((1.0 - static_cast<double>(filtered_high.size()) / personas.size()) * 100.0) 
              << "% con filtro estricto" << std::endl;
    std::cout << "Reducción del " << ((1.0 - static_cast<double>(filtered_medium.size()) / personas.size()) * 100.0) 
              << "% con filtro moderado" << std::endl;
}

} // namespace UpliftModelTest

int main() {
    std::cout << "=== PROGRAMA DE PRUEBAS DEL MODELO DE UPLIFT ===" << std::endl;
    
    try {
        UpliftModelTest::runUpliftModelTests();
        
        std::cout << "\n=== PRUEBAS ESPECÍFICAS ===" << std::endl;
        
        UpliftModelTest::UpliftTreeModel model;
        
        std::vector<PersonaTest> test_cases;
        
        // Caso 1: Joven muy influenciable
        PersonaTest joven_influenciable;
        joven_influenciable.id = 1;
        joven_influenciable.edad = 22;
        joven_influenciable.sexo = "Femenino";
        joven_influenciable.ingresos = 25000;
        joven_influenciable.ubicacion = "Madrid";
        joven_influenciable.influenciabilidad_digital = 0.8;
        joven_influenciable.gasto_promedio = 300;
        test_cases.push_back(joven_influenciable);
        
        // Caso 2: Mujer adulta con alto poder adquisitivo
        PersonaTest mujer_alto_poder;
        mujer_alto_poder.id = 2;
        mujer_alto_poder.edad = 35;
        mujer_alto_poder.sexo = "Femenino";
        mujer_alto_poder.ingresos = 60000;
        mujer_alto_poder.ubicacion = "Barcelona";
        mujer_alto_poder.influenciabilidad_digital = 0.6;
        mujer_alto_poder.gasto_promedio = 800;
        test_cases.push_back(mujer_alto_poder);
        
        // Caso 3: Hombre adulto con altos ingresos
        PersonaTest hombre_altos_ingresos;
        hombre_altos_ingresos.id = 3;
        hombre_altos_ingresos.edad = 40;
        hombre_altos_ingresos.sexo = "Masculino";
        hombre_altos_ingresos.ingresos = 75000;
        hombre_altos_ingresos.ubicacion = "Valencia";
        hombre_altos_ingresos.influenciabilidad_digital = 0.4;
        hombre_altos_ingresos.gasto_promedio = 600;
        test_cases.push_back(hombre_altos_ingresos);
        
        // Caso 4: Joven sin recursos
        PersonaTest joven_sin_recursos;
        joven_sin_recursos.id = 4;
        joven_sin_recursos.edad = 20;
        joven_sin_recursos.sexo = "Masculino";
        joven_sin_recursos.ingresos = 18000;
        joven_sin_recursos.ubicacion = "Sevilla";
        joven_sin_recursos.influenciabilidad_digital = 0.5;
        joven_sin_recursos.gasto_promedio = 150;
        test_cases.push_back(joven_sin_recursos);
        
        // Caso 5: Hombre adulto con ingresos moderados
        PersonaTest hombre_ingresos_moderados;
        hombre_ingresos_moderados.id = 5;
        hombre_ingresos_moderados.edad = 45;
        hombre_ingresos_moderados.sexo = "Masculino";
        hombre_ingresos_moderados.ingresos = 40000;
        hombre_ingresos_moderados.ubicacion = "Bilbao";
        hombre_ingresos_moderados.influenciabilidad_digital = 0.3;
        hombre_ingresos_moderados.gasto_promedio = 400;
        test_cases.push_back(hombre_ingresos_moderados);
        
        std::cout << "\nEvaluando casos de prueba específicos:" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        
        for (size_t i = 0; i < test_cases.size(); ++i) {
            const auto& persona = test_cases[i];
            double score = model.evaluateInfluenciability(persona);
            
            std::cout << "Caso " << (i + 1) << ":" << std::endl;
            std::cout << "  Edad: " << persona.edad << ", Sexo: " << persona.sexo << std::endl;
            std::cout << "  Ingresos: " << persona.ingresos << ", Gasto: " << persona.gasto_promedio << std::endl;
            std::cout << "  Influenciabilidad digital: " << persona.influenciabilidad_digital << std::endl;
            std::cout << "  Puntuación de uplift: " << score << std::endl;
            
            std::string interpretacion;
            if (score >= 0.8) {
                interpretacion = "MUY ALTA - Target prioritario";
            } else if (score >= 0.6) {
                interpretacion = "ALTA - Buen candidato";
            } else if (score >= 0.4) {
                interpretacion = "MEDIA - Candidato moderado";
            } else {
                interpretacion = "BAJA - No recomendado";
            }
            
            std::cout << "  Interpretación: " << interpretacion << std::endl;
            std::cout << std::endl;
        }
        
        std::cout << "=== VALIDACIÓN DE LA LÓGICA DEL ÁRBOL ===" << std::endl;
        
        std::vector<std::pair<std::string, double>> expected_scores = {
            {"Joven muy influenciable", 0.8},
            {"Mujer alto poder adquisitivo", 0.9},
            {"Hombre altos ingresos", 0.7},
            {"Joven sin recursos", 0.3},
            {"Hombre ingresos moderados", 0.4}
        };
        
        bool all_correct = true;
        for (size_t i = 0; i < test_cases.size() && i < expected_scores.size(); ++i) {
            double actual = model.evaluateInfluenciability(test_cases[i]);
            double expected = expected_scores[i].second;
            
            if (std::abs(actual - expected) < 0.001) {
                std::cout << "✓ " << expected_scores[i].first << ": " << actual 
                          << " (esperado: " << expected << ")" << std::endl;
            } else {
                std::cout << "✗ " << expected_scores[i].first << ": " << actual 
                          << " (esperado: " << expected << ")" << std::endl;
                all_correct = false;
            }
        }
        
        if (all_correct) {
            std::cout << "\n✓ TODAS LAS VALIDACIONES PASARON CORRECTAMENTE" << std::endl;
        } else {
            std::cout << "\n✗ ALGUNAS VALIDACIONES FALLARON" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error durante las pruebas: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=== PRUEBAS COMPLETADAS EXITOSAMENTE ===" << std::endl;
    return 0;
}
