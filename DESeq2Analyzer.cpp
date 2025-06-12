#include "DESeq2Analyzer.h"
#include<string>
#include<cmath>
#include<algorithm>
#include<numeric>

vector<DESeq2Result> DESeq2Analyzer::analyze(const CountData& data, const string& control_group, const string& treat_group) {
    //计算大小因子
    vector<double>size_factors = calculateSizeFactors(data.count_matrix);
    //归一化
    auto normalized = normalizeCounts(data.count_matrix, size_factors);
    //离散度估计
    vector<double>dispersions = estimateGeneDispersions(normalized, data.sample_names, data.sample_to_group, "HC", "RA");
    //差异分析
    return performWaldTest(normalized, dispersions, data, "HC", "RA");
}

vector<double> DESeq2Analyzer::calculateGeometricMeans(const vector<vector<int>>& counts) {
    vector<double>geo_means(counts.size(), 0.0);
    for (int i = 0;i<counts.size();i++) {
        double log_sum = 0.0;
        int non_zero = 0;
        for (int count : counts[i]) {
            if (count > 0) {
                log_sum += log(count);
                non_zero++;
            }
        }
        geo_means[i] = (non_zero > 0 ? exp(log_sum / non_zero) : 0);
    }
    return geo_means;
}

vector<double> DESeq2Analyzer::calculateSizeFactors(const vector<vector<int>>& counts) {
    vector<double> geo_means = calculateGeometricMeans(counts);
    vector<double> size_factors(counts[0].size(), 0.0);
    //计算表达量与几何均值的比率
    for (int j = 0; j < counts[0].size(); j++) {
        vector<double>ratios;
        for (int i = 0; i < counts.size(); i++) {
            if (geo_means[i] > 0 && counts[i][j] > 0) {
                ratios.push_back(counts[i][j] / geo_means[i]);
            }
        }
        //取每列比率的中位数作为样本的大小因子
        if (!ratios.empty()) {
            auto mid = ratios.begin() + ratios.size() / 2;
            nth_element(ratios.begin(), mid, ratios.end());
            size_factors[j] = *mid;
        }
        else
            size_factors[j] = 1.0;
    }
    return size_factors;
}

vector<vector<double>> DESeq2Analyzer::normalizeCounts(const vector<vector<int>>& counts, const vector<double>& size_factors) {
    vector<vector<double>>normalized(counts.size(), vector<double>(counts[0].size()));
    //大小因子归一化：表达量除以大小因子
    for (int i = 0; i < counts.size(); i++) {
        for (int j = 0; j < counts[0].size(); j++) {
            normalized[i][j] = counts[i][j] / size_factors[j];
        }
    }
    return normalized;
}

vector<double> DESeq2Analyzer::estimateGeneDispersions(const vector<vector<double>>& normalized,
                                                       const vector<string>& sample_names,
                                                       const unordered_map<string, string>& sample_to_group,
                                                       const string& control_group, const string& treatment_group) {

    vector<double>raw_dispersions(normalized.size());
    //获取分组样本索引
    vector<int>control_indices;
    vector<int>treat_indices;
    for (int j = 0; j < normalized[0].size(); j++) {
        auto group = sample_to_group.at(sample_names[j]);
        if (group == control_group)
            control_indices.push_back(j);
        else if (group == treatment_group)
            treat_indices.push_back(j);
    }
    //对每个基因估计离散度
    for (int i = 0; i < normalized.size(); i++) {
        //计算组内均值和方差
        double mean_control = 0.0, var_control = 0.0;
        double mean_treat = 0.0, var_treat = 0.0;
        for (int j : control_indices) {
            mean_control += normalized[i][j];
            var_control += normalized[i][j] * normalized[i][j];
        }
        mean_control = mean_control / control_indices.size();
        var_control = var_control / control_indices.size() - mean_control * mean_control;
        for (int j : treat_indices) {
            mean_treat += normalized[i][j];
            var_treat += normalized[i][j] * normalized[i][j];
        }
        mean_treat = mean_treat / treat_indices.size();
        var_treat = var_treat/ treat_indices.size() - mean_treat * mean_treat;

        //合并方差估计
        double pooled_var = (var_control+var_treat)/2;
        double pooled_mean = (mean_control + mean_treat) / 2;

        //计算离散度(基于负二项分布的均值-方差关系)
        raw_dispersions[i] = max(0.01, (pooled_var-pooled_mean)/(pooled_mean*pooled_mean));
    }
    //计算基因平均表达量
    vector<double>gene_means(normalized.size());
    for (int i = 0; i < normalized.size(); i++) {
        gene_means[i] = accumulate(normalized[i].begin(), normalized[i].end(), 0.0) / normalized[i].size();
    }
    //趋势拟合
    fitDispersionTrend(gene_means,raw_dispersions);
    //应用收缩
    vector<double> final_dispersions;
    for (int i = 0; i < raw_dispersions.size();i++) {
        double trend_disp = predictDispersion(gene_means[i]);
        final_dispersions.push_back(0.5 * raw_dispersions[i] + 0.5 * trend_disp);
    }

    return final_dispersions;
    //return dispersions;
}

