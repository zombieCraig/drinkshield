<?php
// Simple Leaderboard PHP app for the DrinkDuino game
// (c) 2010 Craig Smith <craig@gfxhax.com>  GPL

// At this time (9/13/10) An 'event' manager has not been
// written so we will have to hard code them up.

$event="Ohio Linux Fest";

$dbname = "drinkScorez";

?>
<html xmlns="http://www.w3.org/1999/xhtml" dir="ltr" lang="en-US" xml:lang="en-gb">
  <head>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8" />
    <meta http-equiv="content-language" content="en-US" /> 
    <meta http-equiv="content-style-type" content="text/css" />
    <title>
	<?php echo $event; ?> - DrinkDuino Leaderboard
    </title>
    <link rel="stylesheet" href="./leaderboard.css" type="test/css">
  </head>
  <body bgcolor=white>
	<center>
	<h1 id=event-title><?php echo $event; ?></h1>
	<h1 id=title>DrinkDuino Leaderboards</h1>
	</center>

<?php
$db = NULL;
if(file_exists($dbname)) {
	$db = new PDO("sqlite:".$dbname);
} else {
	echo "<FONT color=red>Warning. DB ($dbname) not found</FONT><BR>";
}
?>
	<table id=score-table>
	<tr id=table-title>
	   <td id=rank-title>Rank</td>
	   <td id=player-title>Player</td>
	   <td id=score-title>Top Scores</td>
	   <td id=attempt-title>Attempts</td>
	</tr>
<?php
if($db) {
	$attempts = 0;
	$rank = 1;
	$namesql = "SELECT p.id,name FROM players as p,scores as s WHERE p.id=s.id ORDER BY score DESC";
	foreach($db->query($namesql) as $entry) {
		if(!$seen[$entry['name']]) {
			$seen[$entry['name']] = 1;
			echo "<TR id=row><TD id=rank>";
			echo $rank."</TD><TD id=player>\n";
			echo "<IMG id=player-pic SRC='pics/".$entry['name']."_thumb.jpg'>";
			echo $entry['name']."</TD>\n";
			echo "<TD id=scores>";
			$scoresql = "SELECT score FROM scores WHERE id=".$entry['id']." ORDER BY score DESC";
			$attempts = 0;
			foreach($db->query($scoresql) as $sc) {
				echo $sc['score']. "<BR>\n";
				$attempts += 1;
			}
			echo "</TD><TD id=attempts>$attempts</TD></TR>\n";
			$rank += 1;
		}
	}
}
?>
	</table>
  </body>
</html>
