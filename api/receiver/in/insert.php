<?php

header("Access-Control-Allow-Origi: *");
header("Content-Type: application/json; charset=UTF-8");

$response = array();

if (isset($_GET['drylevel']) && isset($_GET['wetlevel'])) {
    $drylevel = $_GET['drylevel'];
    $wetlevel = $_GET['wetlevel'];

    $filepath = realpath (dirname(__FILE__));
    require_once($filepath."/db.connect.php");

    $db = new DB_CONNECT();

    $result = mysql_query("INSERT INTO receiver(drylevel,wetlevel) VALUES('$drylevel','$wetlevel')");

    if ($result) {
        $response["success"] = 1;
        $response["message"] = "Values uploaded successfully";
    } else {
        $response["success"] = 0;
        $response["message"] = "Something went wrong";
    }

    echo json_encode($response);
} else {
    $response["success"] - 0;
    $response["message"] = "Parameters are missing";

    echo json_encode($response);
}
?>