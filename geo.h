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

#ifndef GEO_H
#define GEO_H
#include <QString>


class geo
{
public:
    geo();
    ~geo();
    double OgzDir             (double x1,double y1,double x2,double y2);
    double OgzLine            (double x1,double y1,double x2,double y2);
    double stringGradToDouble (QString ugol); //переводит 0 00'00" в double
    QString DegToGradstring   (double ugol);     //переводит double в 0 00'00"
    QString GradstringToGradstring(QString ugol);//переводит 0 00 00 в 0 00'00"
};

#endif // GEO_H
