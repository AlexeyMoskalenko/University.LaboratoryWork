#pragma once

#include <iostream>
#include <string>
#include <conio.h>

#define HC_arr_size(determined_array) (sizeof(determined_array) / sizeof(determined_array[0]))

class HumanCode
{
public:
	const std::string NS = "";
	template<typename T>
	struct diapasone {
		T left_limit;
		T right_limit;
	};
	HumanCode(int = 10);
	template<typename T>
	bool static in_diapasone(diapasone<T> diapasone, T numtocheck, bool strict = true);
	int static choose_menu(std::string list[], short int size, int choose_i = 0, std::string header = "");
	bool isequal(double first_var, double second_var);
	bool static isequal(double first_var, double second_var, unsigned short int comparison_accuracy);
	void static setprecision(double num_for_set);
	void static setprecision(unsigned short int precision_size);
	void static exit(std::string message = "");
	void pause();
	void cls();
private:
	int EQ_ACC;
};

