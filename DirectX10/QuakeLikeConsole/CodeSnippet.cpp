//----------------------------------------------------
// Auteur : Clément Profit
// Date de création : Juillet 2010
// Description : Une console quake-like avec commandes et appel
// d'events
//----------------------------------------------------
 
#include "InputManager.h"
#include "Console.h"
#include "Defines.h"
 
#include "Commands.h"
#include "DataParameters.h"
#include "D3D10Renderer.h"
#include "Utils.h"
 
template<> Console* Singleton<Console>::ms_instance = nullptr;
 
/********** MessageType **********/
 
std::string MessageType::GetMessageType(MessageType::Type type)
{
    std::string messageType = "";
 
    switch (type)
    {
        case MSG_SYSTEM: messageType = "[SYSTEM]"; break;
        case MSG_WARNING: messageType = "[WARNING]"; break;
        case MSG_ERROR: messageType = "[SYS_ERROR]"; break;
        case MSG_DEBUG: messageType = "[SYS_DEBUG]"; break;
        case MSG_GAME: messageType = "[GAME]"; break;
        case MSG_COMMAND: messageType = "[COMMAND]"; break;
        case MSG_BLANK: messageType = ""; break;
    }
 
    return messageType;
}
 
D3DXCOLOR MessageType::GetMessageColour(MessageType::Type type)
{
    D3DXCOLOR messageColour;;
 
    switch (type)
    {
        case MSG_SYSTEM: messageColour = D3DXCOLOR(0.8f, 0.9f, 1.0f, 1.0f); break;
        case MSG_WARNING: messageColour = D3DXCOLOR(0.7f, 0.8f, 1.0f, 1.0f); break;
        case MSG_ERROR: messageColour = D3DXCOLOR(0.2f, 0.4f, 1.0f, 1.0f); break;
        case MSG_DEBUG: messageColour = D3DXCOLOR(0.3f, 0.5f, 1.0f, 1.0f); break;
        case MSG_GAME: messageColour = D3DXCOLOR(0.1f, 0.8f, 1.0f, 1.0f); break;
        case MSG_COMMAND: messageColour = D3DXCOLOR(0.4f, 0.8f, 1.0f, 1.0f); break;
        case MSG_BLANK: messageColour = D3DXCOLOR(1.0, 1.0f, 1.0f, 1.0f); break;
    }
 
    return messageColour;
}
 
/********** TextElement **********/
 
TextElement::TextElement(std::wstring sTextElementText, D3DXCOLOR colour, bool bBlinking) :
m_sTextElement(sTextElementText),
m_bBlinking(bBlinking),
m_colour(colour)
{
}
 
TextElement::~TextElement()
{
}
 
int TextElement::GetLength()
{
    return m_sTextElement.length();
}
 
std::wstring TextElement::GetText()
{
    return m_sTextElement;
}
 
D3DXCOLOR TextElement::GetColour()
{
    return m_colour;
}
 
void TextElement::SetColour(D3DXCOLOR colour)
{
    m_colour = colour;
}
 
bool TextElement::IsBlinking()
{
    return m_bBlinking;
}
 
/********** Line **********/
 
Line::Line(MessageType::Type type) : 
m_type(type),
m_iLastElementLength(0)
{
    // Ajoute le type de message (par ex [Game]) au début de la ligne
    AddTextElementSolo(Console::ToWString(MessageType::GetMessageType(m_type)), MessageType::GetMessageColour(m_type), false);
}
 
Line::~Line()
{
    for (unsigned int i = 0; i < m_textElements.size(); i++)
    {
        SAFE_DELETE(m_textElements[i]);
    }
}
 
unsigned int Line::GetTextElementsCount()
{
    return m_textElements.size();
}
 
void Line::AddTextElement(std::wstring sTextElement, D3DXCOLOR colour, bool bBlinking)
{
    if (CONSOLE->TextIsOutOfWidth(sTextElement))
    {   
        // Après on segmente la ligne
        std::vector<std::wstring> moreLines;
        ParseLines(sTextElement, moreLines);
 
        for (unsigned int i = 0; i < moreLines.size(); i++)
        {
            std::wstring pCurrentString = moreLines[i];
 
            Line* pLine = new Line(m_type);
 
            pLine->AddTextElementSolo(pCurrentString, colour, bBlinking);
 
            CONSOLE->AddLine(pLine);
        }   
    }
    else
    {
        AddTextElementSolo(sTextElement, colour, bBlinking);
    }
}
 
