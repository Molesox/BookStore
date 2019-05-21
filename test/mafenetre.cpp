#include "mafenetre.h"

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
    boutonSupprimerLivre = new QPushButton("Supprimer un livre",this);

    labelLM = new QLabel("Nombre de livres dans le magasin :");
    labelLP = new QLabel("Nombre de livres pris depuis le début :");
    labelC = new QLabel("Nombre de consommateurs dans le magasin :");
    labelLR = new QLabel("Nombre de livres qu'il reste dans la catégorie ");
    labelGenres = new QLabel("Liste des genres :");
    labelLivres = new QLabel("Liste des livres :");
    labelConsommateurs = new QLabel("Liste des consommateurs ");
    labelHorizontal = new QLabel("Régler la taille horizontale de la fenêtre");
    labelVertical = new QLabel("Régler la taille verticale de la fenêtre");
    labelQuitter = new QLabel("Quitter");

    cBGC = new QComboBox(this);
    // A CHANGER QUAND LIE AU RESTE
    cBGC->addItem("Les choix");
    cBGC->addItem("Pas Rigolos");
    //
    cBG = new QComboBox(this);
    // A CHANGER QUAND LIE AU RESTE
    cBG->addItem("Rigolo");
    cBG->addItem("Pas Rigolo");
    //
    cBL = new QComboBox(this);
    // A CHANGER QUAND LIE AU RESTE
    cBL->addItem("Rigolos");
    cBL->addItem("Pas Rigolos");
    //
    cBC = new QComboBox(this);
    // A CHANGER QUAND LIE AU RESTE
    cBC->addItem("J'en ai marre");
    cBC->addItem("Toujours...");
    //
    // REMPLACER LES EXEMPLES PAR UNE BOUCLE QUI AJOUTE TOUS LES ITEMS CORRESPONDANT DANS LES DONNEES OU DIRECTEMENT LA LISTE

    lcd_lm = new QLCDNumber(this);
    lcd_lp = new QLCDNumber(this);
    lcd_c = new QLCDNumber(this);
    // LES TROIS AU-DESSUS DOIVENT ETRE MIS A JOUR REGULIEREMENT PAR RAPPORT AU PRORGAMME / AVEC UN BOUTON ACTUALISER ?
    lcd_lr = new QLCDNumber(this);

    slider_h = new QSlider(Qt::Horizontal, this);
    slider_h->setRange(1200,1800);
    slider_v = new QSlider(Qt::Horizontal, this);

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
    layoutG0->addWidget(labelLR,3,0);
    layoutG0->addWidget(lcd_lm,0,1,1,2);
    layoutG0->addWidget(lcd_lp,1,1,1,2);
    layoutG0->addWidget(lcd_c,2,1,1,2);
    layoutG0->addWidget(cBGC,3,1,1,1);
    layoutG0->addWidget(lcd_lr,3,2,1,1);
    layoutG0->addWidget(labelGenres,4,0);
    layoutG0->addWidget(labelLivres,5,0);
    layoutG0->addWidget(labelConsommateurs,6,0);
    layoutG0->addWidget(cBG,4,1,1,2);
    layoutG0->addWidget(cBL,5,1,1,2);
    layoutG0->addWidget(cBC,6,1,1,2);

    layoutV0->addWidget(boutonStop);
    layoutV0->addWidget(boutonStart);
    layoutV0->addWidget(boutonScenario1);
    layoutV0->addWidget(boutonScenario2);
    layoutV0->addWidget(boutonScenario3);

    layoutG1->addWidget(labelHorizontal,0,0);
    layoutG1->addWidget(labelVertical,1,0);
    layoutG1->addWidget(labelQuitter,2,0);
    layoutG1->addWidget(slider_h,0,1);
    layoutG1->addWidget(slider_v,1,1);
    layoutG1->addWidget(boutonQuitter,2,1);

    layoutV3->addWidget(boutonAjouterLivre);
    layoutV3->addWidget(boutonSupprimerLivre);

    fenetre->setLayout(layoutH0);
    fenetre->show();


    QObject::connect(boutonQuitter, SIGNAL(clicked()),qApp, SLOT(quit()));
    QObject::connect(boutonAjouterLivre, SIGNAL(clicked()), this, SLOT(ouvrirDialogue()));
    QObject::connect(boutonSupprimerLivre, SIGNAL(clicked()), this, SLOT(ouvrirDialogueDel()));
    //QObject::connect(slider_h, SIGNAL(valueChanged(int)), this, SLOT(changerLargeur(int)));
    QObject::connect(boutonScenario1, SIGNAL(clicked()), this, SLOT(runSOne()));
    QObject::connect(boutonScenario2, SIGNAL(clicked()), this, SLOT(runSTwo()));
    QObject::connect(boutonScenario3, SIGNAL(clicked()), this, SLOT(runSThree()));
    QObject::connect(boutonStart, SIGNAL(clicked()), this, SLOT(start()));
    QObject::connect(boutonStop, SIGNAL(clicked()), this, SLOT(stop()));
    //QObject::connect(cBGC, SIGNAL(currentTextChanged()),this, SLOT(actualiseLCD()));

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
                    bool ok = false;
                    QString id = QInputDialog::getText(this, "Id","Quel est l'id du livre ?", QLineEdit::Normal, QString(), &ok);
                    if (ok && !id.isEmpty())
                    {
                        idf = id;
                        QMessageBox::information(this, "Id", "Merci, l'id \""+ id+ "\" a été enregistré.");
                        QMessageBox::information(this, "Résultat", "Voici le résultat de l'inscription :\nTitre : \""+ namef + "\"\nAuteur : \""+ auteurf+ "\"\nDate : \""+datef+ "\"\nGenre : \""+ genref+"\"\nId : \""+ id);
                        //
                        // A ENVOYER DANS LES INFORMATIONS DE LA LIBRAIRIE !!!!
                        //
                    } else {
                        QMessageBox::critical(this, "Id", "L'id est vide, veuillez recommencer.");
                    }
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
void MaFenetre::ouvrirDialogueDel()
{
    QString namef = "rien";
    QString auteurf = "personne";
    bool ok = false;
    QString name = QInputDialog::getText(this, "Name","Quel est le nom du livre à supprimer ?", QLineEdit::Normal, QString(), &ok);
    if (ok && !name.isEmpty())
    {
        namef = name;
        QMessageBox::information(this, "Name", "Le titre \""+ name+ "\" va être supprimé. Veuillez préciser le nom de l'auteur.");
        bool ok = false;
        QString auteur = QInputDialog::getText(this, "Name","Quel est le nom de l'auteur ?", QLineEdit::Normal, QString(), &ok);
        if (ok && !auteur.isEmpty())
        {
            auteurf = auteur;
            QMessageBox::information(this, "Auteur", "Merci, le livre \""+ name+ "\" écrit par \"" +auteur+ "\" va être supprimé s'il existe.");
            //
            // A SUPPRIMER DANS LES INFORMATIONS DE LA LIBRAIRIE !!!!
            //
        } else {
            QMessageBox::critical(this, "Auteur", "L'auteur est vide, veuillez recommencer.");
        }
    } else {
        QMessageBox::critical(this, "Name", "Le titre est vide, veuillez recommencer.");
    }
}
void MaFenetre::runSOne()
{
    //
    //  LANCER LE SCENARIO 1 DANS LE MAIN
    //
}
void MaFenetre::runSTwo()
{
    //
    //  LANCER LE SCENARIO 2 DANS LE MAIN
    //
}
void MaFenetre::runSThree()
{
    //
    //  LANCER LE SCENARIO 3 DANS LE MAIN
    //
}
void MaFenetre::start()
{
    //
    //  "Relancer le programme" DEPUIS LE MAIN
    //
}
void MaFenetre::stop()
{
    //
    //  "Mettre sur pause" le programme DEPUIS LE MAIN
    //
}
//void MaFenetre::actualiseLCD()
//{
    // REGARDE DANS LE PROGRAMME COMBIEN DE LIRVES IL RESTE DE CE GENRE
//}
