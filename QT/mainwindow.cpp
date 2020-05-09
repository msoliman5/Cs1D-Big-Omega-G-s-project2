#include "mainwindow.h"
#include "ui_mainwindow.h"

QVector <QString> outputNodes;
QVector <Souvenir*> souvenirs;
QVector <QString> teams;

MainWindow::MainWindow(Controller *controller,graph *Graph ,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
     m_controller(controller),
     m_graph(Graph)
{

    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->Login);


    ui->teamcombobox->clear();
    ui->TeamCombosouv->clear();
    ui->stadiumcombobox->clear();
    ui->tripBox->clear();
    for (int i = 0; i < m_controller->listOfTeams.count();i++)
    {
        QString teamname = m_controller->listOfTeams[i]->getTeamName();
        QString stadium = m_controller->listOfTeams[i]->getStadiumName();
        ui->teamcombobox->addItem(teamname);
        ui->TeamCombosouv->addItem(teamname);
        ui->tripBox->addItem(stadium);
        ui->stadiumcombobox->addItem(stadium);
    }

    QStringList playingsurfacearray = {"Grass","AstroTurf GameDay Grass","AstroTurf GameDay Grass 3D"};
    ui->playingsurfacespinbox->addItems(playingsurfacearray);

    QStringList roof = {"Retractable","Open"};
    ui->rooftypespinbox->addItems(roof);

    QStringList typo = {"Retro Modern","Retro Classic","Jewel Box","Modern","Contemporary","Multipurpose"};
    ui->ballparkspinbox->addItems(typo);

    ui->teamlbl->setVisible(false);
    ui->leaguelbl->setVisible(false);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ClearBtn_clicked()
{
    ui->usernamelineedit->clear();
    ui->passwordlineedit->clear();
}


void MainWindow::on_Loginbtn_clicked()
{
    QString username = ui->usernamelineedit->text();
    QString password = ui->passwordlineedit->text();

    QSqlQuery qry;
    // RANK 2 IS USED FOR ADMINS
    // RANK 1 IS USED FOR guest
    qry.prepare("select * from admin where username ='"+username+"' and password ='"+password+"' and rank= '2'");
    if (qry.exec())
    {
        int count = 0;
        while (qry.next())
        {
            count++;
        }
           if(count == 1)
           {
            QMessageBox::information(this, "Login", "Username and password is correct");
            ui->stackedWidget->setCurrentWidget(ui->Admin);
            this->ui->usernamelineedit->setText("");
            this->ui->passwordlineedit->setText("");
           }
           else if (count!= 1)
           {
               if(qry.exec("select * from admin where username ='"+username+"' and password ='"+password+"' and rank= '1'"))
                   count  = 0;
                    while(qry.next())
                    {
                        count++;
                    }
                    if (count == 1)
                    {
                       QMessageBox::information(this,"Login", "Username and Password is correct");
                       ui->stackedWidget->setCurrentWidget(ui->FanOptions);
                       this->ui->usernamelineedit->setText("");
                       this->ui->passwordlineedit->setText("");
                    }
                    else
                    {
                        QMessageBox::warning(this,"Login","Username and password is not correct");
                        this->ui->usernamelineedit->setText("");
                        this->ui->passwordlineedit->setText("");
                    }
           }
    }
}

void MainWindow::keyPressEvent(QKeyEvent* pe)
{
    if(page == 0)
    {
        if(pe->key()== Qt::Key_Return) on_Loginbtn_clicked();
    }
}

void MainWindow::on_gotodisplaybtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Displaytable);
}