void Line::AddTextElementSolo(std::wstring sTextElement, D3DXCOLOR colour, bool bBlinking)
{
    TextElement* pTextElement = new TextElement(sTextElement, colour, bBlinking);
    m_textElements.push_back(pTextElement);
 
    if (bBlinking)
    {
        CONSOLE->AddBlinkingTextElement(pTextElement);
    }
}
 
// Analyse une ligne et la découpe en plusieurs lignes si le nombre
// de caractères de la ligne passé dépasse un certain seuil
void Line::ParseLines(std::wstring sText, std::vector<std::wstring>& moreLines)
{
    std::wstring s = CONSOLE->ToWString(MessageType::GetMessageType( GetType() ));
 
    const wchar_t* str = sText.c_str();
    std::wstring line = L"";
 
    for (unsigned int c = 0; c < sText.length(); c++)
    {   
        if (str[c] == '\n' || CONSOLE->TextIsOutOfWidth(s + line))
        {
            moreLines.push_back(line);
            line = L"";
        }
 
        if (str[c] != '\n')
            line += str[c];
    }
 
    if (line.length() > 0)
    {
        moreLines.push_back(line);
    }
}
 
void Line::GetTextElements(std::vector<TextElement*>& elem)
{
    elem = m_textElements;
}
 
MessageType::Type Line::GetType()
{
    return m_type;
}
 
/****************************** CommandLinePrompt ******************************/
 
CommandLinePrompt::CommandLinePrompt() : 
m_sPromptText(CONSOLE_PROMPT_TEXT),
m_pArrowSprite(nullptr),
m_iCursorPos(0)
{
    m_pArrowSprite = new Sprite2D(D3D10_RENDERER->GetDevice(), L"arrow.png", 10, 12);
 
    m_pArrowSprite->Initialize();
    m_pArrowSprite->SetColor(D3DXCOLOR(1.0f, 0.85f, 0.0f, 0.0f));
}
 
CommandLinePrompt::~CommandLinePrompt()
{
    delete m_pArrowSprite;
}
 
void CommandLinePrompt::Update(float fYPosition)
{
    float fPromptXOffset = CONSOLE->GetStringLength(L"_") + CONSOLE->GetStringLength(m_sPromptText);
 
    float fTotalWidth = GetCommandLengthPosition();
 
    SetArrowPosition(fPromptXOffset + fTotalWidth, fYPosition);
}
 
void CommandLinePrompt::AppendCommandLineText(const std::wstring sText)
{
    unsigned int iPromptCursorPos = GetCursorPos();
 
    std::wstring sCurrentCommandLine = GetCommandLineText();
 
    std::wstring sFirstPart = sCurrentCommandLine.substr(0, iPromptCursorPos);
    std::wstring sLastPart = sCurrentCommandLine.substr(iPromptCursorPos, sCurrentCommandLine.length());
     
    if (sLastPart.length() > 0)
    {
        sFirstPart += sText;
        m_sCommandLineText = sFirstPart + sLastPart;
        MoveCursorToRight();
    }
    else
    {
        m_sCommandLineText += sText;
        MoveCursorToRight();
    }
}
    
void CommandLinePrompt::SetText(const std::wstring sText)
{
    m_sCommandLineText = sText;
}
 
void CommandLinePrompt::Clear()
{
    m_sCommandLineText = L"";
    m_iCursorPos = 0;
}
 
bool CommandLinePrompt::HasText()
{
    return m_sCommandLineText.size() > 0;
}
 
const std::wstring CommandLinePrompt::GetPromptText()
{
    return m_sPromptText;
}
 
const std::wstring CommandLinePrompt::GetCommandLineText()
{
    return m_sCommandLineText;
}
 
void CommandLinePrompt::Render()
{
    m_pArrowSprite->Render();
}
 
void CommandLinePrompt::SetArrowTransparency(float fValue)
{
    m_pArrowSprite->SetTransparency(fValue);
}
 
void CommandLinePrompt::SetArrowPosition(float x, float y)
{
    m_pArrowSprite->SetPosition(x, y);
}
 
void CommandLinePrompt::SetArrowSize(unsigned int iArrowWidth, unsigned int iArrowHeight)
{
    m_pArrowSprite->SetImageSize(iArrowWidth, iArrowHeight);
}
 
