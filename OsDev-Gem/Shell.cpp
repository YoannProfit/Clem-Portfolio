#include "Shell.h"

template<> Shell* Singleton<Shell>::ms_instance = nullptr;
 
Shell::Shell() :

	m_iCurrentXPos(0),
	m_iCurrentYPos(0),
	m_currentForeground(Core::Color::Black),
	m_currentBackground(Core::Color::White),
	m_pPrompt(nullptr),
	m_pHistory(nullptr),
	m_aLegalChars(GEM_INPUT_LEGAL_CHARS),
	m_currentMessageType(MessageType::MSG_SYSTEM),
	m_bPromptVisible(true),
	m_iMaxLinesDisplayed(0),
	m_iMaxCharDisplayed(0),
	m_pCurrentLine(nullptr)
	
{
	// On réserve une ligne pour le prompt "[Bash]$"
	SetMaxLinesDisplayed(GEM_TEXT_VIDEO_MEMORY_HEIGHT);
	SetMaxCharDisplayed(GEM_TEXT_VIDEO_MEMORY_WIDTH);

	m_pPrompt = new CommandLinePrompt;
	m_pHistory = new CommandsHistory;
}

Shell::~Shell()
{
	ClearLines();

	m_aBlinkingElements.Clear();
 
	m_iLinesOffset = 0;
	m_iLinesStart = 0;

	SAFE_DELETE(m_pPrompt);
	SAFE_DELETE(m_pHistory;
}

void Shell::ClearLines()
{
    m_iCurrentXPos = 0;
    m_iCurrentYPos = 0;

	m_aLines.Clear();

	ResetScrolling();

	UpdateCursor();
}

void Shell::UpdateCursor()
{
	uint16 iCursorLocation = m_iCurrentYPos * GEM_VIDEO_MEMORY_HEIGHT + m_iCurrentXPos;

	GEM_IO_PORTS->OutputByte(0x3D4, 14);   
	GEM_IO_PORTS->OutputByte(0x3D5, iCursorLocation >> 8); 

	GEM_IO_PORTS->OutputByte(0x3D4, 15);                  
	GEM_IO_PORTS->OutputByte(0x3D5, iCursorLocation);     
}

void Shell::ScrollUp()
{
    if (m_iLinesStart > 0)
    {
        m_iLinesOffset++;
        m_iLinesStart--;
    }
}

void Shell::ScrollDown()
{
   if (m_iLinesOffset > 0)
    {
        m_iLinesOffset--;
        m_iLinesStart++;
    }
}

void Shell::AddBlankLine()
{
	Line* pLine = new Line(MSG_BLANK);

	pLine->AddTextElement("", m_currentForeground, m_currentBackground);

	AddLine(pLine);

	UpdateLines();
}

void Shell::SetPrintColor(Core::Color foreground, Core::Color background)
{
	m_currentForeground = foreground;
	m_currentBackground = background;
}

void Shell::SetPrintType(MessageType type);
{
	m_currentMessageType = type;
}

void Shell::AddLine(const Line* pLine)
{
	AssertNullPointer(pLine);

    m_aLines.PushBack(pLine);
 
    // On décale toutes les lignes
    if (m_aLines.Size() > m_iMaxLinesDisplayed - 1)
    {
        m_iLinesStart++;
    }

	UpdateLines();
}

void Shell::Print(const CharType c) 
{
	Line* pLine = new Line(m_currentMessageType);

	StringType sCharacter = "";
	sCharacter.PushBack(c);

	pLine->AddTextElement(sCharacter, m_currentForeground, m_currentBackground);

	AddLine(pLine);

	UpdateLines();
}

void Shell::Print(const CharType* sText)
{
	Line* pLine = new Line(m_currentMessageType);

	StringType sCharacter(sText);

	pLine->AddTextElement(sCharacter, m_currentForeground, m_currentBackground);

	AddLine(pLine);

	UpdateLines();
}

void Shell::Print(const StringType& sText)
{
	Line* pLine = new Line(m_currentMessageType);

	pLine->AddTextElement(sText, m_currentForeground, m_currentBackground);

	AddLine(pLine);

	UpdateLines();
}

void Shell::Print(const StringType& sMessage, Core::Colour foregroundColour, 
				  Core::Colour backgroundColour, MessageType type)
{
	Line* pLine = new Line(m_currentMessageType);

	pLine->AddTextElement(sText, foregroundColour, backgroundColour);

	AddLine(pLine);

	UpdateLines();
}

void Shell::PrintInt(uint32 iValue)
{
	StringType sValue = StringType::ToString(iValue);

	Line* pLine = new Line(m_currentMessageType);

	pLine->AddTextElement(sValue, m_currentForeground, m_currentBackground);

	AddLine(pLine);

	UpdateLines();
}

void Shell::PrintFloat(float fValue)
{
	StringType sValue = StringType::ToString(fValue);

	Line* pLine = new Line(m_currentMessageType);

	pLine->AddTextElement(sValue, m_currentForeground, m_currentBackground);

	AddLine(pLine);

	UpdateLines();
}

void Shell::AddCharacter(uint32 iValue)
{
	CharType sValue = StringType::ToChar(iValue);

	if (m_pCurrentLine  == nullptr)
	{
		m_pCurrentLine = new Line(m_currentMessageType);
	}

	m_pCurrentLine->AddTextElement(sValue, m_currentForeground, m_currentBackground);

	UpdateLines();
}

void Shell::AddCharacter(const float fValue)
{
	CharType sValue = StringType::ToChar(fValue);

	if (m_pCurrentLine  == nullptr)
	{
		m_pCurrentLine = new Line(m_currentMessageType);
	}

	m_pCurrentLine->AddTextElement(sValue, m_currentForeground, m_currentBackground);

	UpdateLines();
}

void Shell::AddCharacter(CharType c)
{
	StringType sValue(c);

	if (m_pCurrentLine  == nullptr)
	{
		m_pCurrentLine = new Line(m_currentMessageType);
	}

	m_pCurrentLine->AddTextElement(sValue, m_currentForeground, m_currentBackground);

	UpdateLines();
}

void Shell::AddCharactersLine()
{
	AddLine(m_pCurrentLine);

	m_pCurrentLine = nullptr;
}

void Shell::DisplayCharacter(const uint32 iPosX, const uint32 iPosY, const CharType c, 
			const Core::Color foreground, const Core::Color background)
{
 	GEM_VIDEO_MEMORY->PutCharacter(iPosX, iPosY, c, foreground, background);
}

void Shell::DrawLine(Line* pLine)
{
    Core::Array<TextElement<StringType>*> elems;
    pLine->GetTextElements(elems);

	for (uint16 j = 0; j < elems.size(); j++)
	{
		TextElement* pElement = elems[j];

		Core::Color foreground = pElement->GetForegroundColor();
		Core::Color background = pElement->GetBackgroundColor();

		StringType sText = pElement->GetText();

		bool bBlinking = pElement->IsBlinking();

		for (uint16 k = 0; k < sText.Length(); k++)
		{
			const CharType c = sText[k];

			PutCharacter(m_iCurrentXPos, m_iCurrentYPos, c, foreground, background);

			m_iCurrentPosX++;

			if (bBlinking)
			{
				VideoMemory::TextElement elem = VideoMemory::MakeTextElement(m_iCurrentPosX, m_iCurrentPosY,
					c, foreground, background);

				m_aBlinkingElements.PushBack(elem);
			}

			AssertMsg(m_iCurrentPosX >= m_iMaxCharDisplayed, "Texte dépassant la largeur !");
		}
	}

	m_iCurrentPosX = 0;
}

void Shell::RefreshLines()
{
	m_iCurrentPosX = 0;
	m_iCurrentPosY = 0;

	m_aBlinkingElements.Clear();

	Core::Array<Line*> aLines;

	if (IsLinesHooked())
	{
		aLines = m_aHookedLines;

		/* On force le non-défilement quand on remplace les lignes.
		   Le défilement est donc réinitialisé quand on remet les
		   anciennes lignes.
		*/
		ResetScrolling();
	}
	else
	{
		aLines = m_aLines;
	}

	for (uint16 i = m_iLinesStart; i < aLines.Size() - m_iLinesOffset; i++)
    {
		Line* pLine = aLines[i];

		DrawLine(pLine);

		m_iCurrentPosY++;

		AssertMsg(m_iCurrentPosY >= m_iMaxLinesDisplayed, "Texte dépassant la hauteur !");
	}

	if (IsPromptVisible())
	{
		// On met à jour le prompt de la ligne de commande
		DrawLine( m_pPrompt->GetLine() );
	}

	UpdateCursor();
}
	

void Shell::ResetScrolling()
{
    if (m_aLines.Size() > m_iMaxLinesDisplayed - 1)
    {
        for (uint16 i = 0; i < m_aLines.Size() ; i++)
        {
            if (m_iLinesOffset > 0)
            {
                m_iLinesOffset--;
                m_iLinesStart++;
            }
        }    
    }
}

void Shell::SoloKeyPressed(const KeyEvent<CharType>& arg)
{
    if (!IsVisible())
    {
        return;
    }

	if (IsCharAuthorized(arg.text))
	{
		StringType sText(arg.text);

		m_pPrompt->AppendCommandLineText(sText);

		UpdatePrompt(c);
	}	

	// Touche entrée
    if (arg.key == GEM_KEY_RETURN)
    {
        if (!m_pPrompt->HasText())
        {
            return;
        }

		// todo : faire appel au programme correspondant à la commande
        ResetScrolling();
	}
}

void Shell::SoloKeyReleased(const KeyEvent<CharType>& arg)
{
}

bool Shell::IsCharAuthorized(const CharType key)
{
    for (uint8 c = 0; c < m_aLegalChars.Size(); c++)
    {
        if (m_aLegalChars[c] == key)
        {
            return true;
        }
    }
 
    return false;
}


const StringType Shell::GetMessageType(const MessageType type)
{
    StringType messageType = "";
 
    switch (type)
    {
        case MSG_SYSTEM: messageType = "[SYSTEM]"; break;
        case MSG_WARNING: messageType = "[WARNING]"; break;
        case MSG_ERROR: messageType = "[SYS_ERROR]"; break;
        case MSG_DEBUG: messageType = "[SYS_DEBUG]"; break;
        case MSG_COMMAND: messageType = "[COMMAND]"; break;
        case MSG_BLANK: messageType = ""; break;
    }
 
    return messageType;
}

const Core::Colour Shell::GetMessageColour(const MessageType type)
{
    Core::Colour messageColour  = Core::Colour::White;
 
    switch (type)
    {
		case MSG_SYSTEM: messageColour = Core::Colour::Cyan; break;
        case MSG_WARNING: messageColour = Core::Colour::Blue; break;
        case MSG_ERROR: messageColour = Core::Colour::Red; break;
        case MSG_DEBUG: messageColour = Core::Colour::Grey; break;
        case MSG_COMMAND: messageColour = Core::Colour::White; break;
        case MSG_BLANK: messageColour = Core::Colour::Yellow; break;
    }
 
    return messageColour;
}


void Shell::UpdateBlinkingElements()
{
    if (!IsVisible())
    {
        return;
    }


}

void Shell::ShowPrompt()
{
	m_bPromptVisible = true;
}

void Shell::HidePrompt()
{
	m_bPromptVisible = false;
}

bool Shell::IsPromptVisible()
{
	return m_bPromptVisible;
}

/* Le shell ne gère que des lignes remplacées sans défilement 
   (c'est-à-dire sans pouvoir faire de scroll) */
void Shell::HookLines(const Core::Array<Line*>& aLines)
{
	// On calcule le nombre de lignes en trop
	uint32 iTooManyLinesCount = (aLines.Size() - m_iMaxLinesDisplayed) - 1;

	// On vérifie s'il y en a
	if (iTooManyLinesCount > 0)
	{
		// Si oui, on enlève les lignes les plus anciennes
		for (uint32 i = 0; i < iTooManyLinesCount; i++)
		{
			aLines.PopFront();
		}
	}

	m_aHookedLines = aLines;
}

void Shell::ClearHookedLines()
{
	m_aHookedLines.Clear();
}

bool Shell::IsLinesHooked()
{
	return m_aHookedLines.Size() > 0;
}

uint32 Shell::GetMaxCharDisplayed()
{
	return m_iMaxCharDisplayed;
}

void Shell::SetMaxLinesDisplayed(uint32 iMaxLinesDisplayed)
{
	m_iMaxLinesDisplayed = iMaxLinesDisplayed;
}

void Shell::SetMaxCharDisplayed(uint32 iMaxCharDisplayed)
{
	m_iMaxCharDisplayed = iMaxCharDisplayed;
}

void Shell::TabComplete()
{    
    StringType sCurrentCommandLine = m_pPrompt->GetCommandLineText();
 
	Core::Array<StringType> cmds;
    CONSOLE_COMMANDS_MANAGER->GetCommandsNames(cmds);
     
    // On stocke toutes les commandes qui commence par le même début que la saisie
    // et ceci lorsque la liste est vide
    if (sCurrentCommandLine.Length() > 0 && m_tabHistory.Empty())
    {
        for (int i = 0; i < cmds.Size(); i++)
        {
             StringType cmd = cmds[i];
 
            if (cmd.Size() > sCurrentCommandLine.Size())
            {
                if (cmd.SubString(0, sCurrentCommandLine.Size()) == sCurrentCommandLine)
                {
                    m_tabHistory.PushBack(cmd);
                }
            }
        }        
    }
    // Sinon on stocke toutes les commandes existantes
    else
    {
        for (int i = 0; i < cmds.Size(); i++)
        {
            StringType cmd = cmds[i];
 
            m_tabHistory.PushBack(cmd);
        }
    }
 
    // On échange les commandes de manière circulaire dans la liste
    if (!m_tabHistory.Empty())
    {
        StringType command = m_tabHistory.Front();
 
        m_tabHistory.PopFront();
        m_tabHistory.Pushback(command);
 
        m_pPrompt->SetText(command);
    }
}

uint16 Shell::GetDisplayedLinesCount()
{
	return m_aLines.Size();
}

void Shell::GetCommandLineArguments(const StringType& sCommandLine, DataParameters<CharType, StringType>& param);
{
	StringType sArgument = "";
    uint16 iArgumentCount = 0;
 
    for (uint16 c = 0; c < sCommandLine.Length(); c++)
    {
        if (sCommandLine == ' ')
        {
            if (sArgument.Length() > 0)
            {
                param.SetParam(iArgumentCount, sArgument);
 
                iArgumentCount++;
                sArgument = "";
            }
        }
        else
        {
            sArgument += sCommandLine;
        }
    }
 
    if (sArgument.Length() > 0)
    {
        param.setParam(iArgumentCount, sArgument);
    }
}

void Shell::Clear()
{
	GEM_VIDEO_MEMORY->Clear();
}

// ------------------ TextElement ---------------- //

TextElement::TextElement(StringType sTextElementText, Core::Colour colour, bool bBlinking) :
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
    return m_sTextElement.Length();
}
 
