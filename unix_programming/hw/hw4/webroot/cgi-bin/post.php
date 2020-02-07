#!/usr/bin/php

<?php
    if(isset($_GET['src'])) { highlight_file(__FILE__); die; };

    function printenv() {
        $env = getenv();
        $dummy = $_SERVER['ENV'];
        while(current($env) !== FALSE) {
            echo key($env) . "=" . current($env) . "\n";
            next($env);
        }
    }

    // disable all errors/warnings
    error_reporting(0);
    // disable gzip +1
    if(php_sapi_name() === 'cli') {
        echo "Content-Encoding: none\n";
        echo "Content-Type: text/plain\n";
        echo "\n";
    } else {
        header("Content-Encoding: none");
        header("Content-Type: text/plain");
    }
    $postdata = file_get_contents("php://input");
    // disable gzip +2
    ob_implicit_flush(true);
    ob_end_flush();
    //system("/usr/bin/printenv");
    printenv();
    echo "\n";
    echo $postdata;
?>