unsigned int CommandLinePrompt::GetCursorPos()
{
    return m_iCursorPos;
}
 
void CommandLinePrompt::MoveCursorToLeft()
{
    if (m_iCursorPos > 0)
    {
        m_iCursorPos--;
    }
}
 
void CommandLinePrompt::MoveCursorToRight()
{
    float fPromptXOffset = CONSOLE->GetStringLength(m_sCommandLineText);
 
    if (GetCommandLengthPosition() < fPromptXOffset)
    {
        m_iCursorPos++;
    }
}
 
float CommandLinePrompt::GetCommandLengthPosition()
{
    float fTotalWidth = 0.0f;
 
    for (unsigned int i = 0; i < m_iCursorPos; i++)
    {
        std::wstring s = L"";
        s.push_back(m_sCommandLineText[i]);
 
        fTotalWidth += CONSOLE->GetStringLength(s);
    } 
 
    return fTotalWidth;
}
 
void CommandLinePrompt::ResetCursorPos()
{
    m_iCursorPos = m_sCommandLineText.length();
}
 
/****************************** CommandsHistory ******************************/
 
CommandsHistory::CommandsHistory() : 
m_sLastCommand(L"")
{
}
 
CommandsHistory::~CommandsHistory()
{
    m_history.clear();
}
 
bool CommandsHistory::Empty()
{
    return m_history.size() == 0;
}
 
const std::wstring CommandsHistory::GetPrevious()
{
    if (!Empty())
    {
        if (m_sLastCommand == m_history.back())
        {
        }
 
        const std::wstring sCommandName = m_history.back();
 
        m_history.push_front(sCommandName);
        m_history.pop_back();
 
        m_sLastCommand = sCommandName;
 
        return sCommandName;
    }
    else
    {
        return EMPTY;
    }
}
 
const std::wstring CommandsHistory::GetNext()
{
    if (!Empty())
    {
        if (m_sLastCommand == m_history.front())
        {
        }
 
        const std::wstring sCommandName = m_history.front();
 
        m_history.push_back(sCommandName);
        m_history.pop_front();
         
        m_sLastCommand = sCommandName;
 
        return sCommandName;
    }
    else
    {
        return EMPTY;
    }
}
 
void CommandsHistory::Clear()
{
    m_history.clear();
}
 
void CommandsHistory::AddCommand(const std::wstring sCommandName)
{
    m_history.remove(sCommandName);
    m_history.push_back(sCommandName);
 
    if (m_history.size() > CONSOLE_MAX_COMMAND_HISTORY) 
    {
        m_history.pop_front();
    }
}
 
/****************************** Console ******************************/
 
Console::Console() :
m_elemState(TransparencyState::FADE_IN),
m_iLinesStart(0),
m_iLinesOffset(0),
m_iMaxCharOnLine(0),
m_pFont(nullptr),
m_pConsoleTextSprite(nullptr),
m_pBackgroudImage(nullptr),
m_pBorderImage(nullptr),
m_aLegalChars(INPUT_LEGAL_CHARS),
m_fCharHeight(0),
m_fCharWidth(0),
m_fScrollOffset(1.0f),
m_pPrompt(nullptr)
{
}
 
Console::~Console() 
{
    RemoveAllBlinkingTextElements();
}
 
