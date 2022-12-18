#include <iostream>
#include <tuple>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

template <typename Ch, typename Tr, size_t I, typename... Args>
class TuplePrinter
{
public:
	static void print(basic_ostream<Ch, Tr>& os, const tuple<Args...>& t)
	{
		TuplePrinter<Ch, Tr, I - 1, Args...>::print(os, t);
		if (I < sizeof...(Args)) os << " ";
			os << get<I>(t);
		
	}
};
template <typename Ch, typename Tr, typename... Args>
class TuplePrinter<Ch, Tr, 0, Args...>
{
public:
	static void print(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t)
	{
		os << std::get<0>(t);
	}
};

template <typename Ch, typename Tr, typename... Args>
class TuplePrinter<Ch, Tr, -1, Args...>
{
public:
	static void print(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {}
};

template <typename Ch, typename Tr, typename... Args>
auto operator<<(basic_ostream<Ch, Tr>& os, const tuple<Args...>& t)
{
	TuplePrinter<Ch, Tr, sizeof...(Args) - 1, Args...>::print(os, t);
}

template <typename T>
T inputout(string inputV)
{
	T input;
	stringstream buffer;
	buffer << inputV;
	buffer >> input;
	return input;
}

template<typename... Args>
class InputParser
{
public:
	//ifstream instream;
	char separator;
	char ekran;
	int skip;
	bool issafe = false;
	bool fileend = false;
	bool tupleend = false;
	tuple<Args...> buffer;
	~InputParser() = default;
	ifstream& file;

	InputParser(ifstream& inputFile, size_t _skip, char separate = ',', char safe = '\"') : file(inputFile)
	{
		//file = inputFile;
		if (!file.is_open()) throw ("File is not opened!");
		this->separator = separate;
		this->ekran = safe;
		this->skip = _skip;
		//this->instream& = _file;


		string temp;
		for (int i = 0; i < this->skip; i++)
		{
			getline(file, temp);
		}

		this->buffer = this->gets_s();
	}
	
	tuple<Args...> gets_s()
	{
		char tempchar = 0;
		string tempstring;
		vector<string> output = getandsep();

		auto rotate = [&output]() -> std::string {
			string s = output.back();
			output.pop_back();
			return s;
		};

		if (output.size() != sizeof...(Args) || issafe)	throw ("Invalid string");

		return tuple<Args...>(inputout<Args>(rotate())...);
	}

	vector<string> getandsep()
	{
		string temp;
		vector<string>	sepstr;
		vector <string>	readystr;

		getline(file, temp);
		size_t start = 0, end = 0;
		while ((start = temp.find_first_not_of(separator, end)) != string::npos)
		{
			end = temp.find(separator, start);
			sepstr.push_back(temp.substr(start, end - start));
		}

		string value;
		for (int i = 0; i <sepstr.size();i++)
		{
			value += sepstr[i];

			if (value.find(ekran) != string::npos)
			{
				issafe = !issafe;
			}

			if (!issafe)
			{
				readystr.push_back(value);
				value = "";
			}

			if (issafe)
			{
				value += separator;
			}
		}
		if (file.eof()) this->fileend = true;
		return readystr;
	}
	class CSVIterator
	{
	public:
		InputParser<Args...>* tuples;
		using iterator_category = std::input_iterator_tag;
		using value_type = std::tuple<Args...>;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		CSVIterator(const CSVIterator&) = default;
		//CSVIterator() = default;

		CSVIterator(InputParser<Args...>* _tuple = nullptr) : tuples(_tuple) {};
		CSVIterator& operator=(const CSVIterator&) = default;
		
		
		friend bool operator==(CSVIterator& a, CSVIterator& b) { 
			return a.tuples == b.tuples; 
		}
		friend bool operator!=(CSVIterator& a, CSVIterator& b) { 
			return a.tuples != b.tuples; 
		}
		friend void swap(CSVIterator& a, CSVIterator& b) { 
			swap(a.tuples, b.tuples);
		}
		value_type operator*() { 
			return tuples->buffer; 
		}
		CSVIterator& operator++()
		{
			try
			{
				tuples->buffer = tuples->gets_s();
			}
			catch (runtime_error& e)
			{
				this->tuples->tupleend = true;
			}
			return *this;
		}
	};
	CSVIterator begin() { 
		return CSVIterator(this); 
	}
	CSVIterator end() { 
		return CSVIterator(); 
	}
};

int main()
{
	ifstream fin("cinema_sessions.csv");
	//getline(cin, fin);
	InputParser<int, int, string> filefull(fin, 2, ' ', '/n');


	for (auto x : filefull)
	{
		cout << x;
		cout << "\n";
	}

	return 0;
}
