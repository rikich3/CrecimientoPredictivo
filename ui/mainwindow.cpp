#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QStandardPaths>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>
#include <QScreen>
#include <QPainter>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gestorDatos(new GestorDatos())
    , analizadorTrafico(new AnalizadorTrafico())
{
    ui->setupUi(this);
    
    // Configurar ventana principal
    setWindowTitle("Publicidad Efectiva - Análisis de Tráfico");
    setMinimumSize(900, 700);
    resize(1000, 800);
    
    // Mostrar splash screen
    mostrarSplashScreen();
    
    // Configurar interfaz
    setupUI();
    setupStyleSheet();
    
    // Inicializar datos
    inicializarDatos();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gestorDatos;
    delete analizadorTrafico;
}

void MainWindow::setupUI()
{
    // Widget central
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Layout principal
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(centralWidget);
    layoutPrincipal->setSpacing(20);
    layoutPrincipal->setContentsMargins(20, 20, 20, 20);
    
    // Título principal
    QLabel *titulo = new QLabel("Sistema de Análisis de Publicidad Efectiva");
    titulo->setObjectName("titulo");
    titulo->setAlignment(Qt::AlignCenter);
    layoutPrincipal->addWidget(titulo);
    
    // Configurar grupo Cliente Ideal
    grupoClienteIdeal = new QGroupBox("1. Cliente Ideal");
    grupoClienteIdeal->setObjectName("grupoSeccion");
    QGridLayout *layoutClienteIdeal = new QGridLayout(grupoClienteIdeal);
    layoutClienteIdeal->setSpacing(10);
    
    labelEdadMin = new QLabel("Edad mínima:");
    spinEdadMin = new QSpinBox();
    spinEdadMin->setRange(15, 80);
    spinEdadMin->setValue(18);
    spinEdadMin->setSuffix(" años");
    layoutClienteIdeal->addWidget(labelEdadMin, 0, 0);
    layoutClienteIdeal->addWidget(spinEdadMin, 0, 1);
    
    labelEdadMax = new QLabel("Edad máxima:");
    spinEdadMax = new QSpinBox();
    spinEdadMax->setRange(15, 80);
    spinEdadMax->setValue(65);
    spinEdadMax->setSuffix(" años");
    layoutClienteIdeal->addWidget(labelEdadMax, 0, 2);
    layoutClienteIdeal->addWidget(spinEdadMax, 0, 3);
    
    labelSexo = new QLabel("Sexo:");
    comboSexo = new QComboBox();
    comboSexo->addItems({"Cualquiera", "Masculino", "Femenino"});
    layoutClienteIdeal->addWidget(labelSexo, 1, 0);
    layoutClienteIdeal->addWidget(comboSexo, 1, 1);
    
    checkAccesoInternet = new QCheckBox("Requiere acceso a internet");
    checkAccesoInternet->setChecked(true);
    layoutClienteIdeal->addWidget(checkAccesoInternet, 1, 2, 1, 2);
    
    // Configurar grupo Producto/Servicio
    grupoProducto = new QGroupBox("2. Producto/Servicio");
    grupoProducto->setObjectName("grupoSeccion");
    QVBoxLayout *layoutProducto = new QVBoxLayout(grupoProducto);
    layoutProducto->setSpacing(10);
    
    labelProducto = new QLabel("Categoría del producto:");
    comboProducto = new QComboBox();
    QVector<QString> productos = gestorDatos->obtenerCategoriasProductos();
    for (const QString& producto : productos) {
        comboProducto->addItem(producto);
    }
    layoutProducto->addWidget(labelProducto);
    layoutProducto->addWidget(comboProducto);
    
    // Configurar grupo Región Geográfica
    grupoRegion = new QGroupBox("3. Región Geográfica");
    grupoRegion->setObjectName("grupoSeccion");
    QGridLayout *layoutRegion = new QGridLayout(grupoRegion);
    layoutRegion->setSpacing(10);
    
    labelTipoEspacio = new QLabel("Tipo de espacio:");
    comboTipoEspacio = new QComboBox();
    comboTipoEspacio->addItems({"Espacio Geográfico", "Plataforma Digital"});
    layoutRegion->addWidget(labelTipoEspacio, 0, 0);
    layoutRegion->addWidget(comboTipoEspacio, 0, 1);
    
    labelEspacio = new QLabel("Espacio:");
    comboEspacio = new QComboBox();
    layoutRegion->addWidget(labelEspacio, 1, 0);
    layoutRegion->addWidget(comboEspacio, 1, 1);
    
    // Información adicional
    labelInfo = new QLabel("Configura los parámetros de tu cliente ideal, selecciona el producto/servicio y el espacio donde planeas publicitar.");
    labelInfo->setObjectName("labelInfo");
    labelInfo->setWordWrap(true);
    
    // Botón de análisis
    btnIniciarAnalisis = new QPushButton("🚀 Iniciar Análisis");
    btnIniciarAnalisis->setObjectName("btnAnalisis");
    btnIniciarAnalisis->setMinimumHeight(50);
    
    // Conectar señales
    connect(comboTipoEspacio, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onTipoEspacioChanged);
    connect(btnIniciarAnalisis, &QPushButton::clicked, this, &MainWindow::iniciarAnalisis);
    connect(spinEdadMin, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &MainWindow::onEdadMinChanged);
    connect(spinEdadMax, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &MainWindow::onEdadMaxChanged);
    
    // Inicializar espacios y estado del checkbox
    actualizarEspacios();
    actualizarRequiereInternet();
    
    // Añadir widgets al layout principal
    layoutPrincipal->addWidget(grupoClienteIdeal);
    layoutPrincipal->addWidget(grupoProducto);
    layoutPrincipal->addWidget(grupoRegion);
    layoutPrincipal->addWidget(labelInfo);
    layoutPrincipal->addWidget(btnIniciarAnalisis);
    layoutPrincipal->addStretch();
}