void Console::Initialize() 
{
    new CommandsManager();
    CreateTextFont();
 
    m_fConsoleHeight = (float) D3D10_RENDERER->GetViewportHeight() * CONSOLE_HEIGHT_FACTOR;
    m_fConsoleWidth = (float) D3D10_RENDERER->GetViewportWidth();
 
    /* Calcul des tailles d'un caractère */
    RECT rect = {0, 0, 0, 0};
 
    m_pFont->DrawText(nullptr, L"a", -1, &rect, DT_CALCRECT, D3DXCOLOR());
 
    m_fCharWidth = rect.right - rect.left;
    m_fCharHeight = rect.bottom - rect.top;
 
    m_iMaxLinesDisplayed = (unsigned int) (m_fConsoleHeight / (m_fCharHeight - CONSOLE_HEIGHT_PADDING / 2));
    m_iMaxCharOnLine = (unsigned int) ((m_fConsoleWidth)  / m_fCharWidth);
 
    // Le total des lignes affichables + la hauteur du prompt
    m_iTotalConsoleHeight = m_fConsoleHeight + m_fCharHeight + CONSOLE_HEIGHT_PADDING / 2;
 
    // Image de fond
    m_pBackgroudImage = new Sprite2D(D3D10_RENDERER->GetDevice(), L"D3.jpg", m_fConsoleWidth, m_iTotalConsoleHeight);
    m_pBackgroudImage->Initialize();
    m_pBackgroudImage->SetPosition(0, -m_fConsoleHeight);
 
    // Bord du bas
    m_pBorderImage = new Sprite2D(D3D10_RENDERER->GetDevice(), L"border.png", m_fConsoleWidth, CONSOLE_BORDER_HEIGHT);
    m_pBorderImage->Initialize();
 
    // La ligne de commande et le prompt
    m_pPrompt = new CommandLinePrompt();
     
    // Ajout de lignes prédéfinies
    for (int i = 0; i < 0; i++)
    {
        Line* line = new Line(MessageType::MSG_GAME);
        //line->AddTextElement(L"En analyse, le nombre dérivé en un point d'une fonction à variable et valeurs réelles est le coefficient directeur de la tangente au graphe de cette fonction en ce point. C'est le coefficient directeur de l'approximation affine de cette fonction en ce point ; ce nombre n'est donc défini que si cette tangente ? ou cette approximation ? existe.", D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), false);
 
        line->AddTextElement(std::to_wstring(i), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), false);
 
        AddLine(line);
    }
 
    LoadDefaultCommands();
 
    Print(L"En mathématiques, une métrique ou distance est une fonction qui définit la distance entre les éléments d'un ensemble. Un ensemble muni d'une distance est appelé un espace métrique. Toute distance induit une topologie sur un ensemble mais la réciproque est fausse : un espace topologique n'est pas toujours métrisable.");
 
    INPUT_MANAGER->SetKeyListener(this);
 
    m_pHistory = new CommandsHistory();
 
    // La console est caché dès le début
    SetState(HIDDEN);
 
    Print("Salut!");
}
 
void Console::CreateTextFont()
{
    ID3D10Device* pd3dDevice = D3D10_RENDERER->GetDevice();
 
    D3DX10CreateSprite(pd3dDevice, 0, &m_pConsoleTextSprite);
 
    D3DX10_FONT_DESC fd;
    fd.Height = CONSOLE_FONT_HEIGHT;
    fd.Width = CONSOLE_FONT_WIDTH;
    fd.Weight = 3;
    fd.MipLevels = 0;
    fd.Italic = false;
    fd.CharSet = OUT_DEFAULT_PRECIS;
    fd.Quality = ANTIALIASED_QUALITY;
    fd.PitchAndFamily = DEFAULT_PITCH;
    wcscpy(fd.FaceName, CONSOLE_FONT_NAME);
 
    D3DX10CreateFontIndirect(pd3dDevice, &fd, &m_pFont);
}
 
float Console::GetStringLength(std::wstring sText)
{
    RECT rect = {0, 0, 0, 0};
 
    m_pFont->DrawText(nullptr, sText.c_str(), -1, &rect, DT_CALCRECT, D3DXCOLOR());
 
    float fWidth = rect.right - rect.left;
 
    // On rajoute les espaces qui ne sont pas pris en compte
    if (!sText.empty() && sText.back() == L' ')
    { 
        int c = sText.size() - 1;
 
        while (true)
        {
            if (sText.at(c) == L' ')
            {
                fWidth += 5;
            }
            else
            {
                break;
            }
 
            c--;
 
            // Au cas où...
            if (c < 0)
            {
                break;
            }
        }
    }
 
    return fWidth;
}
 
void Console::AddLine(Line* pLine)
{
    // On n'ajoute pas les lignes qui comportent uniquement
    // l'en-tête [Game] par exemple
    if (pLine->GetTextElementsCount() > 1)
    {
        m_lines.push_back(pLine);
 
        // On décale toutes les lignes
        if (m_lines.size() > m_iMaxLinesDisplayed)
        {
            m_iLinesStart++;
        }
    }
}
 
void Console::IncreaseScrollingPosition()
{
    if (m_iLinesStart > 0)
    {
        m_iLinesOffset++;
        m_iLinesStart--;
    }
}
 
