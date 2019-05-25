#include "mafenetre.h"
#include <iostream>
#include <string>
#include <thread>

#include "Library.h"
#include "Shop.h"
#include "Seller.h"
#include "Logger.h"
#include <random>
#include <chrono>
#include <atomic>
#include <QLCDNumber>


using namespace std;

void custom_thread(Customer *c) {
    hello:
    if (c->visit_shop() == 0) {

        do {
            c->ask_book();
            c->read_book();
        } while (c->return_book() != 0);

        c->quit_shop();

        return;

    } else {
        c->i_will_be_back();
        goto hello;
    }

}



void john_thread(Seller *s) {

    while (true) {
        if (s->give_book() == -2) {
            break;
        }
        s->get_back_book();
    }
}






MaFenetre::MaFenetre() : QWidget()
{
    setFixedSize(1200, 400);

    fenetre = new QWidget;
    fenetre->setBaseSize(1200,400);

    boutonStop = new QPushButton("Stop",this);
    boutonStart = new QPushButton("Start",this);
    boutonScenario1 = new QPushButton("Lancer Scénario 1",this);
    boutonScenario2 = new QPushButton("Lancer Scénario 2",this);
    boutonScenario3 = new QPushButton("Lancer Scénario 3",this);
    boutonQuitter = new QPushButton("Quitter",this);
    boutonAjouterLivre = new QPushButton("Ajouter un Livre",this);

    labelLM = new QLabel("Nombre de livres dans le magasin :");
    labelLP = new QLabel("Nombre de livres pris depuis le début :");
    labelC = new QLabel("Nombre de consommateurs dans le magasin :");
    labelConsommateurs = new QLabel("Liste des consommateurs ");
    labelCustomerState1 = new QLabel("Etat du consommateur sélectionné");
    labelCustomerState2 = new QLabel("AUCUN");

    cBC = new QComboBox(this);
    // A CHANGER QUAND LIE AU RESTE

    //for(auto custom: v_custom){
      // cBC->addItem("custom.getId() trouver comment concatener ");
    //}
    cBC->addItem("Customer[1] ");
    cBC->addItem("Customer[2] ");
    cBC->addItem("Customer[3] ");
    // REMPLACER LES EXEMPLES PAR UNE BOUCLE QUI AJOUTE TOUS LES ITEMS CORRESPONDANT DANS LES DONNEES OU DIRECTEMENT LA LISTE

    lcd_lm = new QLCDNumber(this);
    lcd_lm->display(16559);
    lcd_lp = new QLCDNumber(this);
    lcd_lp->display(0);
    lcd_c = new QLCDNumber(this);
    lcd_c->display(0);

    layoutH0 = new QHBoxLayout;
    layoutV0  = new QVBoxLayout;
    layoutV1  = new QVBoxLayout;
    layoutV2  = new QVBoxLayout;
    layoutV3  = new QVBoxLayout;
    layoutG0 = new QGridLayout;
    layoutG1 = new QGridLayout;

    layoutH0->addLayout(layoutV0);
    layoutH0->addLayout(layoutV1);

    layoutV0->addLayout(layoutG0);

    layoutV1->addLayout(layoutV2);
    layoutV1->addLayout(layoutV3);

    layoutV2->addLayout(layoutG1);

    layoutG0->addWidget(labelLM,0,0);
    layoutG0->addWidget(labelLP,1,0);
    layoutG0->addWidget(labelC,2,0);
    layoutG0->addWidget(lcd_lm,0,1,1,2);
    layoutG0->addWidget(lcd_lp,1,1,1,2);
    layoutG0->addWidget(lcd_c,2,1,1,2);
    layoutG0->addWidget(labelConsommateurs,6,0);
    layoutG0->addWidget(labelCustomerState1,7,0);
    layoutG0->addWidget(labelCustomerState2,7,1);
    layoutG0->addWidget(cBC,6,1,1,2);

    layoutV0->addWidget(boutonStop);
    layoutV0->addWidget(boutonStart);
    layoutV0->addWidget(boutonScenario1);
    layoutV0->addWidget(boutonScenario2);
    layoutV0->addWidget(boutonScenario3);

    layoutG1->addWidget(boutonQuitter,2,1);

    layoutV3->addWidget(boutonAjouterLivre);

    fenetre->setLayout(layoutH0);
    fenetre->show();


    QObject::connect(boutonQuitter, SIGNAL(clicked()),qApp, SLOT(quit()));
    QObject::connect(boutonAjouterLivre, SIGNAL(clicked()), this, SLOT(ouvrirDialogue()));
    QObject::connect(cBC, SIGNAL(activated()),this,SLOT(stateChanged));
    QObject::connect(boutonScenario1, SIGNAL(clicked()), this, SLOT(runSOne()));
    QObject::connect(boutonScenario2, SIGNAL(clicked()), this, SLOT(runSTwo()));
    QObject::connect(boutonScenario3, SIGNAL(clicked()), this, SLOT(runSThree()));
    QObject::connect(boutonStart, SIGNAL(clicked()), this, SLOT(start()));
    QObject::connect(boutonStop, SIGNAL(clicked()), this, SLOT(stop()));

}

