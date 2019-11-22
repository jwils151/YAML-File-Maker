#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream> 

/*
X axis title:			String
Y count					Integer
Y axis tile #1:			String
....
Y axis title #N:		String
Is bin:					yes,no
X Stat error:			yes,no,both
X Sys error:			yes,no,both
Y Stat error:			yes,no,both
Y Sys error:			yes,no,both
Data
X2 values /Y1 value / X1 Stat Error / X1 Sys Error / Y1 Stat Error / Y1 Sys Error
***
X2 values /Y2 value / X2 Stat Error / X2 Sys Error / Y2 Stat Error / Y2 Sys Error
***
Xn values /Yn value / Xn Stat Error / Xn Sys Error / Yn Stat Error / Yn Sys Error

Bin examples
No: Single value
Yes: Range e.g. 3-4



find . -exec yaml_data {} \;

*/


/*
std::istream& safeGetline(std::istream& is, std::string& t)
{
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();

	for(;;) {
		int c = sb->sbumpc();
		switch (c) {
		case '\n':
			return is;
		case '\r':
			if(sb->sgetc() == '\n')
				sb->sbumpc();
			return is;
		case std::streambuf::traits_type::eof():
			// Also handle the case when the last line has no line ending
			if(t.empty())
				is.setstate(std::ios::eofbit);
			return is;
		default:
			t += (char)c;
		}
	}
}



while(!safeGetline(ifs, t).eof())
*/