void Console::DescreaseScrollingPosition()
{
    if (m_iLinesOffset > 0)
    {
        m_iLinesOffset--;
        m_iLinesStart++;
    }
}
 
void Console::SetState(ConsoleState state) 
{
    m_state = state;
}
 
void Console::ToggleState()
{
    if (m_state == SHOWN)
    {
        m_state = MOVING_UP;
    }
    else if (m_state == HIDDEN)
    {
        m_state = MOVING_DOWN;
    }
}
 
void Console::Print(Line* pLine)
{
    AddLine(pLine);
}
 
void Console::Print(const std::wstring& sMessage, D3DXCOLOR colour, MessageType::Type type)
{
    Line* pLine = new Line(type);
 
    pLine->AddTextElement(sMessage, colour, false);
 
    AddLine(pLine);
}
 
void Console::Print(const std::wstring& sMessage, MessageType::Type type)
{
    Line* pLine = new Line(type);
 
    pLine->AddTextElement(sMessage, D3DXCOLOR(1.0f, 1.0f,1.0f, 1.0f), false);
 
    AddLine(pLine);
}
 
void Console::Print(const std::wstring& sMessage, D3DXCOLOR colour)
{
    Line* pLine = new Line(MessageType::MSG_GAME);
 
    pLine->AddTextElement(sMessage, colour, false);
 
    AddLine(pLine);
}
 
void Console::Print(const std::wstring& sMessage)
{
    Line* pLine = new Line(MessageType::MSG_GAME);
 
    pLine->AddTextElement(sMessage, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), false);
 
    AddLine(pLine);
}
 
void Console::Print(const std::string& sMessage)
{
    Line* pLine = new Line(MessageType::MSG_GAME);
 
    pLine->AddTextElement(ToWString(sMessage), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), false);
 
    AddLine(pLine);
}
 
void Console::AddBlankLine()
{
    Line* pLine = new Line(MessageType::MSG_BLANK);
 
    pLine->AddTextElement(L"", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), false);
 
    AddLine(pLine);
}
 
bool Console::IsVisible()
{
    return m_state != HIDDEN;
}
 
void Console::AddToHistory(const std::wstring& cmd) 
{
    m_pHistory->AddCommand(cmd);
}
 
bool Console::IsCharAuthorized(const wchar_t key)
{
    for (unsigned int c = 0; c < m_aLegalChars.size(); c++)
    {
        if (m_aLegalChars[c] == key)
        {
            return true;
        }
    }
 
    return false;
}
 
void Console::AddBlinkingTextElement(TextElement* elem)
{
    m_blinkingTextElements.push_back(elem);
}
 
void Console::RemoveAllBlinkingTextElements()
{
    m_blinkingTextElements.clear();
}
 
void Console::Clear()
{
    for (unsigned int i = 0; i < m_lines.size(); i++)
    {
        delete m_lines[i];
    }
 
    m_lines.clear();
 
    RemoveAllBlinkingTextElements();
 
    m_iLinesOffset = 0;
    m_iLinesStart = 0;
}
 
void Console::LoadDefaultCommands()
{
    CONSOLE_COMMANDS_MANAGER->AddCommand( new Command_QUIT() );
    CONSOLE_COMMANDS_MANAGER->AddCommand( new Command_HELP() );
    CONSOLE_COMMANDS_MANAGER->AddCommand( new Command_EXIT() );
    CONSOLE_COMMANDS_MANAGER->AddCommand( new Command_VERSION() );
    CONSOLE_COMMANDS_MANAGER->AddCommand( new Command_CLS() );
    CONSOLE_COMMANDS_MANAGER->AddCommand( new Command_CHGBG() );
}
 
void Console::TabComplete()
{   
    std::wstring sCurrentCommandLine = m_pPrompt->GetCommandLineText();
 
    std::vector<std::string> cmds;
    CONSOLE_COMMANDS_MANAGER->GetCommandsNames(cmds);
     
    // On stocke toutes les commandes qui commence par le même début que la saisie
    // et ceci lorsque la liste est vide
    if (sCurrentCommandLine.length() > 0 && m_tabHistory.empty())
    {
        for (int i = 0; i < cmds.size(); i++)
        {
            std::wstring cmd = ToWString(cmds[i]);
 
            if (cmd.size() > sCurrentCommandLine.size())
            {
                if (cmd.substr(0, sCurrentCommandLine.size()) == sCurrentCommandLine)
                {
                    m_tabHistory.push_back(cmd);
                }
            }
        }       
    }
    // Sinon on stocke toutes les commandes existantes
    else
    {
        for (int i = 0; i < cmds.size(); i++)
        {
            std::wstring cmd = ToWString(cmds[i]);
 
            m_tabHistory.push_back(cmd);
        }
    }
 
    // On échange les commandes de manière circulaire dans la liste
    if (!m_tabHistory.empty())
    {
        std::wstring command = m_tabHistory.front();
 
        m_tabHistory.pop_front();
        m_tabHistory.push_back(command);
 
        m_pPrompt->SetText(command);
    }
}
 
