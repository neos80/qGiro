/*
 * qGiro
 * Copyright (c) 2015-2019 Oleg Kosorukov
 *
 * qGiro is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software. If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "geo.h"
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QFileDialog>
#include <QtMath>
#include <QCloseEvent>
#include <QStandardPaths>
#include <QProcess>

static QString openFile = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();

    ui->date_ism->setDate(QDate::currentDate());
    //ui->date_pism->setDate(QDate::currentDate());
    ui->tabWidget->setCurrentIndex(0);

    statusBar()->showMessage(tr("Работаем"), 3000);

    //qDebug()<< "Разделитель числа" << QString::number(0.1).at(1);
    QString ds = QString::number(0.1).at(1);


    QString s = "[0-9]{1,6}[" + ds +"]{1,1}[0-9]{1,7}$";
    QRegExp regShirota(s);

    s = "[?-]{0,1}[0-9]{1,6}[?"+ ds + "]{0,1}[0-9]{1,4}";
    QRegExp regXY(s);

    s = "[?-]{0,1}[0-9]{1,3}\\s+[0-9]{1,2}\\s+[0-9]{1,2}[" + ds +"]{1,1}[0-9]{1,2}";
    QRegExp regAz(s);

    ui->EditShirotado->setValidator(   new QRegExpValidator(regShirota,this) );
    ui->EditShirotaposle->setValidator(new QRegExpValidator(regShirota,this) );

    ui->EditXkPunkt1do->setValidator(   new QRegExpValidator(regXY,this) );
    ui->EditYkPunkt1do->setValidator(   new QRegExpValidator(regXY,this) );
    ui->EditXkPunkt2do->setValidator(   new QRegExpValidator(regXY,this) );
    ui->EditYkPunkt2do->setValidator(   new QRegExpValidator(regXY,this) );

    ui->EditXkPunkt1posle->setValidator(new QRegExpValidator(regXY,this) );
    ui->EditYkPunkt1posle->setValidator(new QRegExpValidator(regXY,this) );
    ui->EditXkPunkt2posle->setValidator(new QRegExpValidator(regXY,this) );
    ui->EditYkPunkt2posle->setValidator(new QRegExpValidator(regXY,this) );

    ui->EditAzimut1do->setValidator(    new QRegExpValidator(regAz,this) );
    ui->EditAzimut2do->setValidator(    new QRegExpValidator(regAz,this) );

    ui->EditAzimut1posle->setValidator(new QRegExpValidator(regAz,this) );
    ui->EditAzimut2posle->setValidator(new QRegExpValidator(regAz,this) );

    ui->EditVnecentr1->setValidator(new QRegExpValidator(regAz,this) );
    ui->EditVnecentr2->setValidator(new QRegExpValidator(regAz,this) );
    ui->EditAzimut1giro1pr->setValidator(new QRegExpValidator(regAz,this) );
    ui->EditAzimut1giro2pr->setValidator(new QRegExpValidator(regAz,this) );
    ui->EditAzimut1giro1ob->setValidator(new QRegExpValidator(regAz,this) );
    ui->EditAzimut1giro2ob->setValidator(new QRegExpValidator(regAz,this) );

    ui->EditAmendment1->setStyleSheet("background-color: lightGray");
    ui->EditAmendment2->setStyleSheet("background-color: lightGray");
    ui->EditAmendment3->setStyleSheet("background-color: lightGray");
    ui->EditAmendment4->setStyleSheet("background-color: lightGray");

    ui->EditDirectional1->setStyleSheet("background-color: lightGray");
    ui->EditDirectional2->setStyleSheet("background-color: lightGray");
    ui->EditDirectional3->setStyleSheet("background-color: lightGray");
    ui->EditDirectional4->setStyleSheet("background-color: lightGray");
    ui->EditDirectional5->setStyleSheet("background-color: lightGray");
    ui->EditDirectional6->setStyleSheet("background-color: lightGray");
    ui->EditDirectional7->setStyleSheet("background-color: lightGray");
    ui->EditDirectional8->setStyleSheet("background-color: lightGray");

    ui->EditDirectional1_2->setStyleSheet("background-color: lightGray");
    ui->EditDirectional3_4->setStyleSheet("background-color: lightGray");
    ui->EditDirectional5_6->setStyleSheet("background-color: lightGray");
    ui->EditDirectional7_8->setStyleSheet("background-color: lightGray");

    ui->EditDirectional1_4d->setStyleSheet("background-color: lightGray");
    ui->EditDirectional5_8d->setStyleSheet("background-color: lightGray");

    ui->EditDirectionAlverage->setStyleSheet("background-color: lightGray");
    ui->EditDirectionPolygonometry->setStyleSheet("background-color: lightGray");
    ui->EditDirectionD->setStyleSheet("background-color: lightGray");
    ui->Edit_av->setStyleSheet("background-color: lightGray");
    ui->Edit_M->setStyleSheet("background-color: lightGray");

    qDebug() << "Параметр запуска =" << QApplication::arguments() << QApplication::arguments().count();
    if (QApplication::arguments().count()>1){
        //Если открыт файл программой то
        //nameFile =
        qDebug()<< QApplication::arguments().at(1);
        fileOpen(QApplication::arguments().at(1));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::setCurrentFile(const QString &fileName)
//{
//    curFile = fileName;
//    textEdit->document()->setModified(false);
//    setWindowModified(false);

//    QString shownName = curFile;
//    if (curFile.isEmpty())
//        shownName = "untitled.txt";
//    setWindowFilePath(shownName);
//}

void MainWindow::on_EditNameGiro1_returnPressed()
{
    ui->EditNameGiro2->setFocus();
}

void MainWindow::on_EditNameGiro2_returnPressed()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->EditShirotado->setFocus();
}

void MainWindow::on_EditShirotado_returnPressed()
{
    double s = ui->EditShirotado->text().toDouble();
    ui->EditShirotado->setText( QString::number( s, 'f', 6 ));
    ui->EditNamePunkt1do->setFocus();
}

void MainWindow::on_EditNamePunkt1do_returnPressed()
{
    ui->EditXkPunkt1do->setFocus();
}

void MainWindow::on_EditXkPunkt1do_returnPressed()
{
    double s = ui->EditXkPunkt1do->text().toDouble();
    ui->EditXkPunkt1do->setText( QString::number( s, 'f', 4 ));
    ui->EditYkPunkt1do->setFocus();
}

void MainWindow::on_EditYkPunkt1do_returnPressed()
{
    double s = ui->EditYkPunkt1do->text().toDouble();
    ui->EditYkPunkt1do->setText( QString::number( s, 'f', 4 ));
    ui->EditNamePunkt2do->setFocus();
}

void MainWindow::on_EditNamePunkt2do_returnPressed()
{
    ui->EditXkPunkt2do->setFocus();
}

void MainWindow::on_EditXkPunkt2do_returnPressed()
{
    double s = ui->EditXkPunkt2do->text().toDouble();
    ui->EditXkPunkt2do->setText( QString::number( s, 'f', 4 ));
    ui->EditYkPunkt2do->setFocus();
}

void MainWindow::on_EditYkPunkt2do_returnPressed()
{
    double s = ui->EditYkPunkt2do->text().toDouble();
    ui->EditYkPunkt2do->setText( QString::number( s, 'f', 4 ));
    ui->EditAzimut1do->setFocus();
}

void MainWindow::on_EditAzimut1do_returnPressed()
{
    ui->EditAzimut2do->setFocus();
}

void MainWindow::on_EditAzimut2do_returnPressed()
{
    ui->EditShirotaposle->setFocus();
}

void MainWindow::on_EditShirotaposle_returnPressed()
{
    double s = ui->EditShirotaposle->text().toDouble();
    ui->EditShirotaposle->setText( QString::number( s, 'f', 6 ));
    ui->EditNamePunkt1posle->setFocus();
}

void MainWindow::on_EditNamePunkt1posle_returnPressed()
{
    ui->EditXkPunkt1posle->setFocus();
}

void MainWindow::on_EditXkPunkt1posle_returnPressed()
{
    double s = ui->EditXkPunkt1posle->text().toDouble();
    ui->EditXkPunkt1posle->setText( QString::number( s, 'f', 4 ));
    ui->EditYkPunkt1posle->setFocus();
}

void MainWindow::on_EditYkPunkt1posle_returnPressed()
{
    double s = ui->EditYkPunkt1posle->text().toDouble();
    ui->EditYkPunkt1posle->setText( QString::number( s, 'f', 4 ));
    ui->EditNamePunkt2posle->setFocus();
}

void MainWindow::on_EditNamePunkt2posle_returnPressed()
{
    ui->EditXkPunkt2posle->setFocus();
}

void MainWindow::on_EditXkPunkt2posle_returnPressed()
{
    double s = ui->EditXkPunkt2posle->text().toDouble();
    ui->EditXkPunkt2posle->setText( QString::number( s, 'f', 4 ));
    ui->EditYkPunkt2posle->setFocus();
}

void MainWindow::on_EditYkPunkt2posle_returnPressed()
{
    double s = ui->EditYkPunkt2posle->text().toDouble();
    ui->EditYkPunkt2posle->setText( QString::number( s, 'f', 4 ));
    ui->EditAzimut1posle->setFocus();
}

void MainWindow::on_EditAzimut1posle_returnPressed()
{
    ui->EditAzimut2posle->setFocus();
}

void MainWindow::on_EditAzimut2posle_returnPressed()
{
    ui->tabWidget->setCurrentIndex(1);
    ui->EditPNamePunkt1->setFocus();
}

void MainWindow::on_EditXkPunkt1Podz_returnPressed()
{
    double s = ui->EditXkPunkt1Podz->text().toDouble();
    ui->EditXkPunkt1Podz->setText( QString::number( s, 'f', 4 ));
    ui->EditYkPunkt1Podz->setFocus();
}

void MainWindow::on_EditYkPunkt1Podz_returnPressed()
{
    double s = ui->EditYkPunkt1Podz->text().toDouble();
    ui->EditYkPunkt1Podz->setText( QString::number( s, 'f', 4 ));
    ui->EditPNamePunkt2->setFocus();
}

void MainWindow::on_EditPNamePunkt2_returnPressed()
{
    ui->EditXkPunkt2Podz->setFocus();
}

void MainWindow::on_EditPNamePunkt1_returnPressed()
{
    ui->EditXkPunkt1Podz->setFocus();
}

void MainWindow::on_EditXkPunkt2Podz_returnPressed()
{
    double s = ui->EditXkPunkt2Podz->text().toDouble();
    ui->EditXkPunkt2Podz->setText( QString::number( s, 'f', 4 ));
    ui->EditYkPunkt2Podz->setFocus();
}

void MainWindow::on_EditYkPunkt2Podz_returnPressed()
{
    double s = ui->EditYkPunkt2Podz->text().toDouble();
    ui->EditYkPunkt2Podz->setText( QString::number( s, 'f', 4 ));
    ui->EditVnecentr1->setFocus();

}

void MainWindow::on_EditVnecentr1_returnPressed()
{
    ui->EditAzimut1giro1pr->setFocus();
}


void MainWindow::on_EditVnecentr2_returnPressed()
{
    ui->EditAzimut1giro1ob->setFocus();
}

void MainWindow::on_EditTypeGiro_returnPressed()
{
    ui->EditNameGiro1->setFocus();
}

void MainWindow::fileOpen(QString fileName){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }   else {
        QMessageBox msgBox;
        msgBox.setText("Открытие файла...");
        msgBox.setInformativeText("При открытии файла все данные в программе будут заменены новыми, \n продолжить?");
        msgBox.setIcon(QMessageBox::Warning);
        QPushButton *yes = msgBox.addButton(tr("Да"), QMessageBox::ActionRole);
        msgBox.addButton(QObject::tr("Отмена"), QMessageBox::ActionRole);
        msgBox.setDefaultButton(yes);
        msgBox.exec();
        if(msgBox.clickedButton() != yes)
        {
            statusBar()->showMessage(tr("Открытие файла отменено..."), 3000);
            return;
        }

        QTextStream readStream(&file);
        readStream.setCodec("UTF-8");


        if (!readStream.atEnd()){
            ui->date_ism->setDate(QDate::fromString(readStream.readLine(),"dd.MM.yyyy")) ;
            ui->EditTypeGiro->setText(readStream.readLine()) ;
            ui->EditNameGiro1->setText(readStream.readLine()) ;
            ui->EditNameGiro2->setText(readStream.readLine()) ;

            ui->textEdit_kolontitul->setText(readStream.readLine()) ;
            ui->textEdit_Vved->setText(readStream.readLine()) ;

            ui->EditShirotado->setText(readStream.readLine()) ;
            ui->EditNamePunkt1do->setText(readStream.readLine()) ;
            ui->EditXkPunkt1do->setText(readStream.readLine()) ;
            ui->EditYkPunkt1do->setText(readStream.readLine()) ;
            ui->EditNamePunkt2do->setText(readStream.readLine()) ;
            ui->EditXkPunkt2do->setText(readStream.readLine()) ;
            ui->EditYkPunkt2do->setText(readStream.readLine()) ;
            ui->EditAzimut1do->setText(readStream.readLine()) ;
            ui->EditAzimut2do->setText(readStream.readLine()) ;

            ui->EditShirotaposle->setText(readStream.readLine()) ;
            ui->EditNamePunkt1posle->setText(readStream.readLine()) ;
            ui->EditNamePunkt2posle->setText(readStream.readLine()) ;
            ui->EditXkPunkt1posle->setText(readStream.readLine()) ;
            ui->EditYkPunkt1posle->setText(readStream.readLine()) ;
            ui->EditXkPunkt2posle->setText(readStream.readLine()) ;
            ui->EditYkPunkt2posle->setText(readStream.readLine()) ;
            ui->EditAzimut1posle->setText(readStream.readLine()) ;
            ui->EditAzimut2posle->setText(readStream.readLine()) ;

            ui->EditPNamePunkt1->setText(readStream.readLine()) ;
            ui->EditXkPunkt1Podz->setText(readStream.readLine()) ;
            ui->EditYkPunkt1Podz->setText(readStream.readLine()) ;

            ui->EditPNamePunkt2->setText(readStream.readLine()) ;
            ui->EditXkPunkt2Podz->setText(readStream.readLine()) ;
            ui->EditYkPunkt2Podz->setText(readStream.readLine()) ;


            ui->EditVnecentr1->setText(readStream.readLine()) ;
            ui->EditAzimut1giro1pr->setText(readStream.readLine()) ;

            ui->EditAzimut1giro2pr->setText(readStream.readLine()) ;

            ui->EditVnecentr2->setText(readStream.readLine()) ;
            ui->EditAzimut1giro1ob->setText(readStream.readLine()) ;

            ui->EditAzimut1giro2ob->setText(readStream.readLine()) ;

            // добавил в 0.2
            ui->EditPPKPunkt1->setText(readStream.readLine()) ;
            ui->EditPKolcoPunkt1->setText(readStream.readLine()) ;
            ui->EditPPKPunkt2->setText(readStream.readLine()) ;
            ui->EditPKolcoPunkt2->setText(readStream.readLine()) ;
            ui->EditPismo->setText(readStream.readLine()) ;
            ui->textEdit_Tema->setText(readStream.readLine()) ;

        }

        file.close();
    }
    openFile = fileName;
    on_EditNamePunkt1do_editingFinished();
    statusBar()->showMessage(tr("файл открыт..."), 3000);

}

void MainWindow::on_actionOpen_triggered()
{
    //    QIODevice::ReadOnly    Открыть файл для чтения
    //    QIODevice::WriteOnly   Открыть файл для записи (таким методом можно просто создать файл)
    //    QIODevice::ReadWrite   Открыть файл для чтения и записи
    //    QIODevice::Append      Открыть файл для дополнения файла в конец

    //    QString tempstr;
    //    QFile file( QCoreApplication::applicationDirPath() +"/demo.mgiro");

    //        while(!readStream.atEnd())
    //        {
    //            tempstr = readStream.readLine();
    //            qDebug() << tempstr;
    //        }

    //    int r = QMessageBox::warning(this,"Открытие файла" , "При открытии файла все данные в программе будут заменены новыми, \n продолжить?",QMessageBox::Yes|QMessageBox::No);
    //    if (r==QMessageBox::No) {
    //        statusBar()->showMessage(tr("Открытие файла отменено..."), 3000);
    //        return;
    //    }

    QString nameFile;
    nameFile = QFileDialog::getOpenFileName(this,tr("Выберите файл проекта"),QStandardPaths::DocumentsLocation +"/demo.mgiro",tr("Проект Giro (*.mgiro)") );
    if(nameFile.isEmpty())
    {return;}
    fileOpen(nameFile);
    qDebug() << nameFile;

}



void MainWindow::on_actionNew_triggered()
{
    int r = QMessageBox::warning(this,"Новый документ" , "Данные будут удалены, \n продолжить?",QMessageBox::Yes|QMessageBox::No);
    if (r==QMessageBox::No) {
        statusBar()->showMessage(tr("Создание нового документа отменено..."), 3000);
        //setCurrentFile("");
        return;
    }
    QSettings settings("itg2002", "QtGiro");
    settings.beginGroup("config");
    ui->EditTypeGiro->setText(settings.value("TypeGiro").toString());
    ui->EditNameGiro1->setText(settings.value("NameGiro1").toString());
    ui->EditNameGiro2->setText(settings.value("NameGiro2").toString());
    settings.endGroup();

    ui->date_ism->setDate(QDate::currentDate()); //дата выполнения измерений

    ui->EditShirotado->setText("");
    ui->EditNamePunkt1do->setText("");
    ui->EditNamePunkt2do->setText("");
    ui->EditXkPunkt1do->setText("");
    ui->EditYkPunkt1do->setText("");
    ui->EditXkPunkt2do->setText("");
    ui->EditYkPunkt2do->setText("");
    ui->EditAzimut1do->setText("");
    ui->EditAzimut2do->setText("");
    ui->EditAmendment1->setText("");
    ui->EditAmendment2->setText("");

    ui->EditShirotaposle->setText("");
    ui->EditNamePunkt1posle->setText("");
    ui->EditNamePunkt2posle->setText("");
    ui->EditXkPunkt1posle->setText("");
    ui->EditYkPunkt1posle->setText("");
    ui->EditXkPunkt2posle->setText("");
    ui->EditYkPunkt2posle->setText("");
    ui->EditAzimut1posle->setText("");
    ui->EditAzimut2posle->setText("");
    ui->EditAmendment3->setText("");
    ui->EditAmendment4->setText("");

    ui->EditPNamePunkt1->setText("");
    ui->EditXkPunkt1Podz->setText("");
    ui->EditYkPunkt1Podz->setText("");
    ui->EditPNamePunkt2->setText("");
    ui->EditXkPunkt2Podz->setText("");
    ui->EditYkPunkt2Podz->setText("");

    ui->EditVnecentr1->setText("");
    ui->EditAzimut1giro1pr->setText("");
    ui->EditAzimut1giro2pr->setText("");


    ui->EditVnecentr2->setText("");
    ui->EditAzimut1giro1ob->setText("");
    ui->EditAzimut1giro2ob->setText("");

    ui->EditDirectional1->setText("");
    ui->EditDirectional2->setText("");
    ui->EditDirectional3->setText("");
    ui->EditDirectional4->setText("");
    ui->EditDirectional5->setText("");
    ui->EditDirectional6->setText("");
    ui->EditDirectional7->setText("");
    ui->EditDirectional8->setText("");

    ui->EditDirectional1_2->setText("");
    ui->EditDirectional3_4->setText("");
    ui->EditDirectional5_6->setText("");
    ui->EditDirectional7_8->setText("");

    ui->EditDirectional1_4d->setText("");
    ui->EditDirectional5_8d->setText("");

    ui->EditDirectionAlverage->setText("");
    ui->EditDirectionPolygonometry->setText("");
    ui->EditDirectionD->setText("");

    // добавил в 0.2
    ui->EditPPKPunkt1->setText("") ;
    ui->EditPKolcoPunkt1->setText("") ;
    ui->EditPPKPunkt2->setText("") ;
    ui->EditPKolcoPunkt2->setText("") ;
    ui->EditPismo->setText("") ;
    ui->textEdit_Tema->setText("") ;
    ui->textEdit->clear();
    ui->textEdit->append("Лог программы:");

    on_EditNamePunkt1do_editingFinished();
    statusBar()->showMessage(tr("Новый файл..."), 3000);
}



void MainWindow::on_actionExampl_triggered()
{
    QString nameFile = "demo.mgiro";
    //nameFile = QFileDialog::getOpenFileName(this,tr("Выберите файл проекта"),QStandardPaths::DocumentsLocation +"/demo.mgiro",tr("Проект Giro (*.mgiro)") );
    if(nameFile.isEmpty())
    {return;}
    fileOpen(nameFile);

    on_EditNamePunkt1do_editingFinished();
    statusBar()->showMessage(tr("Пример загружен..."), 3000);
}

void MainWindow::on_actionExit_triggered()
{
    qDebug()<<"Программа завершена, настройки сохранены.";
    writeSettings();
    close();
}

void MainWindow::on_actionAboutPrg_triggered()
{
    QMessageBox::about(this,"О программе", "qGiro METRO 0.3 \n"
                       "Вы можете поддержать автора морально и материально, передав любую сумму денег "
                       "можно через Яндекс-Деньги перечислив на счет 41001551127461\n"
                       "Проект выложен на гитхабе под лицензие GPLv3 \n"
                       "www.github.com/neos80/qGiro/ \n"
                       "\n"
                       "Отдельно хочу поблагодарить за поддержку в процессе написания и тестирования:  \n"
                       "Савченко А.В.     (ДСМ)\n"
                       "Юкин Б.Ю.         (Инжтоннельгеодезия)\n"
                       "Чесноков Ю.М.     (Инжтоннельгеодезия)\n"
                       "\n"
                       "Бывший сотрудник ООО Инжтоннельгеодезия:\n"
                       "Нач.Тех. Отдела Косоруков О.М. \n"
                       " ©2015-2019 г. ");
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this,"aboutQt");
}

//Load ConfigFile
void MainWindow::readSettings()
{
    QSettings settings("itg2002", "QtGiro");
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value  ("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
    settings.beginGroup("config");
    ui->EditTypeGiro->setText(settings.value("TypeGiro").toString());
    ui->EditNameGiro1->setText(settings.value("NameGiro1").toString());
    ui->EditNameGiro2->setText(settings.value("NameGiro2").toString());
    settings.endGroup();
}

//Save ConfigFile
void MainWindow::writeSettings() {
    QSettings settings("itg2002", "QtGiro");
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
    //    out << ui->EditTypeGiro->text() << "\n";
    //    out << ui->EditNameGiro1->text() << "\n";
    //    out << ui->EditNameGiro2->text() << "\n";
    settings.beginGroup("config");
    settings.setValue("TypeGiro", ui->EditTypeGiro->text());
    settings.setValue("NameGiro1", ui->EditNameGiro1->text());
    settings.setValue("NameGiro2", ui->EditNameGiro2->text());
    settings.endGroup();
}


void MainWindow::on_EditNamePunkt1do_editingFinished()
{
    //Расчет
    ui->label_12->setText(ui->EditNamePunkt1do->text()+"-"+ui->EditNamePunkt2do->text() );
    ui->label_63->setText(ui->EditNamePunkt1do->text()+"-"+ui->EditNamePunkt2do->text() );
    ui->label_13->setText(ui->EditNamePunkt2do->text()+"-"+ui->EditNamePunkt1do->text() );
    ui->label_62->setText(ui->EditNamePunkt2do->text()+"-"+ui->EditNamePunkt1do->text() );

    ui->label_16->setText(ui->EditPNamePunkt1->text()+"-"+ui->EditPNamePunkt2->text() );
    ui->label_61->setText(ui->EditPNamePunkt1->text()+"-"+ui->EditPNamePunkt2->text() );
    ui->label_18->setText(ui->EditPNamePunkt2->text()+"-"+ui->EditPNamePunkt1->text() );
    ui->label_27->setText(ui->EditPNamePunkt2->text()+"-"+ui->EditPNamePunkt1->text() );

    //подземные
    ui->label_48->setText("Измеренный азимут (прямо): "   + ui->EditPNamePunkt1->text() + " - " + ui->EditPNamePunkt2->text());
    ui->label_47->setText("Измеренный азимут (обратно): " + ui->EditPNamePunkt2->text() + " - " + ui->EditPNamePunkt1->text());
    ui->label_48->setText("Измеренный азимут (прямо): "   + ui->EditPNamePunkt1->text() + " - " + ui->EditPNamePunkt2->text());
    ui->label_47->setText("Измеренный азимут (обратно): " + ui->EditPNamePunkt2->text() + " - " + ui->EditPNamePunkt1->text());

}


void MainWindow::on_EditNamePunkt1posle_editingFinished()
{
    on_EditNamePunkt1do_editingFinished();
}

void MainWindow::on_EditPNamePunkt1_editingFinished()
{
    on_EditNamePunkt1do_editingFinished();
}

void MainWindow::on_EditPNamePunkt2_editingFinished()
{
    on_EditNamePunkt1do_editingFinished();
}

void MainWindow::on_pushButtonRaschet_clicked()
{
    geo zadacha;
    double Rm = 6370000;     //Радиус земли
    double ro = 57.29583333; //206265

    double dirPya       = zadacha.OgzDir(ui->EditXkPunkt1Podz->text().toDouble(),
                                         ui->EditYkPunkt1Podz->text().toDouble(),
                                         ui->EditXkPunkt2Podz->text().toDouble(),
                                         ui->EditYkPunkt2Podz->text().toDouble());

    double dirBaseDo    = zadacha.OgzDir(ui->EditXkPunkt1do->text().toDouble(),
                                         ui->EditYkPunkt1do->text().toDouble(),
                                         ui->EditXkPunkt2do->text().toDouble(),
                                         ui->EditYkPunkt2do->text().toDouble());

    double dirBasePosle = zadacha.OgzDir(ui->EditXkPunkt1posle->text().toDouble(),
                                         ui->EditYkPunkt1posle->text().toDouble(),
                                         ui->EditXkPunkt2posle->text().toDouble(),
                                         ui->EditYkPunkt2posle->text().toDouble());

    ui->EditDirectionPolygonometry->setText(zadacha.DegToGradstring(dirPya));

    double Vnecentr1 =  zadacha.stringGradToDouble(ui->EditVnecentr1->text());
    qDebug() << ui->EditVnecentr1->text() << Vnecentr1;
    double Vnecentr2 =  zadacha.stringGradToDouble(ui->EditVnecentr2->text());
    qDebug() << ui->EditVnecentr2->text() << Vnecentr2;

    //Поправка
    double Amendment1 = dirBaseDo - zadacha.stringGradToDouble(ui->EditAzimut1do->text());
    if (Amendment1<-180)Amendment1 = 360+Amendment1;
    double Amendment2 = dirBaseDo - zadacha.stringGradToDouble(ui->EditAzimut2do->text());
    if (Amendment2<-180)Amendment2 = 360+Amendment2;
    double Amendment3 = dirBasePosle - zadacha.stringGradToDouble(ui->EditAzimut1posle->text());
    if (Amendment3<-180)Amendment3 = 360+Amendment3;
    double Amendment4 = dirBasePosle - zadacha.stringGradToDouble(ui->EditAzimut2posle->text());
    if (Amendment4<-180)Amendment4 = 360+Amendment4;
    ui->EditAmendment1->setText(zadacha.DegToGradstring(Amendment1));
    ui->EditAmendment2->setText(zadacha.DegToGradstring(Amendment2));
    ui->EditAmendment3->setText(zadacha.DegToGradstring(Amendment3));
    ui->EditAmendment4->setText(zadacha.DegToGradstring(Amendment4));

    qDebug()<< "----------------------------Первый гироблок-----------------------------------";

    //Первый гироблок ПЗ26444-ПЗ26339 P328-P685
    double Azimut1giro1pr = zadacha.stringGradToDouble(ui->EditAzimut1giro1pr->text());
    //Разность ординат P328-ПЗ26444
    double rOrd1 = ui->EditYkPunkt1do->text().toDouble()-ui->EditYkPunkt1Podz->text().toDouble();
    qDebug()<<"Разность ординат P328-ПЗ26444="<<rOrd1;
    //Разность ординат P684-ПЗ26444
    double mer1 = ui->EditShirotado->text().toDouble();
    double popMer1 = (ro/Rm)*rOrd1*qTan(qDegreesToRadians(mer1));
    qDebug()<<"Азимут гиро   "<< zadacha.DegToGradstring(Azimut1giro1pr);
    qDebug()<<"Поправка      "<< zadacha.DegToGradstring(Amendment1);
    qDebug()<<"внецентренное "<< zadacha.DegToGradstring(Vnecentr1);
    qDebug()<<"Поправка за сближение меридианов P328-ПЗ26444="<<zadacha.DegToGradstring(popMer1)<<" Тангенс меридиана= "<<qTan(qDegreesToRadians(mer1));
    double aDir1 = Azimut1giro1pr + Vnecentr1+Amendment1+popMer1;
    if (aDir1>360)aDir1-=360;
    ui->EditDirectional1->setText(zadacha.DegToGradstring(aDir1));
    qDebug()<<"Дирекционный угол P328-ПЗ26444="<<zadacha.DegToGradstring(aDir1) << aDir1;

    qDebug()<< "------------------------------------------------------------------------------";

    //Первый гироблок ПЗ26444-ПЗ26339 P684-P480
    //Разность ординат P684-ПЗ26444
    double rOrd2 = ui->EditYkPunkt1posle->text().toDouble()-ui->EditYkPunkt1Podz->text().toDouble();
    qDebug()<<"Разность ординат P684-ПЗ26444="<<rOrd2;

    double mer2 = ui->EditShirotaposle->text().toDouble();
    double popMer2 = (ro/Rm)*rOrd2*qTan(qDegreesToRadians(mer2));
    qDebug()<<"Азимут гиро   "<< zadacha.DegToGradstring(Azimut1giro1pr);
    qDebug()<<"внецентренное "<< zadacha.DegToGradstring(Vnecentr1);
    qDebug()<<"Поправка      "<< zadacha.DegToGradstring(Amendment3);
    qDebug()<<"Поправка за сближение меридианов P684-ПЗ26444="<<zadacha.DegToGradstring(popMer2)<<" Тангенс меридиана= "<<qTan(qDegreesToRadians(mer2));
    double aDir2 = Azimut1giro1pr + Vnecentr1 +Amendment3 + popMer2;
    if (aDir2>360)aDir2-=360;
    ui->EditDirectional2->setText(zadacha.DegToGradstring(aDir2));
    qDebug()<<"Дирекционный угол P684-ПЗ26444="<<zadacha.DegToGradstring(aDir2) << aDir2;

    qDebug()<< "------------------------------------------------------------------------------";

    //Первый гироблок ПЗ26339-ПЗ26444 P328-P685
    double Azimut1giro1ob = zadacha.stringGradToDouble(ui->EditAzimut1giro1ob->text());
    //Разность ординат P328-ПЗ26339
    double rOrd3 = ui->EditYkPunkt1do->text().toDouble()-ui->EditYkPunkt2Podz->text().toDouble();
    qDebug()<<"Разность ординат P328-ПЗ26339="<<rOrd3;
    //Разность ординат P684-ПЗ26339

    double popMer3 = (ro/Rm)*rOrd3*qTan(qDegreesToRadians(mer1));
    qDebug()<<"Азимут гиро   "<< zadacha.DegToGradstring(Azimut1giro1ob);
    qDebug()<<"внецентренное "<< zadacha.DegToGradstring(Vnecentr2);
    qDebug()<<"Поправка      "<< zadacha.DegToGradstring(Amendment1);
    qDebug()<<"Поправка за сближение меридианов P328-ПЗ26339="<<zadacha.DegToGradstring(popMer3)<<" Тангенс меридиана= "<<qTan(qDegreesToRadians(mer1));
    double aDir3 = Azimut1giro1ob + Vnecentr2+Amendment1+popMer3;
    if (aDir3>360)aDir3-=360;
    ui->EditDirectional3->setText(zadacha.DegToGradstring(aDir3));
    qDebug()<<"Дирекционный угол P328-ПЗ26339="<<zadacha.DegToGradstring(aDir3) << aDir3;

    qDebug()<< "------------------------------------------------------------------------------";

    //Первый гироблок ПЗ26339-ПЗ26444 P684-P480
    //Разность ординат P684-ПЗ26339
    double rOrd4 = ui->EditYkPunkt1posle->text().toDouble() - ui->EditYkPunkt2Podz->text().toDouble();
    double popMer4 = (ro/Rm)*rOrd4*qTan(qDegreesToRadians(mer2));

    qDebug()<<"Разность ординат P684-ПЗ26339="<<rOrd4<<ui->EditYkPunkt1posle->text()<<ui->EditYkPunkt2Podz->text();
    qDebug()<<"Азимут гиро   "<< zadacha.DegToGradstring(Azimut1giro1ob);
    qDebug()<<"внецентренное "<< zadacha.DegToGradstring(Vnecentr2);
    qDebug()<<"Поправка      "<< zadacha.DegToGradstring(Amendment3);
    qDebug()<<"Поправка за сближение меридианов P684-ПЗ26339="<<zadacha.DegToGradstring(popMer4)<<" Тангенс меридиана= "<<qTan(qDegreesToRadians(mer2));
    double aDir4 = Azimut1giro1ob + Vnecentr2+Amendment3+popMer4;
    if (aDir4>360)aDir4-=360;
    ui->EditDirectional4->setText(zadacha.DegToGradstring(aDir4));
    qDebug()<<"Дирекционный угол P684-ПЗ26339="<<zadacha.DegToGradstring(aDir4) << aDir4;

    qDebug()<< "--------------------------Второй гироблок-------------------------------------";

    //Второй гироблок ПЗ26444-ПЗ26339 P328-P685
    double Azimut1giro2pr = zadacha.stringGradToDouble(ui->EditAzimut1giro2pr->text());
    qDebug()<<"Разность ординат P328-ПЗ26444="<<rOrd1;
    qDebug()<<"Азимут гиро   "<< zadacha.DegToGradstring(Azimut1giro2pr);
    qDebug()<<"Поправка      "<< zadacha.DegToGradstring(Amendment2);
    qDebug()<<"внецентренное "<< zadacha.DegToGradstring(Vnecentr1);
    qDebug()<<"Поправка за сближение меридианов P328-ПЗ26444="<<zadacha.DegToGradstring(popMer1)<<" Тангенс меридиана= "<<qTan(qDegreesToRadians(mer1));
    double aDir5 = Azimut1giro2pr + Vnecentr1 + Amendment2 + popMer1;
    if (aDir5>360)aDir5-=360;
    ui->EditDirectional5->setText(zadacha.DegToGradstring(aDir5));
    qDebug()<<"Дирекционный угол P328-ПЗ26444="<<zadacha.DegToGradstring(aDir5) << aDir5;
    qDebug()<< "------------------------------------------------------------------------------";

    //Первый гироблок ПЗ26444-ПЗ26339 P684-P480
    //Разность ординат P684-ПЗ26444
    qDebug()<<"Разность ординат P684-ПЗ26444="<<rOrd2;
    qDebug()<<"Азимут гиро   "<< zadacha.DegToGradstring(Azimut1giro2pr);
    qDebug()<<"внецентренное "<< zadacha.DegToGradstring(Vnecentr1);
    qDebug()<<"Поправка      "<< zadacha.DegToGradstring(Amendment4);
    qDebug()<<"Поправка за сближение меридианов P684-ПЗ26444="<<zadacha.DegToGradstring(popMer2)<<" Тангенс меридиана= "<<qTan(qDegreesToRadians(mer2));
    double aDir6 = Azimut1giro2pr + Vnecentr1 +Amendment4 + popMer2;
    if (aDir6>360)aDir6-=360;
    ui->EditDirectional6->setText(zadacha.DegToGradstring(aDir6));
    qDebug()<<"Дирекционный угол P684-ПЗ26444="<<zadacha.DegToGradstring(aDir6) << aDir6;

    qDebug()<< "------------------------------------------------------------------------------";

    double Azimut1giro2ob = zadacha.stringGradToDouble(ui->EditAzimut1giro2ob->text());

    qDebug()<<"Азимут гиро   "<< zadacha.DegToGradstring(Azimut1giro2ob);
    qDebug()<<"внецентренное "<< zadacha.DegToGradstring(Vnecentr2);
    qDebug()<<"Поправка      "<< zadacha.DegToGradstring(Amendment2);
    qDebug()<<"Поправка за сближение меридианов P328-ПЗ26339="<<zadacha.DegToGradstring(popMer3)<<" Тангенс меридиана= "<<qTan(qDegreesToRadians(mer1));
    double aDir7 = Azimut1giro2ob + Vnecentr2+Amendment2+popMer3;
    if (aDir7>360)aDir7-=360;
    ui->EditDirectional7->setText(zadacha.DegToGradstring(aDir7));
    qDebug()<<"Дирекционный угол P328-ПЗ26339="<<zadacha.DegToGradstring(aDir7) << aDir7;

    qDebug()<< "------------------------------------------------------------------------------";

    qDebug()<<"Разность ординат P684-ПЗ26339="<<rOrd4<<ui->EditYkPunkt1posle->text()<<ui->EditYkPunkt2Podz->text();
    qDebug()<<"Азимут гиро   "<< zadacha.DegToGradstring(Azimut1giro2ob);
    qDebug()<<"внецентренное "<< zadacha.DegToGradstring(Vnecentr2);
    qDebug()<<"Поправка      "<< zadacha.DegToGradstring(Amendment4);
    qDebug()<<"Поправка за сближение меридианов P684-ПЗ26339="<<zadacha.DegToGradstring(popMer4)<<" Тангенс меридиана= "<<qTan(qDegreesToRadians(mer2));
    double aDir8 = Azimut1giro2ob + Vnecentr2+Amendment4+popMer4;
    if (aDir8>360)aDir8-=360;
    ui->EditDirectional8->setText(zadacha.DegToGradstring(aDir8));
    qDebug()<<"Дирекционный угол P684-ПЗ26339="<<zadacha.DegToGradstring(aDir8) << aDir8;


    //средние углы
    double aDir1_2 = (aDir1+aDir2)/2;
    ui->EditDirectional1_2->setText(zadacha.DegToGradstring(aDir1_2));
    double aDir3_4 = (aDir3+aDir4)/2;
    ui->EditDirectional3_4->setText(zadacha.DegToGradstring(aDir3_4));
    double aDir5_6 = (aDir5+aDir6)/2;
    ui->EditDirectional5_6->setText(zadacha.DegToGradstring(aDir5_6));
    double aDir7_8 = (aDir7+aDir8)/2;
    ui->EditDirectional7_8->setText(zadacha.DegToGradstring(aDir7_8));

    if (aDir1_2 <180){
        if (aDir3_4>180)aDir3_4-=180;
        if (aDir5_6>180)aDir5_6-=180;
        if (aDir7_8>180)aDir7_8-=180;
    } else {
        if (aDir3_4<180)aDir3_4+=180;
        if (aDir5_6<180)aDir5_6+=180;
        if (aDir7_8<180)aDir7_8+=180;
    }

    ui->EditDirectional1_4d->setText(QString::number(((aDir1_2-aDir3_4)*3600), 'f', 1)+"\"");
    ui->EditDirectional5_8d->setText(QString::number(((aDir5_6-aDir7_8)*3600), 'f', 1)+"\"");
    double aDirAlverage = (aDir1_2+aDir3_4+aDir5_6+aDir7_8)/4;
    ui->EditDirectionAlverage->setText(zadacha.DegToGradstring(aDirAlverage));

    double aDird = (dirPya-aDirAlverage)*3600;
    ui->EditDirectionD->setText(QString::number(aDird, 'f', 1)+"\"");


    //Логи:

    //предварительные расчеты
    ui->textEdit->append("Проверка дирекционных углов:");
    ui->textEdit->append("Дирекционный угол " + ui->EditNamePunkt1do->text()    + " " + ui->EditNamePunkt2do->text()    + "= "  + QString::number(dirBaseDo,    'f', 8 ) + ", в градусной мере, обычной "+zadacha.DegToGradstring(dirBaseDo));
    ui->textEdit->append("Дирекционный угол " + ui->EditNamePunkt1posle->text() + " " + ui->EditNamePunkt2posle->text() + "= "  + QString::number(dirBasePosle, 'f', 8 ) + ", в градусной мере, обычной "+zadacha.DegToGradstring(dirBasePosle));
    ui->textEdit->append("Дирекционный угол " + ui->EditPNamePunkt1->text()     + " " + ui->EditPNamePunkt2->text()     + "= "  + QString::number(dirPya, 'f', 8 )       + ", в градусной мере, обычной "+zadacha.DegToGradstring(dirPya));

    ui->textEdit->append("");
    ui->textEdit->append("Проверка гироскопических азимутов:");
    ui->textEdit->append("Азимут1 гироблок1 " + QString::number( zadacha.stringGradToDouble(ui->EditAzimut1do->text()),'f',8)    + ", в градусной мере, обычной "+ui->EditAzimut1do->text());
    ui->textEdit->append("Азимут1 гироблок2 " + QString::number( zadacha.stringGradToDouble(ui->EditAzimut2do->text()),'f',8)    + ", в градусной мере, обычной "+ui->EditAzimut2do->text());
    ui->textEdit->append("Азимут2 гироблок1 " + QString::number( zadacha.stringGradToDouble(ui->EditAzimut1posle->text()),'f',8) + ", в градусной мере, обычной "+ui->EditAzimut1posle->text());
    ui->textEdit->append("Азимут2 гироблок2 " + QString::number( zadacha.stringGradToDouble(ui->EditAzimut2posle->text()),'f',8) + ", в градусной мере, обычной "+ui->EditAzimut2posle->text());
    ui->textEdit->append("-------------------------------------------------------------------------------------------");

    ui->textEdit->append("");
    ui->textEdit->append("Расчет поправок:");
    ui->textEdit->append("поправка1 Азимут1 гироблок1: " + zadacha.DegToGradstring(dirBaseDo)+ " - " + ui->EditAzimut1do->text() + " = " + zadacha.DegToGradstring(Amendment1));
    ui->textEdit->append("поправка2 Азимут1 гироблок2: " + zadacha.DegToGradstring(dirBaseDo)+ " - " + ui->EditAzimut2do->text() + " = "  + zadacha.DegToGradstring(Amendment2));
    ui->textEdit->append("поправка3 Азимут2 гироблок1: " + zadacha.DegToGradstring(dirBasePosle)+ " - " + ui->EditAzimut1posle->text() + " = " + zadacha.DegToGradstring(Amendment3));
    ui->textEdit->append("поправка4 Азимут2 гироблок2: " + zadacha.DegToGradstring(dirBasePosle)+ " - " + ui->EditAzimut2posle->text() + " = " + zadacha.DegToGradstring(Amendment4));


    //ui->EditPNamePunkt1->text()  ПЗ26444
    //ui->EditPNamePunkt2->text()  ПЗ26339

    //ui->EditNamePunkt1do->text() P328
    //ui->EditNamePunkt2do->text() P685

    //ui->EditNamePunkt1posle->text()
    //ui->EditNamePunkt2posle->text()

    QString mOp = ui->EditPNamePunkt1->text()+ "-" +ui->EditPNamePunkt2->text()+ " " + ui->EditNamePunkt1do->text()+ "-" + ui->EditNamePunkt2do->text() + "= ";
    QString mOz = ui->EditPNamePunkt1->text()+ "-" +ui->EditPNamePunkt2->text()+ " " + ui->EditNamePunkt1posle->text()+ "-" + ui->EditNamePunkt2posle->text() + "= ";

    QString mDp = ui->EditPNamePunkt2->text()+ "-" +ui->EditPNamePunkt1->text()+ " " + ui->EditNamePunkt1do->text()+ "-" + ui->EditNamePunkt2do->text() + "= ";
    QString mDz = ui->EditPNamePunkt2->text()+ "-" +ui->EditPNamePunkt1->text()+ " " + ui->EditNamePunkt1posle->text()+ "-" + ui->EditNamePunkt2posle->text() + "= ";

    ui->textEdit->append("");
    ui->textEdit->append("-------------------------------------------------------------------------------------------");
    ui->textEdit->append("поправка1: " + zadacha.DegToGradstring(dirBaseDo)+ " - " + ui->EditAzimut1do->text() + " = " + zadacha.DegToGradstring(Amendment1));
    //Разность ординат P328-ПЗ26444
    ui->textEdit->append("Y(" + ui->EditNamePunkt1do->text()+ ") =" + ui->EditYkPunkt1do->text());
    ui->textEdit->append("Y(" + ui->EditPNamePunkt1->text()+ ") =" + ui->EditYkPunkt1Podz->text());
    ui->textEdit->append("Разность ординат " + ui->EditNamePunkt1do->text()+ "-" + ui->EditPNamePunkt1->text()+"= "+ QString::number(rOrd1, 'f',4));
    ui->textEdit->append("Внецентренное: " + ui->EditVnecentr1->text());
    ui->textEdit->append("Поправка за сближение меридианов: " + zadacha.DegToGradstring(popMer1));
    ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro1pr) + "+ (" +  zadacha.DegToGradstring(Vnecentr1)+ ") +" + zadacha.DegToGradstring(Amendment1)+ "+ (" + zadacha.DegToGradstring(popMer1)+ ") = " + zadacha.DegToGradstring(aDir1));
    ui->textEdit->append("Дирекционный угол 1 (поправка1,гироблок1,"+ mOp + zadacha.DegToGradstring(aDir1)) ;

    ui->textEdit->append("-------------------------------------------------------------------------------------------");

    ui->textEdit->append("поправка3: " + zadacha.DegToGradstring(dirBasePosle)+ " - " + ui->EditAzimut1posle->text() + " = " + zadacha.DegToGradstring(Amendment3));
    //Разность ординат P684-ПЗ26444
    ui->textEdit->append("Y(" + ui->EditNamePunkt1posle->text()+ ") =" + ui->EditYkPunkt1posle->text());
    ui->textEdit->append("Y(" + ui->EditPNamePunkt1->text()+ ") =" + ui->EditYkPunkt1Podz->text());
    ui->textEdit->append("Разность ординат " + ui->EditNamePunkt1posle->text()+ "-" + ui->EditPNamePunkt1->text()+"= "+ QString::number(rOrd2, 'f',4));
    ui->textEdit->append("Внецентренное: " + ui->EditVnecentr1->text());
    ui->textEdit->append("Поправка за сближение меридианов: " + zadacha.DegToGradstring(popMer2));
    ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro1pr) + "+ (" +  zadacha.DegToGradstring(Vnecentr1)+ ") +" + zadacha.DegToGradstring(Amendment3)+ "+ (" + zadacha.DegToGradstring(popMer2)+ ") = " + zadacha.DegToGradstring(aDir2));
    ui->textEdit->append("Дирекционный угол 2 (поправка3,гироблок1,"+ mOz + zadacha.DegToGradstring(aDir2)) ;

    ui->textEdit->append("-------------------------------------------------------------------------------------------");

    ui->textEdit->append("поправка1: " + zadacha.DegToGradstring(dirBaseDo)+ " - " + ui->EditAzimut1do->text() + " = " + zadacha.DegToGradstring(Amendment1));
    //Разность ординат P684-ПЗ26444
    ui->textEdit->append("Y(" + ui->EditNamePunkt1do->text()+ ") =" + ui->EditYkPunkt1do->text());
    ui->textEdit->append("Y(" + ui->EditPNamePunkt2->text()+ ") =" + ui->EditYkPunkt2Podz->text());
    ui->textEdit->append("Разность ординат " + ui->EditNamePunkt1do->text()+ "-" + ui->EditPNamePunkt2->text()+"= "+ QString::number(rOrd3, 'f',4));
    ui->textEdit->append("Внецентренное: " + ui->EditVnecentr2->text());
    ui->textEdit->append("Поправка за сближение меридианов: " + zadacha.DegToGradstring(popMer3));
    ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro1ob) + "+ (" +  zadacha.DegToGradstring(Vnecentr2)+ ") +" + zadacha.DegToGradstring(Amendment1)+ "+ (" + zadacha.DegToGradstring(popMer3)+ ") = " + zadacha.DegToGradstring(aDir3));
    ui->textEdit->append("Дирекционный угол 3 (поправка1,гироблок1,"+ mDp + zadacha.DegToGradstring(aDir3)) ;

    ui->textEdit->append("-------------------------------------------------------------------------------------------");

    ui->textEdit->append("поправка3: " + zadacha.DegToGradstring(dirBasePosle)+ " - " + ui->EditAzimut1posle->text() + " = " + zadacha.DegToGradstring(Amendment3));
    //Разность ординат P684-ПЗ26444
    ui->textEdit->append("Y(" + ui->EditNamePunkt1posle->text()+ ") =" + ui->EditYkPunkt1posle->text());
    ui->textEdit->append("Y(" + ui->EditPNamePunkt2->text()+ ") =" + ui->EditYkPunkt2Podz->text());
    ui->textEdit->append("Разность ординат " + ui->EditNamePunkt1posle->text()+ "-" + ui->EditPNamePunkt1->text()+"= "+ QString::number(rOrd4, 'f',4));
    ui->textEdit->append("Внецентренное: " + ui->EditVnecentr2->text());
    ui->textEdit->append("Поправка за сближение меридианов: " + zadacha.DegToGradstring(popMer4));
    ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro1ob) + "+ (" +  zadacha.DegToGradstring(Vnecentr2)+ ") +" + zadacha.DegToGradstring(Amendment3)+ "+ (" + zadacha.DegToGradstring(popMer4)+ ") = " + zadacha.DegToGradstring(aDir4));
    ui->textEdit->append("Дирекционный угол 4 (поправка3,гироблок1,"+ mDz + zadacha.DegToGradstring(aDir4)) ;

    ui->textEdit->append("-------------------------------------------------------------------------------------------");




    ui->textEdit->append("поправка2: " + zadacha.DegToGradstring(dirBaseDo)+ " - " + ui->EditAzimut2do->text() + " = " + zadacha.DegToGradstring(Amendment2));
    //Разность ординат P328-ПЗ26444
    ui->textEdit->append("Y(" + ui->EditNamePunkt1do->text()+ ") =" + ui->EditYkPunkt1do->text());
    ui->textEdit->append("Y(" + ui->EditPNamePunkt1->text()+ ") =" + ui->EditYkPunkt1Podz->text());
    ui->textEdit->append("Разность ординат " + ui->EditNamePunkt1do->text()+ "-" + ui->EditPNamePunkt1->text()+"= "+ QString::number(rOrd1, 'f',4));
    ui->textEdit->append("Внецентренное: " + ui->EditVnecentr1->text());
    ui->textEdit->append("ВнецентренноеП: " + zadacha.DegToGradstring(Vnecentr1));
    ui->textEdit->append("ВнецентренноеПр: " + QString::number(Vnecentr1,'f',6));
    ui->textEdit->append("Поправка за сближение меридианов: " + zadacha.DegToGradstring(popMer1));
    ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro2pr) + "+ (" +  zadacha.DegToGradstring(Vnecentr1)+ ") +" + zadacha.DegToGradstring(Amendment2)+ "+ (" + zadacha.DegToGradstring(popMer1)+ ") = " + zadacha.DegToGradstring(aDir5));
    ui->textEdit->append("Дирекционный угол 5 (поправка2,гироблок2,"+ mOp + zadacha.DegToGradstring(aDir5)) ;

    ui->textEdit->append("-------------------------------------------------------------------------------------------");

    ui->textEdit->append("поправка4: " + zadacha.DegToGradstring(dirBasePosle)+ " - " + ui->EditAzimut2posle->text() + " = " + zadacha.DegToGradstring(Amendment4));
    //Разность ординат P684-ПЗ26444
    ui->textEdit->append("Y(" + ui->EditNamePunkt1posle->text()+ ") =" + ui->EditYkPunkt1posle->text());
    ui->textEdit->append("Y(" + ui->EditPNamePunkt1->text()+ ") =" + ui->EditYkPunkt1Podz->text());
    ui->textEdit->append("Разность ординат " + ui->EditNamePunkt1posle->text()+ "-" + ui->EditPNamePunkt1->text()+"= "+ QString::number(rOrd2, 'f',4));
    ui->textEdit->append("Внецентренное: " + ui->EditVnecentr1->text());
    ui->textEdit->append("Поправка за сближение меридианов: " + zadacha.DegToGradstring(popMer2));
    ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro2pr) + "+ (" +  zadacha.DegToGradstring(Vnecentr1)+ ") +" + zadacha.DegToGradstring(Amendment4)+ "+ (" + zadacha.DegToGradstring(popMer2)+ ") = " + zadacha.DegToGradstring(aDir6));
    ui->textEdit->append("Дирекционный угол 6 (поправка4,гироблок2,"+ mOz + zadacha.DegToGradstring(aDir6)) ;

    ui->textEdit->append("-------------------------------------------------------------------------------------------");


    ui->textEdit->append("поправка2: " + zadacha.DegToGradstring(dirBaseDo)+ " - " + ui->EditAzimut2do->text() + " = " + zadacha.DegToGradstring(Amendment2));
    //Разность ординат P684-ПЗ26444
    ui->textEdit->append("Y(" + ui->EditNamePunkt1do->text()+ ") =" + ui->EditYkPunkt1do->text());
    ui->textEdit->append("Y(" + ui->EditPNamePunkt2->text()+ ") =" + ui->EditYkPunkt2Podz->text());
    ui->textEdit->append("Разность ординат " + ui->EditNamePunkt1do->text()+ "-" + ui->EditPNamePunkt2->text()+"= "+ QString::number(rOrd3, 'f',4));
    ui->textEdit->append("Внецентренное: " + ui->EditVnecentr2->text());
    ui->textEdit->append("Поправка за сближение меридианов: " + zadacha.DegToGradstring(popMer3));
    ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro2ob) + "+ (" +  zadacha.DegToGradstring(Vnecentr2)+ ") +" + zadacha.DegToGradstring(Amendment2)+ "+ (" + zadacha.DegToGradstring(popMer3)+ ") = " + zadacha.DegToGradstring(aDir7));
    ui->textEdit->append("Дирекционный угол 7 (поправка2,гироблок2,"+ mDp + zadacha.DegToGradstring(aDir7)) ;

    ui->textEdit->append("-------------------------------------------------------------------------------------------");

    ui->textEdit->append("поправка4: " + zadacha.DegToGradstring(dirBasePosle)+ " - " + ui->EditAzimut2posle->text() + " = " + zadacha.DegToGradstring(Amendment4));
    //Разность ординат P684-ПЗ26444
    ui->textEdit->append("Y(" + ui->EditNamePunkt1posle->text()+ ") =" + ui->EditYkPunkt1posle->text());
    ui->textEdit->append("Y(" + ui->EditPNamePunkt2->text()+ ") =" + ui->EditYkPunkt2Podz->text());
    ui->textEdit->append("Разность ординат " + ui->EditNamePunkt1posle->text()+ "-" + ui->EditPNamePunkt1->text()+"= "+ QString::number(rOrd4, 'f',4));
    ui->textEdit->append("Внецентренное: " + ui->EditVnecentr2->text());
    ui->textEdit->append("Поправка за сближение меридианов: " + zadacha.DegToGradstring(popMer4));
    ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro2ob) + "+ (" +  zadacha.DegToGradstring(Vnecentr2)+ ") +" + zadacha.DegToGradstring(Amendment4)+ "+ (" + zadacha.DegToGradstring(popMer4)+ ") = " + zadacha.DegToGradstring(aDir8));
    ui->textEdit->append("Дирекционный угол 8 (поправка4,гироблок2,"+ mDz + zadacha.DegToGradstring(aDir8)) ;

    ui->textEdit->append("-------------------------------------------------------------------------------------------");

    //(aDir1_2-aDir3_4)*3600
    //(aDir5_6-aDir7_8)*3600

    //qSqrt
    //    double mOne = qSqrt(qPow(((aDir1_2-aDir3_4)*3600),2)/4);
    //    double mTwo = qSqrt(qPow(((aDir5_6-aDir7_8)*3600),2)/4);
    double m1=qPow(((aDir1_2-aDir3_4)*3600),2);
    double m2=qPow(((aDir5_6-aDir7_8)*3600),2);
    double mSKO = qSqrt((m1+m2)/8);
    qDebug() << "dAdir=" << (aDir1_2-aDir3_4)*3600 << (aDir5_6-aDir7_8)*3600;
    qDebug() << "SKO=" << m1 << m2 << mSKO;
    ui->Edit_av->setText(QString::number(mSKO, 'f',1)+"\"");

    double Mbig = mSKO*qSqrt(0.5);
    ui->Edit_M->setText(QString::number(Mbig, 'f',1)+"\"");


    double daDir1_2kv = qPow((aDir1_2 - aDirAlverage)*3600,2);
    double daDir3_4kv = qPow((aDir3_4 - aDirAlverage)*3600,2);
    double daDir5_6kv = qPow((aDir5_6 - aDirAlverage)*3600,2);
    double daDir7_8kv = qPow((aDir7_8 - aDirAlverage)*3600,2);

    double mnew = qSqrt((daDir1_2kv+daDir3_4kv+daDir5_6kv+daDir7_8kv)/3);
    ui->textEdit->append("Cр Кв ошибка 4х измерений (бессель)=" + QString::number(mnew) + "\"");

    double LinePya       = zadacha.OgzLine(ui->EditXkPunkt1Podz->text().toDouble(),
                                           ui->EditYkPunkt1Podz->text().toDouble(),
                                           ui->EditXkPunkt2Podz->text().toDouble(),
                                           ui->EditYkPunkt2Podz->text().toDouble());

    double LineBaseDo    = zadacha.OgzLine(ui->EditXkPunkt1do->text().toDouble(),
                                           ui->EditYkPunkt1do->text().toDouble(),
                                           ui->EditXkPunkt2do->text().toDouble(),
                                           ui->EditYkPunkt2do->text().toDouble());

    double LineBasePosle = zadacha.OgzLine(ui->EditXkPunkt1posle->text().toDouble(),
                                           ui->EditYkPunkt1posle->text().toDouble(),
                                           ui->EditXkPunkt2posle->text().toDouble(),
                                           ui->EditYkPunkt2posle->text().toDouble());


    QString line;
    QString lineSave;

    //    Работа с docx    
    QString name = QCoreApplication::applicationDirPath() + "/templateFull.docx";
    QZipReader unzip(name); // файл шаблон

    //Буффер шаблона document
    QByteArray ba; //файл-массив байт из архива
    QTextStream t( &ba );
    t.setCodec("UTF-8");
    QByteArray bas; //файл-массив байт в архив
    QTextStream out(&bas);   //,QIODevice::ReadWrite
    out.setCodec("UTF-8");

    //Буффер шаблона header
    QByteArray baz; //файл-массив байт из архива
    QTextStream tz( &baz );
    tz.setCodec("UTF-8");
    QByteArray basz; //файл-массив байт в архив
    QTextStream outz(&basz);   //,QIODevice::ReadWrite
    outz.setCodec("UTF-8");

//#ifdef Q_OS_WIN
//    QString file = pathDirTemplate + QDir::separator() + ui->comboBox_template->itemText(ui->comboBox_template->currentIndex()) + ".docx";
//    qDebug()<<"Шаблон взят"<<file ;
//    QFile templatefile( QDir::toNativeSeparators(file));
//#endif

//#ifdef Q_OS_LINUX
//    //QFile templatefile( QCoreApplication::applicationDirPath() + "/templateFull.docx");
//    QString file = pathDirTemplate + QDir::separator() + ui->comboBox_template->itemText(ui->comboBox_template->currentIndex()) + ".docx";
//    qDebug()<<"Шаблон взят"<<file ;
//    QFile templatefile( QDir::toNativeSeparators(file));
//#endif



#ifdef Q_OS_WIN
    QFile fileSave( "C:/Windows/Temp/OtchetFull.docx");
#else
    QFile fileSave(QCoreApplication::applicationDirPath() + "/OtchetFull.docx");
#endif

    QVector<QZipReader::FileInfo> files = unzip.fileInfoList();
    QZipReader::FileInfo fi;
    QZipWriter zip(fileSave.fileName()); //        QZipWriter zip(QLatin1String("C:/Windows/Temp/OtchetFull.docx"));
    if (zip.status() != QZipWriter::NoError)
        return; else {
        zip.setCompressionPolicy(QZipWriter::AutoCompress);
        //читаем содержимое архива
        for (int fl = 0; fl < files.size(); ++fl) {
            fi = files.at(fl);
            if (files.at(fl).filePath == "word/header1.xml") {
                baz = unzip.fileData(fi.filePath);
                //если нет ошибки расспаковки, то сохран¤ем файл
                if (fi.size != baz.size()) qDebug() << "unzip error header1";
                //читаем шаблон и пишим  результат в файл
                qDebug()<<"start Header";
                while ( !tz.atEnd() ) {
                    // read and parse the command line
                    line = tz.readLine();         // line of text excluding '\n'
                    // олонтитул
                    lineSave = line.replace("textEdit_kolontitul",ui->textEdit_kolontitul->toPlainText());
                    basz.append( lineSave);
                }

                //пишем результат обработки шаблона
                zip.setCreationPermissions(QFile::permissions(files.at(fl).filePath));
                zip.addFile(files.at(fl).filePath, basz);
                qDebug()<<"end header";
            } else {
                if (files.at(fl).filePath == "word/document.xml") {
                    ba = unzip.fileData(fi.filePath);
                    //если нет ошибки расспаковки, то сохран¤ем файл
                    if (fi.size != ba.size()) qDebug() << "unzip error document";
                    qDebug()<<"start Document";
                    //читаем шаблон и пишим  результат в файл
                    while ( !t.atEnd() ) {
                        // read and parse the command line
                        line = t.readLine();         // line of text excluding '\n'
                        qDebug()<< line;
                        //dateism
                        lineSave = line.replace("dateism",ui->date_ism->date().toString("dd.MM.yyyy г."));
                        //Колонтитул
                        lineSave = line.replace("textEdit_kolontitul",ui->textEdit_kolontitul->toPlainText());
                        //textEdit_Vved
                        lineSave = lineSave.replace("textEdit_Vved",ui->textEdit_Vved->toPlainText());
                        //pzodin-pzdva
                        lineSave = lineSave.replace("pzodin",ui->EditPNamePunkt1->text());
                        lineSave = lineSave.replace("pzdva",ui->EditPNamePunkt2->text());
                        //Punkt1do-Punkt2do
                        lineSave = lineSave.replace("PunktOdindo",ui->EditNamePunkt1do->text());
                        lineSave = lineSave.replace("PunktDvado",ui->EditNamePunkt2do->text());
                        //Punkt1posle-Punkt2posle
                        lineSave = lineSave.replace("PunktOdinposle",ui->EditNamePunkt1posle->text());
                        lineSave = lineSave.replace("PunktDvaposle",ui->EditNamePunkt2posle->text());
                        //giroteodolitN
                        lineSave = lineSave.replace("giroteodolitN",ui->EditTypeGiro->text());
                        //shirotasr
                        lineSave = lineSave.replace("shirotasr",QString::number((ui->EditShirotado->text().toDouble()+ui->EditShirotaposle->text().toDouble())/2,'f',4));
                        //NgiroOdin
                        lineSave = lineSave.replace("NgiroOdin",ui->EditNameGiro1->text());
                        //NgiroDva
                        lineSave = lineSave.replace("NgiroDva",ui->EditNameGiro2->text());
                        //PopravkaOdin
                        lineSave = lineSave.replace("PopravkaOdin",zadacha.GradstringToGradstring(ui->EditAmendment1->text()) );
                        //PopravkaDva
                        lineSave = lineSave.replace("PopravkaDva",zadacha.GradstringToGradstring(ui->EditAmendment3->text()) );
                        //PopravkaTri
                        lineSave = lineSave.replace("PopravkaTri",zadacha.GradstringToGradstring(ui->EditAmendment2->text()) );
                        //PopravkaChetire
                        lineSave = lineSave.replace("PopravkaChetire",zadacha.GradstringToGradstring(ui->EditAmendment4->text()) );
                        //dlinnaBaseDo
                        lineSave = lineSave.replace("dlinnaBaseDo",QString::number(LineBaseDo,'f',3) );
                        //dlinnaBasePosle
                        lineSave = lineSave.replace("dlinnaBasePosle",QString::number(LineBasePosle,'f',3) );
                        //dlinnaP
                        lineSave = lineSave.replace("dlinnaP",QString::number(LinePya,'f',3) );
                        //mOne Edit_m1
                        //          lineSave = lineSave.replace("mOne",(ui->Edit_m1->text()) );
                        //mTwo Edit_m2
                        //          lineSave = lineSave.replace("mTwo",(ui->Edit_m2->text()) );
                        //dAdir1_4
                        lineSave = lineSave.replace("dAdirOne",ui->EditDirectional1_4d->text() );
                        //dAdir5_8
                        lineSave = lineSave.replace("dAdirDva",ui->EditDirectional5_8d->text() );
                        //mav
                        lineSave = lineSave.replace("mav",(ui->Edit_av->text()) );
                        //Mbig Edit_M
                        lineSave = lineSave.replace("Mbig",(ui->Edit_M->text()) );
                        //aDir1_2kv
                        lineSave = lineSave.replace("aDirODkv",zadacha.GradstringToGradstring( ui->EditDirectional1_2->text()  ) );
                        //aDir3_4kv
                        lineSave = lineSave.replace("aDirTCkv",zadacha.GradstringToGradstring( ui->EditDirectional3_4->text()  ) );
                        //aDir5_6kv
                        lineSave = lineSave.replace("aDirPSkv",zadacha.GradstringToGradstring( ui->EditDirectional5_6->text()  ) );
                        //aDir7_8kv
                        lineSave = lineSave.replace("aDirCVkv",zadacha.GradstringToGradstring( ui->EditDirectional7_8->text()  ) );
                        //aDir_av
                        lineSave = lineSave.replace("aDir_av",zadacha.GradstringToGradstring( ui->EditDirectionAlverage->text()  ) );
                        //Azimut1do
                        lineSave = lineSave.replace("AzimutOdindo",zadacha.GradstringToGradstring( ui->EditAzimut1do->text()  ) );
                        //Azimut2do
                        lineSave = lineSave.replace("AzimutDvado",zadacha.GradstringToGradstring( ui->EditAzimut2do->text()  ) );
                        //Azimut1posle
                        lineSave = lineSave.replace("AzimutOdinposle",zadacha.GradstringToGradstring( ui->EditAzimut1posle->text()  ) );
                        //Azimut2posle
                        lineSave = lineSave.replace("AzimutDvaposle",zadacha.GradstringToGradstring( ui->EditAzimut2posle->text()  ) );
                        //dirBaseDo
                        lineSave = lineSave.replace("dirBaseDo",zadacha.GradstringToGradstring( zadacha.DegToGradstring(dirBaseDo)  ) );
                        //dirBasePosle
                        lineSave = lineSave.replace("dirBasePosle",zadacha.GradstringToGradstring( zadacha.DegToGradstring(dirBasePosle)  ) );

                        //YPKT1do
                        lineSave = lineSave.replace("YPKTOdindo",(ui->EditYkPunkt1do->text()) );
                        //YPKT1posle
                        lineSave = lineSave.replace("YPKTOdinposle",(ui->EditYkPunkt1posle->text()) );
                        //Ypozodin
                        lineSave = lineSave.replace("Ypozodin",(ui->EditYkPunkt1Podz->text()) );
                        //Ypozdva
                        lineSave = lineSave.replace("Ypozdva",(ui->EditYkPunkt2Podz->text()) );

                        //EditAzimut1giro1pr
                        lineSave = lineSave.replace("AzimutOdingiroOdinpr",zadacha.GradstringToGradstring(ui->EditAzimut1giro1pr->text()) );
                        //EditAzimut1giro1ob
                        lineSave = lineSave.replace("AzimutOdingiroOdinob",zadacha.GradstringToGradstring(ui->EditAzimut1giro1ob->text()) );
                        //EditAzimut1giro2pr
                        lineSave = lineSave.replace("AzimutOdingiroDvapr",zadacha.GradstringToGradstring(ui->EditAzimut1giro2pr->text()) );
                        //EditAzimut1giro2ob
                        lineSave = lineSave.replace("AzimutOdingiroDvaob",zadacha.GradstringToGradstring(ui->EditAzimut1giro2ob->text()) );

                        //popMer1
                        lineSave = lineSave.replace("popMerOdin",zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer1)) );
                        //popMer2
                        lineSave = lineSave.replace("popMerDva",zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer2)) );
                        //popMer3
                        lineSave = lineSave.replace("popMerTri",zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer3)) );
                        //popMer4
                        lineSave = lineSave.replace("popMerChet",zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer4)) );
                        //Vnecentr1
                        lineSave = lineSave.replace("VnecentrOdin",zadacha.GradstringToGradstring(ui->EditVnecentr1->text()) );
                        //Vnecentr2
                        lineSave = lineSave.replace("VnecentrDva",zadacha.GradstringToGradstring(ui->EditVnecentr2->text()) );

                        //Formula1
                        QString Formula1 = zadacha.GradstringToGradstring(zadacha.DegToGradstring(Azimut1giro1pr));
                        if (Vnecentr1>0){Formula1.append("+");}
                        Formula1.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Vnecentr1)));
                        Formula1.append("+");
                        Formula1.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Amendment1)));
                        if (popMer1>0){Formula1.append("+");}
                        Formula1.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer1)));
                        Formula1.append("=");
                        Formula1.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(aDir1)));
                        lineSave = lineSave.replace("FormulaOdin",Formula1 );
                        //          ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro1pr) + "+ (" +  zadacha.DegToGradstring(Vnecentr1)+ ") +" + zadacha.DegToGradstring(Amendment1)+ "+ (" + zadacha.DegToGradstring(popMer1)+ ") = " + zadacha.DegToGradstring(aDir1));

                        //Formula2
                        QString Formula2 = zadacha.GradstringToGradstring(zadacha.DegToGradstring(Azimut1giro1pr));
                        if (Vnecentr1>0){Formula2.append("+");}
                        Formula2.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Vnecentr1)));
                        Formula2.append("+");
                        Formula2.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Amendment3)));
                        if (popMer2>0){Formula2.append("+");}
                        Formula2.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer2)));
                        Formula2.append("=");
                        Formula2.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(aDir2)));
                        lineSave = lineSave.replace("FormulaDva",Formula2 );
                        //          ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro1pr) + "+ (" +  zadacha.DegToGradstring(Vnecentr1)+ ") +" + zadacha.DegToGradstring(Amendment3)+ "+ (" + zadacha.DegToGradstring(popMer2)+ ") = " + zadacha.DegToGradstring(aDir2));

                        //Formula3
                        QString Formula3 = zadacha.GradstringToGradstring(zadacha.DegToGradstring(Azimut1giro1ob));
                        if (Vnecentr2>0){Formula3.append("+");}
                        Formula3.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Vnecentr2)));
                        Formula3.append("+");
                        Formula3.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Amendment1)));
                        if (popMer3>0){Formula3.append("+");}
                        Formula3.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer3)));
                        Formula3.append("=");
                        Formula3.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(aDir3)));
                        lineSave = lineSave.replace("FormulaTri",Formula3 );
                        //          ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro1ob) + "+ (" +  zadacha.DegToGradstring(Vnecentr2)+ ") +" + zadacha.DegToGradstring(Amendment1)+ "+ (" + zadacha.DegToGradstring(popMer3)+ ") = " + zadacha.DegToGradstring(aDir3));

                        //Formula4
                        QString Formula4 = zadacha.GradstringToGradstring(zadacha.DegToGradstring(Azimut1giro1ob));
                        if (Vnecentr2>0){
                            Formula4.append("+");}
                        Formula4.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Vnecentr2)));
                        Formula4.append("+");
                        Formula4.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Amendment3)));
                        if (popMer4>0){
                            Formula4.append("+");}
                        Formula4.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer4)));
                        Formula4.append("=");
                        Formula4.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(aDir4)));
                        lineSave = lineSave.replace("FormulaChet",Formula4 );
                        //          ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro1ob) + "+ (" +  zadacha.DegToGradstring(Vnecentr2)+ ") +" + zadacha.DegToGradstring(Amendment3)+ "+ (" + zadacha.DegToGradstring(popMer4)+ ") = " + zadacha.DegToGradstring(aDir4));

                        //Formula5
                        QString Formula5 = zadacha.GradstringToGradstring(zadacha.DegToGradstring(Azimut1giro2pr));
                        if (Vnecentr1>0){
                            Formula5.append("+");}
                        Formula5.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Vnecentr1)));
                        Formula5.append("+");
                        Formula5.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Amendment2)));
                        if (popMer1>0){
                            Formula5.append("+");}
                        Formula5.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer1)));
                        Formula5.append("=");
                        Formula5.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(aDir5)));
                        lineSave = lineSave.replace("FormulaPyat",Formula5 );
                        //          ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro2pr) + "+ (" +  zadacha.DegToGradstring(Vnecentr1)+ ") +" + zadacha.DegToGradstring(Amendment2)+ "+ (" + zadacha.DegToGradstring(popMer1)+ ") = " + zadacha.DegToGradstring(aDir5));

                        //Formula6
                        QString Formula6 = zadacha.GradstringToGradstring(zadacha.DegToGradstring(Azimut1giro2pr));
                        if (Vnecentr1>0){
                            Formula6.append("+");}
                        Formula6.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Vnecentr1)));
                        Formula6.append("+");
                        Formula6.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Amendment4)));
                        if (popMer2>0){
                            Formula6.append("+");}
                        Formula6.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer2)));
                        Formula6.append("=");
                        Formula6.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(aDir6)));
                        lineSave = lineSave.replace("FormulaShest",Formula6 );
                        //          ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro2pr) + "+ (" +  zadacha.DegToGradstring(Vnecentr1)+ ") +" + zadacha.DegToGradstring(Amendment4)+ "+ (" + zadacha.DegToGradstring(popMer2)+ ") = " + zadacha.DegToGradstring(aDir6));

                        //Formula7
                        QString Formula7 = zadacha.GradstringToGradstring(zadacha.DegToGradstring(Azimut1giro2ob));
                        if (Vnecentr2>0){
                            Formula7.append("+");}
                        Formula7.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Vnecentr2)));
                        Formula7.append("+");
                        Formula7.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Amendment2)));
                        if (popMer3>0){
                            Formula7.append("+");}
                        Formula7.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer3)));
                        Formula7.append("=");
                        Formula7.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(aDir7)));
                        lineSave = lineSave.replace("FormulaSem",Formula7 );
                        //          ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro2ob) + "+ (" +  zadacha.DegToGradstring(Vnecentr2)+ ") +" + zadacha.DegToGradstring(Amendment2)+ "+ (" + zadacha.DegToGradstring(popMer3)+ ") = " + zadacha.DegToGradstring(aDir7));

                        //Formula8
                        QString Formula8 = zadacha.GradstringToGradstring(zadacha.DegToGradstring(Azimut1giro2ob));
                        if (Vnecentr2>0){
                            Formula8.append("+");}
                        Formula8.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Vnecentr2)));
                        Formula8.append("+");
                        Formula8.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(Amendment4)));
                        if (popMer4>0){
                            Formula8.append("+");}
                        Formula8.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(popMer4)));
                        Formula8.append("=");
                        Formula8.append(zadacha.GradstringToGradstring(zadacha.DegToGradstring(aDir8)));
                        lineSave = lineSave.replace("FormulaVosem",Formula8 );
                        //          ui->textEdit->append("Формула= " + zadacha.DegToGradstring(Azimut1giro2ob) + "+ (" +  zadacha.DegToGradstring(Vnecentr2)+ ") +" + zadacha.DegToGradstring(Amendment4)+ "+ (" + zadacha.DegToGradstring(popMer4)+ ") = " + zadacha.DegToGradstring(aDir8));

                        out << lineSave;

                    }
                    //пишем результат обработки шаблона
                    zip.setCreationPermissions(QFile::permissions(files.at(fl).filePath));
                    zip.addFile(files.at(fl).filePath, bas);
                    qDebug() << "end Document";
                    //TO-DO Убрать что выше
                } else {
                    zip.setCreationPermissions(QFile::permissions(files.at(fl).filePath));
                    zip.addFile(files.at(fl).filePath, unzip.fileData(files.at(fl).filePath));
                }
            } // else header
        }
    }
    unzip.close();
    zip.close();
}



void MainWindow::closeEvent(QCloseEvent *event){

    on_actionExit_triggered();
    event->accept();

}

void MainWindow::on_pushButtonOtchet_clicked()
{
    on_pushButtonRaschet_clicked();

    QSettings settings("HKEY_CLASSES_ROOT\\Applications\\winword.exe\\shell\\edit\\command",QSettings::NativeFormat);
    QString s = settings.value("Default").toString();
    s=s.split('"').value(1);
    QStringList arguments;
    arguments << "C:/Windows/Temp/OtchetFull.docx";

    QProcess *myProcess = new QProcess();
    myProcess->start(QDir::toNativeSeparators(s), arguments);
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString nameFile;
    nameFile = QFileDialog::getSaveFileName(this,tr("Выберите файл проекта"),QStandardPaths::DocumentsLocation +"/Отчёт_гироскопия.mgiro",tr("Проект Giro (*.mgiro)") );
    if(nameFile.isEmpty())
    {return;}
    saveFile(nameFile);
}

void MainWindow::saveFile(QString fileName){
    //    QIODevice::ReadOnly    Открыть файл для чтения
    //    QIODevice::WriteOnly   Открыть файл для записи (таким методом можно просто создать файл)
    //    QIODevice::ReadWrite   Открыть файл для чтения и записи
    //    QIODevice::Append      Открыть файл для дополнения файла в конец

    //Сохраняем
    //QFile file( QCoreApplication::applicationDirPath() +"/demo.mgiro");
    QFile file( fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    else {
        QTextStream out(&file);
        out.setCodec("UTF-8");

        out << ui->date_ism->date().toString("dd.MM.yyyy") << "\n";
        out << ui->EditTypeGiro->text() << "\n";
        out << ui->EditNameGiro1->text() << "\n";
        out << ui->EditNameGiro2->text() << "\n";

        out << ui->textEdit_kolontitul->toPlainText().simplified() << "\n";
        out << ui->textEdit_Vved->toPlainText().simplified() << "\n";

        out << ui->EditShirotado->text() << "\n";
        out << ui->EditNamePunkt1do->text() << "\n";
        out << ui->EditXkPunkt1do->text() << "\n";
        out << ui->EditYkPunkt1do->text() << "\n";
        out << ui->EditNamePunkt2do->text() << "\n";
        out << ui->EditXkPunkt2do->text() << "\n";
        out << ui->EditYkPunkt2do->text() << "\n";
        out << ui->EditAzimut1do->text() << "\n";
        out << ui->EditAzimut2do->text() << "\n";

        out << ui->EditShirotaposle->text() << "\n";
        out << ui->EditNamePunkt1posle->text() << "\n";
        out << ui->EditNamePunkt2posle->text() << "\n";
        out << ui->EditXkPunkt1posle->text() << "\n";
        out << ui->EditYkPunkt1posle->text() << "\n";
        out << ui->EditXkPunkt2posle->text() << "\n";
        out << ui->EditYkPunkt2posle->text() << "\n";
        out << ui->EditAzimut1posle->text() << "\n";
        out << ui->EditAzimut2posle->text() << "\n";

        out << ui->EditPNamePunkt1->text() << "\n";
        out << ui->EditXkPunkt1Podz->text() << "\n";
        out << ui->EditYkPunkt1Podz->text() << "\n";
        out << ui->EditPNamePunkt2->text() << "\n";
        out << ui->EditXkPunkt2Podz->text() << "\n";
        out << ui->EditYkPunkt2Podz->text() << "\n";

        out << ui->EditVnecentr1->text() << "\n";
        out << ui->EditAzimut1giro1pr->text() << "\n";
        out << ui->EditAzimut1giro2pr->text() << "\n";

        out << ui->EditVnecentr2->text() << "\n";
        out << ui->EditAzimut1giro1ob->text() << "\n";
        out << ui->EditAzimut1giro2ob->text() << "\n";

        // добавил в 0.2
        out << ui->EditPPKPunkt1->text() << "\n";
        out << ui->EditPKolcoPunkt1->text() << "\n";
        out << ui->EditPPKPunkt2->text() << "\n";
        out << ui->EditPKolcoPunkt2->text() << "\n";
        out << ui->EditPismo->text() << "\n";
        out << ui->textEdit_Tema->toPlainText().simplified() << "\n";
        //конец
    }
    file.close();
    openFile = fileName;
    statusBar()->showMessage(tr("Файл сохранен..."), 3000);
}


void MainWindow::on_actionSave_triggered()
{
    if (openFile =="") {
        on_actionSaveAs_triggered();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Сохранние файла...");
        msgBox.setInformativeText("При сохранении файла все данные в файле будут заменены новыми, \n продолжить?");
        msgBox.setIcon(QMessageBox::Warning);
        QPushButton *yes = msgBox.addButton(tr("Да"), QMessageBox::ActionRole);
        msgBox.addButton(QObject::tr("Отмена"), QMessageBox::ActionRole);
        msgBox.setDefaultButton(yes);
        msgBox.exec();
        if(msgBox.clickedButton() != yes)        {
            statusBar()->showMessage(tr("Сохранение файла отменено..."), 3000);
            return;
        }
        saveFile(openFile);
    }

}

