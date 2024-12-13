#include "User.h"
#include "RecommendationSystem.h"

User::User (std::string &user_name, rank_map &ranks,
            std::shared_ptr<RecommendationSystem> rec_system):
            _username(std::move (user_name)),
            _ranks(std::move (ranks)),
            _rec_system(std::move(rec_system)) {}

std::string User::get_name () const {return _username;}

void User::add_movie_to_user (const std::string &name, int year,
                              const std::vector<double> &features, double rate)
{
  sp_movie new_movie = _rec_system->add_movie_to_rs(name, year, features);
  _ranks[new_movie] = rate;
}

const rank_map &User::get_ranks () const {return _ranks;}

sp_movie User::get_rs_recommendation_by_content () const
{return _rec_system->recommend_by_content(*this);}

sp_movie User::get_rs_recommendation_by_cf (int k) const
{return _rec_system->recommend_by_cf (*this, k);}

double User::get_rs_prediction_score_for_movie (const std::string &name,
                                                int year, int k) const
{
  sp_movie movie_ptr = _rec_system->get_movie (name, year);
  return _rec_system->predict_movie_score (*this, movie_ptr, k);
}

std::ostream& operator<<(std::ostream& os, const User& user)
{
  os << "NAME: " << user._username << std::endl;
  os << *user._rec_system << std::endl;
  return os;
}

