// (c) 2004 Mark Kretschmann <markey@web.de>
// See COPYING file for licensing information.

#include "metabundle.h"
#include "playlistitem.h"
#include "tagdialog.h"

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tstring.h>

#include <qfile.h>
#include <qpushbutton.h>

#include <kcombobox.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klineedit.h>
#include <kmessagebox.h>
#include <knuminput.h>


TagDialog::TagDialog( const KURL& url, QWidget* parent )
    : TagDialogBase( parent )
    , m_metaBundle( MetaBundle( url ) )
    , m_playlistItem( 0 )
{
    init();
}


TagDialog::TagDialog( const MetaBundle& mb, PlaylistItem* item, QWidget* parent )
    : TagDialogBase( parent )
    , m_metaBundle( mb )
    , m_playlistItem( item )
{
    init();
}


////////////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS
////////////////////////////////////////////////////////////////////////////////

void
TagDialog::okPressed() //SLOT
{
    kdDebug() << k_funcinfo << endl;
    
    if ( hasChanged() ) {
        writeTag();
        if ( m_playlistItem ) syncItemText();
    }    
       
    deleteLater();
}


void
TagDialog::checkModified() //SLOT
{
    pushButton_ok->setEnabled( hasChanged() );
}


void
TagDialog::musicbrainzQuery() //SLOT
{
#ifdef HAVE_MUSICBRAINZ
    kdDebug() << k_funcinfo << endl;
    
    MusicBrainzQuery* query = new MusicBrainzQuery( MusicBrainzQuery::File, m_metaBundle.url().path() );
    connect( query, SIGNAL( signalDone( const MusicBrainzQuery::TrackList& ) ),
              this,   SLOT( queryDone( const MusicBrainzQuery::TrackList& ) ) );
    
    if ( !query->start() ) return;

    pushButton_musicbrainz->setEnabled( false );
    pushButton_musicbrainz->setText( i18n( "Working.." ) );
#endif
}


void
TagDialog::queryDone( const MusicBrainzQuery::TrackList& tracklist ) //SLOT
{
#ifdef HAVE_MUSICBRAINZ
    kdDebug() << k_funcinfo << endl;
    
    pushButton_musicbrainz->setEnabled( true );
    pushButton_musicbrainz->setText( m_buttonMbText );

    if ( tracklist.isEmpty() ) {
        KMessageBox::sorry( this, i18n( "Track was not found in MusicBrainz database." ) );
        return;
    }
                
    if ( !tracklist[0].name.isEmpty() )     kLineEdit_title->setText( tracklist[0].name );
    if ( !tracklist[0].artist.isEmpty() )   kLineEdit_artist->setText( tracklist[0].artist );
    if ( !tracklist[0].album.isEmpty() )    kLineEdit_album->setText( tracklist[0].album );
#endif
}


////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////

