<arrow id="arrow_image">
</arrow>

<panel id="panel">

<img class="bar" src="titanbar_done.png" />

<span class="panel_text">

		<span class="panel_entry">

			<span onmouseenter="OnAboutPopUp_in(event, 'arrow_image')"
				  onmouseleave="OnAboutPopUp_out('arrow_image')" 								
				  onmousemove="OnAboutPopUp_move(event, 'arrow_image', true)">
											
				
					<a class="panel_link" href="index.php">
						<span class="no_cursor"><b>Accueil</b></span>
					</a> 
				
				
			</span> -
			
			<span onmouseenter="OnAboutPopUp_in(event, 'arrow_image')"
				  onmouseleave="OnAboutPopUp_out('arrow_image')" 								
				  onmousemove="OnAboutPopUp_move(event, 'arrow_image', true)">
							
					<span class="no_cursor"><b>Heure</b></span></span> : 
					<span class="nombre_heure" id="horloge"></span> - 
							
			<span onmouseenter="OnAboutPopUp_in(event, 'arrow_image')"
				  onmouseleave="OnAboutPopUp_out('arrow_image')" 								
				  onmousemove="OnAboutPopUp_move(event, 'arrow_image', true)">
														
				<span class="no_cursor"><b>Nombre d'articles</b></span>
				
			</span> :

			 <span class="nombre_articles"><i><?php echo GetArticlesCount(); ?></i></span> - 
			 
			<span onmouseenter="OnAboutPopUp_in(event, 'arrow_image')"
				  onmouseleave="OnAboutPopUp_out('arrow_image')" 								
				  onmousemove="OnAboutPopUp_move(event, 'arrow_image', true)">
														
				<span class="no_cursor"><b>Nombre de vidéos</b></span>
				
			</span> :

			 <span class="nombre_videos"><i>21</i></span>
			 
<?php
	if ($is_admin_logged)
	{
		echo " - <span onmouseenter=\"OnAboutPopUp_in(event, 'arrow_image')\"
				  onmouseleave=\"OnAboutPopUp_out('arrow_image')\" 								
				  onmousemove=\"OnAboutPopUp_move(event, 'arrow_image', true)\">
														

				<span class=\"panel_admin_text\"><i>Panel Administrateur</i></span>
				
			</span>";
	}
?>			 

			
		</span>
</span>	

</panel>