void Console::SoloKeyPressed(const KeyEvent& arg)
{
    if (arg.key == DIK_F1)
    {
        ToggleState();
    }
 
    if (!IsVisible())
    {
        return;
    }
 
    if (IsCharAuthorized(arg.text))
    {
        std::wstring wStr;
        wStr.push_back( arg.text );
 
        m_pPrompt->AppendCommandLineText(wStr);
 
        /* On reset l'historique de selection automatique 
        de commande par tabulation */
        m_tabHistory.clear();
    }
 
    if (arg.key == DIK_RETURN)
    {
        if (!m_pPrompt->HasText())
        {
            return;
        }
 
        ResetScrollingPosition();
 
        DataParameters param;
        const std::wstring sCommandLineCaption = m_pPrompt->GetCommandLineText();
 
        GetCommandLineArguments(sCommandLineCaption, param);
 
        if (param.getParametersCount() > 0)
        {
            std::string sCommandName = param.getParameterAsString("0");
 
            // On appel la commande spécifiée
            CONSOLE_COMMANDS_MANAGER->InvokeCommand(sCommandName, param);
 
            AddToHistory( sCommandLineCaption );
        }
             
        m_pPrompt->Clear();
    }
    else if (arg.key == DIK_LEFTARROW)
    {
        m_pPrompt->MoveCursorToLeft();
    }
    else if (arg.key == DIK_RIGHTARROW)
    {
        m_pPrompt->MoveCursorToRight();
    }
    else if (arg.key == DIK_BACK)
    {
        DeletePreviousCharacter();
    }
    else if (arg.key == DIK_DELETE)
    {
        DeleteNextCharacter();
    }
    else if (arg.key == DIK_TAB)
    {
        TabComplete();
        m_pPrompt->ResetCursorPos();
    }
    else if(arg.key == DIK_UP)
    {
        const std::wstring sCommand = m_pHistory->GetPrevious();
 
        if (sCommand != EMPTY)
        {
            m_pPrompt->SetText(sCommand);
            m_pPrompt->ResetCursorPos();
        }
    }
    else if(arg.key == DIK_DOWN)
    {
        const std::wstring sCommand = m_pHistory->GetNext();
 
        if (sCommand != EMPTY)
        {
            m_pPrompt->SetText(sCommand);
            m_pPrompt->ResetCursorPos();
        }
    }
}
 
void Console::SoloKeyReleased(const KeyEvent& arg)
{
}   
 
void Console::RepetitiveKeyPressed(const KeyEvent& arg)
{
    if (!IsVisible())
    {
        return;
    }
 
    if (arg.key == (DIK_PGUP))
    {
        IncreaseScrollingPosition();
    }
 
    if (arg.key == (DIK_PGDN))
    {
        DescreaseScrollingPosition();
    }
}
 
void Console::RepetitiveKeyReleased(const KeyEvent& arg)
{
}   
 
void Console::GetCommandLineArguments(const std::wstring& sCommandLine, DataParameters& param)
{
    std::string sArgument = "";
    unsigned int iArgumentCount = 0;
 
    for (unsigned int c = 0; c < sCommandLine.length(); c++)
    {
        if (sCommandLine[c] == ' ')
        {
            if (sArgument.length() > 0)
            {
                param.setParam(iArgumentCount, sArgument);
 
                iArgumentCount++;
                sArgument = "";
            }
        }
        else
        {
            sArgument += sCommandLine[c];
        }
    }
 
    if (sArgument.length() > 0)
    {
        param.setParam(iArgumentCount, sArgument);
    }
}
 