StringType TextElement::GetText()
{
    return m_sTextElement;
}
 
Core::Colour TextElement::GetColour()
{
    return m_colour;
}
 
void TextElement::SetColour(Core::Colour colour)
{
    m_colour = colour;
}
 
bool TextElement::IsBlinking()
{
    return m_bBlinking;
}
 
// ------------------ Line ---------------- //

Line::Line(MessageType type, Shell* pShellOwner) :
m_type(type),
m_pShellOwner(pShellOwner)
{
	AssertNullPointer(m_pShell);

    // Ajoute le type de message (par ex [Game]) au début de la ligne
    AddTextElementSolo(StringType::ToWString(m_pShellOwner->GetMessageType(m_type)), m_pShellOwner->GetMessageColour(m_type), false);
}
 
Line::~Line()
{
	m_textElements.Clear();
}
 
uint16 Line::GetTextElementsCount()
{
    return m_textElements.Size();
}

/* Ajoute un élément textuel et rajoute des 
   lignes sur le texte spécifié dépasse l'écran */
void Line::AddTextElement(StringType sTextElement, Core::Colour colour, bool bBlinking)
{
    // On segmente la ligne
    Core::Array<StringType> moreLines;
    ParseLines(sTextElement, moreLines);
 
    for (uint32 i = 0; i < moreLines.Size(); i++)
    {
        StringType sCurrentString = moreLines[i];
 
        Line* pLine = new Line(m_type);
 
        pLine->AddTextElementSolo(pCurrentString, colour, bBlinking);
 
        m_pShellOwner->AddLine(pLine);
    }    
}
 