void MaFenetre::changerLargeur(int largeur)
{
    setFixedSize(largeur,1200);
}
void MaFenetre::ouvrirDialogue()
{
    QString namef = "rien";
    QString auteurf = "personne";
    QString datef = "jamais";
    QString genref = "aucun";
    QString idf = "aucun";
    bool ok = false;
    QString name = QInputDialog::getText(this, "Name","Quel est le nom du livre ?", QLineEdit::Normal, QString(), &ok);
    if (ok && !name.isEmpty())
    {
        namef = name;
        QMessageBox::information(this, "Name", "Merci, le titre \""+ name+ "\" a été enregistré.");
        bool ok = false;
        QString auteur = QInputDialog::getText(this, "Auteur","Quel est l'auteur du livre ?", QLineEdit::Normal, QString(), &ok);
        if (ok && !auteur.isEmpty())
        {
            auteurf = auteur;
            QMessageBox::information(this, "Auteur", "Merci, l'auteur \""+ auteur+ "\" a été enregistré.");
            bool ok = false;
            QString date = QInputDialog::getText(this, "Date","Quelle est la date de parution du livre ?", QLineEdit::Normal, QString(), &ok);
            if (ok && !date.isEmpty())
            {
                datef = date;
                QMessageBox::information(this, "Date", "Merci, la date \""+ date+ "\" a été enregistré.");
                bool ok = false;
                QString genre = QInputDialog::getText(this, "Genre","Quel est le genre du livre ?", QLineEdit::Normal, QString(), &ok);
                if (ok && !genre.isEmpty())
                {
                    genref = genre;
                    QMessageBox::information(this, "Genre", "Merci, le genre \""+ genre+ "\" a été enregistré.");
                    QMessageBox::information(this, "Résultat", "Voici le résultat de l'inscription :\nTitre : \""+ namef + "\"\nAuteur : \""+ auteurf+ "\"\nDate : \""+datef+ "\"\nGenre : \""+ genref);
                        //
                        //utiliser "add_book(namef,auteurf,datef,genref)";
                        //int n = lcd_lm->value();
                        //lcd_lm->display(n+1);
                        //
                } else {
                    QMessageBox::critical(this, "Genre", "Le genre est vide, veuillez recommencer.");
                }
            } else {
                QMessageBox::critical(this, "Date", "La date est vide, veuillez recommencer.");
            }
        } else {
            QMessageBox::critical(this, "Auteur", "L'auteur est vide, veuillez recommencer.");
        }
    } else {
        QMessageBox::critical(this, "Name", "Le titre est vide, veuillez recommencer.");
    }
}
void MaFenetre::runSOne()
{


    for (int j = 0; j < 3; ++j) {
        int a[] = {500 + j, 700 - j, 600 - j};
        v_custom.push_back(new Customer(migros, "unknown", 3, a));
    }


    std::mt19937_64 eng{std::random_device{}()};  // or seed however you want
    std::uniform_int_distribution<> dist{10, 100};


    std::thread c_threads[3];
    for (int i = 0; i < 3; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
        c_threads[i] = std::thread(custom_thread, v_custom[i]);

    }

    for (auto &c_thread : c_threads) {
        c_thread.join();
    }

}
void MaFenetre::runSTwo()
{
    //  LANCER LE SCENARIO 2
}
void MaFenetre::runSThree()
{
    //  LANCER LE SCENARIO 3
}
void MaFenetre::start()
{
        lib = new Library(R"(/Users/Maxence/Desktop/BookStore-master/book_dataset.csv)");
        cout << *lib << endl << endl;

        migros = new Shop(lib, 2, 2);
        john = new Seller(migros);

        t_JOHN = new std::thread(john_thread, john);

}
void MaFenetre::stop()
{
    john->quit();
    migros->close();
    t_JOHN->join();
    cout<<"John leaves the shop!"<<endl;

    for (auto c : v_custom) {
        delete c;
    }
    v_custom.erase(v_custom.begin(),v_custom.end());
    Customer::c_id = 0;
    Book::id=0;
    delete lib;
    delete t_JOHN;
    delete migros;
}
void MaFenetre::stateChanged()
{
    labelCustomerState2->setText("//ETAT DU CUSTOMER [...]");
    fenetre->show();
}
//void MaFenetre::actualiseLCD()
//{
  // lcd_lm -> display(lib->getNbBooks());
  //  lcd_lp->display(Customer::getNbBooksTaken());
  // lcd_lp->display(CUstommer::getNbConsommateurMagasin());
//}
