<?php
	include("mysql.class.php");
	require("common.php");
		
	$mysql = new MYSQL;
	$mysql->Connect();
?>

<!DOCTYPE html>

<!-- MAIN PAGE -->

<html>

<head>
	<meta charset="utf-8" />
	 
	<title>Clément Profit - Avancement des connaissances en Game Programming - Liste des articles</title>
	
	<link rel="icon" href="manette.ico" type="image/x-icon">

	
	<link rel="stylesheet" href="style.css">
	
	<script src="dev-blog-cp.js"></script> 

	
</head>


<body>	


<header>

<?php

include "panel.php";

?>


<br />
	<img class="logo" src="planet_c++.png" />
	<img class="programmation_image" alt="Programmation graphique" src="logo.png" />

	<br />
</header>
<br />
<div class="content">

<?php

	include "menu.php";
	
?>
	
	<div class="mur_externe">
	
		<div id="news" class="center">
		
<?php
	
	function PrintArticlesList()
	{
		global $mysql;
		$types = $mysql->Query("select numero from dev_blog_cp_types;");	
		$types_numeros = array();
		
		foreach ($types as $t)
		{			
			array_push($types_numeros, $t['numero']);
		}
			

		foreach ($types_numeros as $numero)
		{						
			$articles = $mysql->Query("select * from dev_blog_cp_articles where type = $numero order by date desc;");
			
			$type_info = $mysql->Query("select * from dev_blog_cp_types where numero = $numero;");

			echo "<img class=\"idea\" src=\"" . $type_info[0]['image_url'] . "\" />&nbsp;" . $type_info[0]['nom'] . " : <br />";
			
			foreach ($articles as $ar)
			{			
				$article_id = htmlentities($ar['id']);		
				$article_titre = utf8_decode($ar['title']);
				$article_date = htmlentities($ar['date']);
				
				echo "<img class=\"square\" src=\"square.png\" />";
				echo "<a href=\"article.php?id=$article_id\"><span class=\"lien_article\">". $article_titre . "</span>
				</a><br /> [" . $article_date . "]<br /><br />";
			}		
			
			echo "<br />";
		}
		
		
	}
	
	echo '
		<img class="idea" src="user.png" />
			
		&nbsp;<b><span class="grand_titre">Listes des articles</span></b>
			
		<br />
		
		<div class="mur_interne">
		<br />
		<u>Liste des articles (par catégorie)</u> :<br /><br />';
		
	PrintArticlesList();
	
	echo '<br /><br /></div><br />';
	
?>
		</div>
	
	</div>
</div>
<br />
<br />

<hr />

<br />
<br />


<?php

include "footer.php";

$mysql->Disconnect();

?>

<br />
</body>

</html>