#ifndef CONSOLE_H
#define CONSOLE_H
 
#include <string>
#include <vector>
#include <list>
#include <d3dx10math.h>
 
#include "Singleton.h"
#include "Sprite2D.h"
#include "InputManager.h"
 
class KeyEvent;
class Line;
class Command;
class DataParameters;
 
struct MessageType
{
    enum Type {MSG_SYSTEM, MSG_WARNING, MSG_ERROR, MSG_DEBUG, MSG_GAME, MSG_COMMAND, MSG_BLANK};
 
    // Type
    static std::string GetMessageType(MessageType::Type type);
 
    // Couleur
    static D3DXCOLOR GetMessageColour(MessageType::Type type);
};
 
/* Un ensemble de mots */
class TextElement
{ 
public:
    TextElement(std::wstring sTextElementText, D3DXCOLOR colour, bool bBlinking);
    virtual ~TextElement();
 
    int GetLength();
 
    std::wstring GetText();
 
    D3DXCOLOR GetColour();
 
    void SetColour(D3DXCOLOR colour);
 
    bool IsBlinking();
 
private:
    std::wstring m_sTextElement;
 
    bool m_bBlinking;
    D3DXCOLOR m_colour; 
};
 
/* Un ensemble de TextElement */
class Line
{
public:
    Line(MessageType::Type type);
    virtual ~Line();
 
    void AddTextElement(std::wstring sTextElement, D3DXCOLOR colour, bool bBlinking);
    void AddTextElementSolo(std::wstring sTextElement, D3DXCOLOR colour, bool bBlinking);
 
    void GetTextElements(std::vector<TextElement*>& elems);
 
    unsigned int GetTextElementsCount();
 
    MessageType::Type GetType();
 
private:
    void ParseLines(std::wstring sText, std::vector<std::wstring>& moreLines);
 
private:
    std::vector<TextElement*> m_textElements;
 
    MessageType::Type m_type;
 
    unsigned int m_iLastElementLength;
};
 
/* Le petit texte de saisie en bas à gauche de la console */
class CommandLinePrompt
{
public:
    CommandLinePrompt();
    virtual ~CommandLinePrompt();
 
    void AppendCommandLineText(const std::wstring sText);
 
    void SetText(const std::wstring sText);
 
    void Clear();
 
    bool HasText();
 
    // Retourne le texte du prompt
    const std::wstring GetPromptText();
    // Retourne le texte de la ligne de commande
    const std::wstring GetCommandLineText();
 
    void Render();
 
    void SetArrowTransparency(float fValue);
    void SetArrowPosition(float x, float y);
    void SetArrowSize(unsigned int iArrowWidth, unsigned int iArrowHeight);
 
    void MoveCursorToLeft();
    void MoveCursorToRight();
 
    unsigned int GetCursorPos();
 
    void Update(float fYPosition);
 
    float GetCommandLengthPosition();
 
    void ResetCursorPos();
 
private:
    std::wstring m_sPromptText;
    std::wstring m_sCommandLineText;
 
    Sprite2D* m_pArrowSprite;
 
    int m_iCursorPos;
};
 
/* L'historique des commandes */
class CommandsHistory
{
public:
    CommandsHistory();
    virtual ~CommandsHistory();
 
    bool Empty();
 
    const std::wstring GetNext();
    const std::wstring GetPrevious();
 
    void Clear();
 
    void AddCommand(const std::wstring sCommandName);
 
private:
    std::list<std::wstring> m_history;
 
    std::wstring m_sLastCommand;
};
 
class Console : public Singleton<Console>, public KeyListener
{
    friend class Line;
    friend class CommandLinePrompt;
 
public:
    enum ConsoleState {MOVING_UP, MOVING_DOWN, HIDDEN, SHOWN};
    enum TransparencyState {FADE_IN, FADE_OUT};
 
    Console();
    virtual ~Console();
     
    void Initialize(); 
 
    // Fonctions de conversion de chaînes de caractères
    static std::string ToString(const std::wstring& sText);
    static std::wstring ToWString(const std::string& sText);
 
    void Render(float fTimeElapsedSinceLastFrame);
 
    void SetState(ConsoleState state);
 
    /** Méthodes d'affichage **/
    void Print(Line* pLine);
    void Print(const std::wstring& sMessage, D3DXCOLOR colour, MessageType::Type type);
    void Print(const std::wstring& sMessage, MessageType::Type type);
    void Print(const std::wstring& sMessage, D3DXCOLOR colour);
    void Print(const std::wstring& sMessage);
    void Print(const std::string& sMessage);
 
    // Ajoute une ligne vide
    void AddBlankLine();
 
    bool IsVisible();
 
    void ToggleState();
 
    void AddToHistory(const std::wstring& cmd);
    bool IsCharAuthorized(const wchar_t key);
 
    // Ajoute une ligne dans la console
    void AddLine(Line* pLine); 
 
    // Ces méthodes permettent de défiler la console
    void IncreaseScrollingPosition();
    void DescreaseScrollingPosition();
    void ResetScrollingPosition();
 
    // Ajoute les éléments qui qui changent de transparence
    void AddBlinkingTextElement(TextElement* pTextElement);
    void RemoveAllBlinkingTextElements();
 
    void Clear();
 
    virtual void SoloKeyPressed(const KeyEvent &arg);
    virtual void SoloKeyReleased(const KeyEvent &arg);
 
    virtual void RepetitiveKeyPressed(const KeyEvent &arg);
    virtual void RepetitiveKeyReleased(const KeyEvent &arg);
 
    unsigned int GetCharactWidth();
 
    bool ChangeBackgroundImage(const std::wstring& sBgFile);
 
private:
    unsigned int GetWidth();
    float GetStringLength(std::wstring sText);
    void UpdateSprites(float fTimeElapsedSinceLastFrame);
    void CreateTextFont();
    void UpdateCommandLineText();
    void LoadDefaultCommands();
    void TabComplete();
    void GetCommandLineArguments(const std::wstring& sCommandLine, DataParameters& param);
    bool TextIsOutOfWidth(const std::wstring sText);
 
    void DeletePreviousCharacter();
    void DeleteNextCharacter();
 
private:
    // Les lignes affichées
    std::vector<Line*> m_lines;
 
    // Hauteur de la console
    float m_fConsoleHeight;
    // Largeur de la console
    float m_fConsoleWidth;
     
    // Hauteur totale de la console en prenant en compte le prompt
    unsigned int m_iTotalConsoleHeight;
 
    // Largeur d'un caractère
    float m_fCharWidth;
    // Hauteur d'un caractère
    float m_fCharHeight;
 
    // Le nombre maximum de lignes affichées à la fois
    unsigned int m_iMaxLinesDisplayed;
    // Le nombre maximul de caractères présent sur une ligne
    unsigned int m_iMaxCharOnLine;
 
    std::vector<TextElement*> m_blinkingTextElements;
 
    TransparencyState m_elemState;
    ConsoleState m_state;
 
    int m_iLinesStart;
    int m_iLinesOffset;
 
    ID3DX10Font* m_pFont;
 
    ID3DX10Sprite* m_pConsoleTextSprite;
 
    Sprite2D* m_pBackgroudImage;
    Sprite2D* m_pBorderImage;
 
    // Chaîne de tous les carcatères autorisés
    std::wstring m_aLegalChars;
 
    CommandLinePrompt* m_pPrompt;
    CommandsHistory* m_pHistory;
 
    float m_fScrollOffset;
 
    std::list<std::wstring> m_tabHistory;
};
 
#endif
