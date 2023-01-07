//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : String.h
// Date de création : Octobre 2015
// Description : Classe générique pour gérer les
// différents types de chaînes de caractères
// 
// Supporte les type char et wchar_t
//----------------------------------------------------

#ifndef STRING_H
#define STRING_H


namespace Core
{
	template <typename T>
	class String : public Array<T>
	{
	public:
		explicit String(String<T> sText)
		{
			for (uint32 i = 0; i < sText.Length(); i++)
			{
				T c = sText[i];

				PushBack(c);
			}
		}

		explicit String(T* sText)
		{
			uint16 iCount = 0;

			while (sText[iCount] != nullptr)
			{
				T c = sText[i];

				PushBack(c);

				iCount++;
			}
		}

		explicit String(T c)
		{
			PushBack(c);
		}

		String<T>& operator += (T c)
		{
			PushBack(c);

			return *this;
		}

		String<T>& operator += (String<T> sText)
		{
			for (uint32 i = 0; i < sText.Length(); i++)
			{
				T c = sText[i];

				PushBack(c);
			}

			return *this;
		}

		uint32 Length()
		{
			return Size();
		}

		String<T> MakeUpper()
		{
			for (uint32 i = 0; m_aData[i] != nullptr; i++)
			{
				uint32 c = m_aData[i];

				m_aData[i] = (c >= 'a' && c <= 'z' ? c + ( 'A' - 'a' ) : c);
			}

			return *this;
		}

		String<T> MakeLower()
		{
			for (uint32 i = 0; m_aData[i] != nullptr; i++)
			{
				uint32 c = m_aData[i];

				m_aData[i] = (c >= 'A' && c <= 'Z' ? c + 0x20 : c);
			}

			return *this;
		}

		String<T> SubString(uint32 iBegin, int32 iLength) const
		{
			// Paramètres incorrects
			if (iLength <= 0) || iBegin >= Size())
			{
				return String<T>("");
			}
				
			// On restreint l'intervalle
			if (iLength + iBegin > Size())
			{
				iLength = Size() - iBegin;
			}

			String<T> other;

			other.Reallocate(iLength + 1);

			for (int32 i = 0; i < iLength; i++)
			{
				other.m_aData[i] = m_aData[i + iBegin];
			}

			other.m_aData[length] = nullptr;

			return other;
		}

		bool operator==(const String<T> other) const
		{
			for (uint32 i = 0; m_aData[i] != nullptr && other.m_aData[i] != nullptr; i++)
			{
				if (m_aData[i] != other.m_aData[i])
				{
					return false;
				}
			}

			return Size() == other.Size();
		}

		T ToChar(uint32 iValue)
		{
			T* buf = GEM_MEMORY_MANAGER->Alloc(sizeof(char));

			itoa(iValue, (char*) buf, 10);

			return buf[0];
		}

		T ToChar(float fValue)
		{
		}

		String<T> ToString(uint32 iValue)
		{
			uint8 n = iValue;
			uint8 iDigitCount = 0;

			while (n != 0)
			{
				n /= 10;
				iDigitCount++;
			}

			T* buf = GEM_MEMORY_MANAGER->Alloc(sizeof(T) * iDigitCount);

			itoa(iValue, (char*)buf, 10);

			return String<T>(buf);
		}

		String<T> ToString(float fValue)
		{
		}
	};

};

#endif