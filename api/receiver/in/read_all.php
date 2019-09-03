<?php

header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");

$response = array(); //Creating Array for JSON response
$filepath = realpath (dirname(__FILE__)); // Include data base connect class
require_once($filepath."/db_connect.php");
$db = new DB_CONNECT();	// Connecting to database 
$result = mysql_query("SELECT *FROM receiver") or die(mysql_error()); // Fire SQL query to get all data from led 

if (mysql_num_rows($result) > 0) { // Check for succesfull execution of query and no results found
    $response["receiver"] = array(); // Storing the returned array in response
    while ($row = mysql_fetch_array($result)) { // While loop to store all the returned response in variable
        $data = array(); // temperoary user array
        $data["drylevel"] = $row["drylevel"];
        $data["wetlevel"] = $row["wetlevel"];
        array_push($response["receiver"], $data); // Push all the items 
    }
    $response["success"] = 1; // On success
    echo json_encode($response); // Show JSON response
} else {
	$response["success"] = 0; // If no data is found
    $response["message"] = "No data on LED found";
    echo json_encode($response); // Show JSON response
}
?>