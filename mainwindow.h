#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>
#include <QMessageBox>
#include <QProgressBar>
#include <QTextEdit>
#include <QSplashScreen>
#include <QTimer>

#include "persona.h"
#include "gestor_datos.h"
#include "analizador_trafico.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void iniciarAnalisis();
    void onTipoEspacioChanged();
    void onEdadMinChanged();
    void onEdadMaxChanged();
    void actualizarRequiereInternet();

private:
    Ui::MainWindow *ui;
    
    // Widgets para Cliente Ideal
    QGroupBox *grupoClienteIdeal;
    QSpinBox *spinEdadMin;
    QSpinBox *spinEdadMax;
    QComboBox *comboSexo;
    QCheckBox *checkAccesoInternet;
    QLabel *labelEdadMin;
    QLabel *labelEdadMax;
    QLabel *labelSexo;
    
    // Widgets para Producto/Servicio
    QGroupBox *grupoProducto;
    QComboBox *comboProducto;
    QLabel *labelProducto;
    
    // Widgets para Región Geográfica
    QGroupBox *grupoRegion;
    QComboBox *comboTipoEspacio;
    QComboBox *comboEspacio;
    QLabel *labelTipoEspacio;
    QLabel *labelEspacio;
    
    // Botón de análisis y información
    QPushButton *btnIniciarAnalisis;
    QLabel *labelInfo;
    
    // Clases del sistema
    GestorDatos *gestorDatos;
    AnalizadorTrafico *analizadorTrafico;
    
    // Métodos del sistema
    void setupUI();
    void setupStyleSheet();
    void inicializarDatos();
    void mostrarResultados(const ResultadoAnalisis& resultado);
    void mostrarSplashScreen();
    void validarEntradas();
    
    // Métodos de la interfaz
    void actualizarEspacios();
    ClienteIdeal obtenerClienteIdeal();
    QString obtenerRutaCSV();
};

#endif // MAINWINDOW_H
