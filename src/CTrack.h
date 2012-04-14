#ifndef CURRENT_TRACK1
#define CURRENT_TRACK1

#include "core/meta/Meta.h"
#include "EngineController.h"
#include <QDebug>
#include <QObject>
#include <QStringList>
#include <QAbstractListModel>

class CTrack : public QAbstractListModel
{
	Q_OBJECT
	public:
		enum CurrentTrackRoles {
			TrackRole = Qt::UserRole + 1,
			ArtistRole
		};
		CTrack( QString trackName,QString artistName );
		int rowCount( const QModelIndex &parent = QModelIndex() ) const;
		QVariant data( const QModelIndex &parent,int role=Qt::DisplayRole ) const;
	private:
		QStringList trackDetails;
};

#endif
