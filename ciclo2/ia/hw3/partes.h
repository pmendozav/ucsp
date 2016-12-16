//void parte_1()
//{
//	map<string, P*> e;
//	double count_o, count_gene;
//	string
//		fname_count, //archivo de conteos
//		fname_test, //archivo de prueba
//		fname_test_result; //resultado de prueba
//	
//	//ModificarDataDeEntrenamiento();
//
//
//	//return;
//	fname_count = "gene2.counts";// "count2.out"; "gene_2.count"
//	fname_test = "gene.dev";
//	fname_test_result = "gene_ok.key";
//
//	EmissionProb(fname_count, e, count_o, count_gene);
//	TestSimpleTagger(e, count_o, count_gene, fname_test, fname_test_result);
//}
//
//void parte_2()
//{
//	string yi, y_i1, y_i2, buf;
//	map<string, map<string, double>> E;
//	map<string, map<string, double>>::iterator it1;
//
//	map<string, double> tags;
//	map<string, double>::iterator it2;
//
//	map<string, double> Q;
//	vector<string> S;
//	map<string, double> counts;
//	double value, q;
//
//
//	EmissionProb2("gene.counts", E, counts);
//	for (it1 = E.begin(); it1 != E.end(); ++it1)
//	{
//		tags = it1->second;
//		for (it2 = tags.begin(); it2 != tags.end(); ++it2)
//		{
//			it2->second /= counts[it2->first];
//		}
//	}
//
//	q_trigram("count1_2.out", Q);
//
//	S = {"O", "I-GENE"};
//	TestViterbi("gene.dev", Q, E, S);
//
//}
//
//void parte_1_1()
//{
//	map<string, map<string, double>> e;
//	map<string, double> counts;
//	double count_o, count_gene;
//	string
//		fname_count, //archivo de conteos
//		fname_test, //archivo de prueba
//		fname_test_result; //resultado de prueba
//
//	//ModificarDataDeEntrenamiento();
//
//	fname_count = "gene2.counts";
//	fname_test = "gene.dev";
//	fname_test_result = "gene_ok_2.key";
//
//
//	EmissionProb2(fname_count, e, counts);
//	TestSimpleTagger2(e, counts, fname_test, fname_test_result);
//}