int main(int argc, char** argv) {
	std::ifstream file;
	std::vector<std::string> x, y, x_low, x_high;
	std::vector<std::string> y_stat, y_sys;
	std::vector<std::string> x_stat, x_sys;
	std::vector<std::vector<std::string> > x_ttl, y_ttl, x_low_ttl, x_high_ttl;
	std::vector<std::vector<std::string> > y_stat_ttl, y_sys_ttl;
	std::vector<std::vector<std::string> > x_stat_ttl, x_sys_ttl;
	size_t found;
	std::vector<std::string> qualifiers;
	std::string x_title;
	std::vector<std::string> y_titles;
	char bin_flag, y_stat_flag, y_sys_flag, x_stat_flag, x_sys_flag;
	int y_count;
	std::string yaml_string;
	std::string input, temp, temp2;
	std::stringstream ss;
	std::ofstream fout;
	bool x_stat_test, x_sys_test, y_stat_test, y_sys_test;

	//open file and error check
	if (argc != 2) { std::cerr << "usage: yaml_data data_filename" << std::endl; return 1; }
	file.open(argv[1]);
	if (file.bad()) { std::cerr << "Problem opening file. Check that it exists" << std::endl; return 1; }

	//make yaml file name
	yaml_string = argv[1];
	std::string::size_type i = yaml_string.rfind('.', yaml_string.length());
	if (i != std::string::npos) { yaml_string.replace(i + 1, 4, "yaml"); }

	//get x title
	std::getline(file, x_title);

	std::cout << x_title << std::endl;

	//get y count
	std::getline(file, temp);
	y_count = atoi(temp.c_str());

	std::cout << y_count << std::endl;

	//get y titles
	for (i = 0; i < y_count; i++) {
		std::getline(file, temp);
		y_titles.push_back(temp);
	}

	//get bin style
	std::getline(file, temp);
	bin_flag = toupper(temp[0]);

	std::cout << "bin " << bin_flag << std::endl;


	//x error
	std::getline(file, temp);
	x_stat_flag = toupper(temp[0]);
	std::getline(file, temp);
	x_sys_flag = toupper(temp[0]);

	std::cout << "x stat " << x_stat_flag << std::endl;
	std::cout << "x sys " << x_sys_flag << std::endl;



	//y error
	std::getline(file, temp);
	y_stat_flag = toupper(temp[0]);
	std::getline(file, temp);
	y_sys_flag = toupper(temp[0]);


	std::cout << "y stat " << y_stat_flag << std::endl;
	std::cout << "y sys " << y_sys_flag << std::endl;


	//process data
	while (std::getline(file, input)) {
		std::stringstream ss(input);
		if (input[0] == '*') {
			//add to main array
			x_ttl.push_back(x);
			y_ttl.push_back(y);
			x_low_ttl.push_back(x_low);
			x_high_ttl.push_back(x_high);
			y_stat_ttl.push_back(y_stat);
			y_sys_ttl.push_back(y_sys);
			x_stat_ttl.push_back(x_stat);
			x_sys_ttl.push_back(x_sys);
			//clear existing for use
			x.clear();
			y.clear();
			x_low.clear();
			x_high.clear();
			y_stat.clear();
			y_sys.clear();
			x_stat.clear();
			x_sys.clear();
		}
		else {

			//get x's depending on bin style
			switch (bin_flag) {
			case 'Y':
				ss >> temp;
				found = temp.find("-");
				if (found != std::string::npos) {
					x_low.push_back(temp.substr(0, found));
					x_high.push_back(temp.substr(found + 1));
				}
				break;
			case 'N':
				ss >> temp;
				x.push_back(temp);
				break;
			default:
				std::cerr << "Specify yes or no for Bin style" << std::endl;
				return 1;
				break;
			}

			//get y
			ss >> temp;
			y.push_back(temp);

			//get x stat errors depending on style
			switch (x_stat_flag) {
			case 'Y':
				ss >> temp;
				x_stat.push_back(temp);
				break;
			case 'N':
				break;
			case 'B':
				ss >> temp;
				x_stat.push_back(temp);
				ss >> temp;
				x_stat.push_back(temp);
				break;
			default:
				std::cerr << "Specify yes, no or both for X stat style" << std::endl;
				return 1;
				break;
			}

			//get x sys errors depending on style
			switch (x_sys_flag) {
			case 'Y':
				ss >> temp;
				x_sys.push_back(temp);
				break;
			case 'N':
				break;
			case 'B':
				ss >> temp;
				x_sys.push_back(temp);
				ss >> temp;
				x_sys.push_back(temp);
				break;
			default:
				std::cerr << "Specify yes, no or both for X sys style" << std::endl;
				return 1;
				break;
			}

			//get y stat errors depending on style
			switch (y_stat_flag) {
			case 'Y':
				ss >> temp;
				y_stat.push_back(temp);
				break;
			case 'N':
				break;
			case 'B':
				ss >> temp;
				y_stat.push_back(temp);
				ss >> temp;
				y_stat.push_back(temp);
				break;
			default:
				std::cerr << "Specify yes, no or both for Y stat style" << std::endl;
				return 1;
				break;
			}

			//get y sys errors depending on style
			switch (y_sys_flag) {
			case 'Y':
				ss >> temp;
				y_sys.push_back(temp);
				break;
			case 'N':
				break;
			case 'B':
				ss >> temp;
				y_sys.push_back(temp);
				ss >> temp;
				y_sys.push_back(temp);
				break;
			default:
				std::cerr << "Specify yes, no or both for Y sys style" << std::endl;
				return 1;
				break;
			}


		}


	}

	for (int i = 0; i < y_count; i++) {
		std::cout << "print loop: " << i << std::endl;
		for (int j = 0; j < y_ttl[i].size(); j++) {
			std::cout << "y: " << y_ttl[i][j] << std::endl;
		}

		for (int j = 0; j < x_ttl[i].size(); j++) {
			std::cout << "x: " << x_ttl[i][j] << std::endl;
		}

		for (int j = 0; j < x_low_ttl[i].size(); j++) {
			std::cout << "x low: " << x_low_ttl[i][j] << std::endl;
		}

		for (int j = 0; j < x_high_ttl[i].size(); j++) {
			std::cout << "x high: " << x_high_ttl[i][j] << std::endl;
		}

		for (int j = 0; j < x_stat_ttl[i].size(); j++) {
			std::cout << "x stat: " << x_stat_ttl[i][j] << std::endl;
		}

		for (int j = 0; j < x_sys_ttl[i].size(); j++) {
			std::cout << "x sys: " << x_sys_ttl[i][j] << std::endl;
		}

		for (int j = 0; j < y_stat_ttl[i].size(); j++) {
			std::cout << "y stat: " << y_stat_ttl[i][j] << std::endl;
		}

		for (int j = 0; j < y_sys_ttl[i].size(); j++) {
			std::cout << "y sys: " << y_sys_ttl[i][j] << std::endl;
		}


	}


	file.close();

	fout.open(yaml_string.c_str());
	if (fout.bad()) { std::cerr << "Problem opening file. Check that you have write permission" << std::endl; return 1; }

	if (fout.is_open()) {
		//main loop
		for (int i = 0; i < y_count; i++) {
			//print independent values
			std::cout << "test" << x_ttl.size() << std::endl;

			if (!x_ttl[i].empty()) {
				std::cout << "test1\n";
				//check tests
				x_stat_test = (x_stat_ttl[i].size() == x_ttl[i].size()) && !x_stat_ttl[i].empty();
				x_sys_test = (x_sys_ttl[i].size() == x_ttl[i].size()) && !x_stat_ttl[i].empty();
				std::cout << "x_stat_test: " << x_stat_test << " x_sys_test: " << x_sys_test << std::endl;

				fout << "independent_variables:" << std::endl;
				fout << "- header: {name: " << x_title << "}" << std::endl;
				fout << "  values:" << std::endl;
				for (int j = 0; j < x_ttl[i].size(); j++) {
					fout << "  - {value: " << x_ttl[i][j] << "}" << std::endl;
					if (!x_stat_ttl[i].empty() || !x_sys_ttl[i].empty()) {
						fout << "  errors:" << std::endl;
					}
					if (!x_stat_ttl[i].empty()) {
						if (x_stat_test) {
							fout << "    - {symerror: " << x_stat_ttl[i][j] << ", label : stat}" << std::endl;
						}
						else {
							fout << "    - asymerror: {minus: " << x_stat[i][(2 * j)+1] << ", plus:" << x_stat_ttl[i][2 * j] << "}" << std::endl;
							fout << "    - label: stat}" << std::endl;
						}
					}
					if (!x_sys_ttl[i].empty()) {
						if (x_sys_test) {
							fout << "    - {symerror: " << x_sys[i][j] << ", label : sys}" << std::endl;
						}
						else {
							fout << "    - asymerror: {minus: " << x_sys_ttl[i][(2 * j)+1] << ", plus:" << x_sys_ttl[i][2 * j] << "}" << std::endl;
							fout << "    - label: sys}" << std::endl;
						}
					}
				}
			}

			if ((!x_low_ttl[i].empty()) && (!x_high_ttl[i].empty())) {
				//check tests
				x_stat_test = (x_stat.size() == x_low.size()) && !x_stat.empty();
				x_sys_test = (x_sys.size() == x_low.size()) && !x_stat.empty();
				std::cout << "x_stat_test: " << x_stat_test << " x_sys_test: " << x_sys_test << std::endl;
				fout << "independent_variables:" << std::endl;
				fout << "- header: {name: " << x_title << "}";
				fout << "  values:" << std::endl;
				for (int j = 0; j < x_low_ttl[i].size(); j++) {
					fout << "  - {low: " << x_low_ttl[i][j] << ", high: " << x_high_ttl[i][j] << "}" << std::endl;
					if (!x_stat_ttl[i].empty() || !x_sys_ttl[i].empty()) {
						fout << "  errors:" << std::endl;
					}
					if (!x_stat_ttl[i].empty()) {
						if (x_stat_test) {
							fout << "    - {symerror: " << x_stat_ttl[i][j] << ", label : stat}" << std::endl;
						}
						else {
							fout << "    - asymerror: {minus: " << x_stat_ttl[i][(2 * j) + 1] << ", plus:" << x_stat_ttl[i][2 * j] << "}" << std::endl;
							fout << "    - label: stat}" << std::endl;
						}
					}
					if (!x_sys_ttl[i].empty()) {
						if (x_sys_test) {
							fout << "    - {symerror: " << x_sys_ttl[i][j] << ", label : sys}" << std::endl;
						}
						else {
							fout << "    - asymerror: {minus: " << x_sys_ttl[i][(2 * j) + 1] << ", plus:" << x_sys_ttl[i][2 * j] << "}" << std::endl;
							fout << "    - label: sys}" << std::endl;
						}
					}
				}
			}

			//print dependent values
			if (!y_ttl[i].empty()) {
				//check tests
				y_stat_test = (y_stat_ttl[i].size() == y_ttl[i].size()) && !y_stat_ttl[i].empty();
				y_sys_test = (y_sys_ttl[i].size() == y_ttl[i].size()) && !y_stat_ttl[i].empty();
				std::cout << "y_stat_test: " << y_stat_test << " y_sys_test: " << y_sys_test << std::endl;
				fout << "dependent_variables:" << std::endl;
				fout << "- header: {name: " << y_titles[i] << "}" << std::endl;
				fout << "  values:" << std::endl;
				for (int j = 0; j < y_ttl[i].size(); j++) {
					fout << "  - {value: " << y_ttl[i][j] << "}" << std::endl;
					if (!y_stat_ttl[i].empty() || !y_sys_ttl[i].empty()) {
						fout << "  errors:" << std::endl;
					}
					if (!y_stat_ttl[i].empty()) {
						if (y_stat_test) {
							fout << "    - {symerror: " << y_stat_ttl[i][j] << ", label : stat}" << std::endl;
						}
						else {
							fout << "    - asymerror: {minus: " << y_stat_ttl[i][(2 * j) + 1] << ", plus:" << y_stat[i][2 * j] << "}" << std::endl;
							fout << "    - label: stat}" << std::endl;
						}
					}
					if (!y_sys_ttl[i].empty()) {
						if (y_sys_test) {
							fout << "    - {symerror: " << y_sys_ttl[i][j] << ", label : sys}" << std::endl;
						}
						else {
							fout << "    - asymerror: {minus: " << y_sys_ttl[i][(2 * j) + 1] << ", plus:" << y_sys_ttl[i][2 * j] << "}" << std::endl;
							fout << "    - label: sys}" << std::endl;
						}
					}
				}
			}
		}

		fout.close();
	}



	return 0;
}