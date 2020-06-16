#pragma once

#include "HumanCode.h"

using namespace HumanCode;

/*
---------------------
      HCCommon
---------------------
*/

HCCommon::DiapasoneInt::DiapasoneInt(LLINT LeftLimit, LLINT RightLimit){
	this->LeftLimit = LeftLimit;
	this->RightLimit = RightLimit;
}

bool HCCommon::DiapasoneInt::InDiapasone(LLINT ToCheck, bool Strict) {
	if (Strict)
		if (this->LeftLimit <= ToCheck and ToCheck <= this->RightLimit)	return true; else return false;
	else
		if (this->LeftLimit < ToCheck and ToCheck < this->RightLimit)		return true; else return false;
}

HCCommon::DiapasoneDouble::DiapasoneDouble(LLDOUBLE LeftLimit, LLDOUBLE RightLimit) {
	this->LeftLimit = LeftLimit;
	this->RightLimit = RightLimit;
}

bool HCCommon::DiapasoneDouble::InDiapasone(LLDOUBLE ToCheck, bool Strict, USINT ComparisonAccuracy){
	if (Strict){
		if ((this->LeftLimit < ToCheck or HCNumbers::IsDoubleEqual(this->LeftLimit, ToCheck, ComparisonAccuracy)) and
			(this->RightLimit > ToCheck or HCNumbers::IsDoubleEqual(this->RightLimit, ToCheck, ComparisonAccuracy))
			)
			return true; else return false;
	}
	else
		if (this->LeftLimit < ToCheck and ToCheck < this->RightLimit)		return true; else return false;
}

/*
---------------------
	  HCConsole
---------------------
*/

void HCConsole::SetLocale() {
	::setlocale(LC_ALL, "Russian");
}

void HCConsole::Exit(std::string Message) {
	std::cout << Message << "\n";
	std::cout << "\n\nДля выхода нажмите любую кнопку... ";
	_getch();
}

int HCConsole::ChooseMenu(std::string List[], USINT Size, USINT ChooseI, std::string Header) {
	const short int KEY_UP = 72;
	const short int KEY_DOWN = 80;
	char act;
	while (true) {
		system("CLS");
		if (Header != "") {
			std::cout << Header << std::endl;
		}
		for (int i = 0; i < Size; i++) {
			printf("[%i] ", i + 1);
			std::cout << List[i];
			if (i == ChooseI) { printf(" <="); };
			std::cout << std::endl;
		}
		act = _getch();
		if (act == 0 or act == 224) {
			act = _getch();
		}
		switch (act) {
		case KEY_UP:
			if (ChooseI > 0) {
				ChooseI -= 1;
			}
			break;
		case KEY_DOWN:
			if (ChooseI < (Size - 1)) {
				ChooseI += 1;
			}
			break;
		case 13:
			return ChooseI;
			system("CLS");
			break;
		}
	}
}

void HCConsole::Pause() {
	_getch();
}

void HCConsole::CLS() {
	system("CLS");
}

void HCConsole::SetPrecision(LLDOUBLE NumForSizeSet){
	int Len, IntVar;
	IntVar = abs(NumForSizeSet);
	Len = std::to_string(IntVar).length();
	if (!IntVar) Len -= 1;
	std::cout.setf(std::ios::fixed);
	std::cout.precision(sizeof(NumForSizeSet) * 2 - Len);
}

void HCConsole::SetPrecision(USINT PrecisionSize) {
	std::cout.setf(std::ios::fixed);
	std::cout.precision(PrecisionSize);
}

/*
---------------------
	  HCFiles
---------------------
*/

void HCFiles::Config::Process(std::string PathToFile) {
	std::string FileContent;
	this->FIn.open(PathToFile, std::ifstream::in);
	if (!this->FIn.is_open()) throw 1;

	while (!this->FIn.eof()) {
		std::getline(this->FIn, FileContent, this->Delimiter);
		this->Exploded.push_back(FileContent);
	}
	if (!this->Exploded.size()) throw 0;

	for (auto it = this->Exploded.begin(); it < this->Exploded.end(); it++) {
		this->CFG[(*it).substr(0, (*it).find(this->Pointer))] = (*it).substr((*it).find(this->Pointer) + 1, (*it).length() - (*it).find(this->Pointer));
	}
}

bool HCFiles::Reader::GetFContent(std::string Filename, std::string* Result) {
	std::ifstream File(Filename);
	if (!File.is_open()) return false;
	char readbuffer[256];
	do {
		File.getline(readbuffer, 255);
		*Result += readbuffer + std::string("\n");
	} while (!File.eof());
	File.close();
	return true;
}

bool HCFiles::Reader::GetFContent(std::string Filename){
	std::ifstream File(Filename);
	if (!File.is_open()) return false;
	
	char readbuffer[256];
	do {
		File.getline(readbuffer, 255);
		this->FileContent += readbuffer + std::string("\n");
	} while (!File.eof());
	File.close();
	return true;
}

/*
---------------------
	  HCText
---------------------
*/

bool HCText::StrReplace(std::string* Base, std::string SubString, std::string Replacement) {
	auto entrypos = Base->find(SubString);
	if (entrypos == std::string::npos) return false;
	Base->replace(entrypos, SubString.length(), Replacement);
	return true;
}

/*
---------------------
	  HCNumbers
---------------------
*/

std::string HCNumbers::CutDouble(LLDOUBLE Number, USINT Precision) {
	std::string res, dnum = std::to_string(Number);
	std::string tdp, tip;
	std::string delim = "";
	if (dnum.find(",") != dnum.npos) delim = ",";
	if (dnum.find(".") != dnum.npos) delim = ".";
	if (delim != "") {
		tip = dnum.substr(0, dnum.find(delim));
		tdp = dnum.substr(dnum.find(delim), dnum.length());
		res = tip + tdp.substr(0, ++Precision);
	}
	return res;
}

bool HCNumbers::IsDoubleEqual(LLDOUBLE FirstVar, LLDOUBLE SecondVar, USINT ComparisonAccuracy) {
	long double Difference;
	long long int CompVar;
	Difference = FirstVar - SecondVar;
	for (int i = 0; i != ComparisonAccuracy; i++) {
		Difference = Difference * 10;
	}
	CompVar = Difference;
	if (CompVar == 0)	return true;
	else				return false;
}


/*не знаю что это за хуйня, но это важно. это точно. наверно.
setprecision(1.0 / 3.0);
setprecision(-1.0 / 3);
setprecision((1.0 / 3) + 1);
setprecision((-1.0 / 3) - 1);*/