void MainWindow::on_teamcombobox_currentTextChanged(const QString &arg1)
{
        ui->tableView->setModel(m_controller->getteamQueryModel(arg1));
        ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_displaystadiumteambtn_clicked()
{
    ui->tableView->setModel(m_controller->getstadiumQueryModelsortedbyteam());
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_disstadiumstadiumbtn_clicked()
{
    ui->tableView->setModel(m_controller->getstadiumQueryModelsortedbystadium());
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_disAmericanTeam_clicked()
{
    ui->tableView->setModel(m_controller->getAmericanLeagueTeams());
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_disNationalbtn_clicked()
{
    ui->tableView->setModel(m_controller->getNationalLeagueTeams());
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_disstadiumtypobtn_clicked()
{
    ui->tableView->setModel(m_controller->getStadiumQueryModelsortedbyTypology());
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_disteamopenroof_clicked()
{
    ui->tableView->setModel(m_controller->getTeamswithopenroofquerymodel());
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_disstaddatebtn_clicked()
{
    ui->tableView->setModel(m_controller->getStadiumQueryModelsortedbydate());
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_disstadiumseats_clicked()
{
    ui->tableView->setModel(m_controller->getStadiumQueryModelsortedbycapacity());
    ui->tableView->resizeColumnsToContents();
}


void MainWindow::on_disstadiumlarcenter_clicked()
{
    ui->tableView->setModel(m_controller->getStadiumQueryModelsortedbydistancetocenterLAR());
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_disstadiumsmalcenter_clicked()
{
    ui->tableView->setModel(m_controller->getStadiumQueryModelsortedbydistancetocentersma());
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_loadTeamInfo_clicked()
{
    QString teamname;
    QString stadium;
    m_controller->readTeamFile();
    ui->teamcombobox->clear();
    ui->TeamCombosouv->clear();
    ui->tripBox->clear();
    for (int i = 0; i < m_controller->listOfTeams.count();i++)
    {
        teamname = m_controller->listOfTeams[i]->getTeamName();
        stadium = m_controller->listOfTeams[i]->getStadiumName();
        ui->teamcombobox->addItem(teamname);
        ui->TeamCombosouv->addItem(teamname);
        ui->tripBox->addItem(stadium);
        ui->stadiumcombobox->addItem(stadium);
    }
}

void MainWindow::on_loadSouvenirInfo_clicked()
{
    m_controller->readSouvenirFile();
}

void MainWindow::on_Backtoadmin_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Admin);
}

void MainWindow::on_loadDatabtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Loadpage);
}

void MainWindow::on_loadstadiumsbtn_clicked()
{
    m_controller->readStadiumsFile();
}

void MainWindow::on_backtopass_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Login);
    page = 0;
}

void MainWindow::on_AddSouv_clicked()
{
    if (ui->LineEditsouv->text() == "") {

        QMessageBox::warning(this, "Error", "Item has no name.");
    }
    else if(ui->spinBoxDollar->value() == 0 &&
            ui->spinBoxCents->value() == 0) {

        QMessageBox::warning(this, "Error", "Item can not cost $0.00.");
    }
    else {

        QString Item;
        QString Price;
        QString team;
        double intPrice;

        team = ui->TeamCombosouv->currentText();
        Item = ui->LineEditsouv->text();
        intPrice = ui->spinBoxDollar->value() + (ui->spinBoxCents->value() / 100.0);
        Price = QString::number(intPrice);

        m_controller->createSouvenir(team,Item,Price);

        ui->LineEditsouv->clear();
        ui->spinBoxDollar->setValue(0);
        ui->spinBoxCents->setValue(0);

        ui->Souveniralltable->setModel(m_controller->getSouvenirQueryModel(ui->TeamCombosouv->currentText()));
        ui->Souveniralltable->resizeColumnsToContents();
    }
}

void MainWindow::on_Souvenirbtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->souvenirspage);
    ui->Souveniralltable->setModel(m_controller->getSouvenirQueryModel(ui->TeamCombosouv->currentText()));
    ui->Souveniralltable->resizeColumnsToContents();
}

void MainWindow::on_BacktoAdmin_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Admin);
}

void MainWindow::on_TeamCombosouv_activated(const QString &arg1)
{
    ui->Souveniralltable->setModel(m_controller->getSouvenirQueryModel(arg1));
    ui->Souveniralltable->resizeColumnsToContents();
}

void MainWindow::on_deleteSouvbtn_clicked()
{
    if (ui->Teamnamelbl->text() == "Team" &&
            ui->itemnamelbl->text() == "Item Name") {

        QMessageBox::warning(this, "Error", "No item selected.");
    }

    else {

        QMessageBox::StandardButton reply =
                QMessageBox::question(this, "Deletion", "Sure to delete?",
                                      QMessageBox::Yes | QMessageBox::No);
        QString name = ui->itemnamelbl->text();
        QString team = ui->Teamnamelbl->text();

        if (reply == QMessageBox::Yes) {

            m_controller->deleteSouvenir(name,team);
            ui->Teamnamelbl->setText("Team");
            ui->itemnamelbl->setText("Item Name");
            ui->spinBoxDollar2->setValue(0);
            ui->spinBoxCents2->setValue(0);

            ui->Souveniralltable->setModel(m_controller->getSouvenirQueryModel(ui->TeamCombosouv->currentText()));
            ui->Souveniralltable->resizeColumnsToContents();
        }
    }
}

