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

	if (isset($_COOKIE['loggued']) && $_COOKIE['loggued'] == true) 
	{		
		$is_admin_logged = true;
	}
	
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

	function PrintNewEntryForm()
	{
		echo '<form id="form" method="post" action="lexique.php">
				<br />
				Entrez une nouvelle entrée : 
				<br />
				<textarea style="overflow:auto; resize:none;" name="titre" rows="1" cols="70"></textarea>    
				<br />	
				<br />
				
				Définition : 
				<br />
				<textarea style="overflow:auto; resize:none;" id="textarea" name="contenu" rows="10" cols="70"></textarea>
				<br />
				<button type="submit" name="validate">Ajouter</button>
				<br />
				<br />
			</form>
			<hr />
			<br />';
	}
	
	if ($is_admin_logged && isset($_GET['delete']) && is_numeric($_GET['delete']))
	{
		$id = $_GET['delete'];
		
		$data = $mysql->Query("delete from dev_blog_cp_lexique where id = $id");
		
		$item_deleted = true;
	}
	
	// Comportement de modification de l'article !
	if ($is_admin_logged && isset($_POST['contenu']) && $_POST['contenu'] != "")
	{
		$content = utf8_encode($_POST['contenu']);
		$titre = utf8_encode($_POST['titre']);

		// On ajoute la nouvelle entrée du lexique
		$data = $mysql->Query("insert into dev_blog_cp_lexique values('', '$titre', '$content');");
		
		$article_added = true;		
		$article_id = $mysql->LastInsertID();
	}		
?>
	
<?php

	// Supporte l'utf-8	
	function MettreLeDebutEnMinuscule($phrase)
	{
		$last_letters = mb_substr($phrase, 1, strlen($phrase)-1, 'utf-8');
		
		$first_letter = mb_substr($phrase, 0, 1, 'utf-8');

		$first_letter = mb_strtolower($first_letter, 'utf-8');
		
		$phrase_final = $first_letter . $last_letters;
		
		return $phrase_final;
	}	
	
	function AjouterPointFinal($phrase)
	{
		$last_char = substr($phrase, -1);
		
		if ($last_char != '.')
		{
			$phrase = $phrase . '.';
		}
		
		return $phrase;
	}
	
	function MotEstTotalementEnMajuscule($mot)
	{
		$mot_test = substr($mot, 0, 2);
		
		return ctype_upper($mot_test);
	}
	
	// Gestion du lexique
	$lexique = array();

	$lexique_data = $mysql->Query("select * from dev_blog_cp_lexique order by nom;");		
	
	$current_entry_id = $_GET['id'];
	
	
?>
	<div class="mur_externe">
	
		<div id="news" class="center">
		<img class="idea" src="dico.png" />
				<span class="grand_titre"><b>Lexique</b></span>
				<span class="petit_titre"> <b>(<?php echo count($lexique_data) . " entrées"
								?>)</b></span><br />

				<br />
					<div class="mur_interne">
						<div class="presentation">

							<?php
								if (isset($item_added) && $item_added == true)
								{
									echo '<span class="red">Entrée ajoutée !' . '</span><br />';
								}
								else if (isset($item_modified) && $item_modified == true)
								{
									echo '<span class="red">Entrée modifiée !' . '</span><br />';
								}
								else if (isset($item_deleted) && $item_deleted == true)
								{
									echo '<span class="red">Entrée supprimée !' . '</span><br />';
								}
								
								global $is_admin_logged;
								
								if ($is_admin_logged)
								{
									PrintNewEntryForm();
								}
								
								//end($lexique_data);
								
								
								foreach ($lexique_data as $value)
								{					
									$id = $value['id'];
									
									$nom = $value['nom'];
									
									if (!MotEstTotalementEnMajuscule($nom))
									{
										$nom = MettreLeDebutEnMinuscule($nom);
									}
									
									$explication = utf8_decode(MettreLeDebutEnMinuscule(
										AjouterPointFinal($value['explication'])));
										
									$ancre = "ancre_" . $id;
									
									$mot = "<span id=\"$ancre\" class=\"orange\">$nom</span>";
										
									if ($current_entry_id == $id)
									{
										$mot = "<span id=\"$ancre\" class=\"red\">$nom</span>";										
									}
	
									$brs = "<br /><br />";
								
									/*
									if (current($lexique_data) == $value)
									{
										//$brs = "<br />";
									}		
									*/
									
									if ($is_admin_logged)
									{
										$delete_link = "<a href=\"javascript:ConfirmerSuppressionItemLexique('$nom', $id);\">
														<img class=\"croix\" src=\"croix.png\" /></a> ";
									}
										
									echo $delete_link . $mot . " : " . $explication . " " . $brs;
								}
								
								//reset($lexique_data);
							?>
							<br />
						</div>
					</div>
		
		
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

	function ConfirmerSuppressionItemLexique(item_nom, id)
	{
		var ok = confirm("Êtes-vous sûr de supprimer cette entrée du lexique nommée : " + 
				 item_nom + " ? (id = " + id + ")");
		
		if (ok)
		{
			window.location.href = 'lexique.php?delete=' + id;
		}		
	}

	var currentScrollValue = 0;
	
	function HidePanel()
	{
		var	textbox = document.getElementById("panel");
		textbox.style.display = "none";				
	}
	
	function ShowPanel()
	{
		var	textbox = document.getElementById("panel");
		
		textbox.style.display = "initial";			
	}	
	
	function CheckPanelShouldBeVisible()
	{
		var currentUrl = document.URL;
		var urlParts   = currentUrl.split('#');
			
		var hashLink = (urlParts.length > 1);
		
		if (hashLink == true)
		{
			HidePanel();
			currentScrollValue = window.pageYOffset;
		}
	}
	
	// On précharge toutes les saisies 
	window.onload = function()
	{
		CalculerHeure('horloge');
		
		CheckPanelShouldBeVisible();
	}
	
	window.onhashchange = function()
	{
		CheckPanelShouldBeVisible();
	}
	
	window.onscroll = function()
	{
		if (window.pageYOffset !== undefined) 
		{
			ShowPanel();
		}
		else
		{
			HidePanel();
		}
		
		if (window.pageYOffset == currentScrollValue
		&& currentScrollValue != 0)
		{
			HidePanel();
		}
	}
	
</script>	
</body>


</html>