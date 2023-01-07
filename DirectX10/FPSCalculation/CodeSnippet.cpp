/* Calulate FPS - Code Snippet */

void CalculateFPS(float fTimeSinceLastFrame)
{
    /* Incrèmente le nombre d'images affichées.
 
       /!\ Doit être déclarée en tant que float et initialisée à 0 au début
       de l'application */
    m_fNumFrames += 1.0f;
 
   /* Ajoute combien de temps s'est passé depuis la dernière
     image de rendu
 
     /!\ Doit être déclarée en tant que float et initialisée à 0 au début
     de l'application */
    m_fTimeElapsed += fTimeSinceLastFrame;
 
    float iFPS = 0;
    float iMilliSecPerFrame = 0;
 
    // Lorsqu'une seconde s'est écoulée
    if (m_fTimeElapsed >= 1.0f)
    {
        iFPS = m_fNumFrames / m_fTimeElapsed;
 
        // Temps moyen, en milisecondes, pour qu'une seule image soit rendue
        iMilliSecPerFrame = 1000.0f / iFPS;
 
        /* Réinitialise le compteur d'images et le compteur de temps entre
           chaque image */
        m_fTimeElapsed = 0.0f;
        m_fNumFrames = 0.0f;
 
        /* On place les nombres dans une chaîne représentant les statistiques
           formatées */
        static char buffer[256];
        sprintf(buffer, "Images par seconde = %.2f | "
            "Millisecondes par image = %.4f", iFPS, iMilliSecPerFrame);
 
        // On met à jour le titre de la fenêtre
        SetWindowTextA(m_hWnd, buffer);
    }
}
