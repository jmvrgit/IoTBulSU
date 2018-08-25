<html>
	<body>
		<?php
		$UID = $_GET["UID"];
		$date = date("c");
		$status = file_put_contents("UIDfile.txt", $UID . "||" . $date . "\r\n" , FILE_APPEND);
		if ($status){
			echo "successful write";
		}
		else {
			echo "failed write";
		}
		?>
	<body>
</html>