<html>
	<body>
		<?php
		$powerdata = $_GET["powerdata"];
		$AR_powerdata = explode( "||", $powerdata);
		$voltage =$AR_powerdata[0];
		$ampere =$AR_powerdata[1];
		$power =$AR_powerdata[2];
		$watthr =$AR_powerdata[3];
		$date = date("c");
		$status = file_put_contents("powerfile.txt", $powerdata . "||" . $date . "\r\n" , FILE_APPEND);
		if ($status){
			echo "successful";
		}
		else {
			echo "failed";
		}
		?>
	<body>
</html>