void Console::Render(float fTimeElapsedSinceLastFrame)
{   
    if (!IsVisible())
    {
        return;
    }
 
    UpdateSprites(fTimeElapsedSinceLastFrame);
 
    // On commence par afficher le texte à partir d'une certaine marge
    float iLastTextElementXPos = CONSOLE_LINE_BEGINNING_WIDTH;
    int iLineID = 0;
    float fLastLineYPos = 0;
    float fScroll = -(m_fScrollOffset * m_fConsoleHeight);
    float fHeightPadding = m_fCharHeight / 2 + CONSOLE_HEIGHT_PADDING;
 
    m_pConsoleTextSprite->Begin(D3DX10_SPRITE_SAVE_STATE);
    for (unsigned int i = m_iLinesStart; i < m_lines.size() - m_iLinesOffset; i++)
    {
        Line* pLine = m_lines[i];
 
        std::vector<TextElement*> elems;
        pLine->GetTextElements(elems);
 
        for (unsigned int j = 0; j < elems.size(); j++)
        {
            TextElement* pElement = elems[j];
 
            float fLastLineYPos = (float) (iLineID) * fHeightPadding;
 
            RECT rectangle = {iLastTextElementXPos, fScroll + fLastLineYPos, 0, 0};
 
            D3DXCOLOR color = pElement->GetColour();
 
            std::wstring sText = pElement->GetText();
 
            m_pFont->DrawText(m_pConsoleTextSprite, sText.c_str(), -1, &rectangle, DT_NOCLIP, color);            
             
            iLastTextElementXPos += GetStringLength( pElement->GetText() ) ;
        }
 
        iLastTextElementXPos = CONSOLE_LINE_BEGINNING_WIDTH;
        iLineID++;
    }
 
    /******** Command Line Prompt ********/
 
    const std::wstring sPromptText = m_pPrompt->GetPromptText();
    const std::wstring sCommandLineText = m_pPrompt->GetCommandLineText();
 
    float y = fScroll + (float) (m_iMaxLinesDisplayed) * fHeightPadding;
 
    RECT promptRectangle = {CONSOLE_LINE_BEGINNING_WIDTH, y, 0, 0};
    RECT commandLineRectangle = {sPromptText.length() * m_fCharWidth, y, 0, 0};
 
    D3DXCOLOR promptColor(1.0f, 0.85f, 0.0f, 1.0f);
    D3DXCOLOR commandLineColor(0.0f, 0.3f, 1.0f, 1.0f);
 
    m_pFont->DrawText(m_pConsoleTextSprite, sPromptText.c_str(), -1, &promptRectangle, DT_NOCLIP, promptColor);      
    m_pFont->DrawText(m_pConsoleTextSprite, sCommandLineText.c_str(), -1, &commandLineRectangle, DT_NOCLIP, commandLineColor);
 
    float fBorderScroll = y + m_fCharHeight + CONSOLE_HEIGHT_PADDING / 2;
    m_pBorderImage->SetPosition(0, fBorderScroll);
 
    float fY = y + (m_fCharHeight / 2 - CONSOLE_HEIGHT_PADDING / 2);
    m_pPrompt->Update(fY);
 
    m_pConsoleTextSprite->End();
 
    // On affiche le prompt après tous les précédents rendus
    m_pPrompt->Render();
}
 
void Console::UpdateSprites(float fTimeElapsedSinceLastFrame)
{
    if (!IsVisible())
    {
        return;
    }
 
    m_pBackgroudImage->Render();
    m_pBorderImage->Render();
 
    // Montrer
    if (m_state == MOVING_DOWN) 
    {      
        m_fScrollOffset -= fTimeElapsedSinceLastFrame * CONSOLE_MOVE_SPEED;
 
        if (m_fScrollOffset <= 0.0)
        {
            m_fScrollOffset = 0.0;
 
            SetState(SHOWN);
        } 
 
        m_pBackgroudImage->SetPosition(0, -m_fScrollOffset * m_fConsoleHeight);
    }
   
    // Cacher
    if (m_state == MOVING_UP) 
    {
        m_fScrollOffset += fTimeElapsedSinceLastFrame * CONSOLE_MOVE_SPEED;
 
        if (m_fScrollOffset >= 1.0)
        {
            m_fScrollOffset = 1.0;
 
            SetState(HIDDEN);
        }
 
        m_pBackgroudImage->SetPosition(0, -m_fScrollOffset * m_fConsoleHeight);
    }
 
    // Mise à jour des éléments qui alterne en couleur alpha
    if (m_state == SHOWN)
    {
        static float alpha = 0.0f;
 
        if (m_elemState == TransparencyState::FADE_IN)
        {
            alpha -= fTimeElapsedSinceLastFrame;
 
            if (alpha < 0.0f)
            {
                m_elemState = TransparencyState::FADE_OUT;
            }
        }
        else if (m_elemState == TransparencyState::FADE_OUT)
        {
            alpha += fTimeElapsedSinceLastFrame;
     
            if (alpha > 1.0f)
            {
                m_elemState = TransparencyState::FADE_IN;
            }
        }
 
        for (unsigned int i = 0; i < m_blinkingTextElements.size(); i++)
        {        
            TextElement* pTextElem = m_blinkingTextElements[i];
            static D3DXCOLOR color = pTextElem->GetColour();
 
            pTextElem->SetColour(D3DXCOLOR(color.r, color.g, color.b, alpha));
        }
 
        m_pPrompt->SetArrowTransparency(alpha);
    }
}
 