void
TagDialog::init()
{
    setWFlags( getWFlags() | Qt::WDestructiveClose );
    
    kLineEdit_title->setText( m_metaBundle.title() );
    kLineEdit_artist->setText( m_metaBundle.artist() );
    kLineEdit_album->setText( m_metaBundle.album() );
    kComboBox_genre->insertStringList( MetaBundle::genreList() );
    kComboBox_genre->setCurrentText( m_metaBundle.genre() );
    kIntSpinBox_track->setValue( m_metaBundle.track().toInt() );
    kIntSpinBox_year->setValue( m_metaBundle.year().toInt() );
    kLineEdit_comment->setText( m_metaBundle.comment() );
    kLineEdit_length->setText( m_metaBundle.prettyLength() );
    kLineEdit_bitrate->setText( m_metaBundle.prettyBitrate() );
    kLineEdit_samplerate->setText( m_metaBundle.prettySampleRate() );
    kLineEdit_location->setText( m_metaBundle.url().isLocalFile() ? m_metaBundle.url().path() : m_metaBundle.url().url() );

    // Connects for modification check
    connect( kLineEdit_title, SIGNAL( textChanged( const QString& ) ), this, SLOT( checkModified() ) );
    connect( kLineEdit_artist, SIGNAL( textChanged( const QString& ) ), this, SLOT( checkModified() ) );
    connect( kLineEdit_album, SIGNAL( textChanged( const QString& ) ), this, SLOT( checkModified() ) );
    connect( kComboBox_genre, SIGNAL( activated( int ) ), this, SLOT( checkModified() ) );
    connect( kComboBox_genre, SIGNAL( textChanged( const QString& ) ), this, SLOT( checkModified() ) );
    connect( kIntSpinBox_track, SIGNAL( valueChanged( int ) ), this, SLOT( checkModified() ) );
    connect( kIntSpinBox_year, SIGNAL( valueChanged( int ) ), this, SLOT( checkModified() ) );
    connect( kLineEdit_comment, SIGNAL( textChanged( const QString& ) ), this, SLOT( checkModified() ) );
    
    // Remember original button text
    m_buttonMbText = pushButton_musicbrainz->text();
    
    connect( pushButton_cancel, SIGNAL( clicked() ), this, SLOT( deleteLater() ) );
    connect( pushButton_ok, SIGNAL( clicked() ), this, SLOT( okPressed() ) );
    pushButton_ok->setEnabled( false );
    
#ifdef HAVE_MUSICBRAINZ
    connect( pushButton_musicbrainz, SIGNAL( clicked() ), this, SLOT( musicbrainzQuery() ) );
#else
    pushButton_musicbrainz->setEnabled( false );
#endif
    
    adjustSize();
}


bool
TagDialog::hasChanged()
{    
    bool modified = false;
    
    modified |= kLineEdit_title->text()        != m_metaBundle.title();
    modified |= kLineEdit_artist->text()       != m_metaBundle.artist();
    modified |= kLineEdit_album->text()        != m_metaBundle.album();
    modified |= kComboBox_genre->currentText() != m_metaBundle.genre();
    modified |= kIntSpinBox_track->value()     != m_metaBundle.track().toInt();
    modified |= kIntSpinBox_year->value()      != m_metaBundle.year().toInt();
    modified |= kLineEdit_comment->text()      != m_metaBundle.comment();

    return modified;
}    


void
TagDialog::writeTag()
{    
    TagLib::FileRef f( QFile::encodeName( m_metaBundle.url().path() ), false );
    
    if ( !f.isNull() ) {
        TagLib::Tag * t = f.tag();
        t->setTitle( QStringToTString( kLineEdit_title->text() ) );
        t->setArtist( QStringToTString( kLineEdit_artist->text() ) );
        t->setAlbum( QStringToTString( kLineEdit_album->text() ) );
        t->setTrack( kIntSpinBox_track->value() );
        t->setYear( kIntSpinBox_year->value() );
        t->setComment( QStringToTString( kLineEdit_comment->text() ) );
        t->setGenre( QStringToTString( kComboBox_genre->currentText() ) );
        
        f.save();
    }
}        


void
TagDialog::syncItemText()
{    
    QListViewItem* item = m_playlistItem->listView()->firstChild();
    
    // Find out if item still exists in the listView
    do {
        if ( item == m_playlistItem ) break;
        item = item->nextSibling();
    }
    while( item );
    
    if ( item ) {
        // Reflect changes in PlaylistItem text
        m_playlistItem->setText( PlaylistItem::Title, kLineEdit_title->text() );
        m_playlistItem->setText( PlaylistItem::Artist, kLineEdit_artist->text() );
        m_playlistItem->setText( PlaylistItem::Album, kLineEdit_album->text() );
        m_playlistItem->setText( PlaylistItem::Genre, kComboBox_genre->currentText() );
        m_playlistItem->setText( PlaylistItem::Album, kIntSpinBox_track->text() );
        m_playlistItem->setText( PlaylistItem::Year, kIntSpinBox_year->text() );
        m_playlistItem->setText( PlaylistItem::Comment, kLineEdit_comment->text() );
    }
}


#include "tagdialog.moc"
