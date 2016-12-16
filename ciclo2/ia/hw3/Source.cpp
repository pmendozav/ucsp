#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <regex>
#include <set>
#include <algorithm>

using namespace std;

#include "helper.h"

#include "Viterbi.h"
#include "ViterbiV2.h"
#include "SimpleTagger.h"

void _replace_parte_1(string fname_train, string fname_train_2, string fname_count)
{
	set<string> 
		sw;
	string
		command,
		fname_count_2;

	command = "python python/count_freqs.py ";
	system((command + fname_train + " > " + fname_count).c_str());

	sw = GetStopWords0(fname_count);

	Replace0(sw, fname_train, fname_train_2);
	
	system((command + fname_train_2 + " > " + fname_count).c_str());
}

void _parte_1(string fname_train_count, string fname_test, string fname_result)
{
	ofstream f;
	SimpleTagger
		tagger;
	string
		buf;
	vector<vector<string>>
		results,
		sentences;
	vector<string>
		words,
		lines;

	lines = ReadFile(fname_test);

	for (int i = 0; i < lines.size(); i++)
	{
		buf = lines[i];
		if (buf.size() == 0)
		{
			sentences.push_back(words);
			words.clear();
			continue;
		}
		words.push_back(buf);
	}

	tagger.Train(fname_train_count);
	results = tagger.Predict(sentences);
	
	f.open(fname_result);
	for (int i = 0; i < results.size(); i++)
	{
		for (int j = 0; j < results[i].size(); j++)
		{
			f << sentences[i][j] << " " << results[i][j] << endl;
		}
		f << endl;
	}
	f.close();
}

void _parte_2(string fname_train_count, string fname_test, string fname_result)
{
	ofstream f;
	_Viterbi
		viterbi;
	string
		buf;
	vector<vector<string>>
		results,
		sentences;
	vector<string>
		words,
		lines;

	lines = ReadFile(fname_test);

	for (int i = 0; i < lines.size(); i++)
	{
		buf = lines[i];
		if (buf.size() == 0)
		{
			sentences.push_back(words);
			words.clear();
			continue;
		}

		words.push_back(buf);
	}

	viterbi.Train(fname_train_count); //gene2.counts

	results = viterbi.Predict(sentences);

	f.open(fname_result);
	for (int i = 0; i < results.size(); i++)
	{
		for (int j = 0; j < results[i].size(); j++)
		{
			f << sentences[i][j] << " " << results[i][j] << endl;
		}

		f << endl;
	}
	f.close();
}

void _replace_parte_3(string fname_train, string fname_train_2, string fname_count)
{
	map<string, set<string>>
		sw;
	string
		command,
		fname_count_2;

	command = "python python/count_freqs.py ";
	system((command + fname_train + " > " + fname_count).c_str());

	sw = GetStopWords3(fname_count);

	Replace3(sw, fname_train, fname_train_2);

	system((command + fname_train_2 + " > " + fname_count).c_str());
}

void _parte_3(string fname_train_count, string fname_test, string fname_result)
{
	ofstream f;
	_ViterbiV2
		viterbi;
	string
		buf;
	vector<vector<string>>
		results,
		sentences;
	vector<string>
		words,
		lines;

	lines = ReadFile(fname_test);

	for (int i = 0; i < lines.size(); i++)
	{
		buf = lines[i];
		if (buf.size() == 0)
		{
			sentences.push_back(words);
			words.clear();
			continue;
		}

		words.push_back(buf);
	}

	viterbi.Train(fname_train_count); //gene2.counts

	results = viterbi.Predict(sentences);

	f.open(fname_result);
	for (int i = 0; i < results.size(); i++)
	{
		for (int j = 0; j < results[i].size(); j++)
		{
			f << sentences[i][j] << " " << results[i][j] << endl;
		}

		f << endl;
	}
	f.close();
}

int main()
{
	//PARTE 1
	//_replace_parte_1("gene.train", "gene_p1.train", "gene_p1.count");
	//_parte_1("gene_p1.count", "gene.dev", "result_part_1.txt");

	//PARTE 2
	//_parte_2("gene_p1.count", "gene.dev", "result_part_2.txt");

	//PARTE 3
	//_replace_parte_3("gene.train", "gene_p3.train", "gene_p3.count");
	_parte_3("gene_p3.count", "gene.dev", "result_part_3.txt");

	cout << "Fin!" << endl;
}