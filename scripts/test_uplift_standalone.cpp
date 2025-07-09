#include <iostream>
#include "../system/uplifting_model.h"
#include "../data_estructures/persona.h"

int main() {
    std::cout << "=== PROGRAMA DE PRUEBAS DEL MODELO DE UPLIFT ===" << std::endl;
    
    try {
        // Ejecutar todas las pruebas del modelo
        UpliftModel::Testing::runUpliftModelTests();
        
        std::cout << "\n=== PRUEBAS ESPECÍFICAS ===" << std::endl;
        
        UpliftModel::UpliftTreeModel model;
        
        // Crear personas de prueba específicas para validar la lógica
        std::vector<Persona> test_cases;
        
        // Caso 1: Joven muy influenciable
        Persona joven_influenciable;
        joven_influenciable.id = 1;
        joven_influenciable.edad = 22;
        joven_influenciable.sexo = "Femenino";
        joven_influenciable.ingresos = 25000;
        joven_influenciable.ubicacion = "Madrid";
        joven_influenciable.influenciabilidad_digital = 0.8;
        joven_influenciable.gasto_promedio = 300;
        test_cases.push_back(joven_influenciable);
        
        // Caso 2: Mujer adulta con alto poder adquisitivo
        Persona mujer_alto_poder;
        mujer_alto_poder.id = 2;
        mujer_alto_poder.edad = 35;
        mujer_alto_poder.sexo = "Femenino";
        mujer_alto_poder.ingresos = 60000;
        mujer_alto_poder.ubicacion = "Barcelona";
        mujer_alto_poder.influenciabilidad_digital = 0.6;
        mujer_alto_poder.gasto_promedio = 800;
        test_cases.push_back(mujer_alto_poder);
        
        // Caso 3: Hombre adulto con altos ingresos
        Persona hombre_altos_ingresos;
        hombre_altos_ingresos.id = 3;
        hombre_altos_ingresos.edad = 40;
        hombre_altos_ingresos.sexo = "Masculino";
        hombre_altos_ingresos.ingresos = 75000;
        hombre_altos_ingresos.ubicacion = "Valencia";
        hombre_altos_ingresos.influenciabilidad_digital = 0.4;
        hombre_altos_ingresos.gasto_promedio = 600;
        test_cases.push_back(hombre_altos_ingresos);
        
        // Caso 4: Joven sin recursos
        Persona joven_sin_recursos;
        joven_sin_recursos.id = 4;
        joven_sin_recursos.edad = 20;
        joven_sin_recursos.sexo = "Masculino";
        joven_sin_recursos.ingresos = 18000;
        joven_sin_recursos.ubicacion = "Sevilla";
        joven_sin_recursos.influenciabilidad_digital = 0.5;
        joven_sin_recursos.gasto_promedio = 150;
        test_cases.push_back(joven_sin_recursos);
        
        // Caso 5: Hombre adulto con ingresos moderados
        Persona hombre_ingresos_moderados;
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
            
            // Interpretar el resultado
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
        
        // Prueba de filtrado con casos específicos
        std::cout << "=== PRUEBA DE FILTRADO CON CASOS ESPECÍFICOS ===" << std::endl;
        
        auto filtered_070 = model.filterByInfluenciability(test_cases, 0.7);
        auto filtered_050 = model.filterByInfluenciability(test_cases, 0.5);
        auto filtered_030 = model.filterByInfluenciability(test_cases, 0.3);
        
        std::cout << "Casos originales: " << test_cases.size() << std::endl;
        std::cout << "Con umbral >= 0.7: " << filtered_070.size() << " casos" << std::endl;
        std::cout << "Con umbral >= 0.5: " << filtered_050.size() << " casos" << std::endl;
        std::cout << "Con umbral >= 0.3: " << filtered_030.size() << " casos" << std::endl;
        
        std::cout << "\n=== VALIDACIÓN DE LA LÓGICA DEL ÁRBOL ===" << std::endl;
        
        // Verificar que los casos esperados tienen las puntuaciones correctas
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
