#include "CTrack.h"

CTrack::CTrack( QString trackName,QString artistName )
{
	trackDetails<<trackName;
	trackDetails<<artistName;
	QHash< int , QByteArray > roles;
	roles[ ArtistRole ]="artist";
	roles[ TrackRole ]="track";
	setRoleNames( roles );
}

int CTrack::rowCount( const QModelIndex &parent ) const
{
	return trackDetails.count();
}

QVariant CTrack::data( const QModelIndex &parent,int role ) const
{
	if( role == TrackRole )
	{
		return trackDetails[ 0 ];
	}
	else if( role == ArtistRole )
	{
			return trackDetails[ 1 ];
	}
	return QVariant();
}
