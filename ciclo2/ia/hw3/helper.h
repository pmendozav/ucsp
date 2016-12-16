using Map1d = map<string, double>;
using Map2d = map<string, map<string, double>>;
using Map3d = map<string, map<string, map<string, double>>>;
using Map3d_dssf = map<int, map<string, map<string, double>>>;
using Map3d_dsss = map<int, map<string, map<string, string>>>;
template<typename T> using vec_t = vector<T>;
using vec_s = vec_t<string>;

struct P
{
	double o, gene;
	P(double o = 0, double gene = 0) : o(o), gene(gene){};
};

double todouble(string s)
{
	return atof(s.c_str());
}

bool IsNumeric(string s)
{
	for (auto c : s)
	{
		if (isdigit(c))
			return true;
	}

	return false;
}

bool IsCapitalized(string s)
{
	int count = 0;
	for (auto c : s)
	{
		if (isupper(c))
			count++;
	}

	return (s.size() == count);
}

bool IsLastCapital(string s)
{
	return (isupper(s[s.size() - 1]));
}

bool CheckRare(int count)
{
	return (count < 5);
}

vector<string> ReadFile(string ifname)
{
	vector<string>
		result;
	ifstream
		f;
	string line;

	f.open(ifname);

	while (!f.eof())
	{
		line = "";
		getline(f, line);

		result.push_back(line);
	}
	f.close();

	return result;
}

unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
	unsigned int pos = txt.find(ch);
	unsigned int initialPos = 0;
	strs.clear();

	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

	return strs.size();
}

set<string> GetStopWords0(string fname)
{
	ifstream f;
	string buf, tag, type;
	char *ptr;
	string word;
	int count;
	set<string> out;
	map<string, int> oc, _oc;
	map<string, int>::iterator it;
	vector<string> tokens;

	f.open(fname);

	while (!f.eof())
	{
		getline(f, buf);

		split(buf, tokens, ' ');
		if (tokens.size() != 4) continue;

		count = todouble(tokens[0]);
		type = tokens[1];
		tag = tokens[2];
		word = tokens[3];

		it = oc.find(word);
		if (it != oc.end())
			it->second += count;
		else
			oc[word] = count;
	}

	f.close();

	for (auto e : oc)
	{
		if (e.second < 5)
			out.insert(e.first);
	}

	return out;
}

void Replace0(set<string> sw, string ifname, string ofname)
{
	ifstream f;
	ofstream f2;
	string buf, result;
	char *w1, *w2;
	bool check;

	f.open(ifname);

	result = "";
	int c = 0;
	while (!f.eof())
	{
		getline(f, buf);

		if (buf.size() == 0)
		{
			result += "\n";
			continue;
		}

		w1 = strtok((char *)buf.c_str(), " ");
		w2 = strtok(NULL, " ");

		if (sw.count(w1) == 1)
			result += "_RARE_ " + string(w2) + "\n";
		else
			result += string(w1) + " " + string(w2) + "\n";
	}

	f.close();

	f2.open(ofname);
	f2.write(result.c_str(), result.size());
	f2.close();
}

map<string, set<string>> GetStopWords3(string fname)
{
	ifstream f;
	string buf, tag, type;
	char *ptr;
	string word;
	int count;
	map<string, set<string>> out;
	map<string, int> oc;
	map<string, int>::iterator it;
	vector<string> tokens;

	set<string>
		s_numeric,
		s_capitalized,
		s_lastCapital,
		s_rare;

	f.open(fname);

	while (!f.eof())
	{
		getline(f, buf);

		split(buf, tokens, ' ');
		if (tokens.size() != 4) continue;

		count = todouble(tokens[0]);
		type = tokens[1];
		tag = tokens[2];
		word = tokens[3];

		it = oc.find(word);
		if (it != oc.end())
			it->second += count;
		else
			oc[word] = count;
	}

	f.close();

	for (auto e : oc)
	{
		word = e.first;
		count = e.second;

		if (CheckRare(count))
		{
			if (IsNumeric(word))
			{
				s_numeric.insert(word);
			}
			else if (IsCapitalized(word))
			{
				s_capitalized.insert(word);
			}
			else if (IsLastCapital(word))
			{
				s_lastCapital.insert(word);
			}
			else
			{
				s_rare.insert(word);
			}
		}
	}

	out["numeric"] = s_numeric;
	out["capitalized"] = s_capitalized;
	out["lastCapital"] = s_lastCapital;
	out["rare"] = s_rare;


	return out;
}

void Replace3(map<string, set<string>> sw, string ifname, string ofname)
{
	ifstream f;
	ofstream f2;
	string buf, result;
	char *w1, *w2;
	bool check;
	vector<string> tokens;
	string word, tag;

	result = "";
	f.open(ifname);

	while (!f.eof())
	{
		getline(f, buf);
		if (buf.size() == 0)
		{
			result += "\n";
			continue;
		}

		split(buf, tokens, ' ');
		if (tokens.size() != 2) continue;

		word = tokens[0];
		tag = tokens[1];

		if (sw["numeric"].count(word) == 1)
			result += "_NUMERIC_ " + tag + "\n";
		else if (sw["capitalized"].count(word) == 1)
			result += "_CAPITALIZED_ " + tag + "\n";
		else if (sw["lastCapital"].count(word) == 1)
			result += "_LASTCAPITAL_ " + tag + "\n";
		else if (sw["rare"].count(word) == 1)
			result += "_RARE_ " + tag + "\n";
		else
			result += word + " " + tag + "\n";
	}
	
	f.close();

	f2.open(ofname);
	f2.write(result.c_str(), result.size());
	f2.close();
}