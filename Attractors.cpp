#include "Attractors.h"

Attractors::Attractors(QWidget *parent)
    : QMainWindow(parent)
{
    label = new QLabel;
    pixmap = QPixmap(600, 600);
    setCentralWidget(label);

    first = true;
    active = false;
    x=0;
    y=0;

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(paint()));

    setWindowTitle("Attractors");

    settings = new QSettings;

    attr = settings->value("attractor", 0).toInt();

    pdja = settings->value("PdJ/a", 1.4).toDouble();
    pdjb = settings->value("PdJ/b", -2.3).toDouble();
    pdjc = settings->value("PdJ/c", 2.4).toDouble();
    pdjd = settings->value("PdJ/d", -2.1).toDouble();


    cliffa = settings->value("Cliff/a", 2).toDouble();
    cliffb = settings->value("Cliff/b", -2).toDouble();
    cliffc = settings->value("Cliff/c", 2).toDouble();
    cliffd = settings->value("Cliff/d", -1.5).toDouble();

    color = settings->value("color", QColor(0,0,0,2)).value<QColor>();

    mx = settings->value("x/m", 100).toInt();
    dx = settings->value("x/d", 600).toInt();

    my = settings->value("y/m", 100).toInt();
    dy = settings->value("y/d", 600).toInt();

    actionPlay = new QAction("&Marche", this);
    actionPause = new QAction("&Pause", this);
    QAction* actionRestart = new QAction("Tout &effacer", this);

    QAction* actionParams = new QAction("&Paramètres", this);
    QAction* actionSave = new QAction("&Sauvegarder", this);

    QAction* actionQuit = new QAction("&Quitter", this);

    QToolBar *toolbar = addToolBar("Actions");
    toolbar->setMovable(false);

    actionPlay->setIcon(QIcon::fromTheme("media-playback-start", QIcon(qApp->applicationDirPath() + "/play.png")));
    actionPause->setIcon(QIcon::fromTheme("media-playback-pause", QIcon(qApp->applicationDirPath() + "/pause.png")));
    actionRestart->setIcon(QIcon::fromTheme("view-refresh", QIcon(qApp->applicationDirPath() + "/restart.png")));

    actionParams->setIcon(QIcon::fromTheme("document-properties", QIcon(qApp->applicationDirPath() + "/params.png")));
    actionSave->setIcon(QIcon::fromTheme("document-save", QIcon(qApp->applicationDirPath() + "/save.png")));

    actionQuit->setIcon(QIcon::fromTheme("system-shutdown", QIcon(qApp->applicationDirPath() + "/quit.png")));

    toolbar->addAction(actionPlay);
    toolbar->addAction(actionPause);
    toolbar->addAction(actionRestart);

    toolbar->addSeparator();

    toolbar->addAction(actionParams);
    toolbar->addAction(actionSave);

    toolbar->addSeparator();

    toolbar->addAction(actionQuit);

    connect(actionPlay, SIGNAL(triggered()), this, SLOT(play()));
    connect(actionPause, SIGNAL(triggered()), this, SLOT(pause()));
    connect(actionRestart, SIGNAL(triggered()), this, SLOT(restart()));

    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));

    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    actionPause->setEnabled(false);

    setFixedSize(dx, dy + 30);

    window = new QDialog(this);
    mainLayout = new QGridLayout;
    layout = new QGridLayout*[2];
    window->setModal(true);
    window->setLayout(mainLayout);
    window->setWindowTitle("Paramètres");

    attractor = new QComboBox;
    QStringList names;
    names << "Peter de Jong" << "Clifford";
    attractor->addItems(names);
    attractor->setCurrentIndex(attr);
    connect(attractor, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAttractor(int)));

    paramWidget = new QWidget;

    spinA = new QDoubleSpinBox*[2];
    spinB = new QDoubleSpinBox*[2];
    spinC = new QDoubleSpinBox*[2];
    spinD = new QDoubleSpinBox*[2];

    QFont text;
    text.setPointSize(15);

    for(int i=0; i<2; i++)
    {
        spinA[i] = new QDoubleSpinBox;
        spinA[i]->setRange(-100,100);
        spinA[i]->setDecimals(3);
        spinA[i]->setSingleStep(0.1);

        spinB[i] = new QDoubleSpinBox;
        spinB[i]->setRange(-100,100);
        spinB[i]->setDecimals(3);
        spinB[i]->setSingleStep(0.1);

        spinC[i] = new QDoubleSpinBox;
        spinC[i]->setRange(-100,100);
        spinC[i]->setDecimals(3);
        spinC[i]->setSingleStep(0.1);

        spinD[i] = new QDoubleSpinBox;
        spinD[i]->setRange(-100,100);
        spinD[i]->setDecimals(3);
        spinD[i]->setSingleStep(0.1);

        layout[i] = new QGridLayout;

        if(i==0) // PdJ
        {
            QLabel* labelXnCos = new QLabel("x<sub>n</sub>)");
            QLabel* labelYnSin = new QLabel("y<sub>n</sub>) - cos(");

            QLabel* labelXnSin = new QLabel("x<sub>n</sub>) - cos(");
            QLabel* labelYnCos = new QLabel("y<sub>n</sub>)");

            QLabel* labelXn1 = new QLabel("x<sub>n+1</sub> = sin(");
            QLabel* labelYn1 = new QLabel("y<sub>n+1</sub> = sin(");

            layout[i]->addWidget(labelXn1, 0, 0);
            layout[i]->addWidget(spinA[i], 0, 1);
            layout[i]->addWidget(labelYnSin, 0, 2);
            layout[i]->addWidget(spinB[i], 0, 3);
            layout[i]->addWidget(labelXnCos, 0, 4);

            layout[i]->addWidget(labelYn1, 1, 0);
            layout[i]->addWidget(spinC[i], 1, 1);
            layout[i]->addWidget(labelXnSin, 1, 2);
            layout[i]->addWidget(spinD[i], 1, 3);
            layout[i]->addWidget(labelYnCos, 1, 4);

            labelXnCos->setFont(text);
            labelYnSin->setFont(text);
            labelYnCos->setFont(text);
            labelXnSin->setFont(text);
            labelXn1->setFont(text);
            labelYn1->setFont(text);

            spinA[i]->setValue(pdja);
            spinB[i]->setValue(pdjb);
            spinC[i]->setValue(pdjc);
            spinD[i]->setValue(pdjd);
        }
        else // Clifford
        {
            QLabel* cosX = new QLabel("cos(");
            QLabel* cosY = new QLabel("cos(");
            QLabel* labelXnCos = new QLabel("x<sub>n</sub>)");
            QLabel* labelYnSin = new QLabel("y<sub>n</sub>) +");

            QLabel* labelXnSin = new QLabel("x<sub>n</sub>) +");
            QLabel* labelYnCos = new QLabel("y<sub>n</sub>)");

            QLabel* labelXn1 = new QLabel("x<sub>n+1</sub> = sin(");
            QLabel* labelYn1 = new QLabel("y<sub>n+1</sub> = sin(");

            QDoubleSpinBox* aPrime = new QDoubleSpinBox;
            aPrime->setRange(-100,100);
            aPrime->setDecimals(3);
            aPrime->setEnabled(false);
            connect(spinA[i], SIGNAL(valueChanged(double)), aPrime, SLOT(setValue(double)));

            QDoubleSpinBox* bPrime = new QDoubleSpinBox;
            bPrime->setRange(-100,100);
            bPrime->setDecimals(3);
            bPrime->setEnabled(false);
            connect(spinB[i], SIGNAL(valueChanged(double)), bPrime, SLOT(setValue(double)));

            layout[i]->addWidget(labelXn1, 0, 0);
            layout[i]->addWidget(spinA[i], 0, 1);
            layout[i]->addWidget(labelYnSin, 0, 2);
            layout[i]->addWidget(spinC[i], 0, 3);
            layout[i]->addWidget(cosX, 0, 4);
            layout[i]->addWidget(aPrime, 0, 5);
            layout[i]->addWidget(labelXnCos, 0, 6);

            layout[i]->addWidget(labelYn1, 1, 0);
            layout[i]->addWidget(spinB[i], 1, 1);
            layout[i]->addWidget(labelXnSin, 1, 2);
            layout[i]->addWidget(spinD[i], 1, 3);
            layout[i]->addWidget(cosY, 1, 4);
            layout[i]->addWidget(bPrime, 1, 5);
            layout[i]->addWidget(labelYnCos, 1, 6);

            labelXnCos->setFont(text);
            labelYnSin->setFont(text);
            labelYnCos->setFont(text);
            labelXnSin->setFont(text);
            labelXn1->setFont(text);
            labelYn1->setFont(text);
            cosX->setFont(text);
            cosY->setFont(text);

            spinA[i]->setValue(cliffa);
            spinB[i]->setValue(cliffb);
            spinC[i]->setValue(cliffc);
            spinD[i]->setValue(cliffd);
        }
    }
    mainLayout->addWidget(attractor, 0, 0, Qt::AlignCenter);
    paramWidget->setLayout(layout[attr]);
    mainLayout->addWidget(paramWidget, 1, 0, Qt::AlignCenter);

    QHBoxLayout* layoutButtons = new QHBoxLayout;
    ok = new QPushButton("&Ok");
    cancel = new QPushButton("&Annuler");

    layoutButtons->addWidget(ok);
    layoutButtons->addWidget(cancel);

    QHBoxLayout* layoutColor = new QHBoxLayout;
    widColor = new QLabel;
    layoutColor->addWidget(widColor);
    QLabel* textColor = new QLabel("Couleur");
    textColor->setFont(text);
    QPushButton* buttonColor = new QPushButton("Changer");
    layoutColor->addWidget(buttonColor);

    smx = new QSpinBox;
    smx->setRange(0,2048);

    sdx = new QSpinBox;
    sdx->setRange(128,2048);
    sdx->setSuffix(" px");

    smy = new QSpinBox;
    smy->setRange(0,2048);

    sdy = new QSpinBox;
    sdy->setRange(128,2048);
    sdy->setSuffix(" px");

    smx->setValue(mx);
    sdx->setValue(dx);
    smy->setValue(my);
    sdy->setValue(dy);

    QLabel* dimensions = new QLabel("Dimensions");
    dimensions->setFont(text);

    QLabel* lx = new QLabel("Zoom en x :");
    QLabel* ly = new QLabel("Zoom en y :");
    QLabel* largeur = new QLabel("Largeur :");
    QLabel* hauteur = new QLabel("Hauteur :");
    QGridLayout* layoutDimensions = new QGridLayout;

    layoutDimensions->addWidget(largeur, 0, 0);
    layoutDimensions->addWidget(sdx, 0, 1);
    layoutDimensions->addWidget(lx, 0, 2);
    layoutDimensions->addWidget(smx, 0, 3);

    layoutDimensions->addWidget(hauteur, 1, 0);
    layoutDimensions->addWidget(sdy, 1, 1);
    layoutDimensions->addWidget(ly, 1, 2);
    layoutDimensions->addWidget(smy, 1, 3);

    QGridLayout* horizontal = new QGridLayout;
    horizontal->addWidget(textColor, 0, 0, Qt::AlignCenter);
    horizontal->addWidget(dimensions, 0, 1, Qt::AlignCenter);


    horizontal->addLayout(layoutColor, 1, 0, Qt::AlignCenter);
    horizontal->addLayout(layoutDimensions, 1, 1, Qt::AlignCenter);

    mainLayout->addLayout(horizontal, 2, 0, Qt::AlignCenter);
    mainLayout->addLayout(layoutButtons, 3, 0, Qt::AlignRight);


    connect(actionParams, SIGNAL(triggered()), this, SLOT(openParams()));
    connect(ok, SIGNAL(clicked()), this, SLOT(validate()));
    connect(cancel, SIGNAL(clicked()), window, SLOT(close()));
    connect(buttonColor, SIGNAL(clicked()), this, SLOT(chooseColor()));

    buttonColor->setFocusProxy(ok);
}

