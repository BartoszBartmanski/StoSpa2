
// stl
#include <fstream>
#include <sstream>

// other header files
#include "reaction.hpp"
#include "voxel.hpp"

namespace StoSpa2 {

void add_diffusion(std::vector<StoSpa2::Voxel>& voxels,
                   const std::vector<std::vector<double>>& jump_rates,
                   const std::function<double (const std::vector<unsigned>&, const double&)>& propensity,
                   const std::vector<int>& stoichiometry_vec) {
    for (unsigned i=0; i<voxels.size(); i++) {
        for (unsigned j=0; j<jump_rates[i].size(); j++) {
            if (i!=j) {
                voxels[i].add_reaction(StoSpa2::Reaction(jump_rates[i][j], propensity, stoichiometry_vec, j));
            }
        }
    }
}

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
