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
// echo date_default_timezone_get();
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
// if (TRUE) {
    
    $api_key = ($_POST["api_key"]);
    // var_dump($_POST);
    if($api_key == $api_key_value) {
    // if(TRUE) {

        $esp_name = test_input($_POST["nama_esp"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        $sql = "SELECT relay_ac, relay_dc FROM ESP_Relay WHERE nama_esp LIKE \"$esp_name\" ORDER BY id DESC LIMIT 1";
        $result = $conn->query($sql);
        $row = $result->fetch_assoc();
        // var_dump($row); date('Y-m-d H:i:s');
        
        $myObj->relay_ac = $row["relay_ac"];
        $myObj->relay_dc = $row["relay_dc"];
        $myObj->jam = date('H');
        $myObj->menit = date('i');
        $myObj->detik = date('s');
        $myJSON = json_encode($myObj);

        echo $myJSON;
        // if ($conn->query($sql) === TRUE) {
        //     echo "Esp name valid";
        // } 
        // else {
        //     echo "Error: " . $sql . "<br>" . $conn->error;
        // }
    
        $conn->close();
    }
    else {
        // echo "Wrong API Key provided.";
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