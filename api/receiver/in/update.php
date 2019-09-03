<?php

header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");

$response = array(); //Creating Array for JSON response

if (isset($_GET['id']) && isset($_GET['status'])) { // Check if we got the field from the user
    $id = $_GET['id'];
    $status= $_GET['status'];
	$filepath = realpath (dirname(__FILE__)); // Include data base connect class
	require_once($filepath."/db_connect.php");
    $db = new DB_CONNECT(); // Connecting to database
    $result = mysql_query("UPDATE led SET status= '$status' WHERE id = '$id'"); // Fire SQL query to update LED status data by id
    
    if ($result) { // Check for succesfull execution of query and no results found
        
        $response["success"] = 1; // successfully updation of LED status (status)
        $response["message"] = "LED Status successfully updated.";
        echo json_encode($response); // Show JSON response
    } else {

    }
} else {
    $response["success"] = 0; // If required parameter is missing
    $response["message"] = "Parameter(s) are missing. Please check the request";
    echo json_encode($response); // Show JSON response
}
?>