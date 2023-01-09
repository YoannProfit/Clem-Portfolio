<?php

if (isset($_GET['file']))
{
	$source_code_file_url = $_GET['file'];

	$content = @file_get_contents($source_code_file_url);
	
	if ($content != false)
	{
		echo htmlentities(utf8_decode($content));
	}
	else
	{
		echo "Fichier inconnu !";
	}
}

?>