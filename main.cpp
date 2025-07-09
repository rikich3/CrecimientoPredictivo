#include "ui/mainwindow.h"
#include "system/uplifting_model.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Configurar parser de línea de comandos
    QCommandLineParser parser;
    parser.setApplicationDescription("Sistema de Predicción de Publicidad Efectiva");
    parser.addHelpOption();
    parser.addVersionOption();
    
    // Opción para ejecutar pruebas del modelo de uplift
    QCommandLineOption testUpliftOption(QStringList() << "t" << "test-uplift",
                                       "Ejecutar pruebas del modelo de uplift");
    parser.addOption(testUpliftOption);
    
    // Procesar argumentos
    parser.process(a);
    
    // Si se especifica la opción de pruebas, ejecutarlas
    if (parser.isSet(testUpliftOption)) {
        std::cout << "=== EJECUTANDO PRUEBAS DEL MODELO DE UPLIFT DESDE MAIN ===" << std::endl;
        
        try {
            UpliftModel::Testing::runUpliftModelTests();
            std::cout << "\n✓ Todas las pruebas del modelo de uplift completadas exitosamente" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "✗ Error durante las pruebas: " << e.what() << std::endl;
            return 1;
        }
        
        return 0; // Salir después de las pruebas
    }
    
    // Comportamiento normal: mostrar la interfaz gráfica
    MainWindow w;
    w.show();
    return a.exec();
}
