

#include "pch.h"
#include <iostream>


#include <vector>

#include <numeric>
#include <string>
#include <functional>

#include <memory>
#include <utility>

//enum  Color { red = 0, blue = 1, yellow, green, end_enum } ;

/////////////////////  1
inline double probability_body(const std::vector<std::vector<double>> && input, int start, int stop)
{
	std::vector<double> temp;
	for (auto v : input)
	{
		auto bIt = v.begin() + start;
		auto eIt = (stop == -1) ? v.end() : v.begin() + stop;
		double sum = std::accumulate(bIt, eIt, 0.0);
		temp.push_back(sum);
	}
	double sum = std::accumulate(temp.begin(), temp.end(), 0.0) / temp.size();
	return sum;
}



void print_probability(const std::vector<std::vector<double>> & input)
{
	double summ = 0.0;
	summ = probability_body(std::forward<const std::vector<std::vector<double>>>(input), 5, -1);
	std::cout << "probability (>5) := " << summ << std::endl;
	summ = probability_body(std::forward<const std::vector<std::vector<double>>>(input), 3, 10);
	std::cout << "probability [3,10] := " << summ << std::endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct Ctx
{
	double aver;
	int num_person;
	int num_points;
} Ctx;


//recursion body function for searching appropriate scope combinations
void get_combination(int index, Ctx * ctx, std::vector<std::vector<int>> * result, std::vector<int>  tmp_ )
{
	if (index == 0) 
	{
		int sum = std::accumulate(tmp_.begin(), tmp_.end(), 0);
		double aver = ((double)sum) / ctx->num_person;
		if (aver == ctx->aver)
			result->push_back(tmp_);
		return;
	}

	for (int i = ctx->num_points; i > 0; i--)
	{
		std::vector<int> tmp = tmp_;
		tmp.push_back(i);
		get_combination(index - 1, ctx, result, tmp);
	}
}

//recursion (head/main) function for searching appropriate scope combinations
std::vector<std::vector<int>> get_correct_point_combinations(const std::vector<std::vector<std::vector<double>>> & input, double average_point)
{
	int num_points = (int)input[0][0].size();
	int num_person = (int)input.size();

	Ctx ctx;
	ctx.aver = average_point;
	ctx.num_person = num_person;
	ctx.num_points = num_points;
	std::vector<std::vector<int>> result;

	std::vector<int> tmp;
	tmp.reserve(num_person);// point for every person
	for (int i = num_points; i > 0; i--)
	{
		tmp.push_back(i);
		get_combination(num_person - 1, &ctx, &result, tmp);
		tmp.clear();
	}
#if 1
	for (auto v : result)
	{
		for (auto x : v)
		{
			std::cout << x << " ";
		}
		std::cout << std::endl;
	}
#endif

	return std::move(result);
}

//probability for average point for team 
double team_probability(const std::vector<std::vector<std::vector<double>>> & input, double average_point)
{
	//get points combinations
	std::vector<std::vector<int>> points_combinations = get_correct_point_combinations(input, average_point);
	
	int num_points = (int)input[0][0].size();
	int num_person = (int)input.size();
	int num_colors = (int)input[0].size();
	
	std::vector<double> chains;//probability for all chains
	for (auto comb : points_combinations)
	{
		double pr_one_chain = 1.0;//probability for one chain
		for (int i = 0; i < (int)comb.size(); i++ )//quantity of team members
		{
			int index_point = comb[i] - 1;//index start from 0, poins from 1
			double pr_one_point = 0.0;
			for (int k = 0; k < num_colors; k++)
				pr_one_point += input[i][k][index_point];//summ fro all darts
			pr_one_point /= num_colors;

			pr_one_chain *= pr_one_point;//probalility of chain is multiplication prob. for all player
		}
		chains.push_back(pr_one_chain);
	}

	return std::accumulate(chains.begin(), chains.end(), 0.0);// ??? summ probability chains
}





int main()
{

	//1 probability for one player

	std::vector<std::vector<double>> probability_input = {
		{0.1, 0.11, 0.12, 0.13, 0.14 ,0.15, 0.16, 0.17, 0.18, 0.19, 0.2, 0.21, 0.22, 0.23, 0.24 ,0.25, 0.26, 0.27, 0.28, 0.29 },
		{0.1, 0.11, 0.12, 0.13, 0.14 ,0.15, 0.16, 0.17, 0.18, 0.19, 0.2, 0.21, 0.22, 0.23, 0.24 ,0.25, 0.26, 0.27, 0.28, 0.29 },
		{0.1, 0.11, 0.12, 0.13, 0.14 ,0.15, 0.16, 0.17, 0.18, 0.19, 0.2, 0.21, 0.22, 0.23, 0.24 ,0.25, 0.26, 0.27, 0.28, 0.29 },
		{0.1, 0.11, 0.12, 0.13, 0.14 ,0.15, 0.16, 0.17, 0.18, 0.19, 0.2, 0.21, 0.22, 0.23, 0.24 ,0.25, 0.26, 0.27, 0.28, 0.29 },
		};

	print_probability(probability_input);

	//2 probability for average point (5) for team - 3 participants

	std::vector<std::vector<double>> probability_input2 = probability_input;//2 player
	std::vector<std::vector<double>> probability_input3 = probability_input;//3 player

	std::vector<std::vector<std::vector<double>>> team;
	team.push_back(probability_input);
	team.push_back(probability_input2);
	team.push_back(probability_input3);

	double res = team_probability(team, 5.0);
	std::cout << "team probability for average 5 points: " << res << std::endl;
}