void MainWindow::on_Souveniralltable_activated(const QModelIndex &index)
{
    QString item;
    QString selectTeam;

    if(index.isValid())
    {
        QSqlQuery qry;
        item = index.data().toString();
        selectTeam = ui->TeamCombosouv->currentText();
        double price;
        int dollars;
        int cents;
        QString Price;
        QString Team;

        ui->itemnamelbl->setText(item);

        qry.prepare("Select * from Souvenir where Souvenir = '"+item+"' and TeamName = '"+selectTeam+"'");

        if (qry.exec())
        {
            while(qry.next())
            {
                Team = qry.value(0).toString();
                price = qry.value(2).toDouble();
                Price = qry.value(2).toString();

                dollars = int(price);
                cents = int((price - int(price)) * 100);
                ui->spinBoxDollar2->setValue(int(dollars));
                ui->spinBoxCents2->setValue(int(cents));
                ui->Teamnamelbl->setText(Team);
            }
        }
    }
}

void MainWindow::on_Editsouvbtn_clicked()
{
    if (ui->Teamnamelbl->text() == "Team" &&
            ui->itemnamelbl->text() == "Item Name") {

        QMessageBox::warning(this, "Error", "No item selected.");
    }
    else if(ui->spinBoxDollar2->value() == 0 &&
            ui->spinBoxCents2->value() == 0) {

        QMessageBox::warning(this, "Error", "Item can not cost $0.00.");
    }
    else {

        QString Price;
        double doublePrice;

        doublePrice = ui->spinBoxDollar2->value() + (ui->spinBoxCents2->value() / 100.0);
        Price = QString::number(doublePrice);

        m_controller->updateSouvenir(ui->itemnamelbl->text(),ui->Teamnamelbl->text(), Price);

        ui->Teamnamelbl->setText("Team");
        ui->itemnamelbl->setText("Item Name");
        ui->spinBoxDollar2->setValue(0);
        ui->spinBoxCents2->setValue(0);

        ui->Souveniralltable->setModel(m_controller->getSouvenirQueryModel(ui->TeamCombosouv->currentText()));
        ui->Souveniralltable->resizeColumnsToContents();
    }
}

void MainWindow::on_EditMlBbtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->stadiuminfopage);
}

void MainWindow::on_stadiumcombobox_activated(const QString &arg1)
{
    int Capacity;
    QString roof;
    QString playingsurface;
    QString ballparktypo;
    int dateopened;
    int dtc;
    QString location;
    QString team;
    QString league;


    ui->stadiumtableview->setModel(m_controller->getMlbinfoQueryModel(arg1));
    ui->stadiumtableview->resizeColumnsToContents();

    ui->stadiumnameedit->setText(arg1);

    QSqlQuery qry;
    qry.prepare("Select * from MLBINFO where StadiumName = '"+arg1+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            Capacity = qry.value(2).toInt();
            location = qry.value(3).toString();
            playingsurface = qry.value(4).toString();
            dateopened = qry.value(6).toInt();
            dtc = qry.value(7).toInt();
            ballparktypo = qry.value(8).toString();
            roof = qry.value(9).toString();
            team = qry.value(0).toString();
            league = qry.value(5).toString();

            ui->capacityspinbox->setValue(Capacity);
            ui->playingsurfacespinbox->setCurrentText(playingsurface);
            ui->rooftypespinbox->setCurrentText(roof);
            ui->ballparkspinbox->setCurrentText(ballparktypo);
            ui->dateopenededit->setValue(dateopened);
            ui->distancespin->setValue(dtc);
            ui->locationedit->setText(location);
            ui->teamlbl->setText(team);
            ui->leaguelbl->setText(league);

        }
    }

}

void MainWindow::on_Backtoadmin_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Admin);
}