/* todo : utils.cpp */
std::string Console::ToString(const std::wstring& sText)
{
    std::string text = "";
 
    text.assign(sText.begin(), sText.end());
 
    return text;
}
 
std::wstring Console::ToWString(const std::string& sText)
{
    std::wstring text = L"";
 
    text.assign(sText.begin(), sText.end());
 
    return text;
}
 
unsigned int Console::GetWidth()
{
    return m_fConsoleWidth;
}
 
bool Console::TextIsOutOfWidth(const std::wstring sText)
{
    // On applique un certain espace entre les bords de la console
    unsigned int iTotalWidth = GetStringLength(sText) + CONSOLE_LINE_BEGINNING_WIDTH * 4;
 
    return iTotalWidth >= GetWidth();
}
 
void Console::ResetScrollingPosition()
{
    if (m_lines.size() > m_iMaxLinesDisplayed)
    {
        for (int i = 0; i < m_lines.size(); i++)
        {
            if (m_iLinesOffset > 0)
            {
                m_iLinesOffset--;
                m_iLinesStart++;
            }
        }   
    }
}
 
 
unsigned int Console::GetCharactWidth()
{
    return m_fCharWidth;
}
 
void Console::DeleteNextCharacter()
{
    unsigned int iPromptCursorPos = m_pPrompt->GetCursorPos();
 
    std::wstring sCurrentCommandLine = m_pPrompt->GetCommandLineText();
 
    std::wstring sFirstPart = sCurrentCommandLine.substr(0, iPromptCursorPos);
    std::wstring sLastPart = sCurrentCommandLine.substr(iPromptCursorPos, sCurrentCommandLine.length());
 
    if (sFirstPart.length() >= 0)
    {
        if (sLastPart.length() > 0 )
        {   
            sLastPart.erase(0, 1);
            m_pPrompt->SetText(sFirstPart + sLastPart);  
        }
    }
}
 
void Console::DeletePreviousCharacter()
{       
    unsigned int iPromptCursorPos = m_pPrompt->GetCursorPos();
 
    std::wstring sCurrentCommandLine = m_pPrompt->GetCommandLineText();
 
    std::wstring sFirstPart = sCurrentCommandLine.substr(0, iPromptCursorPos);
    std::wstring sLastPart = sCurrentCommandLine.substr(iPromptCursorPos, sCurrentCommandLine.length());
 
    if (sFirstPart.length() > 0)
    {
        if (sLastPart.length() > 0 )
        {   
            sFirstPart.pop_back();
            m_pPrompt->SetText(sFirstPart + sLastPart);  
            m_pPrompt->MoveCursorToLeft();
        }
        else if (sCurrentCommandLine.length() > 0)
        {
            sCurrentCommandLine.pop_back();
            m_pPrompt->SetText(sCurrentCommandLine);
            m_pPrompt->MoveCursorToLeft();
        }
    }
}
 
bool Console::ChangeBackgroundImage(const std::wstring& sBgFile)
{
    if (Utils::IsFileReadable(sBgFile))
    {
        delete m_pBackgroudImage;
 
        m_pBackgroudImage = new Sprite2D(D3D10_RENDERER->GetDevice(), sBgFile, m_fConsoleWidth, m_iTotalConsoleHeight);
 
        return true;
    }
    else
    {
        return false;
    }
}
