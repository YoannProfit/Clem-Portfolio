var bPopupEnabled = false;

function OnAboutPopUp_move(event, id, below)
{
	if (bPopupEnabled)
	{
		var tooltipSpan = document.getElementById(id);
		
		var x = event.clientX;
		var	y = event.clientY;
		
		var xOffset = 0;
		var yOffset = 0;
		
		// On met la popup en dessous de la souris (c'est-à-dire en son milieu)
		if (below)
		{
			xOffset = -GetPopupWindowWidth(id) / 5.8;
			yOffset = GetPopupWindowHeight(id) * 0.05;
		}
		else
		{
			xOffset = -GetPopupWindowWidth(id) / 2.0;
			yOffset = -GetPopupWindowHeight(id) * 0.5;			
		}
		
		tooltipSpan.style.top = (y + yOffset) + 'px';
		tooltipSpan.style.left = (x + xOffset) + 'px';	
	}
};


var blink_interval;

function OnAboutPopUp_in_blink(event, id)
{
	var blink = true;

	function ActualiserBlink() 
	{
		var image = document.getElementById(id);
		
		if (blink)
		{
			image.style.backgroundImage = "url(flèche3_colored.png)";
			blink = false;
		}
		else 
		{
			image.style.backgroundImage = "url(flèche3.png)";
			blink = true;
		}
	}

	if (!bPopupEnabled)
	{
		var tooltipSpan = document.getElementById(id);
		
		tooltipSpan.style.display = "initial";	
		tooltipSpan.style.visibility = "visible";

		bPopupEnabled = true;

		blink_interval = setInterval(ActualiserBlink, 400);	
		
		OnAboutPopUp_move(event, id);
	}
};

function OnAboutPopUp_out_blink(id)
{
	if (bPopupEnabled)
	{
		HidePopup(id);
		
		bPopupEnabled = false;
		
		clearInterval(blink_interval);

		var image = document.getElementById(id);
				
		image.style.backgroundImage = "url(flèche3.png)";
	}
};

function OnAboutPopUp_in(event, id)
{
	if (!bPopupEnabled)
	{
		var tooltipSpan = document.getElementById(id);
		
		tooltipSpan.style.display = "initial";	
		tooltipSpan.style.visibility = "visible";
		
		bPopupEnabled = true;
		
		OnAboutPopUp_move(event, id);
	}
};

function OnAboutPopUp_out(id)
{
	if (bPopupEnabled)
	{
		HidePopup(id);
		
		bPopupEnabled = false;
	}
};

function HidePopup(id)
{
	var tooltipSpan = document.getElementById(id);
	
	tooltipSpan.style.display = "none";	
	tooltipSpan.style.visibility = "hidden";
};

function GetPopupWindowWidth(name)
{
	return document.getElementById(name).offsetWidth;
};

function GetPopupWindowHeight(name)
{
	return document.getElementById(name).offsetHeight;
};

function OnAboutMouseExit(id)
{
	clearInterval(blink);
}

function CalculerHeure(el) 
{
	if (typeof el == "string") 
	{ 
		el = document.getElementById(el); 
	}

	function ActualiserHeure() 
	{
		var date = new Date();
		var str = date.getHours();
		
		str += ':' + (date.getMinutes()<10?'0':'') + date.getMinutes();
		str += ':' + (date.getSeconds()<10?'0':'') + date.getSeconds();
		
		el.innerHTML = "<i>" + str + "</i>";
	}
	
	ActualiserHeure();
	
	setInterval(ActualiserHeure, 1000);
}


var loaded_codes = new Object();
var loaded_article_id = new Object();

function ClickCode(code, url)
{	
	var display = document.getElementById("code_" + code).style.display;
	
	if (display == "inline")
	{	
		HideCode("code_" + code);
		document.getElementById("show_" + code).innerHTML = "[afficher]";
	}
	else
	{
		if (loaded_codes["code_" + code] == false)
		{
			MiseAJourCode("code_" + code, url);
		}	
		
		document.getElementById("show_" + code).innerHTML = "[désafficher]";
		
		// On cache les flèches 
		var	arrow_left = document.getElementById("arrow_left_" + code);
		var	arrow_right = document.getElementById("arrow_right_" + code);

		arrow_left.style.display = "none";				
		arrow_right.style.display = "none";
		
		ShowCode("code_" + code);

		NotifyCodeRead(code);
	}
}

