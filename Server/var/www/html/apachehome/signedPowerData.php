<?php
	$UID = $_GET["UID"];
	$powerdata = $_GET["powerdata"];
	
	$date = date("c");
	
	$AR_powerdata = explode( "||", $powerdata);
	$voltage =$AR_powerdata[0];
	$ampere =$AR_powerdata[1];
	$power =$AR_powerdata[2];
	$watthr =$AR_powerdata[3];
	$date = date("c");
	$status = file_put_contents("signedpowerfile.txt",$UID. "||" . $powerdata . "||" . $date . "\r\n" , FILE_APPEND);
	if ($status){
	}
	else {
		echo "failed write\n";
	}
	include_once 'generate_json.php';
?>