void Attractors::play()
{
    actionPlay->setEnabled(false);
    actionPause->setEnabled(true);
    timer->start(0);
    active = true;
}

void Attractors::pause()
{
    actionPause->setEnabled(false);
    actionPlay->setEnabled(true);
    timer->stop();
    active = false;
}

void Attractors::paint()
{
    if(first)
    {
        pixmap = QPixmap(dx, dy);
        pixmap.fill(Qt::transparent);
        first = false;
    }
    else if(active)
    {
        QPainter painter(&pixmap);
        painter.setPen(color);
        int j = 10000;
        while(--j)
        {
            double x_prec = x;

            if(attr == 0) // PdJ
            {
                x = sin(pdja*y) - cos(pdjb*x);
                y = sin(pdjc*x_prec) - cos(pdjd*y);
            }
            else if(attr == 1) // Clifford
            {
                x = sin(cliffa*y) + cliffc*cos(cliffa*x);
                y = sin(cliffb*x_prec) + cliffd*cos(cliffb*y);
            }

            painter.drawPoint((mx*x+dx/2), (-my*y+dy/2));
        }
        painter.end();
    }
    label->setPixmap(pixmap);
}

void Attractors::restart()
{
    first = true;
    x=0;
    y=0;
    paint();
}

void Attractors::save()
{
    pixmap.save("attractor.png");
}