void DESeq2Analyzer::fitDispersionTrend(const vector<double>& means,
                                        const vector<double>& dispersions) {
    //1.数据预处理
    vector<double> log_means, log_disps;
    vector<int> valid_indices;
    //过滤低表达基因，均值阈值可调整
    const double min_mean = 1.0;
    for (int i = 0; i < means.size(); i++) {
        if (means[i] >= min_mean && !isnan(dispersions[i])) {
            log_means.push_back(log(means[i]));
            log_disps.push_back(log(dispersions[i]));
            valid_indices.push_back(i);
        }
    }

    //2.局部加权回归 loess
    const int n = log_means.size();
    const double bandwidth = 0.3;//带宽参数
    //在均值范围内生成拟合点
    double min_logmean = *min_element(log_means.begin(), log_means.end());
    double max_logmean = *max_element(log_means.begin(), log_means.end());
    const int npoints = 100;
    for (int i = 0; i < npoints; i++) {
        double x = min_logmean + i * (max_logmean - min_logmean) / (npoints - 1);
        //计算该权重 epanechnikov
        vector<double>weights(n);
        for (int j = 0; j < n; j++) {
            double u = abs(log_means[j] - x) / bandwidth;
            weights[j] = (u <= 1.0) ? (1.0 - u * u) : 0.0;
        }
        //加权线性回归
        double sum_w = 0.0, sum_wx = 0.0, sum_wy = 0.0, sum_wxx = 0.0, sum_wxy = 0.0;
        for (int j = 0; j < n; j++) {
            double w = weights[j];
            sum_w += w;
            sum_wx += w * log_means[j];
            sum_wy += w * log_disps[j];
            sum_wxx += w * log_means[j] * log_means[j];
            sum_wxy += w * log_means[j] * log_disps[j];
        }
        double slope = (sum_w * sum_wxy - sum_wx * sum_wy) / (sum_w * sum_wxx - sum_wx * sum_wx);
        double intercept = (sum_wy - slope * sum_wx) / sum_w;

        trend_means.push_back(x);
        trend_disps.push_back(slope * x + intercept);
    }
    //3.拟合全局趋势线（用于收缩）
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_xx = 0.0;
    for (int i = 0; i < n; i++) {
        sum_x += log_means[i];
        sum_y += log_disps[i];
        sum_xy += log_means[i] * log_disps[i];
        sum_xx += log_means[i] * log_means[i];
    }
    trend_slope = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    trend_intercept = (sum_y - trend_slope * sum_x) / n;
}

double DESeq2Analyzer::predictDispersion(double mean)const {
    double log_mean = log(mean);
    return exp(trend_slope*log_mean+trend_intercept);
}

vector<DESeq2Result> DESeq2Analyzer::performWaldTest(const vector<vector<double>>normalized,
                                                     const vector<double>& dispersions,
                                                     const CountData& data,
                                                     const string& control_group, const string& treatment_group) {

    vector<DESeq2Result>results;
    //获取分组样本索引
    vector<int>control_indices;
    vector<int>treat_indices;
    for (int j = 0; j < normalized[0].size(); j++) {
        auto group = data.sample_to_group.at(data.sample_names[j]);
        if (group == control_group)
            control_indices.push_back(j);
        else if (group == treatment_group)
            treat_indices.push_back(j);
    }
    //对每个基因进行检验
    for (int i = 0; i < normalized.size(); i++) {
        //计算组内均值
        double mean_control = 0.0;
        double mean_treat = 0.0;
        for (int j : control_indices)
            mean_control += normalized[i][j];
        for (int j : treat_indices)
            mean_treat += normalized[i][j];
        mean_control /= control_group.size();
        mean_treat /= treatment_group.size();
        //计算log2FC
        double log2fc = log2(mean_treat / mean_control);
        //简化版wald检验
        double se = sqrt(dispersions[i]/(1.0/control_indices.size()+1.0/treat_indices.size()));
        double wald_stat = log2fc / se;
        double p_value = 2.0 * (1.0 - 0.5 * (1.0 + erf(abs(wald_stat) / sqrt(2.0))))+ 1e-30;//1e-300
        if (!isinf(log2fc)&&!isnan(log2fc)) {
            results.push_back({ data.gene_names[i],
                               //(mean_control + mean_treat) / 2.0,
                               log2fc,
                               p_value,0.0 });
            //0.0 });
        }
    }
    //BH方法
    performBH(results);

    return results;
}

void DESeq2Analyzer::performBH(vector<DESeq2Result>& results) {
    vector<int>indices(results.size());
    for (int i = 0; i < indices.size(); i++) {
        indices[i] = i;
    }
    //按p值从小到大顺序存储索引
    sort(indices.begin(), indices.end(), [&results](int i, int j) {
        return results[i].p_value < results[j].p_value;
    });
    //计算padj
    for (int i = 0; i < indices.size(); i++) {
        int rank = i + 1; //p值排名
        const int m = indices.size(); //p值总数
        int index = indices[i];
        results[index].padj = results[index].p_value * m/rank;
    }

    //反向单调校正
    //for (int j = indices.size() - 1; j >= 1; j--) {
    //	if (results[indices[j]].padj < results[indices[j - 1]].padj)
    //		results[indices[j - 1]].padj = results[indices[j]].padj;
    //}
}
