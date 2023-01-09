//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Shell.h
// Date de création : Octobre 2015
// Description : Cette classe représente un shell 
// dans lequel on peut entrer des commandes et
// appeler des programmes
//----------------------------------------------------

#ifndef SHELL_H
#define SHELL_H

#include "Types.h"
#include "Singleton.h"
#include "IOPorts.h"
#include "DataParameters.h"

enum MessageType 
{
	MSG_SYSTEM, 
	MSG_WARNING, 
	MSG_ERROR, 
	MSG_DEBUG, 
	MSG_COMMAND, 
	MSG_BLANK
};

class TextDisplayer
{
public:
	virtual void PutCharacter(const Core::TextElement& elem) = 0;
	virtual void ClearLine(uint32 iPosY);
	virtual void RefreshLine(uint32 iPosY);
	virtual void UpdateCursor();
	virtual void Clear();
};

// Le shell
template<typename CharType, typename StringType>
class Shell : public Singleton<Shell>, public KeyListener<CharType>, public TextDisplayer
{
public:
	Shell();
	virtual ~Shell();

	void ClearLines();
	
	void ScrollUp();
	void ScrollDown();
	void ResetScrolling();

	void Print(const CharType* sText);
	void Print(const StringType& sText);
	
   	void Print(const StringType& sMessage, 
		       Core::Colour foregroundColour = Core::Colour::Black, 
		       Core::Colour backgroundColour = Core::Colour::White, 
		       MessageType type = MessageType::MSG_SYSTEM);

	void Print(const Line* pLine);
	void Print(const uint32 iValue);
	void Print(const float fValue);
	void Print(const CharType c);

	void AddCharacter(uint32 iValue);
	void AddCharacter(const float fValue);
	void AddCharacter(CharType c);
	void AddCharactersLine();

    // Ajoute une ligne vide
    void AddBlankLine();	
	// Savoir si le Shell est visible
	bool IsVisible();
	// Ajoute à l'historique une commande
	void AddToHistory(const StringType& sCmd);

    // Ajoute une ligne dans la console
    void AddLine(const Line* pLine); 

	void SetPrintColor(const Core::Color foreground, const Core::Color background);
	void SetPrintType(const MessageType type);

	// Entrée utilisateur
    virtual void SoloKeyPressed(const KeyEvent<CharType>& arg);
    virtual void SoloKeyReleased(const KeyEvent<CharType>& arg);

	void UpdateBlinkingElements();

	// On peut cacher ou afficher le prompt du shell
	void ShowPrompt();
	void HidePrompt();
	bool IsPromptVisible();

	/* Permet de changer et remplacer rapidement les lignes affichées 
	   par d'autres telles que spécifiées en paramètres */
	void HookLines(const Core::Array<Line*>& aLines);
	void ClearHookedLines();
	bool IsLinesHooked();

	// Met à jour le positionnement de toutes les lignes (et en fonction du scroll)
	void RefreshLines();

	uint32 GetMaxCharDisplayed();

	void SetMaxLinesDisplayed(const uint32 iMaxLinesDisplayed);
	void SetMaxCharDisplayed(const uint32 iMaxCharDisplayed);

	uint16 GetDisplayedLinesCount();

protected:
	/* On peut surcharger cette fonction afin de faire afficher les lignes
	   pour un autre mode d'affichage */
	virtual void PutCharacter(const uint32 iPosX, const uint32 iPosY, const CharType c, 
				  const Core::Color foreground, const Core::Color background);

	// Met à jour la position du curseur, peut être surchargée
	virtual void UpdateCursor();

	// Efface la mémoire vidéo
	virtual void Clear();

private:
    // Obtient le texte du type spécifié
    const StringType GetMessageType(const MessageType type);
    // Obtient la couleur du type spécifié
	const Core::Colour GetMessageColour(const MessageType type);
	// Est-ce que le caractère tappé est autorisé ?
	bool IsCharAuthorized();
	// Charge les commandes par défaut
    void LoadDefaultCommands();
	/* Complète un morceau de commande par la touche
	   tabulation */
    void TabComplete();
	// Analyse les paramètres d'une commande tappée
    void GetCommandLineArguments(const StringType& sCommandLine, DataParameters& param);
	// On affiche une unique ligne
	void DrawLine(Line* pLine);

private:
	Core::Array<Line<StringType>*> m_aLines;
	Core::Array<Line<StringType>*> m_aHookedLines;

