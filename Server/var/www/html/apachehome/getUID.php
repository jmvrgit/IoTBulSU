<html>
	<body>
		<?php
		$UID = $_GET["UID"];
		
		$status = file_put_contents("UIDfile.txt", $UID ."\r\n" , FILE_APPEND);
		if ($status){
			echo "successful write";
		}
		else {
			echo "failed write";
		}
		?>
	<body>
</html>