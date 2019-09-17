<?php

header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");

$response = array();
$filepath = realpath(dirname(__FILE__));

require_once($filepath."/db.connect.php");
$db = new DB_CONNECT();
$result = mysql_query("SELECT *FROM receiver WHERE id=(SELECT MAX(id) FROM receiver)") or die(mysql_error());

if(mysql_num_rows($result) > 0) {
    $response["receiver"] = array();
    while($row = mysql_fetch_array($result)) {
        $data["id"] = $row["id"];
        $data["drylevel"] = $row["drylevel"];
        $data["wetlevel"] = $row["wetlevel"];
        array_push($response["receiver"], $data);
    }
    $response["success"] = 1; // On success
    echo json_encode($response); // Show JSON response
} else {
    $response["success"] = 0; // If no data is found
    $response["message"] = "No data on LED found";
    echo json_encode($response); // Show JSON response
}
?>