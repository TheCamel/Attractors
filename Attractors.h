#ifndef ATTRACTORS_H
#define ATTRACTORS_H

#include <QtGui>

class Attractors : public QMainWindow
{
    Q_OBJECT
    
public:
    Attractors(QWidget *parent = 0);
private:
    QLabel* label;
    QAction* actionPlay;
    QAction* actionPause;

    double x;
    double y;
    bool first;
    double pdja, pdjb, pdjc, pdjd;
    double cliffa, cliffb, cliffc, cliffd;
    QTimer* timer;
    bool active;
    QPixmap pixmap;

    QSettings* settings;
    QColor color;
    int attr;

    // settings window
    QGridLayout* mainLayout;
    QWidget* paramWidget;
    QDialog* window;
    QColor colorTmp;
    QLabel* widColor;
    QPushButton* changeColor;

    QVector<QWidget*> hiddens;

    QGridLayout** layout;

    QComboBox* attractor;

    QPushButton* ok;
    QPushButton* cancel;

    // systems
    QDoubleSpinBox** spinA;
    QDoubleSpinBox** spinB;
    QDoubleSpinBox** spinC;
    QDoubleSpinBox** spinD;

    QSpinBox* smx;
    QSpinBox* smy;

    QSpinBox* sdx;
    QSpinBox* sdy;

    int mx, my, dx, dy;

    void drawColor(QColor);

private slots:
    void play();
    void pause();
    void paint();
    void restart();
    void save();
    void openParams();
    void chooseColor();
    void validate();
    void changeAttractor(int);
};

#endif // ATTRACTORS_H
