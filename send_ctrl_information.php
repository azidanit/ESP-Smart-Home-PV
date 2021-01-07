<?php

/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
date_default_timezone_set("Asia/Jakarta");
echo date_default_timezone_get();
$datetime = date('Y-m-d H:i:s');

$servername = "localhost";

// REPLACE with your Database name
$dbname = "u901508721_esp_coba";
// REPLACE with Database user
$username = "u901508721_root";
// REPLACE with Database user password
$password = "azidanIT123123";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "zidan123123";

$api_key= $sensor = $location = $value1 = $value2 = $value3 = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    
    $api_key = ($_POST["api_key"]);
    var_dump($_POST);
    if($api_key == $api_key_value) {

        $curr1 = test_input($_POST["current1"]);
        $curr2 = test_input($_POST["current2"]);
        $voltage = test_input($_POST["voltage"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO Controller_Data (current1, current2, voltage, updated_at) VALUES ($curr1, $curr2, $voltage, '$datetime') ";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}