    uint16 m_iLinesStart;
    uint16 m_iLinesOffset;

	uint32 m_iCurrentXPos;
	uint32 m_iCurrentYPos;

    CommandLinePrompt<StringType>* m_pPrompt;
    CommandsHistory<StringType>* m_pHistory;
	
    Core::Array<StringType> m_tabHistory;

	Core::Color m_currentForeground, m_currentBackground;

	StringType m_aLegalChars;

	MessageType m_currentMessageType;

	Core::Array<VideoMemory::TextElement> m_aBlinkingElements;

	bool m_bPromptVisible;

	// Le nombre maximum de lignes affichées à la fois
	uint32 m_iMaxLinesDisplayed;
	// Le nombre maximul de caractères présent sur une ligne
	uint32 m_iMaxCharDisplayed;

	Line<StringType>* m_pCurrentLine;
};

// ----------------------------------- //

// Un ensemble de mots
template<typename StringType>
class TextElement
{
public:
    TextElement(const StringType& sTextElementText, Core::Colour foregroundColour, 
		Core::Colour backgroundColour, bool bBlinking);

    virtual ~TextElement();
 
	uint8 GetLength();
 
	StringType GetText();
 
    Core::Colour GetForegroundColour();
    Core::Colour GetBackgroundColour();
  
    void SetForegroundColour(Core::Colour colour);
    void SetBackgroundColour(Core::Colour colour);
 
    bool IsBlinking();
 
private:
    StringType m_sTextElement;
 
    bool m_bBlinking;

    Core::Colour m_foregroundColour;    
    Core::Colour m_backgroundColour; 
};
 
// ----------------------------------- //

// Un ensemble de TextElement
template<typename StringType>
class Line
{
public:
    	Line(Shell::MessageType type, Shell* pShellOwner);
    	virtual ~Line();
	
    	void AddTextElement(const StringType& sTextElement, 
							Core::Colour foregroundColour = Core::Colour::White, 
							Core::Colour backgroundColour = Core::Colour::Black, 
							bool bBlinking = false);

    	void AddTextElementSolo(const StringType& sTextElement, 
								Core::Colour foregroundColour = Core::Colour::White, 
								Core::Colour backgroundColour = Core::Colour::Black, 
								bool bBlinking = false);
 
    	void GetTextElements(Core::Array<TextElement<StringType>*>& elems);
 
		size_t GetTextElementsCount();
 
    	Shell::MessageType GetType();
		
		void Clear();

private:
    	void ParseLines(const StringType& sText, Core::Array<StringType>& moreLines);
 
private:
    	Core::Array<TextElement<StringType>*> m_textElements;
 
    	Shell::MessageType m_type;
 
		Shell* m_pShellOwner;
};

// ----------------------------------- //

// L'historique des commandes entrées
template<typename StringType>
class CommandsHistory
{
public:
    CommandsHistory();
    virtual ~CommandsHistory();
 
    bool Empty();
 
    const StringType& GetNext();
    const StringType& GetPrevious();
 
    void Clear();
 
    void AddCommand(const StringType& sCommandName);
 
private:
    Core::List<StringType> m_aHistory;
};
  
// ----------------------------------- //

template<typename StringType>
class CommandLinePrompt
{
public:
    CommandLinePrompt();
    virtual ~CommandLinePrompt();
 
    void AppendCommandLineText(const StringType& sText);
 
    void SetText(const StringType& sText);
 
    void Clear();
 
    bool HasText();

    // Retourne le texte du prompt
    StringType GetPromptText();
    // Retourne le texte de la ligne de commande
    StringType GetCommandLineText();
    // Retourne les deux chaîne précédentes
    StringType GetText();

    void MoveCursorToLeft();
    void MoveCursorToRight();
    void DeleteNextCharacter();
    void DeletePreviousCharacter();

    uint32 GetCursorPos();

    Line<StringType>* GetLine();

private:
    StringType m_sPromptText;
    StringType m_sCommandLineText;

    uint16 m_iCursorPos;

    Line<StringType>* m_pLine;
};
 
#endif
