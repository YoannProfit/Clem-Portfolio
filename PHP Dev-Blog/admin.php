<?php
	include("mysql.class.php");
	
	require("common.php");
	
	$mysql = new MYSQL;
	$mysql->Connect();
	
	if (isset($_COOKIE['loggued']) && $_COOKIE['loggued'] == true) 
	{		
		$is_admin_logged = true;
	}
?>

<!DOCTYPE html>

<!-- MAIN PAGE -->

<html>

<head>
	<meta charset="utf-8" />
	 
	<title>Clément Profit - Avancement des connaissances en Game Programming - Admin Panel</title>
	
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
	if (isset($_POST) && !empty($_POST['login']) && !empty($_POST['password'])) 
	{
	  $_POST['password'] = hash("sha256", $_POST['password']);
	  
	  // Créé les variables $login et $password
	  extract($_POST);
	  
	  // On recupére le password de la table qui correspond au login du visiteur
	  $sql = "select password from membres where login='" . $login . "'";
	  
	  $req = mysql_query($sql) or die('Erreur SQL !<br>' . $sql . '<br>' . mysql_error());

	  $data = mysql_fetch_assoc($req);

	  if ($data['password'] != $password) 
	  {
		echo '<span style="color: white;">
				<div>Mauvais login / password. Merci de recommencer !</div>
			  </span>';
	  }
	  else 
	  {
		$_SESSION['login'] = $login;
		$_SESSION['loggued'] = true;
		
		// on définit une durée de vie de notre cookie (en secondes), donc un an dans notre cas
		$temps = 365*24*3600;

		setcookie ("login", $login, time() + $temps, "/");
		setcookie ("loggued", true, time() + $temps, "/");
	  }    
	}
	else 
	{
	  $champs = '<span style="color: white;"><p><b>(Remplissez tous les champs pour vous connectez !)</b></p>
	  </span>';
	}
	
	if (isset($_GET) && isset($_GET["delete_type"]))
	{	
		$type_id = $_GET["delete_type"];
	
		$mysql->Query("delete from dev_blog_cp_types where id = $type_id;");	

		echo "<span class=\"red\">Catégorie d'articles supprimée !</span> <br />";
	}
	
	$types_numeros = $mysql->Query("select numero from dev_blog_cp_types order by numero;");			
	$numeros = array();
	
	foreach ($types_numeros as $row)
	{			
		array_push($numeros, $row['numero']);
	}
	
	if (isset($_POST))
	{
		foreach ($numeros as $type_numero)
		{		
			// Changer le nom d'une catégorie ! 
			if (isset($_POST["type_$type_numero" . "_nom"]) && $_POST["type_$type_numero" . "_nom"] != "")
			{
				$type_nom = $_POST["type_$type_numero" . "_nom"];

				$data = $mysql->Query("update dev_blog_cp_types set nom = '$type_nom' where
					numero = $type_numero;");
			}
		
			if (isset($_FILES) && isset($_FILES["image_url_type_$type_numero"]))
			{
				$file = $_FILES["image_url_type_$type_numero"];
			
				if (!empty($file))
				{
					$dir = "images";
					
					$return_info = FetchUploadedFile($file, $dir);
				
					//echo $return_info->GetMessage();
									
					$image_url = utf8_encode("./uploads/$dir/" . basename($file["name"]));
					
					// On met à jour l'url de l'image !
					$mysql->Query("update dev_blog_cp_types set image_url = '$image_url' where
								   numero = $type_numero;");
					
				}
			}
		}
		
		// Pour ajouter une nouvelle catégorie (un type d'article) !
		if (isset($_POST["type_nouveau_nom"]) && $_POST["type_nouveau_nom"] != "")
		{
			$nouveau_type_nom = $_POST["type_nouveau_nom"];

			$data = $mysql->Query("select max(numero) as maximum from dev_blog_cp_types;");
			
			$max_numero_number = $data[0]['maximum'];
			
			if (isset($max_numero_number))
			{
				$new_numero_number = ++$max_numero_number;
				
				$data = $mysql->Query("insert into dev_blog_cp_types 
									   values('', '$nouveau_type_nom', 
									   $new_numero_number, 'empty.png');");

				if (isset($_FILES))
				{
					$file = $_FILES["fileToUpload"];
					
					if (!empty($file))
					{
						$dir = "images";
						
						$return_info = FetchUploadedFile($file, "images");
					
						//echo $return_info->GetMessage();
										
						$image_url = utf8_encode("./uploads/$dir/" . basename($file["name"]));
						
						// On met à jour l'url de l'image !
						$mysql->Query("update dev_blog_cp_types set image_url = '$image_url' where
							numero = $new_numero_number;");
					}
				}			
			}
		}	
	}
	

	function PrintArticlesList()
	{
		global $mysql;
		
		$data = $mysql->Query("select * from dev_blog_cp_articles;");

		foreach ($data as $value)
		{			
			$article_id = htmlentities($value['id']);		
			$article_titre = utf8_decode($value['title']);
			$article_taille = strlen($value['content']);
			$article_date = htmlentities($value['date']);
					
			echo '<img class="square" src="square.png" />';
			echo "<a href=\"javascript:ConfirmerSuppressionArticle('$article_titre', $article_id);\"><img class=\"croix\" src=\"croix.png\" /></a> ";

			echo "<a href=\"edit_article.php?id=$article_id\"><span class=\"lien_article\">". $article_titre . "</span></a>
				 ($article_taille caractères) [$article_date]
				 <br />";
		}
	}
	
	function PrintTypesList()
	{
		global $mysql;
		
		$data = $mysql->Query("select * from dev_blog_cp_types order by id;");
	
		echo '<form id="form_modif_type" method="post" action="" enctype="multipart/form-data">';
		
		echo "Liste des catégories : <br /><br />";
				
		foreach ($data as $value)
		{			
			$type_id = htmlentities($value['id']);
			$type_numero = htmlentities($value['numero']);		
			$type_nom = htmlentities($value['nom']);
			$type_image_url = htmlentities(utf8_decode($value['image_url']));

			echo '<img class="square" src="square.png" />';
			echo "<a href=\"javascript:ConfirmerSuppressionType('$type_nom', $type_id);\"><img class=\"croix\" src=\"croix.png\" /></a> ";

			$type_text_area_id = 'type_' . $type_numero;

			echo "<img onclick=\"OpenImageFileUpload('$type_text_area_id');\" class=\"petite_image\" src=\"$type_image_url\" />";
			echo '<span id="' . $type_text_area_id . '_notify"></span>';
			echo "<a class=\"lien\" href=\"javascript:ToggleShowTypeModif('$type_text_area_id');\">$type_nom</a>&nbsp&nbsp";
			
			echo '<textarea style="display: none; resize: none;" name="' . $type_text_area_id . '_nom" id="' . $type_text_area_id . '_nom" rows="1" cols="30"></textarea>';
						
			//$type_id++;
			
			echo '<br />';
		}	
		
		echo '<br />';
		
		echo '<button type="submit" name="validate">Modifier</button>';
		
		echo '</form>';		
	}
	
	if (isset($_COOKIE['loggued']) == false && $_SESSION['loggued'] == false) 
	{	
		echo '
			<div class="mur_interne">
			<div class="presentation">	
			<form method="post" action="">

					<h2>Connexion au Panel</h2>

					<div>
					  <label>Login</label>
					  <div>
						<input type="text" name="login" />
					  </div>
					  
					</div><br/>

					<div>
					  <label>Mot de passe</label>
					  <div>
						<input type="password" name="password" />
					  </div>
					</div>

				<br/><br/><center><button type="submit" name="submit">Connexion</button></center>
				<br />
			</form>		
			</div>
			</div>';
	}
	else
	{
		echo '
			<img class="idea" src="admin_panel.png" />
				
			&nbsp;<b><span class="grand_titre">Panel administrateur</span></b>
				
			<br />
			
			<div class="mur_interne">
			<div class="presentation">	
				<img class="square" src="square.png" />
				<a class="lien" href="edit_article.php">Edition / Ajout d\'articles</a>
	
			<br/>
			<br />
			<img class="square" src="square.png" />
			<a class="lien" href="logout.php">Logout</a></span><br /><br />
			<u>Liste des articles</u> :<br /><br />';
			
		PrintArticlesList();

		echo '<form id="form_ajout_type" method="post" action="" enctype="multipart/form-data">
				<br />
				Nouveau type d\'article (catégorie) :<br /> 
				<br />
				<textarea style="resize: none;" name="type_nouveau_nom" rows="1" cols="40"></textarea>    
				<br /><br />
				Choisir une image de catégorie associée : <br />
				<input type="file" name="fileToUpload"></input>
				<br /><br />
				<button type="submit" name="validate">Ajouter</button>
			  </form>';
		echo '<br /><br />';
		
		PrintTypesList();
		
		echo '<br /></div></div><br />';
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
</body>

</html>