void Line::AddTextElementSolo(StringType sTextElement, Core::Colour colour, bool bBlinking)
{
    TextElement* pTextElement = new TextElement(sTextElement, colour, bBlinking);

    m_textElements.PushBack(pTextElement);
}
 
/* Analyse une ligne et la découpe en plusieurs lignes si le nombre
   de caractères de la ligne passé dépasse un certain seuil */
void Line::ParseLines(StringType sText, Core::Array<StringType>& moreLines)
{
    const CharType* str = sText.Pointer();
    StringType sLine = L"";
 
    for (uint16 c = 0; c < sText.Length(); c++)
    {    
        if (str == '\n' || sLine.Length() > m_pShell->GetMaxCharDisplayed())
        {
            moreLines.push_back(line);
            sLine = L"";
        }
 
        if (str != '\n')
		{
            sLine += str;
		}
    }
 
    if (sLine.Length() > 0)
    {
        moreLines.PushBack(sLine);
    }
}
 
void Line::GetTextElements(Core::Array<TextElement*>& elem)
{
    elem = m_textElements;
}
 
MessageType Line::GetType()
{
    return m_type;
}

void Line::Clear()
{
	m_textElements.Clear();
}

// ------------------ CommandsHistory ---------------- //

CommandsHistory::CommandsHistory()
{
}
 
