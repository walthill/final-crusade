#include "Language.h"

Language::Language()
{
	mCurrentLanguage = ENGLISH;
}
Language::~Language()
{
}

void Language::loadLanguage(LangType lang)
{
	
	//map<string, string> table;
	if (lang == ENGLISH)
	{
		ifstream fileStream(mENG);

		if (fileStream.good())
		{
			while (!fileStream.eof())
			{
				string key;
				string value;
				getline(fileStream, key);
				getline(fileStream, value);
				if (key != "")
				{
					mEngLanguageTable[key] = value;
				}
			}
		}
	}
	else if (lang == SPANISH)
	{
		ifstream fileStream(mESP);
		if (fileStream.good())
		{
			while (!fileStream.eof())
			{
				string key;
				string value;
				getline(fileStream, key);
				getline(fileStream, value);
				if (key != "")
				{
					mEspLanguageTable[key] = value;
				}
			}
		}
	}
	else if (lang == SWAHILI)
	{
		ifstream fileStream(mSWA);
		if (fileStream.good())
		{
			while (!fileStream.eof())
			{
				string key;
				string value;
				getline(fileStream, key);
				getline(fileStream, value);
				if (key != "")
				{
					mSwaLanguageTable[key] = value;
				}
			}
		}
	}
	
}

string Language::getTranslation(string key)
{
	string result = "null";

	if (mCurrentLanguage == ENGLISH)
	{
		result = mEngLanguageTable[key];
	}
	else if (mCurrentLanguage == SPANISH)
	{

		result = mEspLanguageTable[key];
	}
	else if (mCurrentLanguage == SWAHILI)
	{
		result = mSwaLanguageTable[key];
	}

	return result;
}

void Language::setLangauge(LangType lang)
{
	mCurrentLanguage = lang;
	//changedLang = true;
}

LangType Language::getLanguage()
{
	return mCurrentLanguage;
}

string Language::printLanguage()
{
	if (mCurrentLanguage == ENGLISH)
	{
		return "English";
	}
	else if (mCurrentLanguage == SPANISH)
	{
		return "Spanish";
	}
	else if (mCurrentLanguage == SWAHILI)
	{
		return "Swahili";
	}

	return "null";
}