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
		
		
		$servername = "";
		$username = "";
		$password = "";
		$database = "";
		
		//Create msql connection
		$connect = new msqli($servername, $username, $password, $database) or die ("Connection Failed");
		
		function CloseConnection($conn){
			$conn -> close();
		}
		?>
	<body>
</html>