CommandsHistory::~CommandsHistory()
{
    m_aHistory.Clear();
}
 
bool CommandsHistory::Empty()
{
    return m_aHistory.Size() == 0;
}
 
const StringType CommandsHistory::GetPrevious()
{
    if (!Empty())
    {
        const StringType sCommandName = m_aHistory.Back();
 
        m_aHistory.PushFront(sCommandName);
        m_aHistory.PopBack();
 
        m_sLastCommand = sCommandName;
 
        return sCommandName;
    }
    else
    {
        return GEM_EMPTY;
    }
}
 
const StringType CommandsHistory::GetNext()
{
    if (!Empty())
    {
        const StringType sCommandName = m_aHistory.Front();
 
        m_aHistory.PushBack(sCommandName);
        m_aHistory.PopFront();
         
        m_sLastCommand = sCommandName;
 
        return sCommandName;
    }
    else
    {
        return GEM_EMPTY;
    }
}
 
void CommandsHistory::Clear()
{
    m_aHistory.Clear();
}
 
void CommandsHistory::AddCommand(const StringType& sCommandName)
{
    m_aHistory.Remove(sCommandName);
    m_aHistory.PushBack(sCommandName);
 
    if (m_aHistory.Size() > GEM_SHELL_MAX_COMMAND_HISTORY)
    {
        m_aHistory.PopFront();
    }
}
 