void Attractors::openParams()
{
    attractor->setCurrentIndex(attr);

    int i=0;
    spinA[i]->setValue(pdja);
    spinB[i]->setValue(pdjb);
    spinC[i]->setValue(pdjc);
    spinD[i]->setValue(pdjd);

    i=1;
    spinA[i]->setValue(cliffa);
    spinB[i]->setValue(cliffb);
    spinC[i]->setValue(cliffc);
    spinD[i]->setValue(cliffd);

    smx->setValue(mx);
    sdx->setValue(dx);
    smy->setValue(my);
    sdy->setValue(dy);

    colorTmp = color;
    drawColor(colorTmp);
    window->show();
    ok->setFocus();
    ok->setFocusPolicy(Qt::StrongFocus);
}

void Attractors::chooseColor()
{
    QColor pick = QColorDialog::getColor(colorTmp, this, tr("Select color"), QColorDialog::ShowAlphaChannel);
    if(pick.isValid())
    {
        colorTmp = pick;
        drawColor(colorTmp);
    }
}

void Attractors::validate()
{
    attr = attractor->currentIndex();
    settings->setValue("attractor", attr);

    pdja = spinA[0]->value();
    pdjb = spinB[0]->value();
    pdjc = spinC[0]->value();
    pdjd = spinD[0]->value();

    cliffa = spinA[1]->value();
    cliffb = spinB[1]->value();
    cliffc = spinC[1]->value();
    cliffd = spinD[1]->value();

    settings->setValue("PdJ/a", pdja);
    settings->setValue("PdJ/b", pdjb);
    settings->setValue("PdJ/c", pdjc);
    settings->setValue("PdJ/d", pdjd);

    settings->setValue("Cliff/a", cliffa);
    settings->setValue("Cliff/b", cliffb);
    settings->setValue("Cliff/c", cliffc);
    settings->setValue("Cliff/d", cliffd);

    mx = smx->value();
    dx = sdx->value();

    my = smy->value();
    dy = sdy->value();

    settings->setValue("x/m", mx);
    settings->setValue("x/d", dx);
    settings->setValue("y/m", my);
    settings->setValue("y/d", dy);

    pixmap = QPixmap(dx, dy);

    setFixedSize(dx, dy + 30);

    color = colorTmp;
    drawColor(colorTmp);
    window->close();
    restart();
}

