<?php
	$UID = "NO_UID";
	$date = date("c");
	$status = file_put_contents("UIDfile.txt", $UID . "||" . $date . "\r\n" , FILE_APPEND);
	if ($status){
		echo "successful write";
	}
	else {
		echo "failed write";
	}
	include_once 'generate_json.php';
?>