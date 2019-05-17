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
 
#include "geo.h"
#include <stdio.h>
#include <QtMath>
#include <QString>
#include <QDebug>

#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923


geo::geo()
{

}

geo::~geo()
{

}

//OgzDir - Вычисления дирекционного угла
double geo::OgzDir (double x1,double y1,double x2,double y2){
    if ((x2 - x1 != 0) || (y2 - y1 != 0 )) {
        double s = (x2-x1) / sqrt( pow((x2 - x1),2) + (pow((y2 - y1),2)));
        double c = qRadiansToDegrees(M_PI_2 - atan(s/sqrt(1-pow(s,2) ) ) );
        if ((y2 - y1) < 0) return 360-c; else return c;
    } else return 0;
}

//Обратная геодезическая, расстояние
double geo::OgzLine (double x1,double y1,double x2,double y2){
    if ((x2 - x1 != 0) || (y2 - y1 != 0 )) return sqrt( pow((x2 - x1),2) + (pow((y2 - y1),2))); else return 0;
}

//переводим строковый угол в числовой градусной меры.
double geo::stringGradToDouble (QString ugol){        
    int g = ugol.split(" ").value(0).toInt();
    double m = ugol.split(" ").value(1).toDouble();
    double s = ugol.split(" ").value(2).toDouble();
    //qDebug() << ugol.split(" ").value(0).toInt() << ugol.split(" ").value(1).toDouble() << ugol.split(" ").value(2).toDouble();

    if (ugol.split(" ").value(0).at(0).isDigit() == true){
        return (g + m /60 + s /3600);
    } else {
        return (g - m /60 - s /3600);
    }


}

//Функция перевода градусов в " ° ' " "
QString geo::DegToGradstring(double ugol){    
    QString sminus ="";
    if (ugol<0){
        sminus ="-";
        ugol=ugol*(-1);
    }
    int g = static_cast<int>(ugol);
    int m = (ugol-g)*60;
    double dm = (ugol-g)*60;
    double ss = (dm-m)*60;    
    QString mul = (m<10)?("0"):("");
    QString sul = (ss<10)?("0"):("");
    return sminus+QString::number(g)+" "+ mul + QString::number(m)+" "+sul+ QString::number(ss,'f',1);
}

QString geo::GradstringToGradstring(QString ugol){
    return ugol.split(" ").value(0)+"°"+ugol.split(" ").value(1)+"'"+ugol.split(" ").value(2)+"\"";
}

