#include "CurrentTrack.h"

CurrentTrackModel::CurrentTrackModel( QObject *parent )
{
}

CurrentTrackModel::CurrentTrackModel( QString trackName, QString artistName, QObject *parent )
{
	m_TrackName = trackName;
	m_ArtistName = artistName;
}

void
CurrentTrackModel::setTrackName( QString trackName )
{
	m_TrackName = trackName;
}

void
CurrentTrackModel::setArtistName( QString artistName )
{
	m_ArtistName = artistName;
}

QString
CurrentTrackModel::getArtistName()
{
	return m_ArtistName;
}

QString
CurrentTrackModel::getTrackName()
{
	return m_TrackName;
}
