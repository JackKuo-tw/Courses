#!/usr/bin/php

<?php
	echo "Content-Type: text/plain\n\n";
	system("/usr/bin/printenv");
	if (isset($_GET["param"]))
		echo "set";
	else
		echo "not set";
	if (!empty($_GET["param"]))
		echo "not empty";
	else
		echo "empty";
	foreach ($_POST as $key => $value) {
	    echo "Field ".htmlspecialchars($key)." is ".htmlspecialchars($value)."<br>";
	}
	foreach ($_GET as $key => $value) {
	    echo "Field ".htmlspecialchars($key)." is ".htmlspecialchars($value)."<br>";
	}
	echo $_SERVER['REQUEST_METHOD'];
	echo $_SERVER['QUERY_STRING'];
?>
