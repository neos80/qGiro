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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/private/qzipwriter_p.h>
#include <QtGui/private/qzipreader_p.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:

    void on_EditNameGiro1_returnPressed();

    void on_EditNameGiro2_returnPressed();

    void on_EditShirotado_returnPressed();

    void on_EditNamePunkt1do_returnPressed();

    void on_EditXkPunkt1do_returnPressed();

    void on_EditYkPunkt1do_returnPressed();

    void on_EditNamePunkt2do_returnPressed();

    void on_EditXkPunkt2do_returnPressed();

    void on_EditYkPunkt2do_returnPressed();

    void on_EditAzimut1do_returnPressed();

    void on_EditAzimut2do_returnPressed();

    void on_EditShirotaposle_returnPressed();

    void on_EditNamePunkt1posle_returnPressed();

    void on_EditXkPunkt1posle_returnPressed();

    void on_EditYkPunkt1posle_returnPressed();

    void on_EditNamePunkt2posle_returnPressed();

    void on_EditXkPunkt2posle_returnPressed();

    void on_EditYkPunkt2posle_returnPressed();

    void on_EditAzimut1posle_returnPressed();

    void on_EditAzimut2posle_returnPressed();

    void on_EditXkPunkt1Podz_returnPressed();

    void on_EditYkPunkt1Podz_returnPressed();

    void on_EditPNamePunkt2_returnPressed();

    void on_EditPNamePunkt1_returnPressed();

    void on_EditYkPunkt2Podz_returnPressed();

    void on_EditXkPunkt2Podz_returnPressed();

    void on_EditVnecentr1_returnPressed();

    void on_EditVnecentr2_returnPressed();    

    void on_EditTypeGiro_returnPressed();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionNew_triggered();

//    bool maybeSave();

//    void loadFile(const QString &fileName);

//    bool saveFile(const QString &fileName);

//    void setCurrentFile(const QString &fileName);

//    QString strippedName(const QString &fullFileName);

    void on_actionExampl_triggered();

    void on_actionExit_triggered();

    void on_actionAboutPrg_triggered();

    void on_actionAbout_Qt_triggered();

    void readSettings();

    void writeSettings();

    void on_EditNamePunkt1do_editingFinished();

    void on_EditNamePunkt1posle_editingFinished();

    void on_EditPNamePunkt1_editingFinished();

    void on_EditPNamePunkt2_editingFinished();

    void on_pushButtonRaschet_clicked();

    void saveFile(QString fileName);

    void fileOpen(QString fileName);

    void on_pushButtonOtchet_clicked();

    void on_actionSaveAs_triggered();    

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
