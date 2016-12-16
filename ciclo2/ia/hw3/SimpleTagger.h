class SimpleTagger
{
private:
	vec_s S;
	Map2d e;
	Map1d count_e;
	Map1d count_tags;

	void Calc_E(string ifname);
public:

	void Train(string ifname);
	vec_s Predict(vec_s words);
	vec_t<vec_s> Predict(vec_t<vec_s> sentences);
};

void SimpleTagger::Train(string ifname)
{
	Calc_E(ifname);
}

vec_t<vec_s> SimpleTagger::Predict(vec_t<vec_s> sentences)
{
	vec_s asd;
	vec_t<vec_s>
		result;
	int i = 0;
	for (auto e : sentences)
	{
		if (e.size() == 0) continue;
		result.push_back(Predict(e));
	}

	return result;
}

vec_s SimpleTagger::Predict(vec_s words)
{
	Map2d::iterator
		it;
	vec_s
		result;
	string
		tag;
	int
		n;
	double
		p,
		val,
		max_p;

	n = words.size();
	if (n == 0) return result;

	for (auto w : words)
	{
		it = e.find(w);
		
		if (it == e.end())
			it = e.find("_RARE_");

		if (count_e[it->first] < 5)
			it = e.find("_RARE_");

		max_p = -1.0;
		tag = "x";
		for (auto t : it->second)
		{
			p = t.second / count_tags[t.first];

			if (p > max_p)
			{
				max_p = t.second / count_tags[t.first];
				max_p = p;
				tag = t.first;
			}
		}

		result.push_back(tag);
	}

	return result;
}

void SimpleTagger::Calc_E(string ifname)
{
	Map2d::iterator
		it2d;
	Map1d::iterator
		it1d;
	Map1d
		tags;
	vec_s
		tokens;
	ifstream
		f;
	string
		line,
		word,
		tag;
	double
		count;
	set<string>
		_S;

	f.open(ifname);

	do
	{
		line = "";
		getline(f, line);
		if (line.size() == 0) continue;

		split(line, tokens, ' ');
		if (tokens.size() == 0) continue;

		if (tokens[1].compare("WORDTAG") != 0) continue;

		count = todouble(tokens[0]);
		tag = tokens[2];
		word = tokens[3];

		_S.insert(tag);

		if (count_tags.find(tag) == count_tags.end())
			count_tags[tag] = count;
		else
			count_tags[tag] += count;

		if (e.find(word) != e.end())
		{
			if (e[word].find(tag) != e[word].end())
				e[word][tag] += count;
			else
				e[word][tag] = count;
		}
		else
		{
			e[word][tag] = count;
		}
	} while (!f.eof());

	f.close();
	

	for (it2d = e.begin(); it2d != e.end(); ++it2d)
	{
		count_e[it2d->first] = 0;
		for (auto p : it2d->second)
		{
			count_e[it2d->first] += p.second;
		}
	}

	for (auto e : _S)
	{
		S.push_back(e);
	}
}