//----------------------------------------------------
// Auteur : Cl�ment Profit
// Nom du fichier : InputManager.h
// Date de cr�ation : Octobre 2015
// Description : Classe pour g�rer les entr�es clavier
// et souris du syst�me
//----------------------------------------------------

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Singleton.h"
#include "Defines.h"

template<typename CharType>
class KeyEvent
{
public:
	KeyEvent(const int kc, const CharType txt) : key(kc), text(txt) {}
	virtual ~KeyEvent() {}

	// L'ID de la touche
	const int key;
	// La caract�re correspondant de la touche
	const wchar_t CharType;
};

template<typename CharType>
class KeyListener
{
public:	
	KeyListener() : m_bInputEnabled(true) {};

	virtual ~KeyListener() {}

	virtual void RepetitiveKeyPressed(const KeyEvent<CharType> &arg) {};
	virtual void RepetitiveKeyReleased(const KeyEvent<CharType> &arg) {};

	virtual void SoloKeyPressed(const KeyEvent<CharType> &arg) = 0;
	virtual void SoloKeyReleased(const KeyEvent<CharType> &arg) = 0;

	void EnableInput() { m_bInputEnabled = true; }
	void DisableInput() { m_bInputEnabled = false; }

	bool IsInputEnabled() { return m_bInputEnabled; }

private:
	bool m_bInputEnabled;

};

class InputManager : public Singleton<InputManager>
{
public:
	enum KeyModifier
	{
		SHIFT = BIT(0),
		CTRL  = BIT(1),
		ALT   = BIT(2)
	};

	InputManager();
	virtual ~InputManager();

	bool Initialize();

	void Update();

	void GetMouseLocation(int& mouseX, int& mouseY);

	bool IsKeyDown(const char key);
	bool IsMouseButtonDown(const unsigned int button);
	bool IsModifierDown(const KeyModifier modifier);

	void SetBuffered(const bool bBuffered);

	// Enregistre le receveur de touches clavier - ou Add??
	void SetKeyListener(const KeyListener* pKeyListener);

	// Renvoie la signification de la cl�, ex : "F1" pour la touche F1
	std::string GetKeyCodeAsString(const char key);

	// Renvoie la touche correspondante � la cl�
	wchar_t GetKeyCodeAsWChar(const char key);

private:	
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	void UpdateBuffered();
	void UpdateNonBuffered();

private:
	uint8 m_aKeyboardState[256];

	uint16 m_iScreenWidth;
	uint16 m_iScreenHeight;

	uint16 m_iMouseX;
	uint16 m_iMouseY;

	wchar_t m_deadkey;

	bool m_bBufferedMode;

	uint8 m_iModifiers;

	Core::Map<uint8, bool> m_pressedKeys;
 
	/* Pourquoi ici un unique objet ? 
	   Car on a besoin que d'un seul KeyListener car 
	   c'est soit la console, soit la messagerie qui ont besoin des entr�es clavier 
	   � la fois */
	KeyListener* m_pKeyListener;
};

#endif