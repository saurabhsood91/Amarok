/****************************************************************************************
 * Copyright (c) 2009 Alejandro Wainzinger <aikawarazuni@gmail.com>                     *
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

#ifndef UMSCOLLECTION_H
#define UMSCOLLECTION_H

#include <core/collections/Collection.h>
#include <core-impl/collections/support/MemoryCollection.h>
#include <utilities/collectionscanner/Directory.h>

#include <KDirWatch>
#include <KIcon>

#include <solid/device.h>

#include <QtGlobal>
#include <QTimer>

using namespace Collections;

class GenericScanManager;

class UmsPodcastProvider;

class UmsCollection;

class UmsCollectionLocation;

class UmsCollectionFactory : public CollectionFactory
{
    Q_OBJECT

    public:
        UmsCollectionFactory( QObject *parent, const QVariantList &args );
        virtual ~UmsCollectionFactory();

        virtual void init();

    private slots:
        /**
         * Called when solid notifier detects a new device has been added
         */
        void slotAddSolidDevice( const QString &udi );

        /**
         * Called when solid StorageAccess device we are interested in is mounted or
         * unmounted
         */
        void slotAccessibilityChanged( bool accessible, const QString &udi );

        /**
         * Called when solid notifier detects a device has been removed
         */
        void slotRemoveSolidDevice( const QString &udi );

        /**
         * Like @see slotRemoveSolidDevice(), but instructs Collection to eject the
         * device after it has performed necessary teardown operations.
         *
         * Called when user wants to unmount the device from for example Device Notifier
         */
        void slotRemoveAndTeardownSolidDevice( const QString &udi );

        /**
         * Called when "tracked" collection is destroyed
         */
        void slotCollectionDestroyed( QObject *collection );

    private:
        /**
         * Checks whether a solid device is a USB mass-storage one
         */
        bool identifySolidDevice( const QString &udi ) const;

        /**
         * Attempts to create appropriate collection for already identified solid device
         * @param udi. Should emit newCollection() if the collection was successfully
         * created and should become visible to the user.
         */
        void createCollectionForSolidDevice( const QString &udi );

        // maps device udi to active UMS collections
        QMap<QString, UmsCollection *> m_collectionMap;
};

class UmsCollection : public Collection, public Meta::Observer
{
    Q_OBJECT

    public:
        // inherited methods

        UmsCollection( Solid::Device device );
        virtual ~UmsCollection();

        /* TrackProvider methods */
        virtual bool possiblyContainsTrack( const KUrl &url ) const;
        virtual Meta::TrackPtr trackForUrl( const KUrl &url );

        /* Collection methods */
        virtual QueryMaker *queryMaker();
        virtual bool isDirInCollection( const QString &path );
        virtual QString uidUrlProtocol() const;

        virtual QString collectionId() const { return m_device.udi(); }
        virtual QString prettyName() const;
        virtual KIcon icon() const;

        virtual bool hasCapacity() const;
        virtual float usedCapacity() const;
        virtual float totalCapacity() const;

        virtual CollectionLocation *location();

        virtual bool isWritable() const;
        virtual bool isOrganizable() const;

        /* Capability-related methods */
        virtual bool hasCapabilityInterface( Capabilities::Capability::Type type ) const;
        virtual Capabilities::Capability *createCapabilityInterface(
                Capabilities::Capability::Type type );

        /* Meta::Observer methods */
        virtual void metadataChanged( Meta::TrackPtr track );
        using Meta::Observer::metadataChanged; // silence compiler warning about hidder overloads

        /* own methods */
        const KUrl &musicPath() const { return m_musicPath; }
        const KUrl &podcastPath() const { return m_podcastPath; }

        KUrl organizedUrl( Meta::TrackPtr track ) const;

        QSharedPointer<MemoryCollection> memoryCollection() const { return m_mc; }

    signals:
        /**
         * Start a count-down that emits updated() signal after it expires.
         * Resets the timer to original timeout if already running. This is to ensure
         * that we emit update() max. once per <timeout> for batch updates.
         *
         * Timers can only be started from "their" thread so use signals & slots for that.
         */
        void startUpdateTimer();

    public slots:
        /**
         * Destroy the collection (by emitting remove)
         */
        void slotDestroy();

        /**
         * Destroy the collection and try to eject the device from system
         */
        void slotEject();

        void slotTrackAdded( KUrl trackLocation );
        void slotTrackRemoved( const Meta::TrackPtr &track );

    private slots:
        void slotParseTracks();
        void slotParseActionTriggered();
        void slotConfigure();

        void slotDirectoryScanned( CollectionScanner::Directory *dir );

        /**
         * Starts a timer that emits updated() signal after 2 seconds.
         */
        void slotStartUpdateTimer();

    private:
        /** extended constructor */
        void init();

        //static variables relating to the on-disk configuration file
        static QString s_settingsFileName;
        static QString s_musicFolderKey;
        static QString s_musicFilenameSchemeKey;
        static QString s_vfatSafeKey;
        static QString s_asciiOnlyKey;
        static QString s_ignoreTheKey;
        static QString s_replaceSpacesKey;
        static QString s_regexTextKey;
        static QString s_replaceTextKey;
        static QString s_podcastFolderKey;
        static QString s_autoConnectKey;
        static QString s_collectionName;

        Solid::Device m_device;
        QSharedPointer<MemoryCollection> m_mc;
        bool m_tracksParsed;

        bool m_autoConnect;
        QString m_mountPoint;
        KUrl m_musicPath;
        KUrl m_podcastPath;
        QString m_musicFilenameScheme;
        bool m_vfatSafe;
        bool m_asciiOnly;
        bool m_ignoreThe;
        bool m_replaceSpaces;
        QString m_regexText;
        QString m_replaceText;
        QString m_collectionName;

        GenericScanManager *m_scanManager;
        KDirWatch m_watcher;

        QStringList m_supportedMimeTypes;

        UmsPodcastProvider *m_podcastProvider;

        QAction *m_parseAction;
        QAction *m_configureAction;
        QAction *m_ejectAction;
        QTimer m_updateTimer;
};

#endif
