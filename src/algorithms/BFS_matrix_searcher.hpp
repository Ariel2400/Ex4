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
        BFSMatrixSearcher();
        BFSMatrixSearcher(const BFSMatrixSearcher &other);
        BFSMatrixSearcher &operator=(const BFSMatrixSearcher &other);
        virtual SearchStatus search(const Problem &problem , std::string* solution, int* weight);
        ~BFSMatrixSearcher();
};