void MainWindow::on_editstadium_clicked()
{
    if(ui->stadiumnameedit->text() == "")
    {
        QMessageBox::warning(this, "Error", "No item selected.");
    }else
    {
    m_controller->updateMlb(ui->teamlbl->text(),ui->stadiumnameedit->text(),ui->capacityspinbox->text(),
                            ui->locationedit->text(), ui->playingsurfacespinbox->currentText(),ui->leaguelbl->text(),
                            ui->dateopenededit->text(),ui->distancespin->text(),ui->ballparkspinbox->currentText(),
                            ui->rooftypespinbox->currentText());
    ui->stadiumtableview->setModel(m_controller->getMlbinfoQueryModel(ui->stadiumnameedit->text()));
    ui->stadiumtableview->resizeColumnsToContents();

    ui->capacityspinbox->setValue(0);
    ui->stadiumnameedit->clear();
    ui->playingsurfacespinbox->setCurrentIndex(0);
    ui->rooftypespinbox->setCurrentIndex(0);
    ui->ballparkspinbox->setCurrentIndex(0);
    ui->dateopenededit->setValue(0);
    ui->distancespin->setValue(0);
    ui->locationedit->clear();
    }
}

void MainWindow::on_DFSbtn_clicked()
{
    outputNodes = m_graph->DFS(getEnum(ui->tripBox->currentText()));

    ui->planningtable->setRowCount(outputNodes.size()+1);
    ui->planningtable->setColumnCount(2);

    for (int i = 0; i < outputNodes.size();i++)
    {
        QTableWidgetItem *dfsNodeNumber = new QTableWidgetItem();
        QTableWidgetItem *dfsNode = new QTableWidgetItem();

        dfsNodeNumber->setText("Node # " + QString:: number(i+1));
        dfsNode->setText(outputNodes[i]);

        ui->planningtable->setItem(i,0,dfsNodeNumber);
        ui->planningtable->setItem(i,1,dfsNode);

    }
    ui->planningtable->resizeColumnsToContents();
    QTableWidgetItem *distancelbl = new QTableWidgetItem();
    QTableWidgetItem *distancenumber = new QTableWidgetItem();

    distancelbl->setText("Distance : ");
    int distance = m_graph->getDistance();
    distancenumber->setText(QString :: number(distance));
    ui->planningtable->setItem(outputNodes.size(),0,distancelbl);
    ui->planningtable->setItem(outputNodes.size(),1,distancenumber);
    outputNodes.clear();
}

void MainWindow::on_BFSbtn_clicked()
{
    outputNodes = m_graph->bfsTraversal(getEnum(ui->tripBox->currentText()));

    for (int i = 0; i < m_controller->listOfTeams.count();i++)
    {

        for (int j = 0; j < outputNodes.count();j++)
        {

            if(m_controller->listOfTeams[i]->getStadiumName()== outputNodes[j])
            {
                teams.append(m_controller->listOfTeams[i]->getTeamName());
            }
        }
    }
    for (int i = 0; i < m_controller->listOfSouvenirs.count();i++)
    {

        for (int j = 0; j < teams.count();j++)
        {
            if(m_controller->listOfSouvenirs[i]->team()== teams[j])
            {
                souvenirs.append(m_controller->listOfSouvenirs[i]);
            }
        }
    }


    ui->planningtable->setRowCount(outputNodes.size()+1);
    ui->planningtable->setColumnCount(2);

    for (int i = 0; i < outputNodes.size();i++)
    {
        QTableWidgetItem *dfsNodeNumber = new QTableWidgetItem();
        QTableWidgetItem *dfsNode = new QTableWidgetItem();

        dfsNodeNumber->setText("Node # " + QString:: number(i+1));
        dfsNode->setText(outputNodes[i]);

        ui->planningtable->setItem(i,0,dfsNodeNumber);
        ui->planningtable->setItem(i,1,dfsNode);

    }
    ui->planningtable->resizeColumnsToContents();
    QTableWidgetItem *distancelbl = new QTableWidgetItem();
    QTableWidgetItem *distancenumber = new QTableWidgetItem();

    distancelbl->setText("Distance : ");
    int distance = m_graph->getDistance();
    distancenumber->setText(QString :: number(distance));
    ui->planningtable->setItem(outputNodes.size(),0,distancelbl);
    ui->planningtable->setItem(outputNodes.size(),1,distancenumber);
    outputNodes.clear();
}

