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
	
	<title>Clément Profit - Avancement des connaissances en Game Programming</title>
	
	<link rel="icon" href="manette.ico" type="image/x-icon">

	<link rel="stylesheet" href="style.css">
	
	<script type="text/javascript" src="syntaxhighlighter/scripts/shCore.js"></script>
	<script type="text/javascript" src="syntaxhighlighter/scripts/shBrushCpp.js"></script>
		
	<link type="text/css" rel="stylesheet" href="syntaxhighlighter/styles/shCoreDefault.css"/>
	

	<style type="text/css">
		.syntaxhighlighter 
		{
			
			max-width: 51.0vw;
			max-height: 400px;
			overflow-y: auto !important;
			overflow-x: auto !important;

			z-index: 1;
		}
	</style>
	
	<script type="text/javascript" src="dev-blog-cp.js"></script> 
	
	<script type="text/javascript">SyntaxHighlighter.all();</script>
		
</head>


<body>	


<header>

<?php

include "panel.php";

?>

<br />
	<img class="logo" alt="C++" src="planet_c++.png" />

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
			
			$articles_id_values = array();
			
			$data = $mysql->Query("select * from dev_blog_cp_articles order by date desc;");
			$first = true;
			
			foreach ($data as $value)
			{			
				if (!$first)
				{
					echo "<hr />";
					echo "<br />";
				}
				else
				{
					$first = false;
				}
				

				$article_id = htmlentities($value['id']);	
			
				$article_titre = utf8_decode($value['title']);
				$article_type = $value['type'];
				$article_contenu = utf8_decode($value['content']);
				
				$type_info = $mysql->Query("select * from dev_blog_cp_types where numero = $article_type;");			
				
				$type_nom = $type_info[0]['nom'];
				$type_image_url = $type_info[0]['image_url'];

				if (empty($type_nom))
				{
					$type_nom = "Catégorie inconnue";
				}
				
				if (empty($type_image_url))
				{
					$type_image_url = "empty.png";					
				}
				
				echo "<img class=\"idea\" src=\"$type_image_url\" />&nbsp;
				<span class=\"grand_titre\"><b>$article_titre</b></span>
				<span class=\"petit_titre\"> <b>($type_nom)</b></span><br />";

				echo "<br />";
				
				$article_sources_count = 0;
				$article_contenu = RemplacerCodeBalise($article_contenu, $article_id, 
													   &$article_sources_count);
							
				// Gestion du lexique
				$lexique_data = $mysql->Query("select * from dev_blog_cp_lexique;");			
				
				foreach ($lexique_data as $entry) 
				{
					$id = $entry['id'];		
					$mot = $entry['nom'];
					
					$regex = "#<not_parsed[^>]*>[^<]*</not_parsed> | $mot #";
					
					$regex_function = 
						function ($matches)
						{			
							global $id;
							$mot = $matches[0];

							$ancre = "ancre_" . $id;
							
							$chaîne_à_insérer = 
								"<a class=\"lexique_entry\" href=\"lexique.php?id=$id#$ancre\">
									<span class=\"orange\">$mot
								</span></a>";

							return $chaîne_à_insérer;	
						};
								
					$article_contenu = preg_replace_callback($regex, $regex_function, $article_contenu);
				}

				for ($i = 0; $i < $article_sources_count; $i++)
				{
					array_push($articles_id_values, $article_id . "_" . $i);
				}
				
				echo "
					<div class=\"mur_interne\">
						<div class=\"presentation\">
							$article_contenu
							<br />
						</div>
					</div>";
			}
		
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

<script type="text/javascript">

	<?php 
		echo "var articles_id_values = [";
		
		for ($i = 0; $i < count($articles_id_values); $i++)
		{
			echo "\"$articles_id_values[$i]\"";
			
			if (($i + 1) != count($articles_id_values))
			{
				echo ", ";
			}
		}
		
		echo "];\n";
	?>

	// On précharge toutes les saisies 
	window.onload = function()
	{
		CalculerHeure('horloge');
		
		HideAllCodes(articles_id_values);
		
		HideReadCode();
	}
	
</script>	
</body>


</html>