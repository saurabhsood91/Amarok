#ifndef CURRENT_TRACK
#define CURRENT_TRACK

#include <QDebug>
#include <QObject>

class CurrentTrackModel : public QObject{
	Q_OBJECT
	Q_PROPERTY( QString trackName READ getTrackName WRITE setTrackName NOTIFY trackChanged )
	Q_PROPERTY( QString artistName READ getArtistName WRITE setArtistName NOTIFY artistChanged )
	
	public:
		CurrentTrackModel( QObject *parent = 0 );
		CurrentTrackModel( QString trackName, QString artistName, QObject *parent = 0 );
		QString getArtistName();
		QString getTrackName();
		void setTrackName( QString trackName );
		void setArtistName( QString artistName );
	signals:
		void trackChanged();
		void artistChanged();
	private:
		QString m_TrackName;
		QString m_ArtistName;
};

#endif
