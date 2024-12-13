#include "RecommendationSystemLoader.h"
#include "UsersLoader.h"
#define EXIT_FAIL_TEST 2
#define EXIT_SUCCESS_TEST 0



int main(){
  try {

    auto rs = RecommendationSystemLoader::create_rs_from_movies("MoviesDatabase.in_m");
    std::vector<User> users = UsersLoader::create_users("UsersDatabase.in_u", std::move(rs));

    // Check if users were loaded successfully
    if (users.empty()) {
        std::cerr << "Error: No users loaded from the file." << std::endl;
        return EXIT_FAIL_TEST;
    }

    // Loop through each user and print out their recommended movie by content-based filtering
    std::cout << "Content-based Recommendations:" << std::endl;
    for (const auto& user : users) {
        auto recommended_movie = user.get_rs_recommendation_by_content();
        std::cout << "User: " << user.get_name() << " | Recommended Movie: " 
                  << recommended_movie->get_name() << " (" 
                  << recommended_movie->get_year() << ")" << std::endl;
    }

    // Loop through each user and print out their recommended movie by collaborative filtering (CF)
    std::cout << "\nCollaborative Filtering Recommendations:" << std::endl;
    int k = 4;  // Number of similar movies to consider
    for (const auto& user : users) {
        auto recommended_movie = user.get_rs_recommendation_by_cf(k);
        if (recommended_movie) {
            std::cout << "User: " << user.get_name() << " | Recommended Movie: " 
                      << recommended_movie->get_name() << " (" 
                      << recommended_movie->get_year() << ")" << std::endl;
        } else {
            std::cout << "User: " << user.get_name() << " | No recommendation available." 
                      << std::endl;
        }
    }
    return EXIT_SUCCESS_TEST;
    
  } catch (const std::exception& e) {
      std::cerr << "An error occurred: " << e.what() << std::endl;
      return EXIT_FAIL_TEST;
  }
}