class _Viterbi
{
private:
	Map3d
		_3_gram;
	Map2d
		_2_gram;
	Map1d
		_1_gram;

	Map3d q;
	Map2d e;
	vec_s S;
	Map1d count_e;
	Map1d count_tags;
	
	void Calc_E(string ifname);
	void Calc_Q(string ifname);

public:
	void Train(string ifname);
	string Predict(string sentence);
	vec_s Predict(vec_s words);
	vec_t<vec_s> Predict(vec_t<vec_s> sentences);
};

vec_t<vec_s> _Viterbi::Predict(vec_t<vec_s> sentences)
{
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

vec_s _Viterbi::Predict(vec_s words)
{
	vec_s
		result;
	Map3d_dssf
		pi;
	Map3d_dsss
		bp;
	vec_s
		U,
		V,
		W;
	string
		max_w,
		max_u,
		max_v,
		xk;
	int
		n;
	double
		_q,
		_e,
		val,
		max_pi;
	Map2d::iterator 
		it;

	n = words.size();
	if (n == 0) return result;

	pi[0]["*"]["*"] = 1.0;
	
	for (int k = 1; k <= n; k++)
	{
		U = S;
		V = S;
		W = S;
		xk = words[k - 1];

		//
		it = e.find(xk);
		if (it == e.end())
		{
			it = e.find("_RARE_");
		}
		xk = it->first;
		//

		if (k == 1)
			U = W = { "*" };
		if (k == 2)
			W = { "*" };

		for (auto u : U)
		{
			for (auto v : V)
			{
				max_pi = -1;
				max_w = "x";

				for (auto w : W)
				{
					_e = e[xk][v] / count_tags[v];
					_q = _3_gram[w][u][v] / _2_gram[w][u];

					val = pi[k - 1][w][u] * _q * _e;

					if (val > max_pi)
					{
						max_pi = val;
						max_w = w;
					}
				}

				pi[k][u][v] = max_pi;
				bp[k][u][v] = max_w;
			}
		}
	}

	U = S;
	V = S;
	max_pi = -1;
	max_u = max_v = "x";

	for (auto u : U)
	{
		for (auto v : V)
		{
			_q = _3_gram[u][v]["STOP"] / _2_gram[u][v];

			val = pi[n][u][v] * _q;

			if (val > max_pi)
			{
				max_pi = val;
				max_u = u;
				max_v = v;
			}
		}
	}

	result.resize(n + 1);
	result[n - 1] = max_u;
	result[n] = max_v;

	for (int k = n - 2; k >= 0; k--)
		result[k] = bp[k + 2][result[k + 1]][result[k + 2]];

	result.erase(result.begin(), result.begin() + 1);
	return result;
}

string _Viterbi::Predict(string sentence)
{
	vec_s
		result,
		words;
	string
		buf;

	split(sentence, words, ' ');
	
	result = Predict(words);

	buf = "";
	for (int i = 1; i < result.size(); i++)
	{
		buf += result[i];
	}

	return buf;
}

void _Viterbi::Train(string ifname)
{
	Calc_E(ifname);
	Calc_Q(ifname);
}

void _Viterbi::Calc_E(string ifname)
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
	int i = 1;
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
		e[word][tag] = count;
		count_tags[tag] += count;
		count_e[word] += count;
	} while (!f.eof());

	f.close();
	
	for (auto e : _S)
	{
		S.push_back(e);
	}
}

void _Viterbi::Calc_Q(string ifname)
{
	Map3d::iterator
		it3d;
	Map2d::iterator
		it2d;
	Map1d::iterator
		it1d;
	vec_t<vec_s>
		tmp;
	vec_s
		tokens,
		tag_list;
	ifstream 
		f;
	string
		w1,
		w2,
		w3,
		line;
	double
		count;
	int
		n;

	f.open(ifname);
	while (!f.eof())
	{
		line = "";
		getline(f, line);

		if (line.size() == 0) continue;

		split(line, tokens, ' ');
		if (tokens.size() == 0) continue;

		if (tokens[1].compare("1-GRAM") != 0 &&
			tokens[1].compare("2-GRAM") != 0 &&
			tokens[1].compare("3-GRAM") != 0)
			continue;

		count = todouble(tokens[0]);
		
		tokens.erase(tokens.begin(), tokens.begin() + 2);
		n = tokens.size();
		if (n == 1)
			_1_gram[tokens[0]] = count;
		else if (n == 2)
			_2_gram[tokens[0]][tokens[1]] = count; //0 1
		else
		{
			_3_gram[tokens[0]][tokens[1]][tokens[2]] = count; // 0 1 2
			tmp.push_back(tokens);
		}
	}
	f.close();
}