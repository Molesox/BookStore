#include <QApplication>
#include "mafenetre.h"

#include<iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MaFenetre fenetre;
    std::cout<<"ok"<<std::endl;


    return app.exec();
}
