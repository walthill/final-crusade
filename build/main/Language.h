#ifndef LANG_H
#define LANG_H

#include "Trackable.h"
#include <fstream>
#include <string>
#include <map>

using namespace std;

enum LangType
{
	ENGLISH,
	SPANISH,
	SWAHILI,
	NUM_LANGS
};

class Language : public Trackable
{
	private:
		const string mENG = "local\\eng.txt";
		const string mESP = "local\\esp.txt";
		const string mSWA = "local\\swa.txt";

		LangType mCurrentLanguage;

		map<string, string> mEngLanguageTable;
		map<string, string> mEspLanguageTable;
		map<string, string> mSwaLanguageTable;
	public:
//		bool changedLang;

		Language();
		~Language();

		void loadLanguage(LangType lang);
		
		string getTranslation(string key);

		void setLangauge(LangType lang);
		LangType getLanguage();
		string printLanguage();

};

#endif // !LANG_H