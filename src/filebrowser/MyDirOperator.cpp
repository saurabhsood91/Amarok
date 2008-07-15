/*
    Copyright (c) 2008 Dan Meltzer <hydrogen@notyetimplemented.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MyDirOperator.h"
#include "MainWindow.h"

#include <KActionCollection>
#include <kurl.h>

#include <QDir>

MyDirOperator::MyDirOperator ( const KUrl &url, QWidget *parent )
    : KDirOperator( url, parent )
{
     MyDirLister* dirlister = new MyDirLister( true );
     dirlister->setMainWindow( The::mainWindow() );
     setDirLister( dirlister );
}

#include "MyDirOperator.moc"