// ------------------ CommandLinePrompt ---------------- //

CommandLinePrompt::CommandLinePrompt() :
m_sPromptText(GEM_SHELL_PROMPT_TEXT),
m_iCursorPos(0),
m_pLine(nullptr)
{
	m_pLine = new Line(MSG_BLANK); // todo incorporer le texte du prompt directement
}
 
CommandLinePrompt::~CommandLinePrompt()
{
}
 
void CommandLinePrompt::AppendCommandLineText(const StringType& sText)
{
	uint32 iPromptCursorPos = GetCursorPos();
 
    StringType sCurrentCommandLine = GetCommandLineText();
 
    StringType sFirstPart = sCurrentCommandLine.SubString(0, iPromptCursorPos);
    StringType sLastPart = sCurrentCommandLine.SubString(iPromptCursorPos, sCurrentCommandLine.Length());
     
    if (sLastPart.Length() > 0)
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
    
void CommandLinePrompt::SetText(const StringType& sText)
{
    m_sCommandLineText = sText;
}
 
void CommandLinePrompt::Clear()
{
    m_sCommandLineText = "";
    m_iCursorPos = 0;
}
 
bool CommandLinePrompt::HasText()
{
    return m_sCommandLineText.Length() > 0;
}
 
StringType CommandLinePrompt::GetPromptText()
{
    return m_sPromptText;
}
 
StringType CommandLinePrompt::GetCommandLineText()
{
    return m_sCommandLineText;
}

StringType CommandLinePrompt::GetText()
{
	return m_sPromptText + m_sCommandLineText;
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
    uint32 iPromptXOffset = m_sCommandLineText.Length();
 
    if (GetCommandLengthPosition() < iPromptXOffset)
    {
        m_iCursorPos++;
    }
}

void CommandLinePrompt::ResetCursorPos()
{
    m_iCursorPos = m_sCommandLineText.Length();
}

uint32 CommandLinePrompt::GetCursorPos()
{
	return m_iCursorPos;
}

void CommandLinePrompt::DeleteNextCharacter()
{
	uint32 iPromptCursorPos = GetCursorPos();
 
    StringType sCurrentCommandLine = GetCommandLineText();
 
    StringType sFirstPart = sCurrentCommandLine.SubString(0, iPromptCursorPos);
    StringType sLastPart = sCurrentCommandLine.SubString(iPromptCursorPos, sCurrentCommandLine.Length());L
 
    if (sFirstPart.Length() >= 0)
    {
        if (sLastPart.Length() > 0 )
        {    
            sLastPart.Erase(0, 1);

            SetText(sFirstPart + sLastPart);    
        }
    }
}
 
void CommandLinePrompt::DeletePreviousCharacter()
{        
    uin32 iPromptCursorPos = GetCursorPos();
 
    StringType sCurrentCommandLine = GetCommandLineText();
 
    StringType sFirstPart = sCurrentCommandLine.SubString(0, iPromptCursorPos);
    StringType sLastPart = sCurrentCommandLine.SubString(iPromptCursorPos, sCurrentCommandLine.Length());
 
    if (sFirstPart.Length() > 0)
    {
        if (sLastPart.Length() > 0 )
        {    
            sFirstPart.PopBack();

            SetText(sFirstPart + sLastPart);    

            MoveCursorToLeft();
        }
        else if (sCurrentCommandLine.length() > 0)
        {
            sCurrentCommandLine.PopBack();
            SetText(sCurrentCommandLine);

            MoveCursorToLeft();
        }
    }
}

Line* CommandLinePrompt::GetLine()
{
	AssertNullPointer(m_pLine);

	m_pLine->Clear();

	m_pLine->AddTextElementSolo( GetPromptText(), Core::Colour::White);

	// On fait clignoter le caractère '$' du shell
	m_pLine->AddTextElementSolo('$', Core::Colour::Orange, true);

	m_pLine->AddTextElementSolo( GetCommandLineText(), Core::Colour::Yellow);

	return m_pLine;
}
