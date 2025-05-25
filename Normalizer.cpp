//#include "Normalizer.h"
//#include<cmath>
//#include<algorithm>
//#include<vector>
//
//void Normalizer::log2Transform(GeneExpressionData& data) {
//	for (auto& gene_expr : data.expression_matrix) {
//		for (auto& expr_val : gene_expr) {
//			expr_val = log2(expr_val + 1);
//		}
//	}
//
//}
//
//void Normalizer::quantileNormalize(GeneExpressionData& data) {
//	const int num_samples = data.sample_names.size();
//	const int num_genes = data.gene_names.size();
//	//对每个样本单独排序
//	vector<vector<double>>sorted_matrix(num_samples);
//	for (int sample_idx = 0; sample_idx < num_samples; sample_idx++) {
//		vector<double>sample_values;
//		for (int gene_idx = 0; gene_idx < num_genes; gene_idx++) {
//			sample_values.push_back(data.expression_matrix[gene_idx][sample_idx]);
//		}
//		sort(sample_values.begin(), sample_values.end());
//		sorted_matrix[sample_idx] = sample_values;
//	}
//	
//}