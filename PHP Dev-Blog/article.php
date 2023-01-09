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
	 
	<title>Clément Profit - Avancement des connaissances en Game Programming - Article</title>
	
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
			
	<script src="dev-blog-cp.js"></script> 

	<script type="text/javascript">SyntaxHighlighter.all();</script>
	
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

	function PrintNoArticleMessage()
	{
		echo '
			&nbsp;<b><span class="grand_titre">Aucun article sélectionné !</span></b>
				
			<br />
			
			<div class="mur_interne">
			<div class="presentation">	
			Choississez un article !<br />';

		echo '<br /></div></div>';	
	}
	
	if (isset($_GET['id']) && is_numeric($_GET['id']))
	{
		$article_id = $_GET['id'];
		
		$articles_id_values = array();
					
		$data = $mysql->Query("select * from dev_blog_cp_articles where id = $article_id;");	
		
		if (count($data) > 0)
		{
			$article_contenu = utf8_decode($data[0]['content']);
			$article_titre = utf8_decode($data[0]['title']);
			$article_type = $data[0]['type'];
			
			$type_info = $mysql->Query("select * from dev_blog_cp_types where numero = $article_type;");			
			
			$type_nom = $type_info[0]['nom'];
			$type_image_url = $type_info[0]['image_url'];

			echo "<img class=\"idea\" src=\"$type_image_url\" />&nbsp;
			<span class=\"grand_titre\"><b>$article_titre</b></span>
			<span class=\"petit_titre\"> <b>($type_nom)</b></span><br />";
			
			$prev_id = GetPrevArticle($article_id);
			$next_id = GetNextArticle($article_id);
			
			$article_sources_count = 0;			
			$article_contenu = RemplacerCodeBalise($article_contenu, $article_id, 
												   &$article_sources_count);
												   
			for ($i = 0; $i < $article_sources_count; $i++)
			{
				array_push($articles_id_values, $article_id . "_" . $i);
			}
											
			if ($prev_id != 0)
			{
				$prev_url = '&nbsp;<a href="article.php?id=' 
				. $prev_id . '"><img class="petite_image" src="back.png" /></a>';
			}
			
			if ($next_id != 0)
			{
				$next_url = '&nbsp;<a href="article.php?id=' 
				. $next_id . '"><img class="petite_image" src="front.png" /></a>';
			}		
			
			echo 
			'&nbsp;&nbsp;' .
			$prev_url . 
			' <b><span class="grand_titre"><u>Article</u> :</span>' 
			. $next_url . '</b><br /><br /><br />' 
			. $image . '
					
				<br />
				
				<div class="mur_interne">
				<div class="presentation">' . 
				$article_contenu;

			echo '<br /></div></div><br />';
		}
		else
		{
			PrintNoArticleMessage();
		}
	}
	else
	{
		PrintNoArticleMessage();
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
		//localStorage.removeItem("code_read");
	
		CalculerHeure('horloge');
		
		HideAllCodes(articles_id_values);
		
		HideReadCode();
	}
	
</script>	

</body>



</html>