void MainWindow::setupStyleSheet()
{
    setStyleSheet(R"(
        QMainWindow {
            background-color: #f5f5f5;
            color: #2c3e50;
        }
        
        #titulo {
            font-size: 24px;
            font-weight: bold;
            color: #2c3e50;
            margin: 20px 0;
            padding: 15px;
            background-color: #ecf0f1;
            border-radius: 10px;
        }
        
        #grupoSeccion {
            font-size: 14px;
            font-weight: bold;
            color: #34495e;
            border: 2px solid #bdc3c7;
            border-radius: 10px;
            margin: 10px 0;
            padding: 15px;
            background-color: white;
        }
        
        #grupoSeccion::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 10px 0 10px;
            color: #2c3e50;
        }
        
        QSpinBox, QComboBox {
            padding: 8px;
            border: 2px solid #bdc3c7;
            border-radius: 5px;
            background-color: white;
            color: #2c3e50;
            font-size: 12px;
        }
        
        QSpinBox:focus, QComboBox:focus {
            border-color: #3498db;
        }
        
        QCheckBox {
            font-size: 12px;
            color: #2c3e50;
            spacing: 10px;
            background-color: transparent;
        }
        
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
        }
        
        QCheckBox::indicator:unchecked {
            border: 2px solid #bdc3c7;
            border-radius: 3px;
            background-color: white;
        }
        
        QCheckBox::indicator:checked {
            border: 2px solid #27ae60;
            border-radius: 3px;
            background-color: #27ae60;
        }
        
        #btnAnalisis {
            background-color: #3498db;
            color: white;
            font-size: 16px;
            font-weight: bold;
            border: none;
            border-radius: 8px;
            padding: 15px;
            margin: 20px 0;
        }
        
        #btnAnalisis:hover {
            background-color: #2980b9;
        }
        
        #btnAnalisis:pressed {
            background-color: #21618c;
        }
        
        #labelInfo {
            font-size: 12px;
            color: #7f8c8d;
            font-style: italic;
            padding: 10px;
            background-color: #ecf0f1;
            border-radius: 5px;
        }
        
        QLabel {
            font-size: 12px;
            color: #2c3e50;
            background-color: transparent;
        }
        
        QGroupBox {
            font-size: 14px;
            font-weight: bold;
            color: #34495e;
            border: 2px solid #bdc3c7;
            border-radius: 10px;
            margin: 10px 0;
            padding-top: 15px;
            background-color: white;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 10px 0 10px;
            color: #2c3e50;
            background-color: transparent;
        }
    )");
}

void MainWindow::inicializarDatos()
{
    QString rutaCSV = obtenerRutaCSV();
    
    // Intentar cargar datos existentes
    gestorDatos->cargarPoblacionDesdeCSV(rutaCSV);

    
    // Si no hay datos, generar población
    if (gestorDatos->obtenerPoblacion().isEmpty()) {
        gestorDatos->generarPoblacion(50000);
        gestorDatos->guardarPoblacionEnCSV(rutaCSV);
    }
}

