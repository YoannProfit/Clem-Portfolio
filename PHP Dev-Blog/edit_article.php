<?php	
	include("mysql.class.php");
	require("common.php");
	
	$mysql = new MYSQL;
	$mysql->Connect();
	
	function PrintTypeSelector($type_selected)
	{
		global $mysql;
		
		echo '<select name="type" size="1">';
		
		$data = $mysql->Query("select * from dev_blog_cp_types order by numero;");
				
		
		foreach($data as $type)
		{
			$type_numero = $type['numero'];
			$type_nom = $type['nom'];
			
			if ($type_selected == $type_numero) 
			{
				$selected_string = 'selected="selected"';
			}
			else
			{ 
				$selected_string = "";
			}
			
			echo '
			<option value="' . $type_numero . '" ' . $selected_string . ' >' 
			. $type_nom . '</option>';
		}
		
		echo '</select>';
	}
	
	function GetArticleType($article_id)
	{
		global $mysql;
		
		if (isset($article_id) && is_numeric($article_id))
		{
			$data = $mysql->Query("select type from dev_blog_cp_articles where id = $article_id");
			
			if ($data)
			{
				return $data[0]['type'];
			}
		}
		
		return 1;		
	}
	
	function GetArticleTitle($article_id)
	{
		global $mysql;
		
		if (isset($article_id) && is_numeric($article_id))
		{
			$data = $mysql->Query("select title from dev_blog_cp_articles where id = $article_id");
			
			if ($data)
			{
				return utf8_decode($data[0]['title']);
			}
		}		
		
		return "";
	}
	
	function PrintArticleContent($article_id)
	{
		global $mysql;
		
		if (isset($article_id) && is_numeric($article_id))
		{
			$data = $mysql->Query("select content from dev_blog_cp_articles where id = $article_id");
			
			if ($data)
			{
				echo utf8_decode($data[0]['content']);
			}
		}		
	}	
	
	if (isset($_COOKIE['loggued'])
	&& $_COOKIE['loggued'] == true) 
	{		
		$is_admin_logged = true;
	}
	else
	{
		header('Location: admin.php');
	}

	// Mode de modification
	if (isset($_GET['id']))
	{
		$doAdd = false;
		$article_id = $_GET['id'];
	}
	// Sinon mode d'ajout
	else
	{
		$doAdd = true;
	}
	
	if ($is_admin_logged && isset($_GET['delete']) && is_numeric($_GET['delete']))
	{
		$doAdd = false;
		
		$id = $_GET['delete'];
		
		$data = $mysql->Query("delete from dev_blog_cp_articles where id = $id");
		
		$article_deleted = true;
		
		header('Location: admin.php');
	}
	
				
	// Comportement de modification de l'article !
	if ($is_admin_logged && isset($_POST['contenu']) && $_POST['contenu'] != "")
	{
		$content = utf8_encode($_POST['contenu']);
		$title = utf8_encode($_POST['title']);
		$type = $_POST['type'];
		
		// On ajoute l'article
		if ($doAdd)
		{
			$data = $mysql->Query("insert into dev_blog_cp_articles values('', '$title', '$content', $type, 
			NOW());");
			
			$article_added = true;		
			$article_id = $mysql->LastInsertID();
		}
		// On modifie l'article
		else if (isset($article_id))
		{
			$article_modified = true;
			$data = $mysql->Query("update dev_blog_cp_articles set title = '$title', content = '$content', type = $type where id = $article_id;");
		}
	}	
	

?>

<!DOCTYPE html>

<!-- MAIN PAGE -->

<html>

<head>
	<meta charset="utf-8" />
	 
	<title>Clément Profit - Avancement des connaissances en Game Programming - Edition / ajout d'article</title>
	
	<link rel="icon" href="manette.ico" type="image/x-icon">

	
	<link rel="stylesheet" href="style.css">
	<script type="text/javascript" src="dev-blog-cp.js"></script> 
		
</head>


<body>	


<header>

<?php

include "panel.php";

?>


<br />

<?php

if (isset($article_added) && $article_added == true)
{
	echo 'Article ajouté !' . '<br />';
}
else if (isset($article_modified) && $article_modified == true)
{
	echo 'Article modifié !' . '<br />';
}
else if (isset($article_deleted) && $article_deleted == true)
{
	echo 'Article supprimé !' . '<br />';
}
?>
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
		
			<img class="idea" src="article.png" />
				
			&nbsp;<b><span class="grand_titre">Enregistrer ou modifier un article</span></b>
				
			<br />
			
			<div class="mur_interne">
