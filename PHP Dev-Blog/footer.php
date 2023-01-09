<footer>

	<span class="cadre_bas" onmouseenter="OnAboutPopUp_in(event, 'about_tooltip')"
						    onmouseleave="OnAboutPopUp_out('about_tooltip')" 								
						    onmousemove="OnAboutPopUp_move(event, 'about_tooltip', false)">
				  
		<span class="about">
					  
			<tooltip id="about_tooltip">
				<img class="alert" src="alert.png" /> 
					Début des articles : Mars 2018

				<img class="alert" src="alert.png" />

				<br />
					Nombre d'articles : <?php echo GetArticlesCount(); ?>
			</tooltip>
			
		<img id="about_image" class="footer_image" src="programmation.png" />
						
		</span>
	</span>
	
	
	<br /><br />
	<img class="arrow" src="arrow_left.png" />
		<b>Clément Profit - 2018 - Blog de progression des connaissances en programmation graphique et jeux vidéo.</b>
	<img class="arrow" src="arrow_right.png" />
	
</footer>