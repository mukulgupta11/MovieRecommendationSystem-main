#include "RecommendationSystem.h"
#include <cmath>
#include <vector>
#include <algorithm>



RecommendationSystem::RecommendationSystem () = default;

sp_movie RecommendationSystem::add_movie_to_rs (const std::string &name,
                                                int year,
                                                const std::vector<double>
                                                    &features)
{
  sp_movie movie = std::make_shared<Movie>(name, year);
  _movie_features[movie] = features;
  return movie;
}

sp_movie RecommendationSystem::recommend_by_content (const User &user)
{
  ///calculate mean of user's ranks
  rank_map ranks = user.get_ranks();
  double mean = 0;
  int values = 0;
  std::vector<double> features_vec;
  std::vector<double> preferences_vec(_movie_features.begin()->second
  .size()
      , 0);
  for (const auto& pair : ranks ){
    if (pair.second != 0){
      mean += pair.second;
      values += 1;
    }
  }
  mean = mean / values;
  ///calculate preferences_vec
  /// (fixed value of movie's feature * normalized movie's rank by user)
  for (const auto& pair : ranks){
    if (pair.second != 0){
      features_vec = _movie_features.find (pair.first)->second;
      for (int i = 0 ; i < (int) preferences_vec.size(); ++i){
        preferences_vec[i] += (features_vec[i] * (pair.second - mean));
      }
    }
  }
  ///choose movie that has max similarity rate with preferences of user
  double max_sim_rate = 0;
  sp_movie recommended_movie = nullptr;
  double cur_sim_rate;
  bool first_iter = true;
  for (const auto& pair : _movie_features){
    if (ranks[pair.first] == 0){
      cur_sim_rate = get_similarity(preferences_vec, pair.second);
      if (first_iter){
        max_sim_rate = cur_sim_rate;
        recommended_movie = pair.first;
        first_iter = false;
      }
      else if (cur_sim_rate>max_sim_rate){
        max_sim_rate = cur_sim_rate;
        recommended_movie = pair.first;
      }
    }
  }
  return recommended_movie;
}

double RecommendationSystem::get_similarity (std::vector<double> &vector1,
                                       const std::vector<double> &vector2)
{
  if (vector1.size() != vector2.size()){
    return 0;
  }

  double dot_result = 0;
  double norm1 = 0;
  double norm2 = 0;

  for (int i = 0; i < (int)vector1.size(); ++i){
    dot_result += vector1[i]*vector2[i];
    norm1 += vector1[i]*vector1[i];
    norm2 += vector2[i]*vector2[i];
  }
  if (norm1 == 0 || norm2 == 0)
  {
    return 0;
  }
  return (dot_result/(sqrt (norm1) * sqrt (norm2)));
}

sp_movie RecommendationSystem::recommend_by_cf (const User &user, int k)
{
  rank_map ranks = user.get_ranks();
  std::vector<std::pair<sp_movie, double>> predicted_ranks;

  for (const auto& pair : _movie_features){
    if (ranks[pair.first] == 0){
      predicted_ranks.push_back(std::make_pair
                                    (pair.first,predict_movie_score
                                                  (user,pair.first,k)));
    }
  }

  sp_movie recommended_movie = nullptr;
  double max_rank = -1;
  for (const auto& pair : predicted_ranks){
    double cur_rank = pair.second;
    if (cur_rank > max_rank){
      max_rank = cur_rank;
      recommended_movie = pair.first;
    }
  }
  return recommended_movie;
}

double RecommendationSystem::predict_movie_score (const User &user,
                                                  const sp_movie &movie, int k)
{
  std::vector<std::pair<sp_movie , double>> sim_rates;
  rank_map ranks = user.get_ranks();
  for (const auto& pair : _movie_features){
    if (ranks[pair.first] != 0)
    {
      double similarity = get_similarity (_movie_features[movie],
                                          pair.second);
      sim_rates.push_back(std::make_pair (pair.first, similarity));
    }
  }
  std::sort(sim_rates.begin(),sim_rates.end(), sort_by_value);
  double sr = 0;
  double s = 0;
  for (int i = 0; i < k ; ++i)
  {
    sr += sim_rates[i].second * ranks[sim_rates[i].first];
    s += sim_rates[i].second;
  }
  return sr/s;

}

bool RecommendationSystem::sort_by_value (const std::pair<sp_movie ,double>& a,
                    const std::pair<sp_movie ,double>& b)
{return a.second > b.second;}


sp_movie RecommendationSystem::get_movie (const std::string &name, int year)
{
  sp_movie tmp = std::make_shared<Movie>(name, year);
  auto it = _movie_features.find (tmp);
  if (it != _movie_features.end()){
    return it->first;
  }
  return nullptr;
}

std::ostream& operator<<(std::ostream& os,
                         const RecommendationSystem& rec_system)
{
  for (const auto& entry : rec_system._movie_features){
    os << *entry.first << std::endl;
  }
  return os;
}