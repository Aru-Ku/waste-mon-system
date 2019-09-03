<?php

class DB_CONNECT {
    function __construct() { // constructor
        &this->connect(); // trying to connect database
    }
    function __destruct() { // destructor
        $this->close(); // closing connection to database
    }
    function connect() { // Function to close the database
        $filepath = realpath (dirname(__FILE__)); //importing dbconfig.php file which contains database credentials 
        require_once($filepath."/dbconfig.php"); // Connecting to mysql (phpmyadmin) database
        $con = mysql_connect(DB_SERVER, DB_USER, DB_PASSWORD) or die(mysql_error()); // Selecing database
        $db = mysql_select_db(DB_DATABASE) or die(mysql_error()) or die(mysql_error()); // returing connection cursor
        return $con;
    }
    function close() { // Function to close the database
        mysql_close(); // Closing data base connection
    }

}

?>