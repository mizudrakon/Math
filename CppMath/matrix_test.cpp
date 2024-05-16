#include "Matrix.hpp"
#include <iostream>

using std::cout, std::endl;

int main(int argc, char** argv) {

	Matrix M(3, 5);

	std::cout << "** set test **" << endl;
	M.set(2, 4, 8);
	M.set(0, 4, 5);
	M.set(0, 0, 5);
	std::cout << "M[2][4] = " << M.get(2, 4) << endl;
	std::cout << "M[0][4] = " << M.get(0, 4) << endl;
	std::cout << "M[0][0] = " << M.get(0, 0) << endl;
	M.print();
	std::cout << "\n";

	std::cout << "** set_width and set_height test **" << endl;
	std::cout << "--Width--" << endl;
	std::cout << "Before : " << M.get_width() << endl;
	M.set_width(6);
	std::cout << "After : " << M.get_width() << endl;
	std::cout << "--Height--" << endl;
	std::cout << "Before : " << M.get_height() << endl;
	M.set_height(2);
	std::cout << "After : " << M.get_height() << endl;
	M.print();
	std::cout << "\n";

	std::cout << "** get_row and get_col test **" << endl;
	std::vector<int> column4 = M.get_column(4);
	std::cout << "column 4 : [" << column4[0];
	for (int i = 1; i < column4.size(); i++) {
		cout << ", " << column4[i];
	}
	cout << "]" << endl;
	std::vector<int> row0 = M.get_row(0);
	std::cout << "row 0 : [" << row0[0];
	for (int i = 1; i < row0.size(); i++) {
		cout << ", " << row0[i];
	}
	cout << "]" << endl;
	std::cout << "\n";

	std::cout << "** get_rows and get_columns test **" << endl;
	std::cout << "get_columns() :" << endl;
	std::vector<std::vector<int> > columns = M.get_columns();
	for (int i = 0; i < columns.size(); i++) {
		std::cout << "[" << columns[i][0];
		for (int j = 1; j < columns[0].size(); j++) {
			cout << ", " << columns[i][j];
		}
		cout << "]" << endl;
	}
	std::cout << "get_rows() :" << endl;
	std::vector<std::vector<int> > rows = M.get_rows();
	for (int i = 0; i < rows.size(); i++) {
		std::cout << "[" << rows[i][0];
		for (int j = 1; j < rows[0].size(); j++) {
			cout << ", " << rows[i][j];
		}
		cout << "]" << endl;
	}
	std::cout << "\n";

	std::cout << "** clear test **" << endl;
	M.clear();
	M.print();
	std::cout << "\n";

	std::cout << "** fill_with_value test **" << endl;
	M.fill_with_value(6);
	M.print();
	std::cout << "\n";

	std::cout << "** reverse test **" << endl;
	std::cout << "----test 1----" << endl;
	M.set(0, 1, 8);
	M.set(1, 0, 5);
	std::cout << "Before : " << endl;
	M.print();
	std::cout << "After : " << endl;
	M.reverse();
	M.print();

	std::cout << "----test 2----" << endl;
	Matrix M2(3, 3);
	M2.set(0, 0, 1);
	M2.set(0, 1, 2);
	M2.set(0, 2, 3);
	M2.set(1, 0, 4);
	M2.set(1, 1, 5);
	M2.set(1, 2, 6);
	M2.set(2, 0, 7);
	M2.set(2, 1, 8);
	M2.set(2, 2, 9);
	std::cout << "Before : " << endl;
	M2.print();
	std::cout << "After : " << endl;
	M2.reverse();
	M2.print();

	std::cout << "----test 3----" << endl;
	Matrix M3(5, 2);
	M3.set(0, 0, 1);
	M3.set(0, 1, 2);
	M3.set(1, 0, 3);
	M3.set(1, 1, 4);
	M3.set(2, 0, 9);
	M3.set(3, 1, 8);
	M3.set(4, 0, 7);
	std::cout << "Before : " << endl;
	M3.print();
	std::cout << "After : " << endl;
	M3.reverse();
	M3.print();
	std::cout << "\n";

	std::cout << "** is_negative test **" << endl;
	std::cout << "----test 1----" << endl;
	M2.set(0, 0, -1);
	M2.set(1, 1, -5);
	M2.print();
	std::cout << "M2.is_negative() = " << M2.is_negative() << endl;
	std::cout << "----test 2----" << endl;
	M2.set(0, 1, -2);
	M2.set(0, 2, -3);
	M2.set(1, 0, -4);
	M2.set(1, 2, -6);
	M2.set(2, 0, -7);
	M2.set(2, 1, -8);
	M2.set(2, 2, -9);
	M2.print();
	std::cout << "M2.is_negative() = " << M2.is_negative() << endl;
	std::cout << "\n";

	std::cout << "** zero_count test **" << endl;
	std::cout << "----test 1----" << endl;
	M2.print();
	std::cout << "M2.zero_count() = " << M2.zero_count() << endl;
	std::cout << "----test 2----" << endl;
	M2.set(1, 0, 0);
	M2.set(2, 1, 0);
	M2.set(2, 2, 0);
	M2.print();
	std::cout << "M2.zero_count() = " << M2.zero_count() << endl;
	std::cout << "----test 3----" << endl;
	M2.clear();
	M2.print();
	std::cout << "M2.zero_count() = " << M2.zero_count() << endl;


	return 0;

}