void MainWindow::on_MSTbtn_clicked()
{
    outputNodes = m_graph->primMst();

    ui->planningtable->setRowCount(outputNodes.size()+1);
    ui->planningtable->setColumnCount(2);

    for (int i = 0; i < outputNodes.size();i++)
    {
        QTableWidgetItem *dfsNodeNumber = new QTableWidgetItem();
        QTableWidgetItem *dfsNode = new QTableWidgetItem();

        dfsNodeNumber->setText("Node # " + QString:: number(i+1));
        dfsNode->setText(outputNodes[i]);

        ui->planningtable->setItem(i,0,dfsNodeNumber);
        ui->planningtable->setItem(i,1,dfsNode);

    }
    ui->planningtable->resizeColumnsToContents();
    QTableWidgetItem *distancelbl = new QTableWidgetItem();
    QTableWidgetItem *distancenumber = new QTableWidgetItem();

    distancelbl->setText("Distance : ");
    int distance = m_graph->getDistance();
    distancenumber->setText(QString :: number(distance));
    ui->planningtable->setItem(outputNodes.size(),0,distancelbl);
    ui->planningtable->setItem(outputNodes.size(),1,distancenumber);
    outputNodes.clear();
}

//NEW DANIEL STUFF START

void MainWindow::on_backtologin_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Login);
}

void MainWindow::on_custom_button_clicked()
{
    trip = Custom;
    ui->add_combo->setModel(m_controller->loadStadiumNames());
    ui->stackedWidget->setCurrentWidget(ui->TripSelect);
}


void MainWindow::on_dodgers_button_clicked()
{
    trip = Dodgers;
    selectedStadiums.push_back("Dodger Stadium");
    ui->add_combo->setModel(m_controller->loadRemainingStadiumNamesOnly(selectedStadiums));
    ui->delete_combo->setModel(new QStringListModel(QList<QString>::fromVector(selectedStadiums)));
    ui->selected_listView->setModel(new QStringListModel(QList<QString>::fromVector(selectedStadiums)));
    ui->stackedWidget->setCurrentWidget(ui->TripSelect);
}

void MainWindow::on_add_button_clicked()
{
    QString stadium = ui->add_combo->currentText();
    if(stadium == "")
    {
        QMessageBox::warning(this, "ERROR", "NO AVALIABLE STADIUMS! CLICK DONE!", QMessageBox::Ok, QMessageBox::NoButton);
    }
    else
    {
        selectedStadiums.push_back(stadium);
        ui->add_combo->setModel(m_controller->loadRemainingStadiumNamesOnly(selectedStadiums));
        ui->delete_combo->setModel(new QStringListModel(QList<QString>::fromVector(selectedStadiums)));
        ui->selected_listView->setModel(new QStringListModel(QList<QString>::fromVector(selectedStadiums)));
    }
}

void MainWindow::on_delete_button_clicked()
{
    QString stadium = ui->delete_combo->currentText();
    if(selectedStadiums.size() == 0)
    {
        QMessageBox::warning(this, "ERROR", "NO SELECTED STADIUMS! ADD STADIUMS TO VISIT!", QMessageBox::Ok, QMessageBox::NoButton);
    }
    else if(selectedStadiums.size() == 1)
    {
        QMessageBox::warning(this, "ERROR", "AT LEAST ONE STADIUM MUST BE PRESENT! ADD STADIUMS TO VISIT!", QMessageBox::Ok, QMessageBox::NoButton);
    }
    else if(stadium == "Dodger Stadium" && trip == Dodgers)
    {
        QMessageBox::warning(this, "ERROR", "DODGERS TRIP TYPE! CANNOT DELETE DODGER STADIUM!", QMessageBox::Ok, QMessageBox::NoButton);
    }
    else
    {
        selectedStadiums.removeAll(stadium);
        ui->add_combo->setModel(m_controller->loadRemainingStadiumNamesOnly(selectedStadiums));
        ui->delete_combo->setModel(new QStringListModel(QList<QString>::fromVector(selectedStadiums)));
        ui->selected_listView->setModel(new QStringListModel(QList<QString>::fromVector(selectedStadiums)));
    }
}

//NEW DANIEL STUFF END