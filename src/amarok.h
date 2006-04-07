//See COPYING file for licensing information

#ifndef AMAROK_H
#define AMAROK_H

#include <qnamespace.h>
#include <qstring.h>

#include <kurl.h> // recursiveUrlExpand

#include "amarok_export.h"

class KActionCollection;
class KConfig;
class QColor;
class QDateTime;
class QEvent;
class QPixmap;
class QWidget;
class DynamicMode;

namespace amaroK
{
    const int VOLUME_MAX = 100;
    const int SCOPE_SIZE = 9; //= 2**9 = 512
    const int blue       = 0x202050;
    const int VOLUME_SENSITIVITY = 30; //for mouse wheels
    const int GUI_THREAD_ID = 0;

    namespace ColorScheme
    {
        ///eg. base of the amaroK Player-window
        extern QColor Base; //amaroK::blue
        ///eg. text in the amaroK Player-window
        extern QColor Text; //Qt::white
        ///eg. background colour for amaroK::PrettySliders
        extern QColor Background; //brighter blue
        ///eg. outline of slider widgets in Player-window
        extern QColor Foreground; //lighter blue
        ///eg. KListView alternative row color
        extern QColor AltBase; //grey toned base
    }

    /** The version of the playlist XML format. Increase whenever it changes backwards-incompatibly. */
    inline QString xmlVersion() { return "2.4"; }

    /**
     * Convenience function to return the KApplication instance KConfig object
     * pre-set to a specific group.
     * @param group Will pre-set the KConfig object to this group.
     */
    KConfig *config( const QString &group = "General" ); //defined in app.cpp

    /**
     * @return the KActionCollection used by amaroK
     * The KActionCollection is owned by the PlaylistWindow, so you must ensure
     * you don't try to use this before then, but we've taken steps to prevent
     * this eventuality - you should be safe.
     */
    KActionCollection *actionCollection(); //defined in app.cpp

    /**
     * An event handler that handles events in a generic amaroK fashion. Mainly
     * useful for drops, ie offers the amaroK popup for adding tracks to the
     * playlist. You shouldn't pass every event here, ie closeEvents will not be
     * handled as expected! Check the source in app.cpp if you want to see what
     * it can do.
     * @param recipient The object that recieved the event.
     * @param e The event you want handled in a generic fashion.
     * @return true if the event was handled.
     */
    bool genericEventHandler( QWidget *recipient, QEvent *e ); //defined in app.cpp

    /**
     * Invoke the external web browser set in amaroK's configuration.
     * @param url The URL to be opened in the browser.
     * @return True if the browser could be started.
     */
    bool invokeBrowser( const QString& url ); //defined in app.cpp

    /**
     * Obtain an amaroK PNG image as a QPixmap
     */
    QPixmap getPNG( const QString& /*fileName*/ ); //defined in app.cpp

    /**
     * Obtain an amaroK JPG image as a QPixmap
     */
    QPixmap getJPG( const QString& /*fileName*/ ); //defined in app.cpp

    /**
     * The mainWindow is the playlistWindow or the playerWindow depending on
     * the configuration of amaroK
     */
    QWidget *mainWindow(); //defined in app.cpp

    /**
     * Allocate one on the stack, and it'll set the busy cursor for you until it
     * is destroyed
     */
    class OverrideCursor { //defined in app.cpp
    public:
        OverrideCursor( Qt::CursorShape cursor = Qt::WaitCursor );
       ~OverrideCursor();
    };

    /**
     * For saving files to ~/.kde/share/apps/amarok/directory
     * @param directory will be created if not existing, you MUST end the string
     *                  with '/'
     */
    LIBAMAROK_EXPORT QString saveLocation( const QString &directory = QString::null ); //defined in collectionreader.cpp

    /**
     * For recursively expanding the contents of a directory into a KURL::List
     * (playlists are ignored)
     */
    LIBAMAROK_EXPORT KURL::List recursiveUrlExpand( const KURL &url ); //defined in playlistloader.cpp

    QString verboseTimeSince( const QDateTime &datetime ); //defined in contextbrowser.cpp

    QString verboseTimeSince( uint time_t ); //defined in contextbrowser.cpp

    /**
     * @return the LOWERCASE file extension without the preceding '.', or "" if there is none
     */
    inline QString extension( const QString &fileName )
    {
        return fileName.contains( '.' ) ? fileName.mid( fileName.findRev( '.' ) + 1 ).lower() : "";
    }

    /**
     * @return the last directory in @param fileName
     */
    inline QString directory( const QString &fileName )
    {
        return fileName.section( '/', 0, -2 );
    }
  /** Due to xine-lib, we have to make KProcess close all fds, otherwise we get "device is busy" messages
  * Used by AmaroKProcIO and AmaroKProcess, exploiting commSetupDoneC(), a virtual method that
  * happens to be called in the forked process
  * See bug #103750 for more information.
  */
  //TODO ugly hack, fix KProcess for KDE 4.0
    void closeOpenFiles(int out, int in, int err); //defined in scriptmanager.cpp

    /**
    * Returns internal code for database type, DbConnection::sqlite, DbConnection::mysql, or DbConnection::postgresql
    * @param type either "SQLite", "MySQL", or "Postgresql".
    */
    int databaseTypeCode( const QString type ); //defined in configdialog.cpp

    bool repeatNone(); //defined in actionclasses.cpp
    bool repeatTrack();
    bool repeatAlbum();
    bool repeatPlaylist();
    bool favorNone();
    bool favorScores();
    bool favorRatings();
    bool favorLastPlay();

    const DynamicMode *dynamicMode(); //defined in playlist.cpp

    /**
     * Maps the icon name to a system icon or custom amaroK icon, depending on the settings.
     */
    QString icon( const QString& name ); //defined in iconloader.cpp
}


/**
 * Use this to const-iterate over QStringLists, if you like.
 * Watch out for the definition of last in the scope of your for.
 *
 *     QStringList strings;
 *     foreach( strings )
 *         debug() << *it << endl;
 */
#define foreach( x ) \
    for( QStringList::ConstIterator it = x.begin(), end = x.end(); it != end; ++it )

/**
 * You can use this for lists that aren't QStringLists.
 * Watch out for the definition of last in the scope of your for.
 *
 *     BundleList bundles;
 *     foreachType( BundleList, bundles )
 *         debug() << *it.url() << endl;
 */
#define foreachType( Type, x ) \
    for( Type::ConstIterator it = x.begin(), end = x.end(); it != end; ++it )

/**
 * Creates iterators of type @p Type.
 * Watch out for the definitions of last and end in your scope.
 *
 *     BundleList bundles;
 *     for( for_iterators( BundleList, bundles ); it != end; ++it )
 *         debug() << *it.url() << endl;
 */
#define for_iterators( Type, x ) \
    Type::ConstIterator it = x.begin(), end = x.end(), last = x.fromLast()


/// Update this when necessary
#define APP_VERSION "1.4-SVN"

#endif
