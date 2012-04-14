#include "CTrack.h"

CTrack::CTrack()
{
	QHash< int , QByteArray > roles;
	roles[ ArtistRole ]="artist";
	roles[ TrackRole ]="track";
	setRoleNames( roles );
}

CTrack::CTrack( QString trackName,QString artistName )
{
}

int CTrack::rowCount( const QModelIndex &parent ) const
{
	return trackDetails.count();
}

QVariant CTrack::data( const QModelIndex &parent,int role ) const
{
	if( role == TrackRole )
	{
		qDebug()<<"Track Role Called";
		return trackDetails[ 0 ];
	}
	else if( role == ArtistRole )
	{
		qDebug()<<"Artist Role Called";
			return trackDetails[ 1 ];
	}
	return QVariant();
}

void
CTrack::setDetails( QString trackName, QString artistName )
{
	qDebug()<<"Set Details Called\n";
	if( trackDetails.count()==0 )
	{
		qDebug()<<"Currently Empty";
		trackDetails<<trackName;
		trackDetails<<artistName;
	}
	else
	{
		qDebug()<<"Now Updating values";
		trackDetails[ 0 ] = trackName;
		trackDetails[ 1 ] = artistName;
	}
}
