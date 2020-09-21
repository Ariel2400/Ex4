#include <string>

#include "matrix_searcher.hpp"
#include "../matrices/matrix_class.hpp"

struct Step {
    public:
        int row;
        int column;
        std::string path;
        int weight;

        Step(int row, int column, std::string path, int weight) {
            this->row = row;
            this->column = column;
            this->path = path;
            this->weight = weight;
        }
};

class BFSMatrixSearcher: public MatrixSearcher {
    public:
        // constructor
        BFSMatrixSearcher();

        // copy constructor
        BFSMatrixSearcher(const BFSMatrixSearcher &other);

        // copy assignment
        BFSMatrixSearcher &operator=(const BFSMatrixSearcher &other);

        /* problem contains a matrix and start / end coordinates.
           assigns a description of a path from start to end into solution
           and its weight into weight. returns a fitting search status. */
        virtual SearchStatus search(const Problem &problem , std::string* solution, int* weight);

        // destructor
        ~BFSMatrixSearcher();
};