void MainWindow::mostrarSplashScreen()
{
    QSplashScreen *splash = new QSplashScreen();
    splash->setFixedSize(400, 300);
    
    // Crear un pixmap simple para el splash
    QPixmap pixmap(400, 300);
    pixmap.fill(QColor(52, 152, 219));
    
    QPainter painter(&pixmap);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(pixmap.rect(), Qt::AlignCenter, "Publicidad Efectiva\n\nCargando...");
    
    splash->setPixmap(pixmap);
    splash->show();
    
    QTimer::singleShot(2000, splash, &QSplashScreen::close);
    QTimer::singleShot(2000, splash, &QSplashScreen::deleteLater);
}

void MainWindow::onTipoEspacioChanged()
{
    actualizarEspacios();
    actualizarRequiereInternet();
}

void MainWindow::onEdadMinChanged()
{
    if (spinEdadMin->value() > spinEdadMax->value()) {
        spinEdadMax->setValue(spinEdadMin->value());
    }
}

void MainWindow::onEdadMaxChanged()
{
    if (spinEdadMax->value() < spinEdadMin->value()) {
        spinEdadMin->setValue(spinEdadMax->value());
    }
}

void MainWindow::actualizarEspacios()
{
    comboEspacio->clear();
    
    if (comboTipoEspacio->currentText() == "Espacio Geográfico") {
        QVector<QString> distritos = gestorDatos->obtenerDistritos();
        for (const QString& distrito : distritos) {
            comboEspacio->addItem(distrito);
        }
        labelEspacio->setText("Distrito:");
    } else {
        QVector<QString> plataformas = gestorDatos->obtenerPlataformasDigitales();
        for (const QString& plataforma : plataformas) {
            comboEspacio->addItem(plataforma);
        }
        labelEspacio->setText("Plataforma:");
    }
}

void MainWindow::actualizarRequiereInternet()
{
    if (comboTipoEspacio->currentText() == "Plataforma Digital") {
        checkAccesoInternet->setChecked(true);
        checkAccesoInternet->setEnabled(false);
    } else {
        checkAccesoInternet->setEnabled(true);
    }
}

void MainWindow::iniciarAnalisis()
{
    // Validar entradas
    validarEntradas();
    
    // Obtener datos del formulario
    ClienteIdeal cliente = obtenerClienteIdeal();
    QString espacio = comboEspacio->currentText();
    QString producto = comboProducto->currentText();
    QString tipoEspacio = comboTipoEspacio->currentText();
    
    // Deshabilitar botón durante análisis
    btnIniciarAnalisis->setEnabled(false);
    btnIniciarAnalisis->setText("Analizando...");
    
    // Realizar análisis
    QApplication::processEvents(); // Actualizar UI
    
    int clientesPotenciales = analizadorTrafico->calcularTraficoConUplift(
        gestorDatos->obtenerPoblacion(), cliente, espacio, producto, tipoEspacio
    );
    
    // Crear resultado
    ResultadoAnalisis resultado(clientesPotenciales, espacio, producto, tipoEspacio, cliente);
    
    // Mostrar resultados
    mostrarResultados(resultado);
    
    // Rehabilitar botón
    btnIniciarAnalisis->setEnabled(true);
    btnIniciarAnalisis->setText("🚀 Iniciar Análisis");
}

void MainWindow::validarEntradas()
{
    if (spinEdadMin->value() > spinEdadMax->value()) {
        QMessageBox::warning(this, "Error de Validación", 
                           "La edad mínima no puede ser mayor que la edad máxima.");
        return;
    }
    
    if (comboEspacio->currentText().isEmpty()) {
        QMessageBox::warning(this, "Error de Validación", 
                           "Debe seleccionar un espacio válido.");
        return;
    }
}

ClienteIdeal MainWindow::obtenerClienteIdeal()
{
    return ClienteIdeal(
        spinEdadMin->value(),
        spinEdadMax->value(),
        comboSexo->currentText(),
        checkAccesoInternet->isChecked()
    );
}