<?php
	if ($doAdd && isset($article_id))
	{
		$action = "?id=$article_id";
	}
	else
	{
		$action = "";
	}
	
	if (isset($_FILES))
	{
		$file = $_FILES["fileToUpload"];
		
		if (!empty($file) && !empty($file['name']))
		{
			$return_info = FetchUploadedSourceFile($file);
			
			echo "<br />" . $return_info->GetMessage();			
		}
	}	
?>
			<form id="form" method="post" enctype="multipart/form-data" action="<?php echo $action; ?>">
				<br />
				Titre : 
				<br />
				<textarea name="title" rows="1" cols="80"><?php echo GetArticleTitle($article_id); ?></textarea>    
				<br />	
				<br />
				Type : 
				<br />
				<?php
					$type_selected = GetArticleType($article_id);
		
					PrintTypeSelector($type_selected);
				
				?>
				<br /> <br />
				Raccourcis : <br />
				<a href="javascript:insertImage();"><img title="Image" class="icon_edit_article" src="./img.png" /></a>
				<a href="javascript:insertNewline();"><img title="Saut de ligne" class="icon_edit_article" src="./newline.png" /></a>
				<a href="javascript:insertLink();"><img title="Lien" class="icon_edit_article" src="./link.png" /></a>
				<a href="javascript:insertUrlCode();"><img title="Code" class="icon_edit_article" src="./file.png" /></a>
				<br />
				<br />
				Fichier source à upload : 
				<br />
				<input type="file" name="fileToUpload" accept=".cpp, .h"></input>
				<br />
				<br />
				Article : 
				<br />			

				<textarea class="article_edit" id="textarea" name="contenu" rows="20" cols="90"><?php 
					
					if ($mode_adding == true)
					{
						echo "&lt;span class=&quot;article&quot;&gt;&lt;/span&gt;";
					}
					else
					{
						PrintArticleContent($article_id);
					}
				?></textarea>    
					
				<br />
				<br />
				<button type="submit" name="validate">
				<?php echo (!$doAdd || isset($article_id)) ? "Mettre à jour" : "Ajouter" ?></button>
				
				
				<br/>
				</form>	
				
				<br/>
			</div>
			<hr /><br />
		
			<img class="idea" src="file_article.png" />
							
			&nbsp;<b><span class="grand_titre">Aperçu</span></b>
				
			<div class="mur_interne">
				<div class="presentation">
<?php

	// Mode ajout
	if ($mode_adding)
	{
		if (isset($content))
		{	
			echo utf8_decode($content) . '<br />';
		}
	}
	// Mode édition
	else if (isset($article_id))
	{
		$data = $mysql->Query("select content from dev_blog_cp_articles where id = $article_id");
		
		echo utf8_decode($data[0]['content']) . '<br /><br />';
	}

?>
				</div>
			</div>
			<br />
		</div>
	
	</div>
</div>
<br />
<br />
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

<script type="text/javascript">

function insertImage()
{
	var img = prompt("Entrez le nom de l'image", "empty.png");

	var txt;
	
	if (img == null || img == "") 
	{
		txt = "empty.png";
	} 
	else 
	{
		txt = img;
	} 

	insertTextAtCursor(textarea, '<img class="image" src="' + txt + '" />');
};

function insertNewline()
{
	insertTextAtCursor(textarea, '<br />');
};

function insertLink()
{
	var url = prompt("Entrez l'url", "");

	var txt;
	
	if (url == null || url == "") 
	{
		txt = "";
	} 
	else 
	{
		txt = url;
	} 

	insertTextAtCursor(textarea, '<a href="' + txt + '" target="_blank" >' + txt + '</a>');
};

function insertUrlCode()
{
	var url = prompt("Entrez l'url du code", "./sources/common/");
	var file_name = prompt("Entrez le nom du fichier", "");

	var txt;
	
	if (url == null || url == "") 
	{
		txt = "";
	} 
	else 
	{
		txt = url;
	} 
	
	insertTextAtCursor(textarea, '<code src="' + txt + '">' + file_name + '</code>');
};

function insertTextAtCursor(textarea, value) 
{
	if (textarea.selectionStart || textarea.selectionStart == '0') 
	{
		var startPos = textarea.selectionStart;

		var endPos = textarea.selectionEnd;

		textarea.value = textarea.value.substring(0, startPos)

		+ value

		+ textarea.value.substring(endPos, textarea.value.length);
		
		textarea.setSelectionRange(endPos + value.length, endPos + value.length);
	} 
	else 
	{
		textarea.value += value;
	}
};
	
</script>	

</html>