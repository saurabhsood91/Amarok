/****************************************************************************************
 * Copyright (c) 2009 Nikolaj Hald Nielsen <nhn@kde.org>                                *
 * Copyright (c) 2011 Sven Krohlas <sven@getamarok.com>e.org>                           *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/
 
#ifndef AMAZONURLRUNNER_H
#define AMAZONURLRUNNER_H

#include "amarokurls/AmarokUrlRunnerBase.h"

#include <KIcon>

#include <QObject>

/**
        @author Nikolaj Hald Nielsen <nhn@kde.org>
        @author Sven Krohlas <sven@getamarok.com>
*/
class AmazonUrlRunner : public QObject, public AmarokUrlRunnerBase
{
    Q_OBJECT
public:
    AmazonUrlRunner();

    virtual ~AmazonUrlRunner();

    virtual QString command() const;
    virtual QString prettyCommand() const;
    virtual KIcon icon() const;
    virtual bool run( AmarokUrl url );

signals:
    void search( const QString &request );
};

#endif // AMAZONURLRUNNER_H