void MainWindow::mostrarResultados(const ResultadoAnalisis& resultado)
{
    QDialog *dialogo = new QDialog(this);
    dialogo->setWindowTitle("Resultados del Análisis");
    dialogo->setFixedSize(600, 450);
    dialogo->setModal(true);
    
    // Centrar diálogo
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    dialogo->move((screenGeometry.width() - dialogo->width()) / 2,
                  (screenGeometry.height() - dialogo->height()) / 2);
    
    QVBoxLayout *layout = new QVBoxLayout(dialogo);
    layout->setSpacing(15);
    
    // Título
    QLabel *titulo = new QLabel("📊 Análisis de Tráfico Publicitario");
    titulo->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; text-align: center;");
    titulo->setAlignment(Qt::AlignCenter);
    layout->addWidget(titulo);
    
    // Resultado principal
    QLabel *resultadoPrincipal = new QLabel(QString("🎯 Clientes Potenciales: %1")
                                           .arg(resultado.clientesPotenciales));
    resultadoPrincipal->setStyleSheet("font-size: 28px; font-weight: bold; color: #e74c3c; "
                                     "background-color: #f8f9fa; padding: 20px; border-radius: 10px; "
                                     "border: 3px solid #e74c3c;");
    resultadoPrincipal->setAlignment(Qt::AlignCenter);
    layout->addWidget(resultadoPrincipal);
    
    // Información del análisis
    QTextEdit *infoAnalisis = new QTextEdit();
    infoAnalisis->setReadOnly(true);
    infoAnalisis->setMaximumHeight(150);
    
    QString infoTexto = QString(
        "🏢 <b>Espacio:</b> %1 (%2)<br>"
        "🛍️ <b>Producto/Servicio:</b> %3<br>"
        "👥 <b>Cliente Ideal:</b> %4 - %5 años, %6<br>"
        "🌐 <b>Requiere Internet:</b> %7<br><br>"
        "📈 <b>Población Total Analizada:</b> %8 personas"
    ).arg(resultado.espacio)
     .arg(resultado.tipoEspacio)
     .arg(resultado.producto)
     .arg(resultado.clienteIdeal.edadMin)
     .arg(resultado.clienteIdeal.edadMax)
     .arg(resultado.clienteIdeal.sexo)
     .arg(resultado.clienteIdeal.requiereInternet ? "Sí" : "No")
     .arg(gestorDatos->obtenerPoblacion().size());
    
    infoAnalisis->setHtml(infoTexto);
    infoAnalisis->setStyleSheet("background-color: #f8f9fa; border: 1px solid #dee2e6; "
                               "border-radius: 5px; padding: 10px;");
    layout->addWidget(infoAnalisis);
    
    // Información adicional
    QLabel *infoAdicional = new QLabel(
        "ℹ️ <i>Este análisis se basa en criterios demográficos y patrones de consumo "
        "digital específicos para el mercado peruano, utilizando datos simulados "
        "representativos de la población objetivo.</i>"
    );
    infoAdicional->setStyleSheet("font-size: 11px; color: #6c757d;");
    infoAdicional->setWordWrap(true);
    layout->addWidget(infoAdicional);
    
    // Botones
    QHBoxLayout *layoutBotones = new QHBoxLayout();
    
    QPushButton *btnNuevoAnalisis = new QPushButton("🔄 Nuevo Análisis");
    btnNuevoAnalisis->setStyleSheet("QPushButton { background-color: #28a745; color: white; "
                                   "font-size: 12px; padding: 8px 16px; border: none; border-radius: 4px; }");
    connect(btnNuevoAnalisis, &QPushButton::clicked, dialogo, &QDialog::accept);
    
    QPushButton *btnCerrar = new QPushButton("❌ Cerrar");
    btnCerrar->setStyleSheet("QPushButton { background-color: #dc3545; color: white; "
                            "font-size: 12px; padding: 8px 16px; border: none; border-radius: 4px; }");
    connect(btnCerrar, &QPushButton::clicked, dialogo, &QDialog::reject);
    
    layoutBotones->addWidget(btnNuevoAnalisis);
    layoutBotones->addWidget(btnCerrar);
    layout->addLayout(layoutBotones);
    
    dialogo->exec();
    delete dialogo;
}

QString MainWindow::obtenerRutaCSV()
{
    QString rutaDatos = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    std::cout << "Ruta de datos: " << rutaDatos.toStdString() << std::endl;
    return rutaDatos + "/poblacion_arequipa.csv";
}
