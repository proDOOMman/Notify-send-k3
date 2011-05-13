#!/bin/sh
MPLAYER=/mnt/us/fbKindle/mplayer/mplayer # mplayer binary (or any other player)
USER=username # GMail account nickname
PASS=password # GMail account password
SOUND1=/mnt/us/KDE-Im-Phone-Ring.ogg # New mail sound
SOUND2=/mnt/us/KDE-Sys-App-Message.ogg # No new mail sound
COUNT=`curl -su $USER:$PASS https://mail.google.com/mail/feed/atom || echo "<fullcount>0</fullcount>"`
COUNT=`echo "$COUNT" | grep "<fullcount>" | sed -e s/"<fullcount>"// -e s/"<\/fullcount>"//`
echo $COUNT
if [ "$COUNT" != "0" ]; then
if [ "$COUNT" = "1" ];then
WORD="mail";
else
WORD="mails";
fi
$MPLAYER "$SOUND1" & /mnt/us/notify/notify-send "There is $COUNT new $WORD";
else
$MPLAYER "$SOUND2" & /mnt/us/notify/notify-send "No new mails =(";
fi
