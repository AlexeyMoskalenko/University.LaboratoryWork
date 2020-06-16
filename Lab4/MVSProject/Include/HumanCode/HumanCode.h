#pragma once

#include <iostream>
#include <conio.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>

#define HC_arr_size(determined_array) (sizeof(determined_array) / sizeof(determined_array[0]))

namespace HumanCode{

	typedef unsigned int		UINT;
	typedef unsigned short int	USINT;
	typedef long long int		LLINT;
	typedef long double			LLDOUBLE;

	const std::string HCNS = "";

	namespace HCCommon{
		class DiapasoneDouble{
		private:
			DiapasoneDouble();
		public:
			LLDOUBLE LeftLimit;
			LLDOUBLE RightLimit;

			DiapasoneDouble(LLDOUBLE LeftLimit, LLDOUBLE RightLimit);
			bool InDiapasone(LLDOUBLE ToCheck, bool Strict = true, USINT ComparisonAccuracy = 4);
		};
		class DiapasoneInt {
		private:
			DiapasoneInt();
		public:
			LLINT LeftLimit;
			LLINT RightLimit;

			DiapasoneInt(LLINT LeftLimit, LLINT RightLimit);
			bool InDiapasone(LLINT ToCheck, bool Strict = true);
		};
	}	 

	namespace HCConsole{
		int	 ChooseMenu(std::string List[], USINT Size, USINT ChooseI = 0, std::string Header = "");
		void SetPrecision(LLDOUBLE NumForSizeSet);
		void SetPrecision(USINT PrecisionSize);

		void SetLocale();
		void Exit(std::string Message = "");
		void Pause();
		void CLS();
	}

	namespace HCFiles {
		class Config {
		public:
			std::map<std::string, std::string> CFG;
			void Process(std::string PathToFile);
			std::vector<std::string> Exploded;
		protected:
			std::ifstream FIn;
			char Delimiter = '\n';
			std::string Pointer = "=";
		};

		class Reader{
		private:
		public:
			std::string FileContent;
			bool static GetFContent(std::string Filename, std::string* Result);
			bool GetFContent(std::string Filename);
		};
	}

	namespace HCText{
		bool StrReplace(std::string* Base, std::string SubString, std::string Replacement);
	}

	namespace HCNumbers{
		std::string CutDouble(LLDOUBLE Number, USINT Precision = 1);
		bool IsDoubleEqual(LLDOUBLE FirstVar, LLDOUBLE SecondVar, USINT ComparisonAccuracy = 4);
	}
}