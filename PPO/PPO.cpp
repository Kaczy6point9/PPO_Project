// PPO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

string nazwa_pliku;

class RECORD
{
public:
	
	virtual void wypisz()
	{
		cout << id << endl;
	}; 
	
	virtual string toText(string line){ return "0"; };
protected:
	
	string id;
	RECORD(string a)
	{
		id = a;
		
	}
	
};

class CoordinateRECORD : public RECORD
{

protected:
	CoordinateRECORD(string nazwa) : RECORD(nazwa){};
};

class ModelRECORD : public CoordinateRECORD
{

protected:
	string serial;

public:
	ModelRECORD(string s) : serial(s), CoordinateRECORD("MODEL "){};

	void wypisz()
	{

		cout << id << " " << serial << endl;

	}

	string toText(string line)
	{
		line = id + " ";
		line += serial;
		return line;
	}


};

class TerRECORD : public CoordinateRECORD
{

protected:

	string serial;
	string resName;
	string chainID;
	string	 resSeq;
	string iCode;


public:

	TerRECORD(string s, string resN, string chain, string resS, string iC) :serial(s), resName(resN), chainID(chain), resSeq(resS), iCode(iC), CoordinateRECORD("TER   "){};

	void wypisz()
	{

		cout << id << " " << serial << resName << " " << chainID << resSeq << iCode <<  endl;

	}

	string toText(string line)
	{
		line = id + " ";
		line += serial;
		line += " ";
		line += resName;
		line += " ";
		line += chainID;
		line += resSeq;
		line += iCode;
		return line;
	}

};

class EndmdlRECORD : public CoordinateRECORD
{

protected:



public:

	EndmdlRECORD() : CoordinateRECORD("ENDMDL"){};

	void wypisz()
	{

		cout << id << endl;

	}

	string toText(string line)
	{
		line = id;
		
		return line;
	}

};

class TitleSRECORD : public RECORD
{
protected:
	TitleSRECORD(string nazwa) :RECORD(nazwa){};
};

class AuthorRECORD : public TitleSRECORD
{
protected:


	string continuation;
	string AuthorList;


public:

	AuthorRECORD(string con, string AL) : continuation(con), AuthorList(AL), TitleSRECORD("AUTHOR"){};

	void wypisz()
	{

		cout << id << " " << continuation << AuthorList << endl;

	}
	string toText(string line)
	{
		line = id + " ";
		line += continuation;
		line += AuthorList;
		return line;
	}
};

class RemarkRECORD : public TitleSRECORD
{
protected:

	string remNum;
	string coment;

	


public:

	RemarkRECORD(string remN, string com) :remNum(remN), coment(com), TitleSRECORD("REMARK"){};

	void wypisz()
	{

		cout << id << " " << remNum << " "<< coment <<  endl;

		
	}
	
	string toText(string line)
	{
		line = id + " ";
		line += remNum;
		line += " ";
		line += coment;
		return line;
	}

};

class TitleRECORD : public TitleSRECORD
{

protected:

	string continuation;
	string title;



public:

	TitleRECORD(string con, string t) : continuation(con), title(t), TitleSRECORD("TITLE "){};

	void wypisz()
	{

		cout << id << continuation << " " << title << endl;

	}

	string toText(string line)
	{
		line = id + continuation;
		line += " ";
		line += title;
		return line;
	}
};


class PDB
{

private:
	vector <RECORD *> pdb;

public:
	
	void wrzucnawektor(RECORD *m)
	{
		pdb.push_back(m);
	}
	vector <RECORD *> ::iterator it;
	void wypisz()
	{
		int linia = 0;
		for (it = pdb.begin(); it != pdb.end(); ++it)
		{
			(*it)->wypisz();
			
		}
	}

	void zapisz()
	{
		//nazwa_pliku = "Z" + nazwa_pliku;
		nazwa_pliku = "C:/Users/Kaczy/Documents/Visual Studio 2013/Projects/PPO/zmienione.pdb";
		ofstream f;
		f.open(nazwa_pliku.c_str());
		string line;
		for (it = pdb.begin(); it != pdb.end(); ++it)
		{
			line = (*it)->toText(line);
			f << line << endl;
		

		}
		f.close();
	}

};


int _tmain(int argc, _TCHAR* argv[])
{
	

	PDB Rekordy;
	
	fstream f;
	cout << "Podaj nazwe pliku do odczytu" << endl;
	//nazwa_pliku += ".pdb";
	cin >> nazwa_pliku;
	nazwa_pliku = "C:/Users/Kaczy/Documents/Visual Studio 2013/Projects/PPO/" + nazwa_pliku;
	f.open(nazwa_pliku.c_str());
	string linia;
	char buffer_lini[81];
	int linienieobs³ugiwane = 0;
	while (getline(f, linia))
	{
		strcpy_s(buffer_lini, linia.c_str());
		if (!strncmp(buffer_lini, "TITLE ", 6))
		{
			char title[70];
			char continuation[3];
			strncpy_s(title, &(buffer_lini[10]),69);
			strncpy_s(continuation, &(buffer_lini[8]), 2);
			TitleRECORD *rekord = new TitleRECORD(continuation, title);
			Rekordy.wrzucnawektor(rekord);
		}
		else if (!strncmp(buffer_lini, "AUTHOR", 6))
		{
			char AL[69];
			char continuation[3];
			strncpy_s(AL, &(buffer_lini[10]), 68);
			strncpy_s(continuation, &(buffer_lini[8]), 2);
			AuthorRECORD *rekord = new AuthorRECORD(continuation, AL);
			Rekordy.wrzucnawektor(rekord);
		}
		else if (!strncmp(buffer_lini, "REMARK", 6))
		{
			char remNUM[4];
			char comment[68];
			strncpy_s(remNUM, &(buffer_lini[7]), 3);
			strncpy_s(comment, &(buffer_lini[11]), 67);
			RemarkRECORD *rekord = new RemarkRECORD(remNUM, comment);
			Rekordy.wrzucnawektor(rekord);
		}
		else if (!strncmp(buffer_lini, "MODEL ", 6))
		{
			char serial[5];
			strncpy_s(serial, &(buffer_lini[10]), 4);
			ModelRECORD *rekord = new ModelRECORD(serial);
			Rekordy.wrzucnawektor(rekord);
		}
		else if (!strncmp(buffer_lini, "ENDMDL", 6))
		{
			EndmdlRECORD *rekord = new EndmdlRECORD();
			Rekordy.wrzucnawektor(rekord);
		}
		else if (!strncmp(buffer_lini, "TER ", 6))
		{
			char serial[6];
			char resName[4];
			char chainID[2];
			char resSeq[5];
			char iCode[2];
			strncpy_s(serial, &(buffer_lini[6]), 4);
			strncpy_s(resName, &(buffer_lini[17]), 3);
			strncpy_s(chainID, &(buffer_lini[21]), 1);
			strncpy_s(resSeq, &(buffer_lini[22]), 4);
			strncpy_s(chainID, &(buffer_lini[23]), 1);
			TerRECORD *rekord = new TerRECORD(serial, resName, chainID, resSeq, iCode);
			Rekordy.wrzucnawektor(rekord);
		}
		else
			linienieobs³ugiwane++;
		
	}
	

	
	Rekordy.zapisz();
	cout << "Ilosc lini nie obsluzonych przez program: " << linienieobs³ugiwane << endl;
	
	
	system("PAUSE");
	return 0;
}
