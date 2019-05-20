#ifndef DEF_MAFENETRE
#define DEF_MAFENETRE

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLCDNumber>
#include <QSlider>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QLCDNumber>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>

class MaFenetre : public QWidget
{
    Q_OBJECT

    public:
    MaFenetre();

    public slots:
    void changerLargeur(int largeur);
    //void changerHauteur(int hauteur);
    void ouvrirDialogue();
    void ouvrirDialogueDel();
    void runSOne();
    void runSTwo();
    void runSThree();
    void start();
    void stop();
    //void actualiseLCD();

    signals:
    void agrandissementMax();

    private:
    QWidget *fenetre;
    QHBoxLayout *layoutH0;
    QVBoxLayout *layoutV0;
    QVBoxLayout *layoutV1;
    QVBoxLayout *layoutV2;
    QVBoxLayout *layoutV3;
    QGridLayout *layoutG0;
    QGridLayout *layoutG1;
    QLabel *labelLM;
    QLabel *labelLP;
    QLabel *labelC;
    QLabel *labelLR;
    QLabel *labelGenres;
    QLabel *labelLivres;
    QLabel *labelConsommateurs;
    QLabel *labelHorizontal;
    QLabel *labelVertical;
    QLabel *labelQuitter;
    QComboBox *cBG;
    QComboBox *cBL;
    QComboBox *cBC;
    QComboBox *cBGC;
    QPushButton *boutonStart;
    QPushButton *boutonStop;
    QPushButton *boutonScenario1;
    QPushButton *boutonScenario2;
    QPushButton *boutonScenario3;
    QPushButton *boutonQuitter;
    QPushButton *boutonAjouterLivre;
    QPushButton *boutonSupprimerLivre;
    QLCDNumber *lcd_lm;
    QLCDNumber *lcd_lp;
    QLCDNumber *lcd_c;
    QLCDNumber *lcd_lr;
    QSlider *slider_h;
    QSlider *slider_v;

};

#endif