void Attractors::changeAttractor(int newAttr)
{
    QWidget* hidden = new QWidget;
    hidden->setLayout(paramWidget->layout());
    hiddens.push_back(hidden);
    paramWidget->setLayout(layout[newAttr]);
    if(hiddens.size() == 2)
    {
        delete hiddens.first();
        hiddens.pop_front();
    }
}

void Attractors::drawColor(QColor m_color)
{
    QPixmap render(34, 22);
    render.fill(Qt::white);
    QPainter painter(&render);
    int count=0;
    painter.setPen(Qt::transparent);

    for(int i=17; i<33; i+=4)
    {
        for(int j=1; j<21; j+=4)
        {
            int rgb = 255-(count%2)*128;
            painter.setBrush(QBrush(QColor(rgb, rgb, rgb)));
            painter.drawRect(i, j, i+4, j+4);
            count++;
        }
    }

    painter.setBrush(QBrush(m_color));
    painter.drawRect(17, 1, 32, 20);

    painter.setBrush(QBrush(QColor(m_color.rgb())));
    painter.drawRect(1, 1, 16, 20);

    painter.setBrush(QBrush(Qt::transparent));
    painter.setPen(Qt::black);
    painter.drawRect(0, 0, 33, 21);

    widColor->setPixmap(render);
}
