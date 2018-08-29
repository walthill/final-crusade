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
	else if (lang == FRENCH)
	{
		ifstream fileStream(mFRE);
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
					mFrLanguageTable[key] = value;
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
	else if (mCurrentLanguage == FRENCH)
	{
		result =mFrLanguageTable[key];
	}
	return result;
}

void Language::setLangauge(LangType lang)
{
	mCurrentLanguage = lang;
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
	else if(mCurrentLanguage==FRENCH)
	{
		return "French";
	}

	return "null";
}