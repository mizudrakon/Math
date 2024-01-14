#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <string>
#include <vector>


//an integer matrix

class Matrix {

public:

	Matrix(const int& rows, const int& cols) {
		builder(rows, cols);
	}

	void builder(const int& rows, const int& cols) {
		m.resize(rows);
		for (int i = 0; i < rows; ++i) {
			m[i].resize(cols);
		}
	}

	void set(const int& row, const int& col, const int& value) {
		m[row][col] = value;
	}

	int get(const int& row, const int& col) {
		return m[row][col];
	}

	void print() {
		for (int i = 0; i < get_height(); ++i) {
			print_row(i);
		}
	}

	void set_width(const int& width) {
		for (int i = 0; i < m.size(); ++i) {
			m[i].resize(width, 0);
		}
	}

	int get_width() {
		return m[0].size();
	}

	void set_height(const int& height) {
		int old_height = get_height();
		m.resize(height);
		for (int i = old_height; i < get_height(); ++i) {
			m[i].resize(get_width(), 0);
		}
	}

	int get_height() {
		return m.size();
	}

	void print_row(const int& x) {
		std::cout << "[" << m[x][0];
		for (int i = 1; i < get_width(); ++i) {
			std::cout << ", " << m[x][i];
		}
		std::cout << "]\n";
	}

	void print_column(const int& x) {
		std::cout << "[" << m[0][x];
		for (int i = 1; i < get_height(); ++i) {
			std::cout << ", " << m[i][x];
		}
		std::cout << "]\n";
	}

	std::vector<int> get_row(const int& x) {
		return m[x];
	}

	std::vector<int> get_column(const int& x) {
		std::vector<int> column;
		column.resize(get_height());
		for (int i = 0; i < get_height(); i++) {
			column[i] = m[i][x];
		}
		return column;
	}

	std::vector<std::vector<int> > get_rows() {
		return m;
	}

	std::vector<std::vector<int> > get_columns() {
		std::vector<std::vector<int> > columns;
		int row = get_width();
		int column = get_height();
		columns.resize(row);
		for (int i = 0; i < row; ++i) {
			columns[i].resize(column);
		}
		for (int i = 0; i < row; i++) {
			columns[i] = get_column(i);
		}
		return columns;
	}

	void clear() {
		fill_with_value(0);
	}

	void fill_with_value(const int& value) {
		for (int i = 0; i < get_height(); i++) {
			for (int j = 0; j < get_width(); j++) {
				m[i][j] = value;
			}
		}
	}

	void reverse() {
		int swap, deep;
		int step = 0;
		int j = 0;
		if (get_height() <= get_width()) {
			for (int i = 0; i < get_height(); i++) {
				step = step + i + 1;
			}
			deep = get_height() - 1;
			while (step != 0) {
				if (deep == j) {
					deep--;
					j = -1;
				}
				else {
					swap = m[deep][j];
					m[deep][j] = m[j][deep];
					m[j][deep] = swap;
				}
				j++;
				step--;
			}
		}
		else {
			for (int i = 0; i < get_width(); i++) {
				step = step + i + 1;
			}
			deep = get_width() - 1;
			while (step != 0) {
				if (deep == j) {
					deep--;
					j = -1;
				}
				else {
					swap = m[deep][j];
					m[deep][j] = m[j][deep];
					m[j][deep] = swap;
				}
				j++;
				step--;
			}
		}
	}

	bool is_negative() {
		for (int i = 0; i < get_height(); i++) {
			for (int j = 0; j < get_width(); j++) {
				if (m[i][j] > 0)
					return false;
			}
		}
		return true;
	}

	int zero_count() {
		int c = 0;
		for (int i = 0; i < get_height(); i++) {
			for (int j = 0; j < get_width(); j++) {
				if (m[i][j] == 0)
					c++;
			}
		}
		return c;
	}



private:

	std::vector<std::vector<int> > m;

};

#endif