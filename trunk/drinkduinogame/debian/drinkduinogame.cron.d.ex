#
# Regular cron jobs for the drinkduinogame package
#
0 4	* * *	root	[ -x /usr/bin/drinkduinogame_maintenance ] && /usr/bin/drinkduinogame_maintenance
