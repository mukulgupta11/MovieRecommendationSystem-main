//
// Created on 2/20/2022.
//

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H
#include "User.h"
#include "Movie.h"
#include <map>
#include "RecommendationSystem.h"

/**
 * Comparator for shared pointers to Movie, used for sorting in the map.
 */
struct rs_comparator {
    bool operator()(const sp_movie& lhs, const sp_movie& rhs) const {
      return *lhs < *rhs;
    }
};


class RecommendationSystem
{
 private:
    std::map<sp_movie, std::vector<double>, rs_comparator> _movie_features;
    /**
    * Comparator for sorting by value in descending order.
    * @param a first pair of movie and score
    * @param b second pair of movie and score
    * @return true if the first pair's value is greater than the second
    */
    static bool sort_by_value (const std::pair<sp_movie ,double>& a,
                             const std::pair<sp_movie ,double>& b);
    /**
    * Calculates the cosine similarity between two feature vectors.
    * @param vector1 the first vector
    * @param vector2 the second vector
    * @return the cosine similarity between vector1 and vector2
    */
    static double get_similarity(std::vector<double>& vector1,
                               const std::vector<double>& vector2);

 public:

    RecommendationSystem();
    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie_to_rs(const std::string& name,
                             int year,
                             const std::vector<double>& features);

    /**
     * a function that calculates the movie with highest
     * score based on movie features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const User& user);



    /**
     * a function that calculates the movie with highest
     * predicted score based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const User& user, int k);


    /**
     * Predict a user rating for a movie given argument using
     * item cf procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const User &user, const sp_movie &movie,
												  int k);


	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
    sp_movie get_movie(const std::string& name, int year);
    /**
    * Outputs the recommendation system to an output stream.
    * @param os output stream
    * @param rec_system recommendation system
    * @return the output stream
    */
	friend std::ostream& operator<<(std::ostream& os,
                                    const RecommendationSystem& rec_system);

};


#endif //RECOMMENDATIONSYSTEM_H