function ShowCode(code)
{
	document.getElementById(code).style.display = "inline";	
}

function HideCode(code)
{
	document.getElementById(code).style.display = "none";
}

function HideAllCodes(articles_id_values)
{
	// On reset les titres
	for (i = 0; i < articles_id_values.length; i++) 
	{				
		var code_id = "code_" + articles_id_values[i];
		
		HideCode(code_id);
		loaded_codes[code_id] = false;
	}			
}


function MiseAJourCode(code, file_url)
{
	var xmlhttp = new XMLHttpRequest();
	
	xmlhttp.onreadystatechange = function() 
	{
		if (xmlhttp.readyState == 4 && xmlhttp.status == 200) 
		{
			var result = '<pre class="brush: cpp">';
			result += xmlhttp.responseText;
			result += '</pre>';
			
			document.getElementById(code).innerHTML = result;

			SyntaxHighlighter.highlight();

			loaded_codes[code] = true;
		}
	};
	
	xmlhttp.open("GET", "getsourcecode.php?file=" + file_url, true);
	xmlhttp.send();
}

function ObjectLength(obj)
{
	var i = 0;
	
	for (var x in obj)
	{
		if (obj.hasOwnProperty(x))
		{
			i++;
		}
	} 
	
	return i;
}

function NotifyCodeRead(code_number)
{
	var text = localStorage.getItem("code_read");
	
	// On réinitialise toutes les visites de codes
	if (text == null)
	{
		// A refaire en php !
		var jsonText = {};

		localStorage.setItem("code_read", JSON.stringify(jsonText));	
	}
	
	text = localStorage.getItem("code_read");
		
	var myCodesUnSerialized = JSON.parse(text);
	
	// On vérifie si on a bien le status du code en mémoire
	myCodesUnSerialized[code_number] = true;

	localStorage.setItem("code_read", JSON.stringify(myCodesUnSerialized));		
}

function HideReadCode()
{
	var text = localStorage.getItem("code_read");
	var myCodesUnSerialized = JSON.parse(text);	
	
	if (myCodesUnSerialized == null)
	{
		return;
	}
	
	for (var code in myCodesUnSerialized) 
	{		
		var read = myCodesUnSerialized[code];
		
		// On cache les flèches 			
		if (read == true)
		{
			var	arrow_left = document.getElementById("arrow_left_" + code);
			var	arrow_right = document.getElementById("arrow_right_" + code);

			arrow_left.style.display = "none";				
			arrow_right.style.display = "none";			
		}
	}
}

function ConfirmerSuppressionArticle(article_titre, id)
{
	var ok = confirm("Êtes-vous sûr de supprimer cet article nommé : " + 
			 article_titre + " ? (id = " + id + ")");
	
	if (ok)
	{
		window.location.href = 'edit_article.php?delete=' + id;
	}
}

function ConfirmerSuppressionType(type_nom, id)
{
	var ok = confirm("Êtes-vous sûr de supprimer ce type nommé : " + 
			 type_nom + " ? (id = " + id + ")");
	
	if (ok)
	{
		window.location.href = 'admin.php?delete_type=' + id;
	}
}

function ToggleShowTypeModif(type_text_area_id)
{
	var	textbox = document.getElementById(type_text_area_id + '_nom');

	if (textbox.style.display == "inline")
	{
		textbox.style.display = "none";		
	}
	else
	{
		textbox.style.display = "inline";	
	}	
}

function OpenImageFileUpload(type_text_area_id)
{
	var input = document.createElement('input');
	input.type = 'file';
	input.name = "image_url_" + type_text_area_id;
	input.style.display = "none";
	input.id = type_text_area_id + '_file';
	
	document.getElementById('form_modif_type').appendChild(input);
	
	input.click();	
	
	document.getElementById(type_text_area_id + '_file').onchange = function() 
	{
		document.getElementById(type_text_area_id + '_notify').innerHTML = '(image changée !)';
	};	
}