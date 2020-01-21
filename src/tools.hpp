
// stl
#include <fstream>
#include <sstream>

// other header files
#include "reaction.hpp"
#include "voxel.hpp"

namespace StoSpa2 {

/**
 * Function that adds a given instance of the Reaction class to all the voxels in the vector of voxels
 * @param voxels vector of Voxel class instances
 * @param r reaction to be added to all the voxels in the given vector
 */
void add_reaction(std::vector<StoSpa2::Voxel>& voxels, const StoSpa2::Reaction& r) {
    for (auto& v : voxels) {
        v.add_reaction(r);
    }
}

/**
 * Function to add diffusion to all the given voxels based on the matrix of jump rates
 * @param voxels vector of Voxel class instances
 * @param jump_rates matrix (vector of vectors) of jump rates between different voxels
 * @param propensity the propensity function used to create a Reaction instance
 * @param stoichiometry_vec the stoichiomery vector used to create a Reaction instance
 */
void add_diffusion(std::vector<StoSpa2::Voxel>& voxels, const std::vector<std::vector<double>>& jump_rates, p_f& propensity, const std::vector<int>& stoichiometry_vec) {
    for (unsigned i=0; i<voxels.size(); i++) {
        for (unsigned j=0; j<jump_rates[i].size(); j++) {
            if (i!=j) {
                voxels[i].add_reaction(StoSpa2::Reaction(jump_rates[i][j], propensity, stoichiometry_vec, j));
            }
        }
    }
}

/**
 * Helper function to split a string of characters based on a separator into a vector of doubles
 * @param input_str string to be split into vector of doubles
 * @param separator the character used to split the string into vector of doubles
 * @return resulting vector of doubles
 */
std::vector<double> split(const std::string& input_str, char separator)
{
    std::istringstream ss(input_str);
    std::string token;

    std::vector<double> output;
    while(getline(ss, token, separator))
    {
        output.push_back(stod(token));
    }

    return output;
}

/**
 * Helper function to read in a matrix from a file. Useful for reading the matrix of jump rates in
 * simulations of diffusion.
 * @param filename path to the file
 * @param separator character used to separate the strings into vectors of doubles
 * @return a matrix (vector of vectors) contained within the file
 */
std::vector<std::vector<double>> read_matrix(const std::string& filename, char separator=' ') {

    std::vector<std::vector<double>> output;

    std::string line;
    std::ifstream matrix_file(filename);

    if (matrix_file.is_open()) {
        while ( getline(matrix_file, line) ) {
            output.push_back(split(line, separator));
        }
        matrix_file.close();
    }

    return output;
}

}
