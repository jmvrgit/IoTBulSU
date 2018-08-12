<?php
// required headers
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");
 
// include database and object files
include_once 'database.php';
include_once 'appliance.php';
 
// instantiate database and product object
$database = new Database();
$db = $database->getConnection();
 
// initialize object
$appliance = new Appliance($db);
 
// get keywords
$keywords=isset($_GET["uid"]) ? $_GET["uid"] : "";
 
// query products
$stmt = $appliance->search($keywords);
$num = $stmt->rowCount();
 
// check if more than 0 record found
if($num>0){
 
   
 
    // retrieve our table contents
    // fetch() is faster than fetchAll()
    // http://stackoverflow.com/questions/2770630/pdofetchall-vs-pdofetch-in-a-loop
    while ($row = $stmt->fetch(PDO::FETCH_ASSOC)){
        // extract row
        // this will make $row['name'] to
        // just $name only
        extract($row);
 
        $appliance_arr=array(
            "uid" => $uid,
			"appl_name" => $appl_name,
			"has_power" => $has_power,
			"has_power_limit" => $has_power_limit,
			"has_time_limit" => $has_time_limit,
			"current_date_time" => $current_date_time,
			"time_limit_value" => $time_limit_value,
			"power_limit_value" => $power_limit_value,
			"current_power_usage" => $current_power_usage,
			"avg_watthr" => $avg_watthr,
			"estimated_cost" => $estimated_cost
        );
 

    }
 
    $json_data = json_encode($appliance_arr, JSON_PRETTY_PRINT);
	echo $json_data;
	
	//create json file
	file_put_contents('appliance_data.json', $json_data);
}
 
else{
	$json_data = json_encode(array("message" => "No appliance found."), JSON_PRETTY_PRINT);
	echo $json_data;
	
	//create json file
	file_put_contents('appliance_data.json', $json_data);
}
?>
