#!/usr/bin/env ruby
#
# amaroK Script for custom scoring
#
# (c) 2006 Gábor Lehel <illissius@gmail.com>
#
# License: GNU General Public License V2

require 'uri'

loop do
    args = gets.chomp.split(" ")

    case args[0]
        when "configure"
            msg  = 'This script does not require any configuration.'
            `qdbus org.kde.amarok /PlayerList popupMessage "#{msg}"`

        when "requestNewScore"
            url = args[1]
            prevscore = args[2].to_f
            playcount = args[3].to_i
            length = args[4].to_i
            percentage = args[5].to_i
            reason = args[6]

            if( playcount <= 0 ) # not supposed to be less, but what the hell.
                newscore = ( prevscore + percentage ) / 2
            else
                newscore = ( ( prevscore * playcount ) + percentage ) / ( playcount + 1 )
            end

            system( "qdbus", "org.kde.amarok", "/Player", "setScoreByPath", URI::decode( url ), newscore.to_s )
    end
end

