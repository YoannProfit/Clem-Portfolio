<?php

	function GetArticlesCount()
	{
		global $mysql;
		
		if (isset($mysql))
		{
			$data = $mysql->Query("select count(*) as c from dev_blog_cp_articles;");
			
			if (!empty($data))
			{
				return $data[0]['c'];
			}		
		}
	}

	function GetNextArticle($current_id)
	{
		global $mysql;
		
		$articles = $mysql->Query("select * from dev_blog_cp_articles order by type;");			
		reset($articles);
					
		foreach ($articles as $art)
		{			
			if ($art['id'] == $current_id)
			{
				$next_article = current($articles);
				
				if (isset($next_article['id']))
				{
					return $next_article['id'];
				}
				else
				{
					return 0;
				}
			}
			else
			{
				next($articles);
			}	
		}
		
		return 0;
	}	
	
	function GetPrevArticle($current_id)
	{
		global $mysql;
		
		$articles = $mysql->Query("select * from dev_blog_cp_articles order by type;");	
		
		// On part de la fin
		$articles = array_reverse($articles);
		reset($articles);
		
		foreach ($articles as $art)
		{		
			if ($art['id'] == $current_id)
			{
				$prev_article = current($articles);
				
				if (isset($prev_article['id']))
				{
					return $prev_article['id'];
				}
				else
				{
					return 0;
				}
			}
			else
			{
				next($articles);
			}	
		}
		
		return 0;	
	}
	
	function RemplacerCodeBalise($contenu, $article_id, $article_sources_count)
	{
		/* Format :
		
			<code src="D3D10Renderer.cpp">D3D10Renderer source</code> 
		*/
		

		$replace_function = function ($matches) {
			global $article_id;
			global $article_sources_count;
			return RemplacerBaliseHidden_callback($matches, $article_id, &$article_sources_count);
		};
		
		// Type 1
		$regex1 = '#<code src=\"(.*)\">(.*)</code>#';
		
		$contenu = preg_replace_callback($regex1, $replace_function, $contenu);
			
		/* Format :
		
			<code title"="stdio.h" >#include <stdio.h></code> 
		*/		
		
		// Pour les codes sources multilignes
		$regex2 = '#<code title=\"(.*)\">\r\n((.|\r\n)*[^(</c*)|(<c*)])</code>#';
		$contenu = preg_replace_callback($regex2, 'RemplacerBaliseShown_callback', $contenu);
		

		// Pour les codes sources en une seule ligne
		$regex3 = '#<code title=\"(.*)\">((.|\r\n)*[^(</c*)|(<c*)])</code>#';
		$contenu = preg_replace_callback($regex3, 'RemplacerBaliseShown_callback', $contenu);
		
		return $contenu;
	}

	function RemplacerBaliseHidden_callback($matches, $article_id, $code_number)
	{
		$code_name = $article_id . "_" . $code_number++;
		
		$arrow_left = "<img id=\"arrow_left_$code_name\" class=\"arrow_left\" src=\"arrow_left.png\" />";
		$arrow_right = "<img id=\"arrow_right_$code_name\" class=\"arrow_right\" src=\"arrow_right.png\" />";
			
		return "<not_parsed>
				<a href=\"javascript:void(0);\" onclick=\"ClickCode('$code_name', '$matches[1]')\" 
					class=\"nom_fichier\">$arrow_left<u> $matches[2] :</u> <span id=\"show_" . $code_name . "\" style=\"color: red\">[afficher]</span>$arrow_right
					</a>

					<span id=\"code_" . $code_name . "\" class=\"code\"></span>
			   <br /></not_parsed>";	
	}
	
	function RemplacerBaliseShown_callback($matches)
	{
		$code_title = $matches[1];
		$code_content = htmlentities($matches[2], ENT_QUOTES, "UTF-8");
		
		return "<span class=\"nom_fichier\">$code_title :</span><br /><span class=\"code\"><pre class=\"brush: cpp\">$code_content</pre></span>";	
	}	
	
	class ReturnInfo 
	{
		var $_message;
		var $_error_code;	
		
		function ReturnInfo()
		{
			$this->_message = "";
			$this->_error_code = false;
		}
		
		function AppendMessage($message)
		{
			$this->_message .= $message;
		}
		
		function SetSuccess()
		{
			$this->_error_code = true;
		}
		
		function SetFailed()
		{
			$this->_error_code = false;
		}
		
		function GetMessage()
		{
			return $this->_message;
		}
		
		function GetStatus()
		{
			return $this->_error_code;
		}
	}
	
	function FetchUploadedImageFile($file, $directory)
	{
		$target_dir = "uploads/$directory/";
		$target_file = $target_dir . basename($file["name"]);

		$imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));
		
		$return_info = new ReturnInfo;
		$return_info->SetSuccess();		
		
		$check = @getimagesize($file["tmp_name"]);
		
		if ($check !== false) 
		{
			$return_info->AppendMessage("File is an image - " . $check["mime"] . ".");
		}
		else 
		{
			$return_info->AppendMessage("File is not an image.");
			$return_info->SetFailed();
			
			return $return_info; 
		}
		
		if (file_exists($target_file)) 
		{
			$return_info->AppendMessage("Sorry, file already exists.");
			$return_info->SetFailed();
			
			return $return_info;
		}

		if ($file["size"] > 500000) 
		{
			$return_info->AppendMessage("Sorry, your file is too large.");
			$return_info->SetFailed();
			
			return $return_info;
		}

		if ($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
			&& $imageFileType != "gif" ) 
		{
			$return_info->AppendMessage("Sorry, only JPG, JPEG, PNG & GIF files are allowed.");
			$return_info->SetFailed();
			
			return $return_info;
		}

		if (move_uploaded_file($file["tmp_name"], $target_file)) 
		{
			$return_info->AppendMessage("The file ". basename($file["name"]). " has been uploaded.");
			$return_info->SetSuccess();
			
			return $return_info;		
		} 
		else
		{
			$return_info->AppendMessage("Sorry, there was an error uploading your file.");
			$return_info->SetFailed();
			
			return $return_info;	
		}
	}
	
	function FetchUploadedSourceFile($file)
	{
		$target_dir = "sources/";
		$target_file = $target_dir . basename($file["name"]);

		$imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));
		
		$return_info = new ReturnInfo;
		$return_info->SetSuccess();		
		
		if (empty($file))
		{
			$return_info->SetFailed();
			return $return_info;
		}
				
		if (file_exists($target_file)) 
		{
			$return_info->AppendMessage("Désolé, le fichier existe déjà.");
			$return_info->SetFailed();
			
			return $return_info;
		}

		if ($file["size"] > 500000) 
		{
			$return_info->AppendMessage("Désolé, le fichier est trop gros.");
			$return_info->SetFailed();
			
			return $return_info;
		}

		if ($imageFileType != "cpp" && $imageFileType != "h") 
		{
			$return_info->AppendMessage("Désolé, seuls des fichiers de types CPP, H sont autorisés");
			$return_info->SetFailed();
			
			return $return_info;
		}

		if (move_uploaded_file($file["tmp_name"], $target_file)) 
		{
			$return_info->AppendMessage("Le fichier ". basename($file["name"]). " a été envoyé.");
			$return_info->SetSuccess();
			
			return $return_info;		
		} 
		else
		{
			$return_info->AppendMessage("Désolé, il y a eu une erreur d'envoi du fichier.");
			$return_info->SetFailed();
			
			return $return_info;	
		}
	}
?>