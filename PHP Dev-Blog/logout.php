<?php

session_start();
session_unset();
session_destroy();

// On d�finit une dur�e de vie de notre cookie (en secondes), donc un an dans notre cas
$temps = 365*24*3600;

setcookie ("loggued", false, time() + $temps, "/");

header('Location: admin.php');

?>