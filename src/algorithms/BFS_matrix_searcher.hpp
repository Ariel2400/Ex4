#include <string>

#include "matrix_searcher.hpp"
#include "../matrices/matrix_class.hpp"

class BFSMatrixSearcher: public MatrixSearcher {
    public:
        BFSMatrixSearcher();
        BFSMatrixSearcher(const BFSMatrixSearcher &other);
        BFSMatrixSearcher &operator=(const BFSMatrixSearcher &other);
        virtual SearchStatus search(const Problem &problem , std::string* solution);
        ~BFSMatrixSearcher();
};