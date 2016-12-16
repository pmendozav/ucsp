//void EmissionProb(string fname_src, 
//	map<string, P*> &E,
//	double &count_o, 
//	double &count_gene)
//{
//	double tmp[2];
//	ifstream f;
//	string buf;
//	char *ptr;
//	string word, tag;
//	double count;
//
//	f.open(fname_src);
//
//	count_o = count_gene = 0;
//	do
//	{
//		buf = "";
//		getline(f, buf);
//
//		if (buf.size() == 0) continue;
//
//		ptr = strtok((char *)buf.c_str(), " ");
//		count = atof(ptr);
//		strtok(NULL, " ");
//		tag = string(strtok(NULL, " "));
//		if ((ptr = strtok(NULL, " ")) == NULL) break;
//		word = string(ptr);
//
//		if (E.find(word) != E.end())
//		{
//			if (tag.compare("O") == 0)
//			{
//				E[word]->o += count;
//				count_o += count;
//			}
//			else
//			{
//				E[word]->gene += count;
//				count_gene += count;
//			}
//		}
//		else
//		{
//			if (tag.compare("O") == 0)
//			{
//				E[word] = new P(count, 0);
//				count_o += count;
//			}
//			else
//			{
//				E[word] = new P(0, count);
//				count_gene += count;
//			}
//		}
//	} while (!f.eof());
//
//	f.close();
//}
////
//void EmissionProb2(string fname_src,
//	map<string, map<string, double>> &E, 
//	map<string, double> &counts)
//{
//	double tmp[2];
//	ifstream f;
//	string buf;
//	char *ptr;
//	string word, tag;
//	double count;
//
//	f.open(fname_src);
//
//	do
//	{
//		buf = "";
//		getline(f, buf);
//
//		if (buf.size() == 0) continue;
//
//		ptr = strtok((char *)buf.c_str(), " ");
//		count = atof(ptr);
//		strtok(NULL, " ");
//		tag = string(strtok(NULL, " "));
//		if ((ptr = strtok(NULL, " ")) == NULL) break;
//		word = string(ptr);
//
//		//S.insert(tag);
//
//		if (counts.find(tag) == counts.end())
//			counts[tag] = count;
//		else
//			counts[tag] += count;
//
//		//creo que solo deberia ir "E[word][tag] = count;"
//		if (E.find(word) != E.end())
//		{
//			if (E[word].find(tag) != E[word].end())
//				E[word][tag] += count;
//			else
//				E[word][tag] = count;
//		}
//		else
//		{
//			E[word][tag] = count;
//		}
//	} while (!f.eof());
//
//	f.close();
//}
//
//
